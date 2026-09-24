/*
 * Copyright 2024-present MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"
#include "phongo_util.h"
#include "BulkWriteCommand_arginfo.h"

#include "MongoDB/WriteConcern.h"

#define PHONGO_BULKWRITECOMMAND_BYPASS_UNSET -1

zend_class_entry* php_phongo_bulkwritecommand_ce;

/* Creates a mongoc_bulkwriteopts_t from internal options, which should be freed
 * by the caller. */
mongoc_bulkwriteopts_t* phongo_bwc_assemble_opts(php_phongo_bulkwritecommand_t* intern)
{
	mongoc_bulkwriteopts_t* opts = mongoc_bulkwriteopts_new();

	if (intern->bypass != PHONGO_BULKWRITECOMMAND_BYPASS_UNSET) {
		mongoc_bulkwriteopts_set_bypassdocumentvalidation(opts, intern->bypass);
	}

	if (intern->comment) {
		mongoc_bulkwriteopts_set_comment(opts, intern->comment);
	}

	if (intern->let) {
		mongoc_bulkwriteopts_set_let(opts, intern->let);
	}

	mongoc_bulkwriteopts_set_ordered(opts, intern->ordered);
	mongoc_bulkwriteopts_set_verboseresults(opts, intern->verbose);

	return opts;
}

// TODO: Make this a common utility function to share with BulkWrite.c
/* Extracts the "_id" field of a BSON document into a return value. */
static void phongo_bwc_extract_id(bson_t* doc, zval** return_value)
{
	zval*                 id = NULL;
	php_phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);
	state.map.root.type = PHONGO_TYPEMAP_NATIVE_ARRAY;

	/* TODO: Instead of converting the entire document, iterate BSON to obtain
	 * the bson_value_t of the _id field and then use phongo_bson_value_to_zval
	 * or phongo_bson_value_to_zval_legacy to populate the return value. */
	if (!php_phongo_bson_to_zval_ex(doc, &state)) {
		goto cleanup;
	}

	id = php_array_fetchc(&state.zchild, "_id");

	if (id) {
		ZVAL_ZVAL(*return_value, id, 1, 0);
	}

cleanup:
	zval_ptr_dtor(&state.zchild);
}

// TODO: Make this a common utility function to share with BulkWrite.c
/* Returns whether the BSON array's keys are a sequence of integer strings
 * starting with "0". */
static inline bool phongo_bwc_bson_array_has_valid_keys(bson_t* array)
{
	bson_iter_t iter;

	if (bson_empty(array)) {
		return true;
	}

	if (bson_iter_init(&iter, array)) {
		char key[12];
		int  count = 0;

		while (bson_iter_next(&iter)) {
			bson_snprintf(key, sizeof(key), "%d", count);

			if (0 != strcmp(key, bson_iter_key(&iter))) {
				return false;
			}

			count++;
		}
	}

	return true;
}

/* Constructs a new BulkWriteCommand */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommand, __construct)
{
	php_phongo_bulkwritecommand_t* intern;
	zval*                          zoptions = NULL;

	intern = Z_BULKWRITECOMMAND_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(zoptions)
	PHONGO_PARSE_PARAMETERS_END();

	intern->bw      = mongoc_bulkwrite_new();
	intern->bypass  = PHONGO_BULKWRITECOMMAND_BYPASS_UNSET;
	intern->comment = NULL;
	intern->let     = NULL;
	intern->num_ops = 0;
	intern->ordered = true;
	intern->verbose = false;

	if (!zoptions) {
		return;
	}

	if (php_array_existsc(zoptions, "bypassDocumentValidation")) {
		intern->bypass = php_array_fetchc_bool(zoptions, "bypassDocumentValidation") ? 1 : 0;
	}

	if (php_array_existsc(zoptions, "comment")) {
		zval* value = php_array_fetchc_deref(zoptions, "comment");

		intern->comment = ecalloc(1, sizeof(bson_value_t));
		phongo_zval_to_bson_value(value, intern->comment);

		if (EG(exception)) {
			return;
		}
	}

	if (php_array_existsc(zoptions, "let")) {
		zval* value = php_array_fetchc_deref(zoptions, "let");

		if (Z_TYPE_P(value) != IS_OBJECT && Z_TYPE_P(value) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"let\" option to be array or object, %s given", zend_get_type_by_const(Z_TYPE_P(value)));
			return;
		}

		intern->let = bson_new();
		php_phongo_zval_to_bson(value, PHONGO_BSON_NONE, intern->let, NULL);

		if (EG(exception)) {
			return;
		}
	}

	if (php_array_existsc(zoptions, "ordered")) {
		intern->ordered = php_array_fetchc_bool(zoptions, "ordered");
	}

	if (php_array_existsc(zoptions, "verboseResults")) {
		intern->verbose = php_array_fetchc_bool(zoptions, "verboseResults");
	}
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommand, count)
{
	php_phongo_bulkwritecommand_t* intern;

	intern = Z_BULKWRITECOMMAND_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->num_ops);
}

static bool phongo_bwc_parse_hint(zval* zhint, bson_value_t* bhint)
{
	if (Z_TYPE_P(zhint) != IS_STRING && Z_TYPE_P(zhint) != IS_OBJECT && Z_TYPE_P(zhint) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"hint\" option to be string, array, or object, %s given", zend_get_type_by_const(Z_TYPE_P(zhint)));
		return false;
	}

	phongo_zval_to_bson_value(zhint, bhint);

	if (EG(exception)) {
		return false;
	}

	// Catch the edge case where the option yields a BSON array
	if (bhint->value_type != BSON_TYPE_UTF8 && bhint->value_type != BSON_TYPE_DOCUMENT) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"hint\" option to yield string or document but got \"%s\"", php_phongo_bson_type_to_string(bhint->value_type));
		return false;
	}

	return true;
}

static bool phongo_bwc_parse_document(zval* zdocument, bson_t* bdocument, const char* key)
{
	if (Z_TYPE_P(zdocument) != IS_OBJECT && Z_TYPE_P(zdocument) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"%s\" option to be array or object, %s given", key, zend_get_type_by_const(Z_TYPE_P(zdocument)));
		return false;
	}

	php_phongo_zval_to_bson(zdocument, PHONGO_BSON_NONE, bdocument, NULL);

	if (EG(exception)) {
		return false;
	}

	return true;
}

static bool phongo_bwc_parse_array(zval* zarray, bson_t* barray, const char* key)
{
	if (Z_TYPE_P(zarray) != IS_OBJECT && Z_TYPE_P(zarray) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"%s\" option to be array or object, %s given", key, zend_get_type_by_const(Z_TYPE_P(zarray)));
		return false;
	}

	// Explicitly allow MongoDB\BSON\PackedArray for array values
	php_phongo_zval_to_bson(zarray, PHONGO_BSON_ALLOW_ROOT_ARRAY, barray, NULL);

	if (EG(exception)) {
		return false;
	}

	if (!phongo_bwc_bson_array_has_valid_keys(barray)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"%s\" option to yield array but got non-sequential keys", key);
		return false;
	}

	return true;
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommand, deleteMany)
{
	php_phongo_bulkwritecommand_t*     intern;
	char*                              ns;
	size_t                             ns_len;
	zval*                              zfilter;
	zval*                              zoptions = NULL;
	bson_t                             bfilter  = BSON_INITIALIZER;
	mongoc_bulkwrite_deletemanyopts_t* opts     = NULL;
	bson_error_t                       error    = { 0 };

	intern = Z_BULKWRITECOMMAND_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING(ns, ns_len)
	Z_PARAM_ARRAY_OR_OBJECT(zfilter)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(zoptions)
	PHONGO_PARSE_PARAMETERS_END();

	if (strlen(ns) != ns_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Namespace string should not contain null bytes");
		return;
	}

	php_phongo_zval_to_bson(zfilter, PHONGO_BSON_NONE, &bfilter, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	if (zoptions) {
		opts = mongoc_bulkwrite_deletemanyopts_new();

		if (php_array_existsc(zoptions, "hint")) {
			bson_value_t bhint = { 0 };

			if (!phongo_bwc_parse_hint(php_array_fetchc_deref(zoptions, "hint"), &bhint)) {
				bson_value_destroy(&bhint);
				goto cleanup;
			}

			mongoc_bulkwrite_deletemanyopts_set_hint(opts, &bhint);
			bson_value_destroy(&bhint);
		}

		if (php_array_existsc(zoptions, "collation")) {
			bson_t bcollation = BSON_INITIALIZER;

			if (!phongo_bwc_parse_document(php_array_fetchc_deref(zoptions, "collation"), &bcollation, "collation")) {
				bson_destroy(&bcollation);
				goto cleanup;
			}

			mongoc_bulkwrite_deletemanyopts_set_collation(opts, &bcollation);
			bson_destroy(&bcollation);
		}
	}

	if (!mongoc_bulkwrite_append_deletemany(intern->bw, ns, &bfilter, opts, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	intern->num_ops++;

cleanup:
	bson_destroy(&bfilter);
	mongoc_bulkwrite_deletemanyopts_destroy(opts);
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommand, deleteOne)
{
	php_phongo_bulkwritecommand_t*    intern;
	char*                             ns;
	size_t                            ns_len;
	zval*                             zfilter;
	zval*                             zoptions = NULL;
	bson_t                            bfilter  = BSON_INITIALIZER;
	mongoc_bulkwrite_deleteoneopts_t* opts     = NULL;
	bson_error_t                      error    = { 0 };

	intern = Z_BULKWRITECOMMAND_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING(ns, ns_len)
	Z_PARAM_ARRAY_OR_OBJECT(zfilter)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(zoptions)
	PHONGO_PARSE_PARAMETERS_END();

	if (strlen(ns) != ns_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Namespace string should not contain null bytes");
		return;
	}

	php_phongo_zval_to_bson(zfilter, PHONGO_BSON_NONE, &bfilter, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	if (zoptions) {
		opts = mongoc_bulkwrite_deleteoneopts_new();

		if (php_array_existsc(zoptions, "hint")) {
			bson_value_t bhint = { 0 };

			if (!phongo_bwc_parse_hint(php_array_fetchc_deref(zoptions, "hint"), &bhint)) {
				bson_value_destroy(&bhint);
				goto cleanup;
			}

			mongoc_bulkwrite_deleteoneopts_set_hint(opts, &bhint);
			bson_value_destroy(&bhint);
		}

		if (php_array_existsc(zoptions, "collation")) {
			bson_t bcollation = BSON_INITIALIZER;

			if (!phongo_bwc_parse_document(php_array_fetchc_deref(zoptions, "collation"), &bcollation, "collation")) {
				bson_destroy(&bcollation);
				goto cleanup;
			}

			mongoc_bulkwrite_deleteoneopts_set_collation(opts, &bcollation);
			bson_destroy(&bcollation);
		}
	}

	if (!mongoc_bulkwrite_append_deleteone(intern->bw, ns, &bfilter, opts, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	intern->num_ops++;

cleanup:
	bson_destroy(&bfilter);
	mongoc_bulkwrite_deleteoneopts_destroy(opts);
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommand, insertOne)
{
	php_phongo_bulkwritecommand_t* intern;
	char*                          ns;
	size_t                         ns_len;
	zval*                          zdocument;
	bson_t                         bdocument = BSON_INITIALIZER;
	bson_t*                        bson_out  = NULL;
	bson_error_t                   error     = { 0 };

	intern = Z_BULKWRITECOMMAND_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(2, 2)
	Z_PARAM_STRING(ns, ns_len)
	Z_PARAM_ARRAY_OR_OBJECT(zdocument)
	PHONGO_PARSE_PARAMETERS_END();

	if (strlen(ns) != ns_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Namespace string should not contain null bytes");
		return;
	}

	php_phongo_zval_to_bson(zdocument, (PHONGO_BSON_ADD_ID | PHONGO_BSON_RETURN_ID), &bdocument, &bson_out);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!bson_out) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "php_phongo_zval_to_bson() did not return an _id. Please file a bug report.");
		goto cleanup;
	}

	if (!mongoc_bulkwrite_append_insertone(intern->bw, ns, &bdocument, NULL, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	intern->num_ops++;

	phongo_bwc_extract_id(bson_out, &return_value);

cleanup:
	bson_destroy(&bdocument);
	bson_clear(&bson_out);
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommand, replaceOne)
{
	php_phongo_bulkwritecommand_t*     intern;
	char*                              ns;
	size_t                             ns_len;
	zval*                              zfilter;
	zval*                              zreplacement;
	zval*                              zoptions     = NULL;
	bson_t                             bfilter      = BSON_INITIALIZER;
	bson_t                             breplacement = BSON_INITIALIZER;
	mongoc_bulkwrite_replaceoneopts_t* opts         = NULL;
	bson_error_t                       error        = { 0 };

	intern = Z_BULKWRITECOMMAND_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(3, 4)
	Z_PARAM_STRING(ns, ns_len)
	Z_PARAM_ARRAY_OR_OBJECT(zfilter)
	Z_PARAM_ARRAY_OR_OBJECT(zreplacement)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(zoptions)
	PHONGO_PARSE_PARAMETERS_END();

	if (strlen(ns) != ns_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Namespace string should not contain null bytes");
		return;
	}

	php_phongo_zval_to_bson(zfilter, PHONGO_BSON_NONE, &bfilter, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	php_phongo_zval_to_bson(zreplacement, PHONGO_BSON_NONE, &breplacement, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	if (zoptions) {
		opts = mongoc_bulkwrite_replaceoneopts_new();

		if (php_array_existsc(zoptions, "collation")) {
			bson_t bcollation = BSON_INITIALIZER;

			if (!phongo_bwc_parse_document(php_array_fetchc_deref(zoptions, "collation"), &bcollation, "collation")) {
				bson_destroy(&bcollation);
				goto cleanup;
			}

			mongoc_bulkwrite_replaceoneopts_set_collation(opts, &bcollation);
			bson_destroy(&bcollation);
		}

		if (php_array_existsc(zoptions, "hint")) {
			bson_value_t bhint = { 0 };

			if (!phongo_bwc_parse_hint(php_array_fetchc_deref(zoptions, "hint"), &bhint)) {
				bson_value_destroy(&bhint);
				goto cleanup;
			}

			mongoc_bulkwrite_replaceoneopts_set_hint(opts, &bhint);
			bson_value_destroy(&bhint);
		}

		if (php_array_existsc(zoptions, "sort")) {
			bson_t bsort = BSON_INITIALIZER;

			if (!phongo_bwc_parse_document(php_array_fetchc_deref(zoptions, "sort"), &bsort, "sort")) {
				bson_destroy(&bsort);
				goto cleanup;
			}

			mongoc_bulkwrite_replaceoneopts_set_sort(opts, &bsort);
			bson_destroy(&bsort);
		}

		if (php_array_existsc(zoptions, "upsert")) {
			mongoc_bulkwrite_replaceoneopts_set_upsert(opts, php_array_fetchc_bool(zoptions, "upsert"));
		}
	}

	if (!mongoc_bulkwrite_append_replaceone(intern->bw, ns, &bfilter, &breplacement, opts, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	intern->num_ops++;

cleanup:
	bson_destroy(&bfilter);
	bson_destroy(&breplacement);
	mongoc_bulkwrite_replaceoneopts_destroy(opts);
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommand, updateMany)
{
	php_phongo_bulkwritecommand_t*     intern;
	char*                              ns;
	size_t                             ns_len;
	zval*                              zfilter;
	zval*                              zupdate;
	zval*                              zoptions = NULL;
	bson_t                             bfilter  = BSON_INITIALIZER;
	bson_t                             bupdate  = BSON_INITIALIZER;
	mongoc_bulkwrite_updatemanyopts_t* opts     = NULL;
	bson_error_t                       error    = { 0 };

	intern = Z_BULKWRITECOMMAND_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(3, 4)
	Z_PARAM_STRING(ns, ns_len)
	Z_PARAM_ARRAY_OR_OBJECT(zfilter)
	Z_PARAM_ARRAY_OR_OBJECT(zupdate)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(zoptions)
	PHONGO_PARSE_PARAMETERS_END();

	if (strlen(ns) != ns_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Namespace string should not contain null bytes");
		return;
	}

	php_phongo_zval_to_bson(zfilter, PHONGO_BSON_NONE, &bfilter, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	// Explicitly allow MongoDB\BSON\PackedArray for update pipelines
	php_phongo_zval_to_bson(zupdate, PHONGO_BSON_ALLOW_ROOT_ARRAY, &bupdate, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	if (zoptions) {
		opts = mongoc_bulkwrite_updatemanyopts_new();

		if (php_array_existsc(zoptions, "arrayFilters")) {
			bson_t barrayfilters = BSON_INITIALIZER;

			if (!phongo_bwc_parse_array(php_array_fetchc_deref(zoptions, "arrayFilters"), &barrayfilters, "arrayFilters")) {
				bson_destroy(&barrayfilters);
				goto cleanup;
			}

			mongoc_bulkwrite_updatemanyopts_set_arrayfilters(opts, &barrayfilters);
			bson_destroy(&barrayfilters);
		}

		if (php_array_existsc(zoptions, "collation")) {
			bson_t bcollation = BSON_INITIALIZER;

			if (!phongo_bwc_parse_document(php_array_fetchc_deref(zoptions, "collation"), &bcollation, "collation")) {
				bson_destroy(&bcollation);
				goto cleanup;
			}

			mongoc_bulkwrite_updatemanyopts_set_collation(opts, &bcollation);
			bson_destroy(&bcollation);
		}

		if (php_array_existsc(zoptions, "hint")) {
			bson_value_t bhint = { 0 };

			if (!phongo_bwc_parse_hint(php_array_fetchc_deref(zoptions, "hint"), &bhint)) {
				bson_value_destroy(&bhint);
				goto cleanup;
			}

			mongoc_bulkwrite_updatemanyopts_set_hint(opts, &bhint);
			bson_value_destroy(&bhint);
		}

		if (php_array_existsc(zoptions, "upsert")) {
			mongoc_bulkwrite_updatemanyopts_set_upsert(opts, php_array_fetchc_bool(zoptions, "upsert"));
		}
	}

	if (!mongoc_bulkwrite_append_updatemany(intern->bw, ns, &bfilter, &bupdate, opts, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	intern->num_ops++;

cleanup:
	bson_destroy(&bfilter);
	bson_destroy(&bupdate);
	mongoc_bulkwrite_updatemanyopts_destroy(opts);
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommand, updateOne)
{
	php_phongo_bulkwritecommand_t*    intern;
	char*                             ns;
	size_t                            ns_len;
	zval*                             zfilter;
	zval*                             zupdate;
	zval*                             zoptions = NULL;
	bson_t                            bfilter  = BSON_INITIALIZER;
	bson_t                            bupdate  = BSON_INITIALIZER;
	mongoc_bulkwrite_updateoneopts_t* opts     = NULL;
	bson_error_t                      error    = { 0 };

	intern = Z_BULKWRITECOMMAND_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(3, 4)
	Z_PARAM_STRING(ns, ns_len)
	Z_PARAM_ARRAY_OR_OBJECT(zfilter)
	Z_PARAM_ARRAY_OR_OBJECT(zupdate)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(zoptions)
	PHONGO_PARSE_PARAMETERS_END();

	if (strlen(ns) != ns_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Namespace string should not contain null bytes");
		return;
	}

	php_phongo_zval_to_bson(zfilter, PHONGO_BSON_NONE, &bfilter, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	// Explicitly allow MongoDB\BSON\PackedArray for update pipelines
	php_phongo_zval_to_bson(zupdate, PHONGO_BSON_ALLOW_ROOT_ARRAY, &bupdate, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	if (zoptions) {
		opts = mongoc_bulkwrite_updateoneopts_new();

		if (php_array_existsc(zoptions, "arrayFilters")) {
			bson_t barrayfilters = BSON_INITIALIZER;

			if (!phongo_bwc_parse_array(php_array_fetchc_deref(zoptions, "arrayFilters"), &barrayfilters, "arrayFilters")) {
				bson_destroy(&barrayfilters);
				goto cleanup;
			}

			mongoc_bulkwrite_updateoneopts_set_arrayfilters(opts, &barrayfilters);
			bson_destroy(&barrayfilters);
		}

		if (php_array_existsc(zoptions, "collation")) {
			bson_t bcollation = BSON_INITIALIZER;

			if (!phongo_bwc_parse_document(php_array_fetchc_deref(zoptions, "collation"), &bcollation, "collation")) {
				bson_destroy(&bcollation);
				goto cleanup;
			}

			mongoc_bulkwrite_updateoneopts_set_collation(opts, &bcollation);
			bson_destroy(&bcollation);
		}

		if (php_array_existsc(zoptions, "hint")) {
			bson_value_t bhint = { 0 };

			if (!phongo_bwc_parse_hint(php_array_fetchc_deref(zoptions, "hint"), &bhint)) {
				bson_value_destroy(&bhint);
				goto cleanup;
			}

			mongoc_bulkwrite_updateoneopts_set_hint(opts, &bhint);
			bson_value_destroy(&bhint);
		}

		if (php_array_existsc(zoptions, "sort")) {
			bson_t bsort = BSON_INITIALIZER;

			if (!phongo_bwc_parse_document(php_array_fetchc_deref(zoptions, "sort"), &bsort, "sort")) {
				bson_destroy(&bsort);
				goto cleanup;
			}

			mongoc_bulkwrite_updateoneopts_set_sort(opts, &bsort);
			bson_destroy(&bsort);
		}

		if (php_array_existsc(zoptions, "upsert")) {
			mongoc_bulkwrite_updateoneopts_set_upsert(opts, php_array_fetchc_bool(zoptions, "upsert"));
		}
	}

	if (!mongoc_bulkwrite_append_updateone(intern->bw, ns, &bfilter, &bupdate, opts, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	intern->num_ops++;

cleanup:
	bson_destroy(&bfilter);
	bson_destroy(&bupdate);
	mongoc_bulkwrite_updateoneopts_destroy(opts);
}

/* MongoDB\Driver\BulkWriteCommand object handlers */
static zend_object_handlers php_phongo_handler_bulkwritecommand;

static void php_phongo_bulkwritecommand_free_object(zend_object* object)
{
	php_phongo_bulkwritecommand_t* intern = Z_OBJ_BULKWRITECOMMAND(object);

	zend_object_std_dtor(&intern->std);

	if (intern->bw) {
		mongoc_bulkwrite_destroy(intern->bw);
	}

	if (intern->let) {
		bson_clear(&intern->let);
	}

	if (intern->comment) {
		bson_value_destroy(intern->comment);
		efree(intern->comment);
	}

	if (!Z_ISUNDEF(intern->session)) {
		zval_ptr_dtor(&intern->session);
	}
}

static zend_object* php_phongo_bulkwritecommand_create_object(zend_class_entry* class_type)
{
	php_phongo_bulkwritecommand_t* intern = zend_object_alloc(sizeof(php_phongo_bulkwritecommand_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_bulkwritecommand;

	return &intern->std;
}

static HashTable* php_phongo_bulkwritecommand_get_debug_info(zend_object* object, int* is_temp)
{
	zval                           retval = ZVAL_STATIC_INIT;
	php_phongo_bulkwritecommand_t* intern = NULL;

	*is_temp = 1;
	intern   = Z_OBJ_BULKWRITECOMMAND(object);
	array_init(&retval);

	if (intern->bypass != PHONGO_BULKWRITECOMMAND_BYPASS_UNSET) {
		ADD_ASSOC_BOOL_EX(&retval, "bypassDocumentValidation", intern->bypass);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "bypassDocumentValidation");
	}

	if (intern->comment) {
		zval zv;

		if (!phongo_bson_value_to_zval_legacy(intern->comment, &zv)) {
			zval_ptr_dtor(&zv);
			goto done;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "comment", &zv);
	}

	if (intern->let) {
		zval zv;

		if (!php_phongo_bson_to_zval(intern->let, &zv)) {
			zval_ptr_dtor(&zv);
			goto done;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "let", &zv);
	}

	ADD_ASSOC_BOOL_EX(&retval, "ordered", intern->ordered);
	ADD_ASSOC_BOOL_EX(&retval, "verboseResults", intern->verbose);

	if (!Z_ISUNDEF(intern->session)) {
		ADD_ASSOC_ZVAL_EX(&retval, "session", &intern->session);
		Z_ADDREF(intern->session);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "session");
	}

done:
	return Z_ARRVAL(retval);
}

void php_phongo_bulkwritecommand_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_bulkwritecommand_ce                = register_class_MongoDB_Driver_BulkWriteCommand(zend_ce_countable);
	php_phongo_bulkwritecommand_ce->create_object = php_phongo_bulkwritecommand_create_object;

	memcpy(&php_phongo_handler_bulkwritecommand, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_bulkwritecommand.get_debug_info = php_phongo_bulkwritecommand_get_debug_info;
	php_phongo_handler_bulkwritecommand.free_obj       = php_phongo_bulkwritecommand_free_object;
	php_phongo_handler_bulkwritecommand.offset         = XtOffsetOf(php_phongo_bulkwritecommand_t, std);
}
