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


PHONGO_API zend_class_entry *php_phongo_updatebatch_ce;

/* {{{ proto MongoDB\Write\UpdateBatch UpdateBatch::add(array|object $document)
   Adds a new operation to be executed */
PHP_METHOD(UpdateBatch, add)
{
	php_phongo_updatebatch_t *intern;
	zend_error_handling	error_handling;
	zval                  *document;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_updatebatch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &document) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto int UpdateBatch::count()
   Returns how many items have been added to this batch */
PHP_METHOD(UpdateBatch, count)
{
	php_phongo_updatebatch_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_updatebatch_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */

/**
 * Aggregates a collection of update operations, to be executed in batches.
 */
/* {{{ MongoDB\Write\UpdateBatch */

ZEND_BEGIN_ARG_INFO_EX(ai_UpdateBatch_add, 0, 0, 1)
	ZEND_ARG_INFO(0, document) 
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_UpdateBatch_count, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_updatebatch_me[] = {
	PHP_ME(UpdateBatch, add, ai_UpdateBatch_add, ZEND_ACC_PUBLIC)
	PHP_ME(UpdateBatch, count, ai_UpdateBatch_count, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_updatebatch_free_object && php_phongo_updatebatch_create_object */
static void php_phongo_updatebatch_free_object(void *object TSRMLS_DC)
{
	php_phongo_updatebatch_t *intern = (php_phongo_updatebatch_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
}

zend_object_value php_phongo_updatebatch_create_object(zend_class_entry *class_type TSRMLS_DC)
{
	zend_object_value retval;
	php_phongo_updatebatch_t *intern;

	intern = (php_phongo_updatebatch_t *)emalloc(sizeof(php_phongo_updatebatch_t));
	memset(intern, 0, sizeof(php_phongo_updatebatch_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_updatebatch_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(UpdateBatch)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Write", "UpdateBatch", php_phongo_updatebatch_me);
	ce.create_object = php_phongo_updatebatch_create_object;
	php_phongo_updatebatch_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(php_phongo_updatebatch_ce TSRMLS_CC, 1, php_phongo_writebatch_ce);


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
