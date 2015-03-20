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


PHONGO_API zend_class_entry *php_phongo_utcdatetime_ce;

/* {{{ proto MongoDB\Driver\UTCDatetime UTCDatetime::__construct(integer $milliseconds)
   Construct a new UTCDatetime */
PHP_METHOD(UTCDatetime, __construct)
{
	php_phongo_utcdatetime_t    *intern;
	zend_error_handling       error_handling;
	long                      milliseconds;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_utcdatetime_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &milliseconds) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	intern->milliseconds = milliseconds;
}
/* }}} */
/* {{{ proto string UTCDatetime::__toString()
   Returns the string representation of the UTCDatetime */
PHP_METHOD(UTCDatetime, __toString)
{
	php_phongo_utcdatetime_t    *intern;


	intern = (php_phongo_utcdatetime_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETVAL_LONG(intern->milliseconds);
	convert_to_string(return_value);
}
/* }}} */
/* {{{ proto string UTCDatetime::toDateTime()
   Returns DateTime object representing this UTCDateTime */
PHP_METHOD(UTCDatetime, toDateTime)
{
	php_phongo_utcdatetime_t    *intern;


	intern = (php_phongo_utcdatetime_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_new_datetime_from_utcdatetime(return_value, intern->milliseconds TSRMLS_CC);
}
/* }}} */


/* {{{ BSON\UTCDatetime */

ZEND_BEGIN_ARG_INFO_EX(ai_UTCDatetime___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, milliseconds)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_UTCDatetime___toString, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_UTCDatetime_toDateTime, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_utcdatetime_me[] = {
	PHP_ME(UTCDatetime, __construct, ai_UTCDatetime___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(UTCDatetime, __toString, ai_UTCDatetime___toString, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(UTCDatetime, toDateTime, ai_UTCDatetime_toDateTime, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_utcdatetime_t object handlers */
static void php_phongo_utcdatetime_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_utcdatetime_t *intern = (php_phongo_utcdatetime_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_utcdatetime_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_utcdatetime_t *intern;

	intern = (php_phongo_utcdatetime_t *)emalloc(sizeof(php_phongo_utcdatetime_t));
	memset(intern, 0, sizeof(php_phongo_utcdatetime_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_utcdatetime_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(UTCDatetime)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "BSON", "UTCDatetime", php_phongo_utcdatetime_me);
	ce.create_object = php_phongo_utcdatetime_create_object;
	php_phongo_utcdatetime_ce = zend_register_internal_class(&ce TSRMLS_CC);

	zend_class_implements(php_phongo_utcdatetime_ce TSRMLS_CC, 1, php_phongo_type_ce);


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
