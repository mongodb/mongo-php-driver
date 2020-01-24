/*
 * Copyright 2015-2017 MongoDB, Inc.
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

#include <php.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_array_api.h"
#include "phongo_compat.h"
#include "php_phongo.h"
#include "php_bson.h"

#define PHONGO_BULKWRITE_BYPASS_UNSET -1

zend_class_entry* php_phongo_bulkwrite_ce;

/* Extracts the "_id" field of a BSON document into a return value. */
static void php_phongo_bulkwrite_extract_id(bson_t* doc, zval** return_value) /* {{{ */
{
	zval*                 id = NULL;
	php_phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);
	state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

	if (!php_phongo_bson_to_zval_ex(bson_get_data(doc), doc->len, &state)) {
		goto cleanup;
	}

#if PHP_VERSION_ID >= 70000
	id = php_array_fetchc(&state.zchild, "_id");
#else
	id = php_array_fetchc(state.zchild, "_id");
#endif

	if (id) {
		ZVAL_ZVAL(*return_value, id, 1, 0);
	}

cleanup:
	zval_ptr_dtor(&state.zchild);
} /* }}} */

/* Returns whether any top-level field names in the document contain a "$". */
static inline bool php_phongo_bulkwrite_update_has_operators(bson_t* bupdate) /* {{{ */
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
} /* }}} */

/* Returns whether the update document is considered an aggregation pipeline */
static inline bool php_phongo_bulkwrite_update_is_pipeline(bson_t* bupdate) /* {{{ */
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
} /* }}} */

/* Returns whether the BSON array's keys are a sequence of integer strings
 * starting with "0". BSON_APPEND_ARRAY considers it the caller's responsibility
 * to ensure that the array's keys are properly formatted. */
static inline bool php_phongo_bulkwrite_bson_array_has_valid_keys(bson_t* array) /* {{{ */
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
} /* }}} */

/* Appends an array field for the given opts document and key. Returns true on
 * success; otherwise, false is returned and an exception is thrown. */
static bool php_phongo_bulkwrite_opts_append_array(bson_t* opts, const char* key, zval* zarr TSRMLS_DC) /* {{{ */
{
	zval*  value = php_array_fetch(zarr, key);
	bson_t b     = BSON_INITIALIZER;

	if (Z_TYPE_P(value) != IS_OBJECT && Z_TYPE_P(value) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected \"%s\" option to be array or object, %s given", key, zend_get_type_by_const(Z_TYPE_P(value)));
		return false;
	}

	php_phongo_zval_to_bson(value, PHONGO_BSON_NONE, &b, NULL TSRMLS_CC);

	if (EG(exception)) {
		bson_destroy(&b);
		return false;
	}

	if (!php_phongo_bulkwrite_bson_array_has_valid_keys(&b)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "\"%s\" option has invalid keys for a BSON array", key);
		bson_destroy(&b);
		return false;
	}

	if (!BSON_APPEND_ARRAY(opts, key, &b)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error appending \"%s\" option", key);
		bson_destroy(&b);
		return false;
	}

	bson_destroy(&b);
	return true;
} /* }}} */

/* Appends a document field for the given opts document and key. Returns true on
 * success; otherwise, false is returned and an exception is thrown. */
static bool php_phongo_bulkwrite_opts_append_document(bson_t* opts, const char* key, zval* zarr TSRMLS_DC) /* {{{ */
{
	zval*  value = php_array_fetch(zarr, key);
	bson_t b     = BSON_INITIALIZER;

	if (Z_TYPE_P(value) != IS_OBJECT && Z_TYPE_P(value) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected \"%s\" option to be array or object, %s given", key, zend_get_type_by_const(Z_TYPE_P(value)));
		return false;
	}

	php_phongo_zval_to_bson(value, PHONGO_BSON_NONE, &b, NULL TSRMLS_CC);

	if (EG(exception)) {
		bson_destroy(&b);
		return false;
	}

	if (!BSON_APPEND_DOCUMENT(opts, key, &b)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error appending \"%s\" option", key);
		bson_destroy(&b);
		return false;
	}

	bson_destroy(&b);
	return true;
} /* }}} */

#define PHONGO_BULKWRITE_APPEND_BOOL(opt, value)                                                                 \
	if (!BSON_APPEND_BOOL(boptions, (opt), (value))) {                                                           \
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error appending \"%s\" option", (opt)); \
		return false;                                                                                            \
	}

#define PHONGO_BULKWRITE_APPEND_INT32(opt, value)                                                                \
	if (!BSON_APPEND_INT32(boptions, (opt), (value))) {                                                          \
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error appending \"%s\" option", (opt)); \
		return false;                                                                                            \
	}

#define PHONGO_BULKWRITE_OPT_ARRAY(opt)                                                     \
	if (zoptions && php_array_existsc(zoptions, (opt))) {                                   \
		if (!php_phongo_bulkwrite_opts_append_array(boptions, (opt), zoptions TSRMLS_CC)) { \
			return false;                                                                   \
		}                                                                                   \
	}

#define PHONGO_BULKWRITE_OPT_DOCUMENT(opt)                                                     \
	if (zoptions && php_array_existsc(zoptions, (opt))) {                                      \
		if (!php_phongo_bulkwrite_opts_append_document(boptions, (opt), zoptions TSRMLS_CC)) { \
			return false;                                                                      \
		}                                                                                      \
	}

/* Initialize the "hint" option. Returns true on success; otherwise, false is
 * returned and an exception is thrown.
 *
 * The "hint" option must be a string or document. Check for both types and
 * merge into BSON options accordingly. */
static bool php_phongo_bulkwrite_opt_hint(bson_t* boptions, zval* zoptions TSRMLS_DC) /* {{{ */
{
	/* The "hint" option (or "$hint" modifier) must be a string or document.
	 * Check for both types and merge into BSON options accordingly. */
	if (zoptions && php_array_existsc(zoptions, "hint")) {
		zend_uchar type = Z_TYPE_P(php_array_fetchc(zoptions, "hint"));

		if (type == IS_STRING) {
			zval* value = php_array_fetchc(zoptions, "hint");

			if (!bson_append_utf8(boptions, "hint", 4, Z_STRVAL_P(value), Z_STRLEN_P(value))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error appending \"hint\" option");
				return false;
			}
		} else if (type == IS_OBJECT || type == IS_ARRAY) {
			PHONGO_BULKWRITE_OPT_DOCUMENT("hint");
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected \"hint\" option to be string, array, or object, %s given", zend_get_type_by_const(type));
			return false;
		}
	}

	return true;
} /* }}} */

/* Applies options (including defaults) for an update operation. */
static bool php_phongo_bulkwrite_update_apply_options(bson_t* boptions, zval* zoptions TSRMLS_DC) /* {{{ */
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

	if (!php_phongo_bulkwrite_opt_hint(boptions, zoptions TSRMLS_CC)) {
		return false;
	}

	return true;
} /* }}} */

/* Applies options (including defaults) for an delete operation. */
static bool php_phongo_bulkwrite_delete_apply_options(bson_t* boptions, zval* zoptions TSRMLS_DC) /* {{{ */
{
	int32_t limit = 0;

	if (zoptions) {
		limit = php_array_fetchc_bool(zoptions, "limit") ? 1 : 0;
	}

	PHONGO_BULKWRITE_APPEND_INT32("limit", limit);
	PHONGO_BULKWRITE_OPT_DOCUMENT("collation");

	return true;
} /* }}} */

#undef PHONGO_BULKWRITE_APPEND_BOOL
#undef PHONGO_BULKWRITE_APPEND_INT32
#undef PHONGO_BULKWRITE_OPT_DOCUMENT

/* {{{ proto void MongoDB\Driver\BulkWrite::__construct([array $options = array()])
   Constructs a new BulkWrite */
static PHP_METHOD(BulkWrite, __construct)
{
	php_phongo_bulkwrite_t* intern;
	zend_error_handling     error_handling;
	zval*                   options = NULL;
	zend_bool               ordered = 1;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a!", &options) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (options && php_array_existsc(options, "ordered")) {
		ordered = php_array_fetchc_bool(options, "ordered");
	}

	intern->bulk     = mongoc_bulk_operation_new(ordered);
	intern->ordered  = ordered;
	intern->bypass   = PHONGO_BULKWRITE_BYPASS_UNSET;
	intern->num_ops  = 0;
	intern->executed = false;

	if (options && php_array_existsc(options, "bypassDocumentValidation")) {
		zend_bool bypass = php_array_fetchc_bool(options, "bypassDocumentValidation");
		mongoc_bulk_operation_set_bypass_document_validation(intern->bulk, bypass);
		intern->bypass = bypass;
	}
} /* }}} */

/* {{{ proto mixed MongoDB\Driver\BulkWrite::insert(array|object $document)
   Adds an insert operation to the BulkWrite */
static PHP_METHOD(BulkWrite, insert)
{
	php_phongo_bulkwrite_t* intern;
	zval*                   zdocument;
	bson_t                  bdocument = BSON_INITIALIZER, boptions = BSON_INITIALIZER;
	bson_t*                 bson_out   = NULL;
	int                     bson_flags = PHONGO_BSON_ADD_ID;
	bson_error_t            error      = { 0 };
	DECLARE_RETURN_VALUE_USED

	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &zdocument) == FAILURE) {
		return;
	}

	if (return_value_used) {
		bson_flags |= PHONGO_BSON_RETURN_ID;
	}

	php_phongo_zval_to_bson(zdocument, bson_flags, &bdocument, &bson_out TSRMLS_CC);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!mongoc_bulk_operation_insert_with_opts(intern->bulk, &bdocument, &boptions, &error)) {
		phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
		goto cleanup;
	}

	intern->num_ops++;

	if (bson_out && return_value_used) {
		php_phongo_bulkwrite_extract_id(bson_out, &return_value);
	}

cleanup:
	bson_destroy(&bdocument);
	bson_destroy(&boptions);
	bson_clear(&bson_out);
} /* }}} */

/* {{{ proto void MongoDB\Driver\BulkWrite::update(array|object $query, array|object $newObj[, array $updateOptions = array()])
   Adds an update operation to the BulkWrite */
static PHP_METHOD(BulkWrite, update)
{
	php_phongo_bulkwrite_t* intern;
	zval *                  zquery, *zupdate, *zoptions = NULL;
	bson_t                  bquery = BSON_INITIALIZER, bupdate = BSON_INITIALIZER, boptions = BSON_INITIALIZER;
	bson_error_t            error = { 0 };

	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "AA|a!", &zquery, &zupdate, &zoptions) == FAILURE) {
		return;
	}

	php_phongo_zval_to_bson(zquery, PHONGO_BSON_NONE, &bquery, NULL TSRMLS_CC);

	if (EG(exception)) {
		goto cleanup;
	}

	php_phongo_zval_to_bson(zupdate, PHONGO_BSON_NONE, &bupdate, NULL TSRMLS_CC);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!php_phongo_bulkwrite_update_apply_options(&boptions, zoptions TSRMLS_CC)) {
		goto cleanup;
	}

	if (php_phongo_bulkwrite_update_has_operators(&bupdate) || php_phongo_bulkwrite_update_is_pipeline(&bupdate)) {
		if (zoptions && php_array_fetchc_bool(zoptions, "multi")) {
			if (!mongoc_bulk_operation_update_many_with_opts(intern->bulk, &bquery, &bupdate, &boptions, &error)) {
				phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
				goto cleanup;
			}
		} else {
			if (!mongoc_bulk_operation_update_one_with_opts(intern->bulk, &bquery, &bupdate, &boptions, &error)) {
				phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
				goto cleanup;
			}
		}
	} else {
		if (zoptions && php_array_fetchc_bool(zoptions, "multi")) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Replacement document conflicts with true \"multi\" option");
			goto cleanup;
		}

		if (!mongoc_bulk_operation_replace_one_with_opts(intern->bulk, &bquery, &bupdate, &boptions, &error)) {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			goto cleanup;
		}
	}

	intern->num_ops++;

cleanup:
	bson_destroy(&bquery);
	bson_destroy(&bupdate);
	bson_destroy(&boptions);
} /* }}} */

/* {{{ proto void MongoDB\Driver\BulkWrite::delete(array|object $query[, array $deleteOptions = array()])
   Adds a delete operation to the BulkWrite */
static PHP_METHOD(BulkWrite, delete)
{
	php_phongo_bulkwrite_t* intern;
	zval *                  zquery, *zoptions = NULL;
	bson_t                  bquery = BSON_INITIALIZER, boptions = BSON_INITIALIZER;
	bson_error_t            error = { 0 };

	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A|a!", &zquery, &zoptions) == FAILURE) {
		return;
	}

	php_phongo_zval_to_bson(zquery, PHONGO_BSON_NONE, &bquery, NULL TSRMLS_CC);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!php_phongo_bulkwrite_delete_apply_options(&boptions, zoptions TSRMLS_CC)) {
		goto cleanup;
	}

	if (zoptions && php_array_fetchc_bool(zoptions, "limit")) {
		if (!mongoc_bulk_operation_remove_one_with_opts(intern->bulk, &bquery, &boptions, &error)) {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			goto cleanup;
		}
	} else {
		if (!mongoc_bulk_operation_remove_many_with_opts(intern->bulk, &bquery, &boptions, &error)) {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			goto cleanup;
		}
	}

	intern->num_ops++;

cleanup:
	bson_destroy(&bquery);
	bson_destroy(&boptions);
} /* }}} */

/* {{{ proto integer MongoDB\Driver\BulkWrite::count()
   Returns the number of operations that have been added to the BulkWrite */
static PHP_METHOD(BulkWrite, count)
{
	php_phongo_bulkwrite_t* intern;

	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(intern->num_ops);
} /* }}} */

/* {{{ MongoDB\Driver\BulkWrite function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_BulkWrite___construct, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_BulkWrite_insert, 0, 0, 1)
	ZEND_ARG_INFO(0, document)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_BulkWrite_update, 0, 0, 2)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, newObj)
	ZEND_ARG_ARRAY_INFO(0, updateOptions, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_BulkWrite_delete, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_ARRAY_INFO(0, deleteOptions, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_BulkWrite_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_bulkwrite_me[] = {
	/* clang-format off */
	PHP_ME(BulkWrite, __construct, ai_BulkWrite___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(BulkWrite, insert, ai_BulkWrite_insert, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(BulkWrite, update, ai_BulkWrite_update, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(BulkWrite, delete, ai_BulkWrite_delete, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(BulkWrite, count, ai_BulkWrite_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_BulkWrite_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\BulkWrite object handlers */
static zend_object_handlers php_phongo_handler_bulkwrite;

static void php_phongo_bulkwrite_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_bulkwrite_t* intern = Z_OBJ_BULKWRITE(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->bulk) {
		mongoc_bulk_operation_destroy(intern->bulk);
	}

	if (intern->database) {
		efree(intern->database);
	}

	if (intern->collection) {
		efree(intern->collection);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_bulkwrite_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_bulkwrite_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_bulkwrite_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_bulkwrite;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_bulkwrite_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_bulkwrite;

		return retval;
	}
#endif
} /* }}} */

static HashTable* php_phongo_bulkwrite_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	zval                    retval = ZVAL_STATIC_INIT;
	php_phongo_bulkwrite_t* intern = NULL;

	*is_temp = 1;
	intern   = Z_BULKWRITE_OBJ_P(object);
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

	ADD_ASSOC_BOOL_EX(&retval, "executed", intern->executed);
	ADD_ASSOC_LONG_EX(&retval, "server_id", mongoc_bulk_operation_get_hint(intern->bulk));

	if (mongoc_bulk_operation_get_write_concern(intern->bulk)) {
#if PHP_VERSION_ID >= 70000
		zval write_concern;

		php_phongo_write_concern_to_zval(&write_concern, mongoc_bulk_operation_get_write_concern(intern->bulk));
		ADD_ASSOC_ZVAL_EX(&retval, "write_concern", &write_concern);
#else
		zval* write_concern = NULL;
		MAKE_STD_ZVAL(write_concern);

		php_phongo_write_concern_to_zval(write_concern, mongoc_bulk_operation_get_write_concern(intern->bulk));
		ADD_ASSOC_ZVAL_EX(&retval, "write_concern", write_concern);
#endif
	} else {
		ADD_ASSOC_NULL_EX(&retval, "write_concern");
	}

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_bulkwrite_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "BulkWrite", php_phongo_bulkwrite_me);
	php_phongo_bulkwrite_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_bulkwrite_ce->create_object = php_phongo_bulkwrite_create_object;
	PHONGO_CE_FINAL(php_phongo_bulkwrite_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_bulkwrite_ce);

	memcpy(&php_phongo_handler_bulkwrite, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_bulkwrite.get_debug_info = php_phongo_bulkwrite_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_bulkwrite.free_obj = php_phongo_bulkwrite_free_object;
	php_phongo_handler_bulkwrite.offset   = XtOffsetOf(php_phongo_bulkwrite_t, std);
#endif

	zend_class_implements(php_phongo_bulkwrite_ce TSRMLS_CC, 1, spl_ce_Countable);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
