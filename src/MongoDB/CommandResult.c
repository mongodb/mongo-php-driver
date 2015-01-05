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


PHONGO_API zend_class_entry *php_phongo_commandresult_ce;

/* {{{ proto MongoDB\Driver\CommandResult CommandResult::__construct(MongoDB\Driver\Server $server, array|object $responseDocument)
   Constructs a new CommandResult */
PHP_METHOD(CommandResult, __construct)
{
	php_phongo_commandresult_t *intern;
	zend_error_handling       error_handling;
	zval                     *server;
	zval                     *responseDocument;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_commandresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "OA", &server, php_phongo_server_ce, &responseDocument) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */
/* {{{ proto MongoDB\Driver\Cursor CommandResult::getIterator()
   Returns the Cursor iterator */
PHP_METHOD(CommandResult, getIterator)
{
	php_phongo_commandresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_commandresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	php_phongo_cursor_new_from_result(return_value, &intern->result TSRMLS_CC);

	if (intern->result.ce_get_iterator) {
		object_init_ex(return_value, intern->result.ce_get_iterator);
	}

}
/* }}} */
/* {{{ proto self CommandResult::setIteratorClass(string $class)
   Sets the class name of the Cursor implementation to be used */
PHP_METHOD(CommandResult, setIteratorClass)
{
	php_phongo_commandresult_t *intern;
	zend_error_handling       error_handling;
	char                     *class;
	int                       class_len;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_commandresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &class, &class_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */
/* {{{ proto self CommandResult::setIteratorInitCallback(callable $callback)
   Sets a callback to invoke for initializing a custom Cursor */
PHP_METHOD(CommandResult, setIteratorInitCallback)
{
	php_phongo_commandresult_t *intern;
	zend_error_handling       error_handling;
	zval                     *callback;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_commandresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &callback) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */
/* {{{ proto array CommandResult::getResponseDocument()
   Returns the original response document from the server */
PHP_METHOD(CommandResult, getResponseDocument)
{
	php_phongo_commandresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_commandresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	if (intern->result.firstBatch) {
		php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;

		MAKE_STD_ZVAL(state.zchild);
		bson_to_zval(bson_get_data(intern->result.firstBatch), intern->result.firstBatch->len, &state);
		RETURN_ZVAL(state.zchild, 0, 1);
	}
}
/* }}} */
/* {{{ proto MongoDB\Driver\Server CommandResult::getServer()
   Returns the Server object that this cursor is attached to */
PHP_METHOD(CommandResult, getServer)
{
	php_phongo_commandresult_t *intern;
	zend_error_handling       error_handling;
	mongoc_host_list_t        host;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_commandresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	mongoc_cursor_get_host(intern->result.cursor, &host);
	phongo_server_init(return_value, intern->result.hint, &host TSRMLS_CC);
}
/* }}} */

/**
 * Result returned by Server and Manager executeCommand() methods.
 *
 * This object wraps an OP_REPLY. It is constructed after a command is executed
 * on the server but before a Cursor is created in the driver (if applicable).
 * This allows the Cursor implementation to be customized.
 *
 * For commands that do not support cursors (i.e. most commands), getIterator()
 * should return a cursor consisting of a single document, the command result.
 */
/* {{{ MongoDB\Driver\CommandResult */

ZEND_BEGIN_ARG_INFO_EX(ai_CommandResult___construct, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, server, MongoDB\\Driver\\Server, 0)
	ZEND_ARG_INFO(0, responseDocument)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_CommandResult_getIterator, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_CommandResult_setIteratorClass, 0, 0, 1)
	ZEND_ARG_INFO(0, class)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_CommandResult_setIteratorInitCallback, 0, 0, 1)
	ZEND_ARG_INFO(0, callback) /* callable */
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_CommandResult_getResponseDocument, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_CommandResult_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_commandresult_me[] = {
	PHP_ME(CommandResult, __construct, ai_CommandResult___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(CommandResult, getIterator, ai_CommandResult_getIterator, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(CommandResult, setIteratorClass, ai_CommandResult_setIteratorClass, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(CommandResult, setIteratorInitCallback, ai_CommandResult_setIteratorInitCallback, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(CommandResult, getResponseDocument, ai_CommandResult_getResponseDocument, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(CommandResult, getServer, ai_CommandResult_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_commandresult_t object handlers */
static void php_phongo_commandresult_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_commandresult_t *intern = (php_phongo_commandresult_t*)object;

	zend_object_std_dtor(&intern->result.std TSRMLS_CC);

	php_phongo_result_free(&intern->result);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_commandresult_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_commandresult_t *intern = NULL;

	intern = (php_phongo_commandresult_t *)emalloc(sizeof(php_phongo_commandresult_t));
	memset(intern, 0, sizeof(php_phongo_commandresult_t));

	zend_object_std_init(&intern->result.std, class_type TSRMLS_CC);
	object_properties_init(&intern->result.std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_commandresult_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(CommandResult)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "CommandResult", php_phongo_commandresult_me);
	ce.create_object = php_phongo_commandresult_create_object;
	php_phongo_commandresult_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_commandresult_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;
	php_phongo_commandresult_ce->get_iterator = phongo_result_get_iterator;

	zend_class_implements(php_phongo_commandresult_ce TSRMLS_CC, 1, zend_ce_aggregate);


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
