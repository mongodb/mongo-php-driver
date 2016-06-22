/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2016 MongoDB, Inc.                                         |
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
  | Copyright (c) 2014-2016 MongoDB, Inc.                                     |
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


PHONGO_API zend_class_entry *php_phongo_decimal128_ce;

zend_object_handlers php_phongo_handler_decimal128;

/* {{{ proto BSON\Decimal128 Decimal128::__construct(string $value)
   Construct a new BSON Decimal128 type */
PHP_METHOD(Decimal128, __construct)
{
	php_phongo_decimal128_t *intern;
	zend_error_handling      error_handling;
	char                    *value;
	phongo_zpp_char_len      value_len;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_DECIMAL128_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if ( ! bson_decimal128_from_string(value, &intern->decimal)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error parsing decimal string: %s", value);
		return;
	}
}
/* }}} */

/* {{{ proto void Decimal128::__toString()
    */
PHP_METHOD(Decimal128, __toString)
{
	php_phongo_decimal128_t *intern;
	char                     outbuf[BSON_DECIMAL128_STRING];

	intern = Z_DECIMAL128_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	bson_decimal128_to_string(&intern->decimal, outbuf);

	PHONGO_RETURN_STRING(outbuf);
}
/* }}} */


/* {{{ BSON\Decimal128 */

ZEND_BEGIN_ARG_INFO_EX(ai_Decimal128___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Decimal128___toString, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_decimal128_me[] = {
	PHP_ME(Decimal128, __construct, ai_Decimal128___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Decimal128, __toString, ai_Decimal128___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_decimal128_t object handlers */
static void php_phongo_decimal128_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_decimal128_t *intern = Z_OBJ_DECIMAL128(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_decimal128_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_decimal128_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_decimal128_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_decimal128;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_decimal128_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_decimal128;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_decimal128_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_decimal128_t *intern;
	char                  outbuf[BSON_DECIMAL128_STRING] = "";
#if PHP_VERSION_ID >= 70000
	zval                  retval;
#else
	zval                  retval = zval_used_for_init;
#endif

	intern = Z_DECIMAL128_OBJ_P(object);
	*is_temp = 1;
	array_init_size(&retval, 1);

	bson_decimal128_to_string(&intern->decimal, outbuf);
	ADD_ASSOC_STRING(&retval, "dec", outbuf);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Decimal128)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, BSON_NAMESPACE, "Decimal128", php_phongo_decimal128_me);
	php_phongo_decimal128_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_decimal128_ce->create_object = php_phongo_decimal128_create_object;
	PHONGO_CE_INIT(php_phongo_decimal128_ce);

	zend_class_implements(php_phongo_decimal128_ce TSRMLS_CC, 1, php_phongo_type_ce);

	memcpy(&php_phongo_handler_decimal128, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_decimal128.get_debug_info = php_phongo_decimal128_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_decimal128.free_obj = php_phongo_decimal128_free_object;
	php_phongo_handler_decimal128.offset = XtOffsetOf(php_phongo_decimal128_t, std);
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
