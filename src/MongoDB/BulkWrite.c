/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2015 MongoDB, Inc.                                         |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2014-2015 MongoDB, Inc.                                     |
  +---------------------------------------------------------------------------+
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
	php_phongo_bulkwrite_t  *intern;
	zval                     *query;
	zval                     *newObj;
	zval                     *updateOptions = NULL;
	mongoc_update_flags_t     flags = MONGOC_UPDATE_NONE;
	bson_t                   *bquery;
	bson_t                   *bupdate;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "AA|a!", &query, &newObj, &updateOptions) == FAILURE) {
		return;
	}


	bquery = bson_new();
	bupdate = bson_new();

	phongo_zval_to_bson(query, PHONGO_BSON_NONE, bquery, NULL TSRMLS_CC);
	phongo_zval_to_bson(newObj, PHONGO_BSON_NONE, bupdate, NULL TSRMLS_CC);

	if (updateOptions) {
		flags |= php_array_fetchc_bool(updateOptions, "multi") ?  MONGOC_UPDATE_MULTI_UPDATE : 0;
		flags |= php_array_fetchc_bool(updateOptions, "upsert") ? MONGOC_UPDATE_UPSERT : 0;
	}

	if (flags & MONGOC_UPDATE_MULTI_UPDATE) {
		mongoc_bulk_operation_update(intern->bulk, bquery, bupdate, !!(flags & MONGOC_UPDATE_UPSERT));
	} else {
		bson_iter_t iter;
		zend_bool   replaced = 0;

		if (bson_iter_init(&iter, bupdate)) {
			while (bson_iter_next (&iter)) {
				if (!strchr (bson_iter_key (&iter), '$')) {
					mongoc_bulk_operation_replace_one(intern->bulk, bquery, bupdate, !!(flags & MONGOC_UPDATE_UPSERT));
					replaced = 1;
					break;
				}
			}
		}

		if (!replaced) {
			mongoc_bulk_operation_update_one(intern->bulk, bquery, bupdate, !!(flags & MONGOC_UPDATE_UPSERT));
		}
	}

	intern->num_ops++;

	bson_clear(&bquery);
	bson_clear(&bupdate);
}
/* }}} */

/* {{{ proto void BulkWrite::delete(array|object $query[, array $deleteOptions = array()])
   Adds a delete operation to the BulkWrite */
PHP_METHOD(BulkWrite, delete)
{
	php_phongo_bulkwrite_t  *intern;
	zval                     *query;
	zval                     *deleteOptions = NULL;
	bson_t                   *bson;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_BULKWRITE_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A|a!", &query, &deleteOptions) == FAILURE) {
		return;
	}


	bson = bson_new();
	phongo_zval_to_bson(query, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);

	if (deleteOptions && php_array_fetchc_bool(deleteOptions, "limit")) {
		mongoc_bulk_operation_remove_one(intern->bulk, bson);
	} else {
		mongoc_bulk_operation_remove(intern->bulk, bson);
	}

	intern->num_ops++;

	bson_clear(&bson);
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
