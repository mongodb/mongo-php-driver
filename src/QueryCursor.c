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


PHPAPI zend_class_entry *php_phongo_querycursor_ce;

/* {{{ proto MongoDB\Query\QueryCursor QueryCursor::__construct(MongoDB\Server $server, MongoDB\CursorId $cursorId)
   Construct a new QueryCursor */
PHP_METHOD(QueryCursor, __construct)
{
	php_phongo_querycursor_t *intern;
	zend_error_handling	error_handling;
	zval                  *server;
	zval                  *cursorId;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_querycursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "OO", &server, php_phongo_server_ce, &cursorId, php_phongo_cursorid_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\CursorId QueryCursor::getId()
   Returns the CursorId */
PHP_METHOD(QueryCursor, getId)
{
	php_phongo_querycursor_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_querycursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Server QueryCursor::getServer()
   Returns the Server object that this cursor is attached to */
PHP_METHOD(QueryCursor, getServer)
{
	php_phongo_querycursor_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_querycursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto boolean QueryCursor::isDead()
   Checks if a cursor is still alive */
PHP_METHOD(QueryCursor, isDead)
{
	php_phongo_querycursor_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_querycursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto boolean QueryCursor::setBatchSize(integer $batchSize)
   Sets a batch size for the cursor */
PHP_METHOD(QueryCursor, setBatchSize)
{
	php_phongo_querycursor_t *intern;
	zend_error_handling	error_handling;
	long                   batchSize;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_querycursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &batchSize) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto void QueryCursor::current()
    */
PHP_METHOD(QueryCursor, current)
{
	php_phongo_querycursor_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_querycursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto void QueryCursor::next()
    */
PHP_METHOD(QueryCursor, next)
{
	php_phongo_querycursor_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_querycursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto void QueryCursor::key()
    */
PHP_METHOD(QueryCursor, key)
{
	php_phongo_querycursor_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_querycursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto void QueryCursor::valid()
    */
PHP_METHOD(QueryCursor, valid)
{
	php_phongo_querycursor_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_querycursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto void QueryCursor::rewind()
    */
PHP_METHOD(QueryCursor, rewind)
{
	php_phongo_querycursor_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_querycursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */

/**
 * Cursor implementation that is returned after executing a Query.
 *
 * The iteration and internal logic is very similar to CommandCursor, so both
 * classes should likely share code. The documents in the OP_REPLY message
 * returned by the original OP_QUERY is comparable to the first batch of a
 * command cursor, in that both may be available at the time the cursor is
 * constructed.
 */
/* {{{ MongoDB\Query\QueryCursor */

ZEND_BEGIN_ARG_INFO_EX(ai_QueryCursor___construct, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, server, MongoDB\\Server, 0)
	ZEND_ARG_OBJ_INFO(0, cursorId, MongoDB\\CursorId, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_QueryCursor_getId, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_QueryCursor_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_QueryCursor_isDead, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_QueryCursor_setBatchSize, 0, 0, 1)
	ZEND_ARG_INFO(0, batchSize) 
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_QueryCursor_current, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_QueryCursor_next, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_QueryCursor_key, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_QueryCursor_valid, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_QueryCursor_rewind, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_querycursor_me[] = {
	PHP_ME(QueryCursor, __construct, ai_QueryCursor___construct, ZEND_ACC_PUBLIC)
	PHP_ME(QueryCursor, getId, ai_QueryCursor_getId, ZEND_ACC_PUBLIC)
	PHP_ME(QueryCursor, getServer, ai_QueryCursor_getServer, ZEND_ACC_PUBLIC)
	PHP_ME(QueryCursor, isDead, ai_QueryCursor_isDead, ZEND_ACC_PUBLIC)
	PHP_ME(QueryCursor, setBatchSize, ai_QueryCursor_setBatchSize, ZEND_ACC_PUBLIC)
	PHP_ME(QueryCursor, current, ai_QueryCursor_current, ZEND_ACC_PUBLIC)
	PHP_ME(QueryCursor, next, ai_QueryCursor_next, ZEND_ACC_PUBLIC)
	PHP_ME(QueryCursor, key, ai_QueryCursor_key, ZEND_ACC_PUBLIC)
	PHP_ME(QueryCursor, valid, ai_QueryCursor_valid, ZEND_ACC_PUBLIC)
	PHP_ME(QueryCursor, rewind, ai_QueryCursor_rewind, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(QueryCursor)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Query", "QueryCursor", php_phongo_querycursor_me);
	php_phongo_querycursor_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(php_phongo_querycursor_ce TSRMLS_CC, 1, php_phongo_cursor_ce);


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
