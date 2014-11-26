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

/* We need to access the internal bulk structure to access the bulk op count */
#define MONGOC_I_AM_A_DRIVER
#include <mongoc-bulk-operation-private.h>
#undef MONGOC_I_AM_A_DRIVER

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
#include "php_array.h"


PHONGO_API zend_class_entry *php_phongo_writebatch_ce;

/* {{{ proto MongoDB\WriteBatch WriteBatch::__construct(boolean $ordered)
   Constructs a new WriteBatch */
PHP_METHOD(WriteBatch, __construct)
{
	php_phongo_writebatch_t  *intern;
	zend_error_handling       error_handling;
	zend_bool                 ordered = 1;
	(void)return_value_ptr; (void)return_value; (void)return_value_used;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writebatch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &ordered) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	intern->batch = phongo_writebatch_init(ordered);
}
/* }}} */
/* {{{ proto mixed WriteBatch::insert(array|object $document)
   Adds an insert operation to the batch */
PHP_METHOD(WriteBatch, insert)
{
	php_phongo_writebatch_t  *intern;
	zend_error_handling       error_handling;
	zval                     *document;
	bson_t                   *bson;
	bson_t                   *bson_out = NULL;
	int                       bson_flags = PHONGO_BSON_ADD_ID;
	(void)return_value_ptr; (void)return_value_used;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writebatch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &document) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	if (return_value_used) {
		bson_flags |= PHONGO_BSON_RETURN_ID;
	}

	bson = bson_new();
	zval_to_bson(document, bson_flags, bson, &bson_out TSRMLS_CC);
	mongoc_bulk_operation_insert(intern->batch, bson);
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
/* {{{ proto void WriteBatch::update(array|object $query, array|object $newObj[, array $updateOptions = array()])
   Adds an update operation to batch */
PHP_METHOD(WriteBatch, update)
{
	php_phongo_writebatch_t  *intern;
	zend_error_handling       error_handling;
	zval                     *query;
	zval                     *newObj;
	zval                     *updateOptions = NULL;
	mongoc_update_flags_t     limit = 0;
	zend_bool                 upsert = 0;
	bson_t                   *bquery;
	bson_t                   *bupdate;
	(void)return_value_ptr; (void)return_value; (void)return_value_used;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writebatch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "AA|a!", &query, &newObj, &updateOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	bquery = bson_new();
	bupdate = bson_new();

	zval_to_bson(query, PHONGO_BSON_NONE, bquery, NULL TSRMLS_CC);
	zval_to_bson(newObj, PHONGO_BSON_NONE, bupdate, NULL TSRMLS_CC);

	if (updateOptions) {
		limit = php_array_fetch_bool(updateOptions, "limit");
		upsert = php_array_fetch_bool(updateOptions, "upsert");
	}

	if (limit) {
		mongoc_bulk_operation_update_one(intern->batch, bquery, bupdate, upsert);
	} else {
		mongoc_bulk_operation_update(intern->batch, bquery, bupdate, upsert);
	}

	bson_clear(&bquery);
	bson_clear(&bupdate);
}
/* }}} */
/* {{{ proto void WriteBatch::delete(array|object $query[, array $deleteOptions = array()])
   Adds a delete operation to the batch */
PHP_METHOD(WriteBatch, delete)
{
	php_phongo_writebatch_t  *intern;
	zend_error_handling       error_handling;
	zval                     *query;
	zval                     *deleteOptions = NULL;
	bson_t                   *bson;
	(void)return_value_ptr; (void)return_value; (void)return_value_used;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writebatch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A|a!", &query, &deleteOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	bson = bson_new();
	zval_to_bson(query, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);

	if (deleteOptions && php_array_fetch_bool(deleteOptions, "limit")) {
		mongoc_bulk_operation_remove_one(intern->batch, bson);
	} else {
		mongoc_bulk_operation_remove(intern->batch, bson);
	}

	bson_clear(&bson);
}
/* }}} */
/* {{{ proto integer WriteBatch::count()
   Returns the number of operations that have been added to the batch */
PHP_METHOD(WriteBatch, count)
{
	php_phongo_writebatch_t  *intern;
	zend_error_handling       error_handling;
	(void)return_value_ptr; (void)return_value; (void)return_value_used;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writebatch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_LONG(intern->batch->commands.len);
}
/* }}} */


/* {{{ MongoDB\WriteBatch */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteBatch___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, ordered)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteBatch_insert, 0, 0, 1)
	ZEND_ARG_INFO(0, document)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteBatch_update, 0, 0, 2)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, newObj)
	ZEND_ARG_ARRAY_INFO(0, updateOptions, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteBatch_delete, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_ARRAY_INFO(0, deleteOptions, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteBatch_count, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_writebatch_me[] = {
	PHP_ME(WriteBatch, __construct, ai_WriteBatch___construct, ZEND_ACC_PUBLIC)
	PHP_ME(WriteBatch, insert, ai_WriteBatch_insert, ZEND_ACC_PUBLIC)
	PHP_ME(WriteBatch, update, ai_WriteBatch_update, ZEND_ACC_PUBLIC)
	PHP_ME(WriteBatch, delete, ai_WriteBatch_delete, ZEND_ACC_PUBLIC)
	PHP_ME(WriteBatch, count, ai_WriteBatch_count, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_writebatch_t object handlers */
static void php_phongo_writebatch_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_writebatch_t *intern = (php_phongo_writebatch_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->batch) {
		mongoc_bulk_operation_destroy(intern->batch);
	}
	efree(intern);
} /* }}} */

zend_object_value php_phongo_writebatch_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_writebatch_t *intern = NULL;

	intern = (php_phongo_writebatch_t *)emalloc(sizeof(php_phongo_writebatch_t));
	memset(intern, 0, sizeof(php_phongo_writebatch_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writebatch_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteBatch)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	(void)module_number; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB", "WriteBatch", php_phongo_writebatch_me);
	ce.create_object = php_phongo_writebatch_create_object;
	php_phongo_writebatch_ce = zend_register_internal_class(&ce TSRMLS_CC);

	zend_class_implements(php_phongo_writebatch_ce TSRMLS_CC, 1, spl_ce_Countable);


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
