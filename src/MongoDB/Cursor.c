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
#include <mongoc-cursor-private.h>

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


PHONGO_API zend_class_entry *php_phongo_cursor_ce;

/* {{{ proto MongoDB\Driver\Cursor Cursor::__construct(MongoDB\Driver\Server $server, MongoDB\Driver\CursorId $cursorId, array $firstBatch)
   Construct a new Cursor */
PHP_METHOD(Cursor, __construct)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;
	zval                     *server;
	zval                     *cursorId;
	zval                     *firstBatch;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "OOa", &server, php_phongo_server_ce, &cursorId, php_phongo_cursorid_ce, &firstBatch) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */
/* {{{ proto MongoDB\Driver\CursorId Cursor::getId()
   Returns the CursorId */
PHP_METHOD(Cursor, getId)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	php_phongo_cursor_id_new_from_id(return_value, mongoc_cursor_get_id(intern->result->cursor) TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Driver\Server Cursor::getServer()
   Returns the Server object that this cursor is attached to */
PHP_METHOD(Cursor, getServer)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */
/* {{{ proto boolean Cursor::isDead()
   Checks if a cursor is still alive */
PHP_METHOD(Cursor, isDead)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_BOOL(!mongoc_cursor_is_alive(intern->result->cursor));
}
/* }}} */
/* {{{ proto void Cursor::kill()
   Kills this cursor */
PHP_METHOD(Cursor, kill)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;
	int                       hint;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	hint = mongoc_cursor_get_hint(intern->result->cursor);
	mongoc_client_kill_cursor(intern->result->cursor->client, mongoc_cursor_get_id(intern->result->cursor));
}
/* }}} */
/* {{{ proto boolean Cursor::setBatchSize(integer $batchSize)
   Sets a batch size for the cursor */
PHP_METHOD(Cursor, setBatchSize)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;
	long                      batchSize;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &batchSize) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	mongoc_cursor_set_batch_size(intern->result->cursor, batchSize);
}
/* }}} */
/* {{{ proto boolean Cursor::getBatchSize()
   Gets a batch size for the cursor */
PHP_METHOD(Cursor, getBatchSize)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	RETURN_LONG(mongoc_cursor_get_batch_size(intern->result->cursor));
}
/* }}} */

/* {{{ proto mixed Cursor::current()
   Returns the current element */
PHP_METHOD(Cursor, current)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;
	zval                    **data = NULL;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (!intern->it) {
		zend_class_entry *ce;

		ce = Z_OBJCE_P(getThis());
		intern->it = (phongo_cursor_it *)ce->get_iterator(ce, getThis(), 0 TSRMLS_CC);
	}

	intern->it->iterator.funcs->get_current_data(&intern->it->iterator, &data TSRMLS_CC);
	if (data && *data) {
		RETURN_ZVAL(*data, 1, 0);
	}
}
/* }}} */
/* {{{ proto mixed Cursor::key()
   Returns the key of the current element */
PHP_METHOD(Cursor, key)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (!intern->it) {
		zend_class_entry *ce;

		ce = Z_OBJCE_P(getThis());
		intern->it = (phongo_cursor_it *)ce->get_iterator(ce, getThis(), 0 TSRMLS_CC);
	}

	if (intern->it->iterator.funcs->get_current_key) {
		intern->it->iterator.funcs->get_current_key(&intern->it->iterator, return_value TSRMLS_CC);
	} else {
		RETURN_LONG(intern->it->current);
	}
}
/* }}} */
/* {{{ proto void Cursor::next()
   Move forward to next element */
PHP_METHOD(Cursor, next)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (!intern->it) {
		zend_class_entry *ce;

		ce = Z_OBJCE_P(getThis());
		intern->it = (phongo_cursor_it *)ce->get_iterator(ce, getThis(), 0 TSRMLS_CC);
	}

	intern->it->iterator.funcs->move_forward(&intern->it->iterator TSRMLS_CC);
}
/* }}} */
/* {{{ proto void Cursor::rewind()
   Rewind the Iterator to the first element */
PHP_METHOD(Cursor, rewind)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (!intern->it) {
		zend_class_entry *ce;

		ce = Z_OBJCE_P(getThis());
		intern->it = (phongo_cursor_it *)ce->get_iterator(ce, getThis(), 0 TSRMLS_CC);
	}

	intern->it->iterator.funcs->rewind(&intern->it->iterator TSRMLS_CC);
}
/* }}} */
/* {{{ proto boolean Cursor::valid()
   Checks if current position is valid */
PHP_METHOD(Cursor, valid)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (!intern->it) {
		zend_class_entry *ce;

		ce = Z_OBJCE_P(getThis());
		intern->it = (phongo_cursor_it *)ce->get_iterator(ce, getThis(), 0 TSRMLS_CC);
	}

	RETURN_BOOL(intern->it->iterator.funcs->valid(&intern->it->iterator TSRMLS_CC) == SUCCESS);
}
/* }}} */

/**
 * Cursor used to iterate through results of an executed Query or Command.
 *
 * The iteration and internal logic of Query and Command cursors is very
 * similar. The cursor ID and first batch of results, originating from either
 * the OP_REPLY message or command result document), will be used to construct
 * this object.
 *
 * While this Cursor object must be initialized internally, the class itself may
 * be extended to provide custom Cursor behaviors (e.g. return documents as
 * BSON, hydrated classes, stdClass objects).
 */
/* {{{ MongoDB\Driver\Cursor */

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor___construct, 0, 0, 3)
	ZEND_ARG_OBJ_INFO(0, server, MongoDB\\Driver\\Server, 0)
	ZEND_ARG_OBJ_INFO(0, cursorId, MongoDB\\Driver\\CursorId, 0)
	ZEND_ARG_ARRAY_INFO(0, firstBatch, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_getId, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_isDead, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_kill, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_getBatchSize, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_setBatchSize, 0, 0, 1)
	ZEND_ARG_INFO(0, batchSize)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_current, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_key, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_next, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_rewind, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_valid, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_cursor_me[] = {
	PHP_ME(Cursor, __construct, ai_Cursor___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, getId, ai_Cursor_getId, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, getServer, ai_Cursor_getServer, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, isDead, ai_Cursor_isDead, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, kill, ai_Cursor_kill, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, getBatchSize, ai_Cursor_getBatchSize, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, setBatchSize, ai_Cursor_setBatchSize, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, current, ai_Cursor_current, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, key, ai_Cursor_key, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, next, ai_Cursor_next, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, rewind, ai_Cursor_rewind, ZEND_ACC_PUBLIC)
	PHP_ME(Cursor, valid, ai_Cursor_valid, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_cursor_t object handlers */
static void php_phongo_cursor_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *intern = (php_phongo_cursor_t*)object;

	if (intern->it) {
		intern->it->iterator.funcs->dtor(&intern->it->iterator TSRMLS_CC);
		intern->it = NULL;
	}
	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_cursor_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_cursor_t *intern;

	intern = (php_phongo_cursor_t *)emalloc(sizeof(php_phongo_cursor_t));
	memset(intern, 0, sizeof(php_phongo_cursor_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_cursor_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Cursor)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Cursor", php_phongo_cursor_me);
	ce.create_object = php_phongo_cursor_create_object;
	php_phongo_cursor_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_cursor_ce->get_iterator = phongo_cursor_get_iterator;

	zend_class_implements(php_phongo_cursor_ce TSRMLS_CC, 1, spl_ce_Iterator);


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
