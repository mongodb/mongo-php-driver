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

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>
/* PHP array helpers */
#include "php_array_api.h"
/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"

#define BYPASS_UNSET -1

PHONGO_API zend_class_entry *php_phongo_bulkwrite_ce;

zend_object_handlers php_phongo_handler_bulkwrite;

/* Returns whether any top-level field names in the document contain a "$". */
static inline bool php_phongo_bulkwrite_update_has_operators(bson_t *bupdate) /* {{{ */
{
	bson_iter_t iter;

	if (bson_iter_init(&iter, bupdate)) {
		while (bson_iter_next (&iter)) {
			if (strchr(bson_iter_key(&iter), '$')) {
				return true;
			}
		}
	}

	return false;
} /* }}} */

/* Appends a document field for the given opts document and key. Returns true on
 * success; otherwise, false is returned and an exception is thrown. */
static bool php_phongo_bulkwrite_opts_append_document(bson_t *opts, const char *opts_key, zval *zarr, const char *zarr_key TSRMLS_DC)
{
	zval *value = php_array_fetch(zarr, zarr_key);
	bson_t b = BSON_INITIALIZER;

	if (Z_TYPE_P(value) != IS_OBJECT && Z_TYPE_P(value) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected \"%s\" option to be array or object, %s given", zarr_key, zend_get_type_by_const(Z_TYPE_P(value)));
		return false;
	}

	phongo_zval_to_bson(value, PHONGO_BSON_NONE, &b, NULL TSRMLS_CC);

	if (EG(exception)) {
		bson_destroy(&b);
		return false;
	}

	if (!BSON_APPEND_DOCUMENT(opts, opts_key, &b)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error appending \"%s\" option", opts_key);
		bson_destroy(&b);
		return false;
	}

	bson_destroy(&b);
	return true;
}

#define PHONGO_BULKWRITE_APPEND_BOOL(opt, value) \
	if (!BSON_APPEND_BOOL(boptions, (opt), (value))) { \
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error appending \"%s\" option", (opt)); \
		return false; \
	}

#define PHONGO_BULKWRITE_APPEND_INT32(opt, value) \
	if (!BSON_APPEND_INT32(boptions, (opt), (value))) { \
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error appending \"%s\" option", (opt)); \
		return false; \
	}

#define PHONGO_BULKWRITE_OPT_DOCUMENT(opt) \
	if (zoptions && php_array_existsc(zoptions, (opt))) { \
		if (!php_phongo_bulkwrite_opts_append_document(boptions, (opt), zoptions, (opt) TSRMLS_CC)) { \
			return false; \
		} \
	}

/* Applies options (including defaults) for an update operation. */
static bool php_phongo_bulkwrite_update_apply_options(bson_t *boptions, zval *zoptions TSRMLS_DC)/* {{{ */
{
	bool multi = false, upsert = false;

	if (zoptions) {
		if (php_array_existsc(zoptions, "multi")) {
			multi = php_array_fetchc_bool(zoptions, "multi");
		}
		if (php_array_existsc(zoptions, "upsert")) {
			upsert = php_array_fetchc_bool(zoptions, "upsert");
		}
	}

	PHONGO_BULKWRITE_APPEND_BOOL("multi", multi);
	PHONGO_BULKWRITE_APPEND_BOOL("upsert", upsert);
	PHONGO_BULKWRITE_OPT_DOCUMENT("collation");

	return true;
} /* }}} */

/* Applies options (including defaults) for an delete operation. */
static bool php_phongo_bulkwrite_delete_apply_options(bson_t *boptions, zval *zoptions TSRMLS_DC)/* {{{ */
{
	int32_t limit = 0;

	if (zoptions) {
		if (php_array_existsc(zoptions, "limit")) {
			limit = php_array_fetchc_bool(zoptions, "limit") ? 1 : 0;
		}
	}

	PHONGO_BULKWRITE_APPEND_INT32("limit", limit);
	PHONGO_BULKWRITE_OPT_DOCUMENT("collation");

	return true;
} /* }}} */

#undef PHONGO_BULKWRITE_APPEND_BOOL
#undef PHONGO_BULKWRITE_APPEND_INT32
#undef PHONGO_BULKWRITE_OPT_DOCUMENT

/* {{{ proto void BulkWrite::__construct([array $options = array()])
   Constructs a new BulkWrite */
PHP_METHOD(BulkWrite, __construct)
{
	php_phongo_bulkwrite_t  *intern;
	zend_error_handling      error_handling;
	zval                    *options = NULL;
	zend_bool                ordered = 1;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_used)


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

	intern->bulk = phongo_bulkwrite_init(ordered);
	intern->ordered = ordered;
	intern->bypass = BYPASS_UNSET;
	intern->num_ops = 0;

	if (options && php_array_existsc(options, "bypassDocumentValidation")) {
		zend_bool bypass = php_array_fetchc_bool(options, "bypassDocumentValidation");
		mongoc_bulk_operation_set_bypass_document_validation(intern->bulk, bypass);
		intern->bypass = bypass;
	}
}
/* }}} */

/* {{{ proto mixed BulkWrite::insert(array|object $document)
   Adds an insert operation to the BulkWrite */
PHP_METHOD(BulkWrite, insert)
{
	php_phongo_bulkwrite_t  *intern;
	zval                     *document;
	bson_t                   *bson;
	bson_t                   *bson_out = NULL;
	int                       bson_flags = PHONGO_BSON_ADD_ID;
	DECLARE_RETURN_VALUE_USED
	SUPPRESS_UNUSED_WARNING(return_value_ptr)


	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &document) == FAILURE) {
		return;
	}


	if (return_value_used) {
		bson_flags |= PHONGO_BSON_RETURN_ID;
	}

	bson = bson_new();
	phongo_zval_to_bson(document, bson_flags, bson, &bson_out TSRMLS_CC);
	mongoc_bulk_operation_insert(intern->bulk, bson);
	bson_clear(&bson);

	intern->num_ops++;

	if (bson_out && return_value_used) {
		bson_iter_t iter;

		if (bson_iter_init_find(&iter, bson_out, "_id")) {
			php_phongo_objectid_new_from_oid(return_value, bson_iter_oid(&iter) TSRMLS_CC);
			bson_clear(&bson_out);
			return;
		}

		bson_clear(&bson_out);
	}
}
/* }}} */

/* {{{ proto void BulkWrite::update(array|object $query, array|object $newObj[, array $updateOptions = array()])
   Adds an update operation to the BulkWrite */
PHP_METHOD(BulkWrite, update)
{
	php_phongo_bulkwrite_t *intern;
	zval                   *zquery, *zupdate, *zoptions = NULL;
	bson_t                 *bquery, *bupdate, *boptions = NULL;
	bson_error_t            error = {0};
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "AA|a!", &zquery, &zupdate, &zoptions) == FAILURE) {
		return;
	}

	bquery = bson_new();
	bupdate = bson_new();
	boptions = bson_new();

	phongo_zval_to_bson(zquery, PHONGO_BSON_NONE, bquery, NULL TSRMLS_CC);

	if (EG(exception)) {
		goto cleanup;
	}

	phongo_zval_to_bson(zupdate, PHONGO_BSON_NONE, bupdate, NULL TSRMLS_CC);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!php_phongo_bulkwrite_update_apply_options(boptions, zoptions TSRMLS_CC)) {
		goto cleanup;
	}

	if (php_phongo_bulkwrite_update_has_operators(bupdate)) {
		if (zoptions && php_array_existsc(zoptions, "multi") && php_array_fetchc_bool(zoptions, "multi")) {
			if (!mongoc_bulk_operation_update_many_with_opts(intern->bulk, bquery, bupdate, boptions, &error)) {
				phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
				goto cleanup;
			}
		} else {
			if (!mongoc_bulk_operation_update_one_with_opts(intern->bulk, bquery, bupdate, boptions, &error)) {
				phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
				goto cleanup;
			}
		}
	} else {
		if (!bson_validate(bupdate, BSON_VALIDATE_DOT_KEYS|BSON_VALIDATE_DOLLAR_KEYS, NULL)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Replacement document may not contain \"$\" or \".\" in keys");
			goto cleanup;
		}

		if (zoptions && php_array_existsc(zoptions, "multi") && php_array_fetchc_bool(zoptions, "multi")) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Replacement document conflicts with true \"multi\" option");
			goto cleanup;
		}

		if (!mongoc_bulk_operation_replace_one_with_opts(intern->bulk, bquery, bupdate, boptions, &error)) {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			goto cleanup;
		}
	}

	intern->num_ops++;

cleanup:
	bson_clear(&bquery);
	bson_clear(&bupdate);
	bson_clear(&boptions);
}
/* }}} */

/* {{{ proto void BulkWrite::delete(array|object $query[, array $deleteOptions = array()])
   Adds a delete operation to the BulkWrite */
PHP_METHOD(BulkWrite, delete)
{
	php_phongo_bulkwrite_t *intern;
	zval                   *zquery, *zoptions = NULL;
	bson_t                 *bquery, *boptions = NULL;
	bson_error_t            error = {0};
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A|a!", &zquery, &zoptions) == FAILURE) {
		return;
	}

	bquery = bson_new();
	boptions = bson_new();

	phongo_zval_to_bson(zquery, PHONGO_BSON_NONE, bquery, NULL TSRMLS_CC);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!php_phongo_bulkwrite_delete_apply_options(boptions, zoptions TSRMLS_CC)) {
		goto cleanup;
	}

	if (zoptions && php_array_existsc(zoptions, "limit") && php_array_fetchc_bool(zoptions, "limit")) {
		if (!mongoc_bulk_operation_remove_one_with_opts(intern->bulk, bquery, boptions, &error)) {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			goto cleanup;
		}
	} else {
		if (!mongoc_bulk_operation_remove_many_with_opts(intern->bulk, bquery, boptions, &error)) {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			goto cleanup;
		}
	}

	intern->num_ops++;

cleanup:
	bson_clear(&bquery);
	bson_clear(&boptions);
}
/* }}} */

/* {{{ proto integer BulkWrite::count()
   Returns the number of operations that have been added to the BulkWrite */
PHP_METHOD(BulkWrite, count)
{
	php_phongo_bulkwrite_t  *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(intern->num_ops);
}
/* }}} */


/* {{{ MongoDB\Driver\BulkWrite */

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
	PHP_ME(BulkWrite, __construct, ai_BulkWrite___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(BulkWrite, insert, ai_BulkWrite_insert, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(BulkWrite, update, ai_BulkWrite_update, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(BulkWrite, delete, ai_BulkWrite_delete, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(BulkWrite, count, ai_BulkWrite_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeup, ai_BulkWrite_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_bulkwrite_t object handlers */
static void php_phongo_bulkwrite_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_bulkwrite_t *intern = Z_OBJ_BULKWRITE(object);

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

phongo_create_object_retval php_phongo_bulkwrite_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_bulkwrite_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_bulkwrite_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_bulkwrite;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_bulkwrite_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_bulkwrite;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_bulkwrite_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval                      retval;
#else
	zval                      retval = zval_used_for_init;
#endif
	php_phongo_bulkwrite_t   *intern = NULL;


	*is_temp = 1;
	intern = Z_BULKWRITE_OBJ_P(object);
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

	if (intern->bypass != BYPASS_UNSET) {
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
		zval *write_concern = NULL;
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

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(BulkWrite)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "BulkWrite", php_phongo_bulkwrite_me);
	php_phongo_bulkwrite_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_bulkwrite_ce->create_object = php_phongo_bulkwrite_create_object;
	PHONGO_CE_FINAL(php_phongo_bulkwrite_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_bulkwrite_ce);

	memcpy(&php_phongo_handler_bulkwrite, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_bulkwrite.get_debug_info = php_phongo_bulkwrite_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_bulkwrite.free_obj = php_phongo_bulkwrite_free_object;
	php_phongo_handler_bulkwrite.offset = XtOffsetOf(php_phongo_bulkwrite_t, std);
#endif

	zend_class_implements(php_phongo_bulkwrite_ce TSRMLS_CC, 1, spl_ce_Countable);


	return SUCCESS;
}
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
