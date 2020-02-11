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
#include <ext/spl/spl_iterators.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"
#include "php_bson.h"

zend_class_entry* php_phongo_cursor_ce;

/* Check if the cursor is exhausted (i.e. ID is zero) and free any reference to
 * the session. Calling this function during iteration will allow an implicit
 * session to return to the pool immediately after a getMore indicates that the
 * server has no more results to return. */
static void php_phongo_cursor_free_session_if_exhausted(php_phongo_cursor_t* cursor) /* {{{ */
{
	if (mongoc_cursor_get_id(cursor->cursor)) {
		return;
	}

	if (!Z_ISUNDEF(cursor->session)) {
		zval_ptr_dtor(&cursor->session);
		ZVAL_UNDEF(&cursor->session);
	}
} /* }}} */

static void php_phongo_cursor_free_current(php_phongo_cursor_t* cursor) /* {{{ */
{
	if (!Z_ISUNDEF(cursor->visitor_data.zchild)) {
		zval_ptr_dtor(&cursor->visitor_data.zchild);
		ZVAL_UNDEF(&cursor->visitor_data.zchild);
	}
} /* }}} */

/* {{{ MongoDB\Driver\Cursor iterator handlers */
static void php_phongo_cursor_iterator_dtor(zend_object_iterator* iter) /* {{{ */
{
	php_phongo_cursor_iterator* cursor_it = (php_phongo_cursor_iterator*) iter;

	if (!Z_ISUNDEF(cursor_it->intern.data)) {
		zval_ptr_dtor(&cursor_it->intern.data);
	}
} /* }}} */

static int php_phongo_cursor_iterator_valid(zend_object_iterator* iter) /* {{{ */
{
	php_phongo_cursor_t* cursor = ((php_phongo_cursor_iterator*) iter)->cursor;

	if (!Z_ISUNDEF(cursor->visitor_data.zchild)) {
		return SUCCESS;
	}

	return FAILURE;
} /* }}} */

static void php_phongo_cursor_iterator_get_current_key(zend_object_iterator* iter, zval* key) /* {{{ */
{
	php_phongo_cursor_t* cursor = ((php_phongo_cursor_iterator*) iter)->cursor;

	ZVAL_LONG(key, cursor->current);
} /* }}} */

static zval* php_phongo_cursor_iterator_get_current_data(zend_object_iterator* iter) /* {{{ */
{
	php_phongo_cursor_t* cursor = ((php_phongo_cursor_iterator*) iter)->cursor;

	return &cursor->visitor_data.zchild;
} /* }}} */

static void php_phongo_cursor_iterator_move_forward(zend_object_iterator* iter) /* {{{ */
{
	php_phongo_cursor_iterator* cursor_it = (php_phongo_cursor_iterator*) iter;
	php_phongo_cursor_t*        cursor    = cursor_it->cursor;
	const bson_t*               doc;

	php_phongo_cursor_free_current(cursor);

	/* If the cursor has already advanced, increment its position. Otherwise,
	 * the first call to mongoc_cursor_next() will be made below and we should
	 * leave its position at zero. */
	if (cursor->advanced) {
		cursor->current++;
	} else {
		cursor->advanced = true;
	}

	if (mongoc_cursor_next(cursor->cursor, &doc)) {
		if (!php_phongo_bson_to_zval_ex(bson_get_data(doc), doc->len, &cursor->visitor_data)) {
			/* Free invalid result, but don't return as we want to free the
			 * session if the cursor is exhausted. */
			php_phongo_cursor_free_current(cursor);
		}
	} else {
		bson_error_t  error = { 0 };
		const bson_t* doc   = NULL;

		if (mongoc_cursor_error_document(cursor->cursor, &error, &doc)) {
			/* Intentionally not destroying the cursor as it will happen
			 * naturally now that there are no more results */
			phongo_throw_exception_from_bson_error_t_and_reply(&error, doc);
		}
	}

	php_phongo_cursor_free_session_if_exhausted(cursor);
} /* }}} */

static void php_phongo_cursor_iterator_rewind(zend_object_iterator* iter) /* {{{ */
{
	php_phongo_cursor_iterator* cursor_it = (php_phongo_cursor_iterator*) iter;
	php_phongo_cursor_t*        cursor    = cursor_it->cursor;
	const bson_t*               doc;

	/* If the cursor was never advanced (e.g. command cursor), do so now */
	if (!cursor->advanced) {
		cursor->advanced = true;

		if (!phongo_cursor_advance_and_check_for_error(cursor->cursor)) {
			/* Exception should already have been thrown */
			return;
		}
	}

	if (cursor->current > 0) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cursors cannot rewind after starting iteration");
		return;
	}

	php_phongo_cursor_free_current(cursor);

	doc = mongoc_cursor_current(cursor->cursor);

	if (doc) {
		if (!php_phongo_bson_to_zval_ex(bson_get_data(doc), doc->len, &cursor->visitor_data)) {
			/* Free invalid result, but don't return as we want to free the
			 * session if the cursor is exhausted. */
			php_phongo_cursor_free_current(cursor);
		}
	}

	php_phongo_cursor_free_session_if_exhausted(cursor);
} /* }}} */

static zend_object_iterator_funcs php_phongo_cursor_iterator_funcs = {
	php_phongo_cursor_iterator_dtor,
	php_phongo_cursor_iterator_valid,
	php_phongo_cursor_iterator_get_current_data,
	php_phongo_cursor_iterator_get_current_key,
	php_phongo_cursor_iterator_move_forward,
	php_phongo_cursor_iterator_rewind,
	NULL /* invalidate_current is not used */
};

static zend_object_iterator* php_phongo_cursor_get_iterator(zend_class_entry* ce, zval* object, int by_ref) /* {{{ */
{
	php_phongo_cursor_iterator* cursor_it = NULL;
	php_phongo_cursor_t*        cursor    = Z_CURSOR_OBJ_P(object);

	if (by_ref) {
		zend_error(E_ERROR, "An iterator cannot be used with foreach by reference");
	}

	if (cursor->got_iterator) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cursors cannot yield multiple iterators");
		return NULL;
	}

	cursor->got_iterator = true;

	cursor_it = ecalloc(1, sizeof(php_phongo_cursor_iterator));
	zend_iterator_init(&cursor_it->intern);

	ZVAL_COPY(&cursor_it->intern.data, object);
	cursor_it->intern.funcs = &php_phongo_cursor_iterator_funcs;
	cursor_it->cursor       = cursor;
	/* cursor_it->current should already be allocated to zero */

	php_phongo_cursor_free_current(cursor_it->cursor);

	return &cursor_it->intern;
} /* }}} */
/* }}} */

/* {{{ proto void MongoDB\Driver\Cursor::setTypeMap(array $typemap)
   Sets a type map to use for BSON unserialization */
static PHP_METHOD(Cursor, setTypeMap)
{
	php_phongo_cursor_t*  intern;
	php_phongo_bson_state state;
	zval*                 typemap                 = NULL;
	bool                  restore_current_element = false;

	PHONGO_BSON_INIT_STATE(state);

	intern = Z_CURSOR_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "a!", &typemap) == FAILURE) {
		return;
	}

	if (!php_phongo_bson_typemap_to_state(typemap, &state.map)) {
		return;
	}

	/* Check if the existing element needs to be freed before we overwrite
	 * visitor_data, which contains the only reference to it. */
	if (!Z_ISUNDEF(intern->visitor_data.zchild)) {
		php_phongo_cursor_free_current(intern);
		restore_current_element = true;
	}

	php_phongo_bson_typemap_dtor(&intern->visitor_data.map);

	intern->visitor_data = state;

	/* If the cursor has a current element, we just freed it and should restore
	 * it with a new type map applied. */
	if (restore_current_element && mongoc_cursor_current(intern->cursor)) {
		const bson_t* doc = mongoc_cursor_current(intern->cursor);

		if (!php_phongo_bson_to_zval_ex(bson_get_data(doc), doc->len, &intern->visitor_data)) {
			php_phongo_cursor_free_current(intern);
		}
	}
} /* }}} */

static int php_phongo_cursor_to_array_apply(zend_object_iterator* iter, void* puser) /* {{{ */
{
	zval* data;
	zval* return_value = (zval*) puser;

	data = iter->funcs->get_current_data(iter);

	if (EG(exception)) {
		return ZEND_HASH_APPLY_STOP;
	}
	if (Z_ISUNDEF_P(data)) {
		return ZEND_HASH_APPLY_STOP;
	}
	Z_TRY_ADDREF_P(data);
	add_next_index_zval(return_value, data);

	return ZEND_HASH_APPLY_KEEP;
} /* }}} */

static void php_phongo_cursor_id_new_from_id(zval* object, int64_t cursorid) /* {{{ */
{
	php_phongo_cursorid_t* intern;

	object_init_ex(object, php_phongo_cursorid_ce);

	intern     = Z_CURSORID_OBJ_P(object);
	intern->id = cursorid;
} /* }}} */

/* {{{ proto array MongoDB\Driver\Cursor::toArray()
   Returns an array of all result documents for this cursor */
static PHP_METHOD(Cursor, toArray)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	array_init(return_value);

	if (spl_iterator_apply(getThis(), php_phongo_cursor_to_array_apply, (void*) return_value) != SUCCESS) {
		zval_dtor(return_value);
		RETURN_NULL();
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\CursorId MongoDB\Driver\Cursor::getId()
   Returns the CursorId for this cursor */
static PHP_METHOD(Cursor, getId)
{
	php_phongo_cursor_t* intern;

	intern = Z_CURSOR_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_cursor_id_new_from_id(return_value, mongoc_cursor_get_id(intern->cursor));
} /* }}} */

/* {{{ proto MongoDB\Driver\Server MongoDB\Driver\Cursor::getServer()
   Returns the Server object to which this cursor is attached */
static PHP_METHOD(Cursor, getServer)
{
	php_phongo_cursor_t* intern;

	intern = Z_CURSOR_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	phongo_server_init(return_value, intern->client, intern->server_id);
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Cursor::isDead()
   Checks if a cursor is still alive */
static PHP_METHOD(Cursor, isDead)
{
	php_phongo_cursor_t* intern;

	intern = Z_CURSOR_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_BOOL(!mongoc_cursor_more(intern->cursor));
} /* }}} */

/* {{{ MongoDB\Driver\Cursor function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_setTypeMap, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, typemap, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Cursor_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_cursor_me[] = {
	/* clang-format off */
	PHP_ME(Cursor, setTypeMap, ai_Cursor_setTypeMap, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Cursor, toArray, ai_Cursor_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Cursor, getId, ai_Cursor_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Cursor, getServer, ai_Cursor_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Cursor, isDead, ai_Cursor_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_Cursor_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_Cursor_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Cursor object handlers */
static zend_object_handlers php_phongo_handler_cursor;

static void php_phongo_cursor_free_object(zend_object* object) /* {{{ */
{
	php_phongo_cursor_t* intern = Z_OBJ_CURSOR(object);

	zend_object_std_dtor(&intern->std);

	/* If this Cursor was created in a different process, reset the client so
	 * that mongoc_cursor_destroy does not issue a killCursors command for an
	 * active cursor owned by a parent process. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	if (intern->cursor) {
		mongoc_cursor_destroy(intern->cursor);
	}

	if (intern->database) {
		efree(intern->database);
	}

	if (intern->collection) {
		efree(intern->collection);
	}

	if (!Z_ISUNDEF(intern->query)) {
		zval_ptr_dtor(&intern->query);
	}

	if (!Z_ISUNDEF(intern->command)) {
		zval_ptr_dtor(&intern->command);
	}

	if (!Z_ISUNDEF(intern->read_preference)) {
		zval_ptr_dtor(&intern->read_preference);
	}

	if (!Z_ISUNDEF(intern->session)) {
		zval_ptr_dtor(&intern->session);
	}

	php_phongo_bson_typemap_dtor(&intern->visitor_data.map);

	php_phongo_cursor_free_current(intern);
} /* }}} */

static zend_object* php_phongo_cursor_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_cursor_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_cursor_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	PHONGO_SET_CREATED_BY_PID(intern);

	intern->std.handlers = &php_phongo_handler_cursor;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_cursor_get_debug_info(zval* object, int* is_temp) /* {{{ */
{
	php_phongo_cursor_t* intern;
	zval                 retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_CURSOR_OBJ_P(object);

	array_init_size(&retval, 10);

	if (intern->database) {
		ADD_ASSOC_STRING(&retval, "database", intern->database);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "database");
	}

	if (intern->collection) {
		ADD_ASSOC_STRING(&retval, "collection", intern->collection);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "collection");
	}

	if (!Z_ISUNDEF(intern->query)) {
		ADD_ASSOC_ZVAL_EX(&retval, "query", &intern->query);
		Z_ADDREF(intern->query);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "query");
	}

	if (!Z_ISUNDEF(intern->command)) {
		ADD_ASSOC_ZVAL_EX(&retval, "command", &intern->command);
		Z_ADDREF(intern->command);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "command");
	}

	if (!Z_ISUNDEF(intern->read_preference)) {
		ADD_ASSOC_ZVAL_EX(&retval, "readPreference", &intern->read_preference);
		Z_ADDREF(intern->read_preference);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "readPreference");
	}

	if (!Z_ISUNDEF(intern->session)) {
		ADD_ASSOC_ZVAL_EX(&retval, "session", &intern->session);
		Z_ADDREF(intern->session);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "session");
	}

	ADD_ASSOC_BOOL_EX(&retval, "isDead", !mongoc_cursor_more(intern->cursor));

	ADD_ASSOC_LONG_EX(&retval, "currentIndex", intern->current);

	if (!Z_ISUNDEF(intern->visitor_data.zchild)) {
		ADD_ASSOC_ZVAL_EX(&retval, "currentDocument", &intern->visitor_data.zchild);
		Z_ADDREF(intern->visitor_data.zchild);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "currentDocument");
	}

	{
		zval server;

		phongo_server_init(&server, intern->client, intern->server_id);
		ADD_ASSOC_ZVAL_EX(&retval, "server", &server);
	}

	return Z_ARRVAL(retval);

} /* }}} */
/* }}} */

void php_phongo_cursor_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Cursor", php_phongo_cursor_me);
	php_phongo_cursor_ce                = zend_register_internal_class(&ce);
	php_phongo_cursor_ce->create_object = php_phongo_cursor_create_object;
	PHONGO_CE_FINAL(php_phongo_cursor_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_cursor_ce);
	php_phongo_cursor_ce->get_iterator = php_phongo_cursor_get_iterator;

	zend_class_implements(php_phongo_cursor_ce, 1, php_phongo_cursor_interface_ce);

	memcpy(&php_phongo_handler_cursor, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_cursor.get_debug_info = php_phongo_cursor_get_debug_info;
	php_phongo_handler_cursor.free_obj       = php_phongo_cursor_free_object;
	php_phongo_handler_cursor.offset         = XtOffsetOf(php_phongo_cursor_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
