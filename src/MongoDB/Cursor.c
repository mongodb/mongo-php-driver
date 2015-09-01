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
#include <mongoc-cursor-private.h>
#include <mongoc-bulk-operation-private.h>

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


PHONGO_API zend_class_entry *php_phongo_cursor_ce;

zend_object_handlers php_phongo_handler_cursor;

/* {{{ proto void Cursor::setTypeMap(array $typemap)
   Sets a type map to use for BSON unserialization */
PHP_METHOD(Cursor, setTypeMap)
{
	php_phongo_cursor_t *intern;
	php_phongo_bson_state     state = PHONGO_BSON_STATE_INITIALIZER;
	zval                     *typemap = NULL;
	SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_CURSOR_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a!", &typemap) == FAILURE) {
		return;
	}

	php_phongo_bson_typemap_to_state(typemap, &state.map TSRMLS_CC);

	intern->visitor_data = state;
}
/* }}} */

static int php_phongo_cursor_to_array_apply(zend_object_iterator *iter, void *puser TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *data;
	zval *return_value = (zval*)puser;

	data = iter->funcs->get_current_data(iter TSRMLS_CC);

	if (EG(exception)) {
		return ZEND_HASH_APPLY_STOP;
	}
	if (Z_ISUNDEF_P(data)) {
		return ZEND_HASH_APPLY_STOP;
	}
	Z_TRY_ADDREF_P(data);
	add_next_index_zval(return_value, data);
#else
	zval **data;
	zval *return_value = (zval*)puser;

	iter->funcs->get_current_data(iter, &data TSRMLS_CC);

	if (EG(exception)) {
		return ZEND_HASH_APPLY_STOP;
	}
	if (data == NULL || *data == NULL) {
		return ZEND_HASH_APPLY_STOP;
	}
	Z_ADDREF_PP(data);
	add_next_index_zval(return_value, *data);
#endif

	return ZEND_HASH_APPLY_KEEP;
}
/* }}} */

/* {{{ proto array Cursor::toArray()
   Returns an array of all result documents for this cursor */
PHP_METHOD(Cursor, toArray)
{
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	array_init(return_value);

	if (spl_iterator_apply(getThis(), php_phongo_cursor_to_array_apply, (void*)return_value TSRMLS_CC) != SUCCESS) {
		zval_dtor(return_value);
		RETURN_NULL();
	}
}
/* }}} */

/* {{{ proto MongoDB\Driver\CursorId Cursor::getId()
   Returns the CursorId for this cursor */
PHP_METHOD(Cursor, getId)
{
	php_phongo_cursor_t      *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_CURSOR_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_cursor_id_new_from_id(return_value, mongoc_cursor_get_id(intern->cursor) TSRMLS_CC);
}
/* }}} */

/* {{{ proto MongoDB\Driver\Server Cursor::getServer()
   Returns the Server object to which this cursor is attached */
PHP_METHOD(Cursor, getServer)
{
	php_phongo_cursor_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_CURSOR_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	phongo_server_init(return_value, intern->cursor->client, intern->server_id TSRMLS_CC);
}
/* }}} */

/* {{{ proto boolean Cursor::isDead()
   Checks if a cursor is still alive */
PHP_METHOD(Cursor, isDead)
{
	php_phongo_cursor_t      *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_CURSOR_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_BOOL(!mongoc_cursor_is_alive(intern->cursor));
}
/* }}} */

/* {{{ MongoDB\Driver\Cursor */

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_setTypeMap, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, typemap, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_toArray, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_getId, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_isDead, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_cursor_me[] = {
	PHP_ME(Server, __construct, NULL, ZEND_ACC_FINAL|ZEND_ACC_PRIVATE)
	PHP_ME(Cursor, setTypeMap, ai_Cursor_setTypeMap, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Cursor, toArray, ai_Cursor_toArray, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Cursor, getId, ai_Cursor_getId, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Cursor, getServer, ai_Cursor_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Cursor, isDead, ai_Cursor_isDead, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_cursor_t object handlers */
static void php_phongo_cursor_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *intern = Z_OBJ_CURSOR(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	php_phongo_cursor_free(intern);

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_cursor_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_cursor_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_cursor;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_cursor_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_cursor;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_cursor_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t    *intern;
#if PHP_VERSION_ID <= 70000
	zval                    retval;
#else
	zval                    retval = zval_used_for_init;
#endif


	*is_temp = 1;
	intern = Z_CURSOR_OBJ_P(object);

	array_init_size(&retval, 2);
	if (intern->cursor) {
#if PHP_VERSION_ID >= 70000
		zval zcursor;

		php_phongo_cursor_to_zval(&zcursor, intern->cursor);
		add_assoc_zval_ex(&retval, ZEND_STRS("cursor"), &zcursor);
#else
		zval *zcursor = NULL;

		MAKE_STD_ZVAL(zcursor);

		php_phongo_cursor_to_zval(zcursor, intern->cursor);
		add_assoc_zval_ex(&retval, ZEND_STRS("cursor"), zcursor);
#endif
	} else {
		add_assoc_null_ex(&retval, ZEND_STRS("cursor"));
	}

	ADD_ASSOC_LONG_EX(&retval, "server_id", intern->server_id);

	return Z_ARRVAL(retval);

} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Cursor)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Cursor", php_phongo_cursor_me);
	php_phongo_cursor_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_cursor_ce->create_object = php_phongo_cursor_create_object;
	PHONGO_CE_INIT(php_phongo_cursor_ce);
	php_phongo_cursor_ce->get_iterator = php_phongo_cursor_get_iterator;

	memcpy(&php_phongo_handler_cursor, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_cursor.get_debug_info = php_phongo_cursor_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_cursor.free_obj = php_phongo_cursor_free_object;
	php_phongo_handler_cursor.offset = XtOffsetOf(php_phongo_cursor_t, std);
#endif

	zend_class_implements(php_phongo_cursor_ce TSRMLS_CC, 1, zend_ce_traversable);


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
