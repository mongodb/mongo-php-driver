/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2014 MongoDB, Inc.                                         |
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
  | Copyright (c) 2014, MongoDB, Inc.                                         |
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
/* Our Compatability header */
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_batch_ce;

/* {{{ proto MongoDB\Write\Batch Batch::__construct()
   Constructs a new CRUD Batch */
PHP_METHOD(Batch, __construct)
{
	php_phongo_batch_t    *intern;
	zend_error_handling	error_handling;
	zend_bool             ordered = true;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_batch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &ordered) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	intern->batch = phongo_batch_init(ordered);
}
/* }}} */
/* {{{ proto MongoDB\Write\Batch Batch::insert(array|object $document)
   Adds a new document to be inserted */
PHP_METHOD(Batch, insert)
{
	php_phongo_batch_t    *intern;
	zend_error_handling	   error_handling;
	zval                  *document;
	bson_t                *bson;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_batch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &document) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	bson = bson_new();
	php_phongo_bson_encode_array(bson, document TSRMLS_CC);
	mongoc_bulk_operation_insert(intern->batch, bson);
	bson_destroy(bson);
}
/* }}} */
/* {{{ proto MongoDB\Write\Batch Batch::update(array|object $query, array|object $update, integer $limit, boolean $upsert)
   Add a update operation to batch */
PHP_METHOD(Batch, update)
{
	php_phongo_batch_t    *intern;
	zend_error_handling	error_handling;
	zval                  *query;
	zval                  *update;
	long                   limit;
	zend_bool             upsert;
	bson_t                *bquery;
	bson_t                *bupdate;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_batch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "AAlb", &query, &update, &limit, &upsert) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	bquery = bson_new();
	bupdate = bson_new();

	php_phongo_bson_encode_array(bquery, query TSRMLS_CC);
	php_phongo_bson_encode_array(bupdate, update TSRMLS_CC);

	if (limit > 1) {
		mongoc_bulk_operation_update(intern->batch, bquery, bupdate, upsert);
	} else {
		mongoc_bulk_operation_update_one(intern->batch, bquery, bupdate, upsert);
	}

	bson_destroy(bquery);
	bson_destroy(bupdate);
}
/* }}} */
/* {{{ proto MongoDB\Write\Batch Batch::delete(array|object $query, boolean $limit)
   Add a delete operation to batch */
PHP_METHOD(Batch, delete)
{
	php_phongo_batch_t    *intern;
	zend_error_handling	error_handling;
	zval                  *query;
	zend_bool             limit;
	bson_t                *bson;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_batch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Ab", &query, &limit) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	bson = bson_new();

	php_phongo_bson_encode_array(bson, query TSRMLS_CC);

	if (limit) {
		mongoc_bulk_operation_remove_one(intern->batch, bson);
	} else {
		mongoc_bulk_operation_remove(intern->batch, bson);
	}

	bson_destroy(bson);
}
/* }}} */
/* {{{ proto integer Batch::count()
   Counts how many operations are in the patch */
PHP_METHOD(Batch, count)
{
	php_phongo_batch_t    *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_batch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */


/* {{{ MongoDB\Write\Batch */

ZEND_BEGIN_ARG_INFO_EX(ai_Batch___construct, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Batch_insert, 0, 0, 1)
	ZEND_ARG_INFO(0, document)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Batch_update, 0, 0, 4)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, update)
	ZEND_ARG_INFO(0, limit)
	ZEND_ARG_INFO(0, upsert)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Batch_delete, 0, 0, 2)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Batch_count, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_batch_me[] = {
	PHP_ME(Batch, __construct, ai_Batch___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Batch, insert, ai_Batch_insert, ZEND_ACC_PUBLIC)
	PHP_ME(Batch, update, ai_Batch_update, ZEND_ACC_PUBLIC)
	PHP_ME(Batch, delete, ai_Batch_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Batch, count, ai_Batch_count, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_batch_free_object && php_phongo_batch_create_object */
static void php_phongo_batch_free_object(void *object TSRMLS_DC)
{
	php_phongo_batch_t *intern = (php_phongo_batch_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
}

zend_object_value php_phongo_batch_create_object(zend_class_entry *class_type TSRMLS_DC)
{
	zend_object_value retval;
	php_phongo_batch_t *intern;

	intern = (php_phongo_batch_t *)emalloc(sizeof(php_phongo_batch_t));
	memset(intern, 0, sizeof(php_phongo_batch_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_batch_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Batch)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Write", "Batch", php_phongo_batch_me);
	ce.create_object = php_phongo_batch_create_object;
	php_phongo_batch_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(php_phongo_batch_ce TSRMLS_CC, 1, spl_ce_Countable);


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
