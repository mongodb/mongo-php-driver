/*
 * Copyright 2015-present MongoDB, Inc.
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

#include "phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"
#include "BulkWrite_arginfo.h"

#include "MongoDB/WriteConcern.h"

#define PHONGO_BULKWRITE_BYPASS_UNSET -1

zend_class_entry* phongo_bulkwrite_ce;

/* Extracts the "_id" field of a BSON document into a return value. */
static void phongo_bulkwrite_extract_id(bson_t* doc, zval** return_value)
{
	zval*             id = NULL;
	phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);
	state.map.root.type = PHONGO_TYPEMAP_NATIVE_ARRAY;

	if (!phongo_bson_to_zval_ex(doc, &state)) {
		goto cleanup;
	}

	id = php_array_fetchc(&state.zchild, "_id");

	if (id) {
		ZVAL_ZVAL(*return_value, id, 1, 0);
	}

cleanup:
	zval_ptr_dtor(&state.zchild);
}

/* Returns whether any top-level field names in the document contain a "$". */
static inline bool phongo_bulkwrite_update_has_operators(bson_t* bupdate)
{
	bson_iter_t iter;

	if (bson_iter_init(&iter, bupdate)) {
		while (bson_iter_next(&iter)) {
			if (strchr(bson_iter_key(&iter), '$')) {
				return true;
			}
		}
	}

	return false;
}

/* Returns whether the update document is considered an aggregation pipeline */
static inline bool phongo_bulkwrite_update_is_pipeline(bson_t* bupdate)
{
	bson_iter_t iter;
	bson_iter_t child;
	const char* key;
	int         i = 0;
	char*       i_str;

	if (!bson_iter_init(&iter, bupdate)) {
		return false;
	}

	while (bson_iter_next(&iter)) {
		key   = bson_iter_key(&iter);
		i_str = bson_strdup_printf("%d", i++);

		if (strcmp(key, i_str)) {
			bson_free(i_str);
			return false;
		}

		bson_free(i_str);

		if (BSON_ITER_HOLDS_DOCUMENT(&iter)) {
			if (!bson_iter_recurse(&iter, &child)) {
				return false;
			}
			if (!bson_iter_next(&child)) {
				return false;
			}
			key = bson_iter_key(&child);
			if (key[0] != '$') {
				return false;
			}
		} else {
			return false;
		}
	}

	/* should return false when the document is empty */
	return i != 0;
}

/* Returns whether the BSON array's keys are a sequence of integer strings
 * starting with "0". BSON_APPEND_ARRAY considers it the caller's responsibility
 * to ensure that the array's keys are properly formatted. */
static inline bool phongo_bulkwrite_bson_array_has_valid_keys(bson_t* array)
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

/* Appends an array field for the given opts document and key. Returns true on
 * success; otherwise, false is returned and an exception is thrown. */
static bool phongo_bulkwrite_opts_append_array(bson_t* opts, const char* key, zval* zarr)
{
	zval*  value = php_array_fetch_deref(zarr, key);
	bson_t b     = BSON_INITIALIZER;

	if (Z_TYPE_P(value) != IS_OBJECT && Z_TYPE_P(value) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"%s\" option to be array or object, %s given", key, zend_get_type_by_const(Z_TYPE_P(value)));
		return false;
	}

	// Explicitly allow MongoDB\BSON\PackedArray for array values
	phongo_zval_to_bson(value, PHONGO_BSON_ALLOW_ROOT_ARRAY, &b, NULL);

	if (EG(exception)) {
		bson_destroy(&b);
		return false;
	}

	if (!phongo_bulkwrite_bson_array_has_valid_keys(&b)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "\"%s\" option has invalid keys for a BSON array", key);
		bson_destroy(&b);
		return false;
	}

	if (!BSON_APPEND_ARRAY(opts, key, &b)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"%s\" option", key);
		bson_destroy(&b);
		return false;
	}

	bson_destroy(&b);
	return true;
}

/* Appends a document field for the given opts document and key. Returns true on
 * success; otherwise, false is returned and an exception is thrown. */
static bool phongo_bulkwrite_opts_append_document(bson_t* opts, const char* key, zval* zarr)
{
	zval*  value = php_array_fetch_deref(zarr, key);
	bson_t b     = BSON_INITIALIZER;

	if (Z_TYPE_P(value) != IS_OBJECT && Z_TYPE_P(value) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"%s\" option to be array or object, %s given", key, zend_get_type_by_const(Z_TYPE_P(value)));
		return false;
	}

	phongo_zval_to_bson(value, PHONGO_BSON_NONE, &b, NULL);

	if (EG(exception)) {
		bson_destroy(&b);
		return false;
	}

	if (!BSON_APPEND_DOCUMENT(opts, key, &b)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"%s\" option", key);
		bson_destroy(&b);
		return false;
	}

	bson_destroy(&b);
	return true;
}

#define PHONGO_BULKWRITE_APPEND_BOOL(opt, value)                                                       \
	if (!BSON_APPEND_BOOL(boptions, (opt), (value))) {                                                 \
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"%s\" option", (opt)); \
		return false;                                                                                  \
	}

#define PHONGO_BULKWRITE_APPEND_INT32(opt, value)                                                      \
	if (!BSON_APPEND_INT32(boptions, (opt), (value))) {                                                \
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"%s\" option", (opt)); \
		return false;                                                                                  \
	}

#define PHONGO_BULKWRITE_OPT_ARRAY(opt)                                       \
	if (zoptions && php_array_existsc(zoptions, (opt))) {                     \
		if (!phongo_bulkwrite_opts_append_array(boptions, (opt), zoptions)) { \
			return false;                                                     \
		}                                                                     \
	}

#define PHONGO_BULKWRITE_OPT_DOCUMENT(opt)                                       \
	if (zoptions && php_array_existsc(zoptions, (opt))) {                        \
		if (!phongo_bulkwrite_opts_append_document(boptions, (opt), zoptions)) { \
			return false;                                                        \
		}                                                                        \
	}

/* Initialize the "hint" option. Returns true on success; otherwise, false is
 * returned and an exception is thrown.
 *
 * The "hint" option must be a string or document. Check for both types and
 * merge into BSON options accordingly. */
static bool phongo_bulkwrite_opt_hint(bson_t* boptions, zval* zoptions)
{
	/* The "hint" option (or "$hint" modifier) must be a string or document.
	 * Check for both types and merge into BSON options accordingly. */
	if (zoptions && php_array_existsc(zoptions, "hint")) {
		zend_uchar type = Z_TYPE_P(php_array_fetchc_deref(zoptions, "hint"));

		if (type == IS_STRING) {
			zval* value = php_array_fetchc_deref(zoptions, "hint");

			if (!bson_append_utf8(boptions, ZEND_STRL("hint"), Z_STRVAL_P(value), Z_STRLEN_P(value))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"hint\" option");
				return false;
			}
		} else if (type == IS_OBJECT || type == IS_ARRAY) {
			PHONGO_BULKWRITE_OPT_DOCUMENT("hint");
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"hint\" option to be string, array, or object, %s given", zend_get_type_by_const(type));
			return false;
		}
	}

	return true;
}

/* Applies options (including defaults) for an update operation. */
static bool phongo_bulkwrite_update_apply_options(bson_t* boptions, zval* zoptions)
{
	bool multi = false, upsert = false;

	if (zoptions) {
		multi  = php_array_fetchc_bool(zoptions, "multi");
		upsert = php_array_fetchc_bool(zoptions, "upsert");
	}

	PHONGO_BULKWRITE_APPEND_BOOL("multi", multi);
	PHONGO_BULKWRITE_APPEND_BOOL("upsert", upsert);
	PHONGO_BULKWRITE_OPT_ARRAY("arrayFilters");
	PHONGO_BULKWRITE_OPT_DOCUMENT("collation");
	PHONGO_BULKWRITE_OPT_DOCUMENT("sort");

	if (!phongo_bulkwrite_opt_hint(boptions, zoptions)) {
		return false;
	}

	return true;
}

/* Applies options (including defaults) for a delete operation. */
static bool phongo_bulkwrite_delete_apply_options(bson_t* boptions, zval* zoptions)
{
	int32_t limit = 0;

	if (zoptions) {
		limit = php_array_fetchc_bool(zoptions, "limit") ? 1 : 0;
	}

	PHONGO_BULKWRITE_APPEND_INT32("limit", limit);
	PHONGO_BULKWRITE_OPT_DOCUMENT("collation");

	if (!phongo_bulkwrite_opt_hint(boptions, zoptions)) {
		return false;
	}

	return true;
}

#undef PHONGO_BULKWRITE_APPEND_BOOL
#undef PHONGO_BULKWRITE_APPEND_INT32
#undef PHONGO_BULKWRITE_OPT_DOCUMENT

/* Constructs a new BulkWrite */
static PHP_METHOD(MongoDB_Driver_BulkWrite, __construct)
{
	PHONGO_INTERN_FROM_THIS(bulkwrite);

	zval*     options = NULL;
	zend_bool ordered = 1;

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	if (options && php_array_existsc(options, "ordered")) {
		ordered = php_array_fetchc_bool(options, "ordered");
	}

	intern->bulk     = mongoc_bulk_operation_new(ordered);
	intern->ordered  = ordered;
	intern->bypass   = PHONGO_BULKWRITE_BYPASS_UNSET;
	intern->let      = NULL;
	intern->num_ops  = 0;
	intern->executed = false;

	if (options && php_array_existsc(options, "bypassDocumentValidation")) {
		zend_bool bypass = php_array_fetchc_bool(options, "bypassDocumentValidation");
		mongoc_bulk_operation_set_bypass_document_validation(intern->bulk, bypass);
		intern->bypass = bypass;
	}

	if (options && php_array_existsc(options, "let")) {
		zval* value = php_array_fetchc_deref(options, "let");

		if (Z_TYPE_P(value) != IS_OBJECT && Z_TYPE_P(value) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"let\" option to be array or object, %s given", zend_get_type_by_const(Z_TYPE_P(value)));
			return;
		}

		intern->let = bson_new();
		phongo_zval_to_bson(value, PHONGO_BSON_NONE, intern->let, NULL);

		if (EG(exception)) {
			return;
		}

		mongoc_bulk_operation_set_let(intern->bulk, intern->let);
	}

	if (options && php_array_existsc(options, "comment")) {
		zval* value = php_array_fetchc_deref(options, "comment");

		intern->comment = ecalloc(1, sizeof(bson_value_t));
		phongo_zval_to_bson_value(value, intern->comment);

		if (EG(exception)) {
			/* Exception should already have been thrown */
			return;
		}

		mongoc_bulk_operation_set_comment(intern->bulk, intern->comment);
	}
}

/* Adds an insert operation to the BulkWrite */
static PHP_METHOD(MongoDB_Driver_BulkWrite, insert)
{
	PHONGO_INTERN_FROM_THIS(bulkwrite);

	zval*        zdocument;
	bson_t       bdocument = BSON_INITIALIZER, boptions = BSON_INITIALIZER;
	bson_t*      bson_out = NULL;
	bson_error_t error    = { 0 };

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY_OR_OBJECT(zdocument)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_zval_to_bson(zdocument, (PHONGO_BSON_ADD_ID | PHONGO_BSON_RETURN_ID), &bdocument, &bson_out);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!bson_out) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "phongo_zval_to_bson() did not return an _id. Please file a bug report.");
		goto cleanup;
	}

	if (!mongoc_bulk_operation_insert_with_opts(intern->bulk, &bdocument, &boptions, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	intern->num_ops++;

	phongo_bulkwrite_extract_id(bson_out, &return_value);

cleanup:
	bson_destroy(&bdocument);
	bson_destroy(&boptions);
	bson_clear(&bson_out);
}

/* Adds an update operation to the BulkWrite */
static PHP_METHOD(MongoDB_Driver_BulkWrite, update)
{
	PHONGO_INTERN_FROM_THIS(bulkwrite);

	zval *       zquery, *zupdate, *zoptions = NULL;
	bson_t       bquery = BSON_INITIALIZER, bupdate = BSON_INITIALIZER, boptions = BSON_INITIALIZER;
	bson_error_t error = { 0 };

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_ARRAY_OR_OBJECT(zquery)
	Z_PARAM_ARRAY_OR_OBJECT(zupdate)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(zoptions)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_zval_to_bson(zquery, PHONGO_BSON_NONE, &bquery, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	// Explicitly allow MongoDB\BSON\PackedArray for update pipelines
	phongo_zval_to_bson(zupdate, PHONGO_BSON_ALLOW_ROOT_ARRAY, &bupdate, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!phongo_bulkwrite_update_apply_options(&boptions, zoptions)) {
		goto cleanup;
	}

	if (phongo_bulkwrite_update_has_operators(&bupdate) || phongo_bulkwrite_update_is_pipeline(&bupdate)) {
		if (zoptions && php_array_fetchc_bool(zoptions, "multi")) {
			if (!mongoc_bulk_operation_update_many_with_opts(intern->bulk, &bquery, &bupdate, &boptions, &error)) {
				phongo_throw_exception_from_bson_error_t(&error);
				goto cleanup;
			}
		} else {
			if (!mongoc_bulk_operation_update_one_with_opts(intern->bulk, &bquery, &bupdate, &boptions, &error)) {
				phongo_throw_exception_from_bson_error_t(&error);
				goto cleanup;
			}
		}
	} else {
		if (zoptions && php_array_fetchc_bool(zoptions, "multi")) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Replacement document conflicts with true \"multi\" option");
			goto cleanup;
		}

		if (!mongoc_bulk_operation_replace_one_with_opts(intern->bulk, &bquery, &bupdate, &boptions, &error)) {
			phongo_throw_exception_from_bson_error_t(&error);
			goto cleanup;
		}
	}

	intern->num_ops++;

cleanup:
	bson_destroy(&bquery);
	bson_destroy(&bupdate);
	bson_destroy(&boptions);
}

/* Adds a delete operation to the BulkWrite */
static PHP_METHOD(MongoDB_Driver_BulkWrite, delete)
{
	PHONGO_INTERN_FROM_THIS(bulkwrite);

	zval *       zquery, *zoptions = NULL;
	bson_t       bquery = BSON_INITIALIZER, boptions = BSON_INITIALIZER;
	bson_error_t error = { 0 };

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_ARRAY_OR_OBJECT(zquery)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(zoptions)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_zval_to_bson(zquery, PHONGO_BSON_NONE, &bquery, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!phongo_bulkwrite_delete_apply_options(&boptions, zoptions)) {
		goto cleanup;
	}

	if (zoptions && php_array_fetchc_bool(zoptions, "limit")) {
		if (!mongoc_bulk_operation_remove_one_with_opts(intern->bulk, &bquery, &boptions, &error)) {
			phongo_throw_exception_from_bson_error_t(&error);
			goto cleanup;
		}
	} else {
		if (!mongoc_bulk_operation_remove_many_with_opts(intern->bulk, &bquery, &boptions, &error)) {
			phongo_throw_exception_from_bson_error_t(&error);
			goto cleanup;
		}
	}

	intern->num_ops++;

cleanup:
	bson_destroy(&bquery);
	bson_destroy(&boptions);
}

/* Returns the number of operations that have been added to the BulkWrite */
static PHP_METHOD(MongoDB_Driver_BulkWrite, count)
{
	PHONGO_INTERN_FROM_THIS(bulkwrite);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->num_ops);
}

/* MongoDB\Driver\BulkWrite object handlers */
static zend_object_handlers phongo_handler_bulkwrite;

static void phongo_bulkwrite_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(bulkwrite, object);

	zend_object_std_dtor(&intern->std);

	if (intern->bulk) {
		mongoc_bulk_operation_destroy(intern->bulk);
	}

	if (intern->let) {
		bson_clear(&intern->let);
	}

	if (intern->comment) {
		bson_value_destroy(intern->comment);
		efree(intern->comment);
	}

	if (intern->database) {
		efree(intern->database);
	}

	if (intern->collection) {
		efree(intern->collection);
	}

	if (!Z_ISUNDEF(intern->session)) {
		zval_ptr_dtor(&intern->session);
	}
}

static zend_object* phongo_bulkwrite_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(bulkwrite, class_type);

	intern->std.handlers = &phongo_handler_bulkwrite;

	return &intern->std;
}

static HashTable* phongo_bulkwrite_get_debug_info(zend_object* object, int* is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(bulkwrite, object);

	zval retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	array_init(&retval);

	if (intern->database) {
		ADD_ASSOC_STRING(&retval, "database", intern->database);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "database");
	}

	if (intern->collection) {
		ADD_ASSOC_STRING(&retval, "collection", intern->collection);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "collection");
	}

	ADD_ASSOC_BOOL_EX(&retval, "ordered", intern->ordered);

	if (intern->bypass != PHONGO_BULKWRITE_BYPASS_UNSET) {
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

		if (!phongo_bson_to_zval(intern->let, &zv)) {
			zval_ptr_dtor(&zv);
			goto done;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "let", &zv);
	}

	ADD_ASSOC_BOOL_EX(&retval, "executed", intern->executed);
	ADD_ASSOC_LONG_EX(&retval, "server_id", mongoc_bulk_operation_get_server_id(intern->bulk));

	if (!Z_ISUNDEF(intern->session)) {
		ADD_ASSOC_ZVAL_EX(&retval, "session", &intern->session);
		Z_ADDREF(intern->session);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "session");
	}

	if (mongoc_bulk_operation_get_write_concern(intern->bulk)) {
		zval write_concern;

		phongo_writeconcern_init(&write_concern, mongoc_bulk_operation_get_write_concern(intern->bulk));
		ADD_ASSOC_ZVAL_EX(&retval, "write_concern", &write_concern);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "write_concern");
	}

done:
	return Z_ARRVAL(retval);
}

void phongo_bulkwrite_init_ce(INIT_FUNC_ARGS)
{
	phongo_bulkwrite_ce                = register_class_MongoDB_Driver_BulkWrite(zend_ce_countable);
	phongo_bulkwrite_ce->create_object = phongo_bulkwrite_create_object;

	memcpy(&phongo_handler_bulkwrite, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_bulkwrite.get_debug_info = phongo_bulkwrite_get_debug_info;
	phongo_handler_bulkwrite.free_obj       = phongo_bulkwrite_free_object;
	phongo_handler_bulkwrite.offset         = XtOffsetOf(phongo_bulkwrite_t, std);
}
