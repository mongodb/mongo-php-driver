/*
 * Copyright 2014-2017 MongoDB, Inc.
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

#include <php.h>
#include <Zend/zend_interfaces.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_writeconcernerror_ce;

/* {{{ proto integer MongoDB\Driver\WriteConcernError::getCode()
   Returns the MongoDB error code */
static PHP_METHOD(WriteConcernError, getCode)
{
	php_phongo_writeconcernerror_t* intern;

	intern = Z_WRITECONCERNERROR_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(intern->code);
} /* }}} */

/* {{{ proto mixed MongoDB\Driver\WriteConcernError::getInfo()
   Returns additional metadata for the error */
static PHP_METHOD(WriteConcernError, getInfo)
{
	php_phongo_writeconcernerror_t* intern;

	intern = Z_WRITECONCERNERROR_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!Z_ISUNDEF(intern->info)) {
#if PHP_VERSION_ID >= 70000
		RETURN_ZVAL(&intern->info, 1, 0);
#else
		RETURN_ZVAL(intern->info, 1, 0);
#endif
	}
} /* }}} */

/* {{{ proto string MongoDB\Driver\WriteConcernError::getMessage()
   Returns the actual error message from the server */
static PHP_METHOD(WriteConcernError, getMessage)
{
	php_phongo_writeconcernerror_t* intern;

	intern = Z_WRITECONCERNERROR_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	PHONGO_RETURN_STRING(intern->message);
} /* }}} */

/* {{{ MongoDB\Driver\WriteConcernError function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcernError_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_writeconcernerror_me[] = {
	/* clang-format off */
	PHP_ME(WriteConcernError, getCode, ai_WriteConcernError_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteConcernError, getInfo, ai_WriteConcernError_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteConcernError, getMessage, ai_WriteConcernError_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_WriteConcernError_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_WriteConcernError_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\WriteConcernError object handlers */
static zend_object_handlers php_phongo_handler_writeconcernerror;

static void php_phongo_writeconcernerror_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcernerror_t* intern = Z_OBJ_WRITECONCERNERROR(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->message) {
		efree(intern->message);
	}

	if (!Z_ISUNDEF(intern->info)) {
		zval_ptr_dtor(&intern->info);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_writeconcernerror_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcernerror_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_writeconcernerror_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_writeconcernerror;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeconcernerror_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_writeconcernerror;

		return retval;
	}
#endif
} /* }}} */

static HashTable* php_phongo_writeconcernerror_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcernerror_t* intern;
	zval                            retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_WRITECONCERNERROR_OBJ_P(object);

	array_init_size(&retval, 3);
	ADD_ASSOC_STRING(&retval, "message", intern->message);
	ADD_ASSOC_LONG_EX(&retval, "code", intern->code);
	if (!Z_ISUNDEF(intern->info)) {
#if PHP_VERSION_ID >= 70000
		Z_ADDREF(intern->info);
		ADD_ASSOC_ZVAL_EX(&retval, "info", &intern->info);
#else
		Z_ADDREF_P(intern->info);
		ADD_ASSOC_ZVAL_EX(&retval, "info", intern->info);
#endif
	} else {
		ADD_ASSOC_NULL_EX(&retval, "info");
	}

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_writeconcernerror_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteConcernError", php_phongo_writeconcernerror_me);
	php_phongo_writeconcernerror_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_writeconcernerror_ce->create_object = php_phongo_writeconcernerror_create_object;
	PHONGO_CE_FINAL(php_phongo_writeconcernerror_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_writeconcernerror_ce);

	memcpy(&php_phongo_handler_writeconcernerror, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeconcernerror.get_debug_info = php_phongo_writeconcernerror_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_writeconcernerror.free_obj = php_phongo_writeconcernerror_free_object;
	php_phongo_handler_writeconcernerror.offset   = XtOffsetOf(php_phongo_writeconcernerror_t, std);
#endif
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
