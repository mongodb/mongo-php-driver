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

zend_object_handlers php_phongo_handler_cursor;

/* {{{ proto MongoDB\Driver\Cursor Cursor::__construct(MongoDB\Driver\Server $server, MongoDB\Driver\CursorId $cursorId, array $firstBatch)
   Construct a new Cursor */
PHP_METHOD(Cursor, __construct)
{
	php_phongo_cursor_t      *intern;
	zend_error_handling       error_handling;
	zval                     *server;
	zval                     *cursorId;
	zval                     *firstBatch;
	(void)return_value; (void)return_value_ptr; (void)return_value_used;


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
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_cursor_id_new_from_id(return_value, mongoc_cursor_get_id(intern->result->cursor) TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Driver\Server Cursor::getServer()
   Returns the Server object that this cursor is attached to */
PHP_METHOD(Cursor, getServer)
{
	php_phongo_cursor_t      *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	/* FIXME: NOT IMPLEMENTED YET */
	RETURN_NULL();
}
/* }}} */
/* {{{ proto boolean Cursor::isDead()
   Checks if a cursor is still alive */
PHP_METHOD(Cursor, isDead)
{
	php_phongo_cursor_t      *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_BOOL(!mongoc_cursor_is_alive(intern->result->cursor));
}
/* }}} */
/* {{{ proto void Cursor::kill()
   Kills this cursor */
PHP_METHOD(Cursor, kill)
{
	php_phongo_cursor_t      *intern;
	int                       hint;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	hint = mongoc_cursor_get_hint(intern->result->cursor);
	mongoc_client_kill_cursor(intern->result->cursor->client, mongoc_cursor_get_id(intern->result->cursor));

	RETURN_NULL();
}
/* }}} */
/* {{{ proto integer Cursor::setBatchSize(integer $batchSize)
   Sets a bulk size for the cursor, returning the previous size */
PHP_METHOD(Cursor, setBatchSize)
{
	php_phongo_cursor_t      *intern;
	long                      batchSize;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &batchSize) == FAILURE) {
		return;
	}

	RETVAL_LONG(mongoc_cursor_get_batch_size(intern->result->cursor));
	mongoc_cursor_set_batch_size(intern->result->cursor, batchSize);
}
/* }}} */
/* {{{ proto boolean Cursor::getBatchSize()
   Gets a bulk size for the cursor */
PHP_METHOD(Cursor, getBatchSize)
{
	php_phongo_cursor_t      *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(mongoc_cursor_get_batch_size(intern->result->cursor));
}
/* }}} */

/* {{{ proto mixed Cursor::current()
   Returns the current element */
PHP_METHOD(Cursor, current)
{
	php_phongo_cursor_t      *intern;
	zval                    **data = NULL;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

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
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!intern->it) {
		zend_class_entry *ce;

		ce = Z_OBJCE_P(getThis());
		intern->it = (phongo_cursor_it *)ce->get_iterator(ce, getThis(), 0 TSRMLS_CC);
	}

	if (intern->it->iterator.funcs->get_current_key) {
		ITERATOR_GET_CURRENT_KEY(intern->it->iterator, return_value);
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
	(void)return_value; (void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

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
	(void)return_value; (void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

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
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_cursor_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!intern->it) {
		zend_class_entry *ce;

		ce = Z_OBJCE_P(getThis());
		intern->it = (phongo_cursor_it *)ce->get_iterator(ce, getThis(), 0 TSRMLS_CC);
	}

	RETURN_BOOL(intern->it->iterator.funcs->valid(&intern->it->iterator TSRMLS_CC) == SUCCESS);
}
/* }}} */

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
	php_phongo_cursor_t *intern = NULL;

	intern = (php_phongo_cursor_t *)ecalloc(1, sizeof *intern);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_cursor_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_cursor;

	return retval;
} /* }}} */

HashTable *php_phongo_cursor_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t    *intern;
	zval                    retval = zval_used_for_init;


	*is_temp = 1;
	intern = (php_phongo_cursor_t *)zend_object_store_get_object(object TSRMLS_CC);

	array_init_size(&retval, 2);

	if (!intern->it) {
		zend_class_entry *ce;

		ce = Z_OBJCE_P(object);
		intern->it = (phongo_cursor_it *)ce->get_iterator(ce, object, 0 TSRMLS_CC);
	}
	add_assoc_long_ex(&retval, ZEND_STRS("current_index"), intern->it->current);


	if (intern->result) {
		zval *result = NULL;

		MAKE_STD_ZVAL(result);
		php_phongo_result_to_zval(result, intern->result);
		add_assoc_zval_ex(&retval, ZEND_STRS("result"), result);
	}

	return Z_ARRVAL(retval);

} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Cursor)
{
	(void)type; (void)module_number;
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Cursor", php_phongo_cursor_me);
	php_phongo_cursor_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_cursor_ce->create_object = php_phongo_cursor_create_object;
	php_phongo_cursor_ce->get_iterator = phongo_cursor_get_iterator;

	memcpy(&php_phongo_handler_cursor, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_cursor.get_debug_info = php_phongo_cursor_get_debug_info;

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
