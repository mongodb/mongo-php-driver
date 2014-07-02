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

/* {{{ proto MongoDB\Command\CommandResult CommandResult::__construct(MongoDB\Server $server, array|object $responseDocument)
   Constructs a new CommandResult */
PHP_METHOD(CommandResult, __construct)
{
	php_phongo_commandresult_t *intern;
	zend_error_handling	error_handling;
	zval                  *server;
	zval                  *responseDocument;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_commandresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "OA", &server, php_phongo_server_ce, &responseDocument) == FAILURE) {
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
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_commandresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Server CommandResult::getServer()
   Returns the Server object that this cursor is attached to */
PHP_METHOD(CommandResult, getServer)
{
	php_phongo_commandresult_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_commandresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */

/**
 * Result returned by Server and Manager executeCommand() methods.
 */
/* {{{ MongoDB\Command\CommandResult */

ZEND_BEGIN_ARG_INFO_EX(ai_CommandResult___construct, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, server, MongoDB\\Server, 0)
	ZEND_ARG_INFO(0, responseDocument) 
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_CommandResult_getResponseDocument, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_CommandResult_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_commandresult_me[] = {
	PHP_ME(CommandResult, __construct, ai_CommandResult___construct, ZEND_ACC_PUBLIC)
	PHP_ME(CommandResult, getResponseDocument, ai_CommandResult_getResponseDocument, ZEND_ACC_PUBLIC)
	PHP_ME(CommandResult, getServer, ai_CommandResult_getServer, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_commandresult_free_object && php_phongo_commandresult_create_object */
static void php_phongo_commandresult_free_object(void *object TSRMLS_DC)
{
	php_phongo_commandresult_t *intern = (php_phongo_commandresult_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
}

zend_object_value php_phongo_commandresult_create_object(zend_class_entry *class_type TSRMLS_DC)
{
	zend_object_value retval;
	php_phongo_commandresult_t *intern;

	intern = (php_phongo_commandresult_t *)emalloc(sizeof(php_phongo_commandresult_t));
	memset(intern, 0, sizeof(php_phongo_commandresult_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_commandresult_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(CommandResult)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Command", "CommandResult", php_phongo_commandresult_me);
	ce.create_object = php_phongo_commandresult_create_object;
	php_phongo_commandresult_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_commandresult_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

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
