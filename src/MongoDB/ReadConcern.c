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
#include <mongoc-read-concern-private.h>

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


PHONGO_API zend_class_entry *php_phongo_readconcern_ce;

zend_object_handlers php_phongo_handler_readconcern;

/* {{{ proto MongoDB\Driver\ReadConcern ReadConcern::__construct([string $level])
   Constructs a new ReadConcern */
PHP_METHOD(ReadConcern, __construct)
{
	php_phongo_readconcern_t *intern;
	zend_error_handling       error_handling;
	char                     *level = NULL;
	int                       level_len = 0;


	(void)return_value; (void)return_value_ptr; (void)return_value_used;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_readconcern_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s!", &level, &level_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	intern->read_concern = mongoc_read_concern_new();

	if (level) {
		mongoc_read_concern_set_level(intern->read_concern, level);
	}
}
/* }}} */

/* {{{ proto string|null ReadConcern::getLevel()
   Returns the ReadConcern "level" option */
PHP_METHOD(ReadConcern, getLevel)
{
	php_phongo_readconcern_t *intern;
	const char *level;
	(void)return_value_ptr; (void)return_value_used;

	intern = (php_phongo_readconcern_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	level = mongoc_read_concern_get_level(intern->read_concern);

	if (level) {
		RETURN_STRING(level, 1);
	}

	RETURN_NULL();
}
/* }}} */

/**
 * Value object for read concern used in issuing read operations.
 */
/* {{{ MongoDB\Driver\ReadConcern */

ZEND_BEGIN_ARG_INFO_EX(ai_ReadConcern___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, level)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_ReadConcern_getLevel, 0, 0, 0)
ZEND_END_ARG_INFO();

static zend_function_entry php_phongo_readconcern_me[] = {
	PHP_ME(ReadConcern, __construct, ai_ReadConcern___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(ReadConcern, getLevel, ai_ReadConcern_getLevel, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_readconcern_t object handlers */
static void php_phongo_readconcern_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_readconcern_t *intern = (php_phongo_readconcern_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->read_concern) {
		mongoc_read_concern_destroy(intern->read_concern);
	}
	efree(intern);
} /* }}} */

zend_object_value php_phongo_readconcern_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_readconcern_t *intern = NULL;

	intern = (php_phongo_readconcern_t *)ecalloc(1, sizeof *intern);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_readconcern_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_readconcern;

	return retval;
} /* }}} */

HashTable *php_phongo_readconcern_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	zval retval = zval_used_for_init;
	const mongoc_read_concern_t *read_concern = phongo_read_concern_from_zval(object TSRMLS_CC);


	*is_temp = 1;
	php_phongo_read_concern_to_zval(&retval, read_concern);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(ReadConcern)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ReadConcern", php_phongo_readconcern_me);
	php_phongo_readconcern_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_readconcern_ce->create_object = php_phongo_readconcern_create_object;
	PHONGO_CE_INIT(php_phongo_readconcern_ce);

	memcpy(&php_phongo_handler_readconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_readconcern.get_debug_info = php_phongo_readconcern_get_debug_info;

	zend_declare_class_constant_stringl(php_phongo_readconcern_ce, ZEND_STRL("LOCAL"), ZEND_STRL(MONGOC_READ_CONCERN_LEVEL_LOCAL) TSRMLS_CC);
	zend_declare_class_constant_stringl(php_phongo_readconcern_ce, ZEND_STRL("MAJORITY"), ZEND_STRL(MONGOC_READ_CONCERN_LEVEL_MAJORITY) TSRMLS_CC);

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
