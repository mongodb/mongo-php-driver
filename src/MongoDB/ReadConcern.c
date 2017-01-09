/*
 * Copyright 2015-2017 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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


PHONGO_API zend_class_entry *php_phongo_readconcern_ce;

zend_object_handlers php_phongo_handler_readconcern;

/* {{{ proto void ReadConcern::__construct([string $level])
   Constructs a new ReadConcern */
PHP_METHOD(ReadConcern, __construct)
{
	php_phongo_readconcern_t *intern;
	zend_error_handling       error_handling;
	char                     *level = NULL;
	phongo_zpp_char_len       level_len = 0;


	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_used)


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_READCONCERN_OBJ_P(getThis());

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
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

	intern = Z_READCONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	level = mongoc_read_concern_get_level(intern->read_concern);

	if (level) {
		PHONGO_RETURN_STRING(level);
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ proto array ReadConcern::bsonSerialize()
   */
PHP_METHOD(ReadConcern, bsonSerialize)
{
	const mongoc_read_concern_t *read_concern = phongo_read_concern_from_zval(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_read_concern_to_zval(return_value, read_concern);
	convert_to_object(return_value);
}
/* }}} */

/**
 * Value object for read concern used in issuing read operations.
 */
/* {{{ MongoDB\Driver\ReadConcern */

ZEND_BEGIN_ARG_INFO_EX(ai_ReadConcern___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, level)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ReadConcern_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_readconcern_me[] = {
	PHP_ME(ReadConcern, __construct, ai_ReadConcern___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(ReadConcern, getLevel, ai_ReadConcern_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(ReadConcern, bsonSerialize, ai_ReadConcern_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_readconcern_t object handlers */
static void php_phongo_readconcern_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_readconcern_t *intern = Z_OBJ_READCONCERN(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->read_concern) {
		mongoc_read_concern_destroy(intern->read_concern);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_readconcern_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_readconcern_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_readconcern_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_readconcern;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_readconcern_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_readconcern;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_readconcern_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval retval;
#else
	zval retval = zval_used_for_init;
#endif
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
	PHONGO_CE_FINAL(php_phongo_readconcern_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_readconcern_ce);

	zend_class_implements(php_phongo_readconcern_ce TSRMLS_CC, 1, php_phongo_serializable_ce);

	memcpy(&php_phongo_handler_readconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_readconcern.get_debug_info = php_phongo_readconcern_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_readconcern.free_obj = php_phongo_readconcern_free_object;
	php_phongo_handler_readconcern.offset = XtOffsetOf(php_phongo_readconcern_t, std);
#endif

	zend_declare_class_constant_stringl(php_phongo_readconcern_ce, ZEND_STRL("LOCAL"), ZEND_STRL(MONGOC_READ_CONCERN_LEVEL_LOCAL) TSRMLS_CC);
	zend_declare_class_constant_stringl(php_phongo_readconcern_ce, ZEND_STRL("MAJORITY"), ZEND_STRL(MONGOC_READ_CONCERN_LEVEL_MAJORITY) TSRMLS_CC);
	zend_declare_class_constant_stringl(php_phongo_readconcern_ce, ZEND_STRL("LINEARIZABLE"), ZEND_STRL(MONGOC_READ_CONCERN_LEVEL_LINEARIZABLE) TSRMLS_CC);

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
