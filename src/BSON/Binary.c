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


PHONGO_API zend_class_entry *php_phongo_binary_ce;

zend_object_handlers php_phongo_handler_binary;

/* {{{ proto BSON\Binary Binary::__construct(string $data, int $type)
   Construct a new BSON Binary type */
PHP_METHOD(Binary, __construct)
{
	php_phongo_binary_t    *intern;
	zend_error_handling     error_handling;
	char                   *data;
	int                     data_len;
	long                    type;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_binary_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &data, &data_len, &type) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	intern->data = estrndup(data, data_len);
	intern->data_len = data_len;
	intern->type = type;
}
/* }}} */
/* {{{ proto string Binary::getData()
    */
PHP_METHOD(Binary, getData)
{
	php_phongo_binary_t      *intern;


	intern = (php_phongo_binary_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_STRINGL(intern->data, intern->data_len, 1);
}
/* }}} */
/* {{{ proto integer Binary::getType()
    */
PHP_METHOD(Binary, getType)
{
	php_phongo_binary_t      *intern;


	intern = (php_phongo_binary_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(intern->type);
}
/* }}} */


/* {{{ BSON\Binary */

ZEND_BEGIN_ARG_INFO_EX(ai_Binary___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Binary_getData, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Binary_getType, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_binary_me[] = {
	PHP_ME(Binary, __construct, ai_Binary___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Binary, getData, ai_Binary_getData, ZEND_ACC_PUBLIC)
	PHP_ME(Binary, getType, ai_Binary_getType, ZEND_ACC_PUBLIC)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_binary_t object handlers */
static void php_phongo_binary_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_binary_t *intern = (php_phongo_binary_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->data) {
		efree(intern->data);
	}
	efree(intern);
} /* }}} */

zend_object_value php_phongo_binary_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_binary_t *intern;

	intern = (php_phongo_binary_t *)emalloc(sizeof(php_phongo_binary_t));
	memset(intern, 0, sizeof(php_phongo_binary_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_binary_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_binary;

	return retval;
} /* }}} */

HashTable *php_phongo_binary_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_binary_t *intern;
	zval                 retval = zval_used_for_init;

	intern = (php_phongo_binary_t *)zend_object_store_get_object(object TSRMLS_CC);
	*is_temp = 1;
	array_init_size(&retval, 2);

	add_assoc_stringl_ex(&retval, ZEND_STRS("data"), intern->data, intern->data_len, 1);
	add_assoc_long_ex(&retval, ZEND_STRS("type"), intern->type);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Binary)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, BSON_NAMESPACE, "Binary", php_phongo_binary_me);
	php_phongo_binary_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_binary_ce->create_object = php_phongo_binary_create_object;
	PHONGO_CE_INIT(php_phongo_binary_ce);

	zend_class_implements(php_phongo_binary_ce TSRMLS_CC, 1, php_phongo_type_ce);

	memcpy(&php_phongo_handler_binary, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_binary.get_debug_info = php_phongo_binary_get_debug_info;

	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_GENERIC"), BSON_SUBTYPE_BINARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_FUNCTION"), BSON_SUBTYPE_FUNCTION TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_OLD_BINARY"), BSON_SUBTYPE_BINARY_DEPRECATED TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_OLD_UUID"), BSON_SUBTYPE_UUID_DEPRECATED TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_UUID"), BSON_SUBTYPE_UUID TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_MD5"), BSON_SUBTYPE_MD5 TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_USER_DEFINED"), BSON_SUBTYPE_USER TSRMLS_CC);

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
