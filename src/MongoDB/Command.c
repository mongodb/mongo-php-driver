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


PHONGO_API zend_class_entry *php_phongo_command_ce;

zend_object_handlers php_phongo_handler_command;

/* {{{ proto void Command::__construct(array|object $document)
   Constructs a new Command */
PHP_METHOD(Command, __construct)
{
	php_phongo_command_t     *intern;
	zend_error_handling       error_handling;
	zval                     *document;
	bson_t                   *bson = bson_new();
	SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_COMMAND_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &document) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	phongo_zval_to_bson(document, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);
	intern->bson = bson;
}
/* }}} */

/**
 * Value object for a database command document.
 */
/* {{{ MongoDB\Driver\Command */

ZEND_BEGIN_ARG_INFO_EX(ai_Command___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, document)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Command_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_command_me[] = {
	PHP_ME(Command, __construct, ai_Command___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeup, ai_Command_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_command_t object handlers */
static void php_phongo_command_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_command_t *intern = Z_OBJ_COMMAND(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->bson) {
		bson_clear(&intern->bson);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_command_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_command_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_command_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_command;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_command_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_command;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_command_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_command_t  *intern;
#if PHP_VERSION_ID >= 70000
	zval                   retval;
#else
	zval                   retval = zval_used_for_init;
#endif


	*is_temp = 1;
	intern = Z_COMMAND_OBJ_P(object);

	array_init_size(&retval, 1);

	if (intern->bson) {
#if PHP_VERSION_ID >= 70000
			zval zv;
#else
			zval *zv;
#endif

		phongo_bson_to_zval(bson_get_data(intern->bson), intern->bson->len, &zv);
#if PHP_VERSION_ID >= 70000
		ADD_ASSOC_ZVAL_EX(&retval, "command", &zv);
#else
		ADD_ASSOC_ZVAL_EX(&retval, "command", zv);
#endif
	} else {
		ADD_ASSOC_NULL_EX(&retval, "command");
	}

	return Z_ARRVAL(retval);

} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Command)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Command", php_phongo_command_me);
	php_phongo_command_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_command_ce->create_object = php_phongo_command_create_object;
	PHONGO_CE_FINAL(php_phongo_command_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_command_ce);

	memcpy(&php_phongo_handler_command, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_command.get_debug_info = php_phongo_command_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_command.free_obj = php_phongo_command_free_object;
	php_phongo_handler_command.offset = XtOffsetOf(php_phongo_command_t, std);
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
