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

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_timestamp_init(php_phongo_timestamp_t *intern, phongo_long increment, phongo_long timestamp TSRMLS_DC)
{
	if (increment < 0 || increment > UINT32_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected increment to be an unsigned 32-bit integer, %" PHONGO_LONG_FORMAT " given", increment);
		return false;
	}

	if (timestamp < 0 || timestamp > UINT32_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected timestamp to be an unsigned 32-bit integer, %" PHONGO_LONG_FORMAT " given", timestamp);
		return false;
	}

	intern->increment = increment;
	intern->timestamp = timestamp;
	intern->initialized = true;

	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_timestamp_init_from_hash(php_phongo_timestamp_t *intern, HashTable *props TSRMLS_DC)
{
#if PHP_VERSION_ID >= 70000
	zval *increment, *timestamp;

	if ((increment = zend_hash_str_find(props, "increment", sizeof("increment")-1)) && Z_TYPE_P(increment) == IS_LONG &&
	    (timestamp = zend_hash_str_find(props, "timestamp", sizeof("timestamp")-1)) && Z_TYPE_P(timestamp) == IS_LONG) {
		return php_phongo_timestamp_init(intern, Z_LVAL_P(increment), Z_LVAL_P(timestamp) TSRMLS_CC);
	}
#else
	zval **increment, **timestamp;

	if (zend_hash_find(props, "increment", sizeof("increment"), (void**) &increment) == SUCCESS && Z_TYPE_PP(increment) == IS_LONG &&
	    zend_hash_find(props, "timestamp", sizeof("timestamp"), (void**) &timestamp) == SUCCESS && Z_TYPE_PP(timestamp) == IS_LONG) {
		return php_phongo_timestamp_init(intern, Z_LVAL_PP(increment), Z_LVAL_PP(timestamp) TSRMLS_CC);
	}
#endif

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"increment\" and \"timestamp\" integer fields", ZSTR_VAL(php_phongo_timestamp_ce->name));
	return false;
}

/* {{{ proto BSON\Timestamp Timestamp::__construct(integer $increment, int $timestamp)
   Construct a new BSON Timestamp (4bytes increment, 4bytes timestamp) */
PHP_METHOD(Timestamp, __construct)
{
	php_phongo_timestamp_t    *intern;
	zend_error_handling       error_handling;
	phongo_long               increment;
	phongo_long               timestamp;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_TIMESTAMP_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &increment, &timestamp) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	php_phongo_timestamp_init(intern, increment, timestamp TSRMLS_CC);
}
/* }}} */

/* {{{ proto Timestamp::__set_state(array $properties)
*/
PHP_METHOD(Timestamp, __set_state)
{
	php_phongo_timestamp_t *intern;
	HashTable              *props;
	zval                   *array;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_timestamp_ce);

	intern = Z_TIMESTAMP_OBJ_P(return_value);
	props = Z_ARRVAL_P(array);

	php_phongo_timestamp_init_from_hash(intern, props TSRMLS_CC);
}
/* }}} */

/* {{{ proto string Timestamp::__toString()
   Returns [increment:timestamp] */
PHP_METHOD(Timestamp, __toString)
{
	php_phongo_timestamp_t    *intern;
	char                      *retval;
	int                        retval_len;


	intern = Z_TIMESTAMP_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	retval_len = spprintf(&retval, 0, "[%" PRIu32 ":%" PRIu32 "]", intern->increment, intern->timestamp);
	PHONGO_RETVAL_STRINGL(retval, retval_len);
	efree(retval);
}
/* }}} */

/* {{{ proto Timestamp::__wakeup()
*/
PHP_METHOD(Timestamp, __wakeup)
{
	php_phongo_timestamp_t *intern;
	HashTable              *props;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_TIMESTAMP_OBJ_P(getThis());
	props = zend_std_get_properties(getThis() TSRMLS_CC);

	php_phongo_timestamp_init_from_hash(intern, props TSRMLS_CC);
}
/* }}} */

/* {{{ BSON\Timestamp */

ZEND_BEGIN_ARG_INFO_EX(ai_Timestamp___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, increment)
	ZEND_ARG_INFO(0, timestamp)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Timestamp___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Timestamp_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_timestamp_me[] = {
	PHP_ME(Timestamp, __construct, ai_Timestamp___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Timestamp, __set_state, ai_Timestamp___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Timestamp, __toString, ai_Timestamp_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Timestamp, __wakeup, ai_Timestamp_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_timestamp_t object handlers */
static void php_phongo_timestamp_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_timestamp_t *intern = Z_OBJ_TIMESTAMP(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_timestamp_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_timestamp_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_timestamp_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_timestamp;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_timestamp_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_timestamp;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_timestamp_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_timestamp_t *intern;
#if PHP_VERSION_ID >= 70000
	zval                    retval;
#else
	zval                    retval = zval_used_for_init;
#endif


	*is_temp = 1;
	intern =  Z_TIMESTAMP_OBJ_P(object);

	array_init(&retval);

	ADD_ASSOC_LONG_EX(&retval, "increment", intern->increment);
	ADD_ASSOC_LONG_EX(&retval, "timestamp", intern->timestamp);

	return Z_ARRVAL(retval);
} /* }}} */

HashTable *php_phongo_timestamp_get_properties(zval *object TSRMLS_DC) /* {{{ */
{
	php_phongo_timestamp_t *intern;
	HashTable              *props;

	intern = Z_TIMESTAMP_OBJ_P(object);
	props = zend_std_get_properties(object TSRMLS_CC);

	if (!intern->initialized) {
		return props;
	}

#if PHP_VERSION_ID >= 70000
	{
		zval increment, timestamp;

		ZVAL_LONG(&increment, intern->increment);
		zend_hash_str_update(props, "increment", sizeof("increment")-1, &increment);

		ZVAL_LONG(&timestamp, intern->timestamp);
		zend_hash_str_update(props, "timestamp", sizeof("timestamp")-1, &timestamp);
	}
#else
	{
		zval *increment, *timestamp;

		MAKE_STD_ZVAL(increment);
		ZVAL_LONG(increment, intern->increment);
		zend_hash_update(props, "increment", sizeof("increment"), &increment, sizeof(increment), NULL);

		MAKE_STD_ZVAL(timestamp);
		ZVAL_LONG(timestamp, intern->timestamp);
		zend_hash_update(props, "timestamp", sizeof("timestamp"), &timestamp, sizeof(timestamp), NULL);
	}
#endif

	return props;
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
	PHONGO_CE_FINAL(php_phongo_timestamp_ce);

	zend_class_implements(php_phongo_timestamp_ce TSRMLS_CC, 1, php_phongo_type_ce);
	memcpy(&php_phongo_handler_timestamp, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_timestamp.get_properties = php_phongo_timestamp_get_properties;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_timestamp.free_obj = php_phongo_timestamp_free_object;
	php_phongo_handler_timestamp.offset = XtOffsetOf(php_phongo_timestamp_t, std);
#endif

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
