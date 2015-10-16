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

/* We need to access the internal bulk structure to access the bulk op count */
#include <mongoc-bulk-operation-private.h>

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


PHONGO_API zend_class_entry *php_phongo_bulkwrite_ce;

zend_object_handlers php_phongo_handler_bulkwrite;

/* {{{ proto MongoDB\Driver\BulkWrite BulkWrite::__construct([array $options = array()])
   Constructs a new BulkWrite */
PHP_METHOD(BulkWrite, __construct)
{
	php_phongo_bulkwrite_t  *intern;
	zend_error_handling      error_handling;
	zval                    *options = NULL;
	zend_bool                ordered = 1;
	(void)return_value_ptr; (void)return_value; (void)return_value_used;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_bulkwrite_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a!", &options) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (options && php_array_exists(options, "ordered")) {
		ordered = php_array_fetch_bool(options, "ordered");
	}

	intern->bulk = phongo_bulkwrite_init(ordered);
}
/* }}} */
/* {{{ proto mixed BulkWrite::insert(array|object $document)
   Adds an insert operation to the bulk */
PHP_METHOD(BulkWrite, insert)
{
	php_phongo_bulkwrite_t  *intern;
	zval                     *document;
	bson_t                   *bson;
	bson_t                   *bson_out = NULL;
	int                       bson_flags = PHONGO_BSON_ADD_ID;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_bulkwrite_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &document) == FAILURE) {
		return;
	}


	if (return_value_used) {
		bson_flags |= PHONGO_BSON_RETURN_ID;
	}
	bson_flags |= PHONGO_BSON_ADD_ODS|PHONGO_BSON_ADD_CHILD_ODS;

	bson = bson_new();
	zval_to_bson(document, bson_flags, bson, &bson_out TSRMLS_CC);
	mongoc_bulk_operation_insert(intern->bulk, bson);
	bson_clear(&bson);

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
   Adds an update operation to bulk */
PHP_METHOD(BulkWrite, update)
{
	php_phongo_bulkwrite_t  *intern;
	zval                     *query;
	zval                     *newObj;
	zval                     *updateOptions = NULL;
	mongoc_update_flags_t     flags = MONGOC_UPDATE_NONE;
	bson_t                   *bquery;
	bson_t                   *bupdate;
	(void)return_value_ptr; (void)return_value; (void)return_value_used;


	intern = (php_phongo_bulkwrite_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "AA|a!", &query, &newObj, &updateOptions) == FAILURE) {
		return;
	}


	bquery = bson_new();
	bupdate = bson_new();

	zval_to_bson(query, PHONGO_BSON_NONE, bquery, NULL TSRMLS_CC);
	zval_to_bson(newObj, PHONGO_BSON_NONE, bupdate, NULL TSRMLS_CC);

	if (updateOptions) {
		flags |= php_array_fetch_bool(updateOptions, "multi") ?  MONGOC_UPDATE_MULTI_UPDATE : 0;
		flags |= php_array_fetch_bool(updateOptions, "upsert") ? MONGOC_UPDATE_UPSERT : 0;
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

	bson_clear(&bquery);
	bson_clear(&bupdate);
}
/* }}} */
/* {{{ proto void BulkWrite::delete(array|object $query[, array $deleteOptions = array()])
   Adds a delete operation to the bulk */
PHP_METHOD(BulkWrite, delete)
{
	php_phongo_bulkwrite_t  *intern;
	zval                     *query;
	zval                     *deleteOptions = NULL;
	bson_t                   *bson;
	(void)return_value_ptr; (void)return_value; (void)return_value_used;


	intern = (php_phongo_bulkwrite_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A|a!", &query, &deleteOptions) == FAILURE) {
		return;
	}


	bson = bson_new();
	zval_to_bson(query, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);

	if (deleteOptions && php_array_fetch_bool(deleteOptions, "limit")) {
		mongoc_bulk_operation_remove_one(intern->bulk, bson);
	} else {
		mongoc_bulk_operation_remove(intern->bulk, bson);
	}

	bson_clear(&bson);
}
/* }}} */
/* {{{ proto integer BulkWrite::count()
   Returns the number of operations that have been added to the bulk */
PHP_METHOD(BulkWrite, count)
{
	php_phongo_bulkwrite_t  *intern;
	(void)return_value_ptr; (void)return_value; (void)return_value_used;


	intern = (php_phongo_bulkwrite_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(intern->bulk->commands.len);
}
/* }}} */


/* {{{ MongoDB\Driver\BulkWrite */

ZEND_BEGIN_ARG_INFO_EX(ai_BulkWrite___construct, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_BulkWrite_insert, 0, 0, 1)
	ZEND_ARG_INFO(0, document)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_BulkWrite_update, 0, 0, 2)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, newObj)
	ZEND_ARG_ARRAY_INFO(0, updateOptions, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_BulkWrite_delete, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_ARRAY_INFO(0, deleteOptions, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_BulkWrite_count, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_bulkwrite_me[] = {
	PHP_ME(BulkWrite, __construct, ai_BulkWrite___construct, ZEND_ACC_PUBLIC)
	PHP_ME(BulkWrite, insert, ai_BulkWrite_insert, ZEND_ACC_PUBLIC)
	PHP_ME(BulkWrite, update, ai_BulkWrite_update, ZEND_ACC_PUBLIC)
	PHP_ME(BulkWrite, delete, ai_BulkWrite_delete, ZEND_ACC_PUBLIC)
	PHP_ME(BulkWrite, count, ai_BulkWrite_count, ZEND_ACC_PUBLIC)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_bulkwrite_t object handlers */
static void php_phongo_bulkwrite_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_bulkwrite_t *intern = (php_phongo_bulkwrite_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->bulk) {
		mongoc_bulk_operation_destroy(intern->bulk);
	}
	efree(intern);
} /* }}} */

zend_object_value php_phongo_bulkwrite_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_bulkwrite_t *intern = NULL;

	intern = (php_phongo_bulkwrite_t *)ecalloc(1, sizeof *intern);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_bulkwrite_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_bulkwrite;

	return retval;
} /* }}} */

HashTable *php_phongo_bulkwrite_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	zval                     retval = zval_used_for_init;
	php_phongo_bulkwrite_t *intern = NULL;


	*is_temp = 1;
	intern = (php_phongo_bulkwrite_t *)zend_object_store_get_object(object TSRMLS_CC);
	array_init(&retval);

	if (intern->bulk->database) {
		add_assoc_string_ex(&retval, ZEND_STRS("database"), intern->bulk->database, 1);
	} else {
		add_assoc_null_ex(&retval, ZEND_STRS("database"));
	}

	if (intern->bulk->collection) {
		add_assoc_string_ex(&retval, ZEND_STRS("collection"), intern->bulk->collection, 1);
	} else {
		add_assoc_null_ex(&retval, ZEND_STRS("collection"));
	}

	add_assoc_bool_ex(&retval, ZEND_STRS("ordered"), intern->bulk->ordered);
	add_assoc_bool_ex(&retval, ZEND_STRS("executed"), intern->bulk->executed);
	add_assoc_long_ex(&retval, ZEND_STRS("server_id"), intern->bulk->hint);

	if (intern->bulk->write_concern) {
		zval *write_concern = NULL;
		MAKE_STD_ZVAL(write_concern);

		php_phongo_write_concern_to_zval(write_concern, intern->bulk->write_concern);
		add_assoc_zval_ex(&retval, ZEND_STRS("write_concern"), write_concern);
	} else {
		add_assoc_null_ex(&retval, ZEND_STRS("write_concern"));
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
	PHONGO_CE_INIT(php_phongo_bulkwrite_ce);

	memcpy(&php_phongo_handler_bulkwrite, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_bulkwrite.get_debug_info = php_phongo_bulkwrite_get_debug_info;

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
