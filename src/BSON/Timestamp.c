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
/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_timestamp_ce;

zend_object_handlers php_phongo_handler_timestamp;

/* {{{ proto BSON\Timestamp Timestamp::__construct(integer $increment, int $timestamp)
   Construct a new BSON Timestamp (4bytes increment, 4bytes timestamp) */
PHP_METHOD(Timestamp, __construct)
{
	php_phongo_timestamp_t    *intern;
	zend_error_handling       error_handling;
	long                      increment;
	long                      timestamp;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_timestamp_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &increment, &timestamp) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	intern->increment = increment;
	intern->timestamp = timestamp;
}
/* }}} */
/* {{{ proto string Timestamp::__toString()
   Returns [increment:timestamp] */
PHP_METHOD(Timestamp, __toString)
{
	php_phongo_timestamp_t    *intern;
	char                      *retval;
	int                        retval_len;


	intern = (php_phongo_timestamp_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	retval_len = spprintf(&retval, 0, "[%d:%d]", intern->increment, intern->timestamp);
	RETVAL_STRINGL(retval, retval_len, 0);
}
/* }}} */

/* {{{ BSON\Timestamp */

ZEND_BEGIN_ARG_INFO_EX(ai_Timestamp___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, increment)
	ZEND_ARG_INFO(0, timestamp)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Timestamp___toString, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_timestamp_me[] = {
	PHP_ME(Timestamp, __construct, ai_Timestamp___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Timestamp, __toString, ai_Timestamp___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_timestamp_t object handlers */
static void php_phongo_timestamp_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_timestamp_t *intern = (php_phongo_timestamp_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_timestamp_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_timestamp_t *intern;

	intern = (php_phongo_timestamp_t *)emalloc(sizeof(php_phongo_timestamp_t));
	memset(intern, 0, sizeof(php_phongo_timestamp_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_timestamp_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_timestamp;

	return retval;
} /* }}} */

HashTable *php_phongo_timestamp_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_timestamp_t *intern;
	zval                    retval = zval_used_for_init;


	*is_temp = 1;
	intern = (php_phongo_timestamp_t *)zend_object_store_get_object(object TSRMLS_CC);

	array_init(&retval);

	add_assoc_long_ex(&retval, ZEND_STRS("increment"), intern->increment);
	add_assoc_long_ex(&retval, ZEND_STRS("timestamp"), intern->timestamp);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Timestamp)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, BSON_NAMESPACE, "Timestamp", php_phongo_timestamp_me);
	php_phongo_timestamp_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_timestamp_ce->create_object = php_phongo_timestamp_create_object;
	PHONGO_CE_INIT(php_phongo_timestamp_ce);

	zend_class_implements(php_phongo_timestamp_ce TSRMLS_CC, 1, php_phongo_type_ce);

	memcpy(&php_phongo_handler_timestamp, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_timestamp.get_debug_info = php_phongo_timestamp_get_debug_info;


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
