/*
 * Copyright 2014-present MongoDB, Inc.
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
#include <ext/spl/spl_iterators.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_bson.h"
#include "phongo_client.h"
#include "phongo_error.h"
#include "phongo_util.h"

#include "MongoDB/Cursor.h"
#include "MongoDB/Server.h"
#include "Cursor_arginfo.h"

zend_class_entry* php_phongo_cursor_ce;

/* Check if the cursor is exhausted (i.e. ID is zero) and free any reference to
 * the session. Calling this function during iteration will allow an implicit
 * session to return to the pool immediately after a getMore indicates that the
 * server has no more results to return. */
static void php_phongo_cursor_free_session_if_exhausted(php_phongo_cursor_t* cursor)
{
	if (mongoc_cursor_get_id(cursor->cursor)) {
		return;
	}

	if (!Z_ISUNDEF(cursor->session)) {
		zval_ptr_dtor(&cursor->session);
		ZVAL_UNDEF(&cursor->session);
	}
}

static void php_phongo_cursor_free_current(php_phongo_cursor_t* cursor)
{
	if (!Z_ISUNDEF(cursor->visitor_data.zchild)) {
		zval_ptr_dtor(&cursor->visitor_data.zchild);
		ZVAL_UNDEF(&cursor->visitor_data.zchild);
	}
}

/* Sets a type map to use for BSON unserialization */
static PHP_METHOD(MongoDB_Driver_Cursor, setTypeMap)
{
	php_phongo_cursor_t*  intern;
	php_phongo_bson_state state;
	zval*                 typemap                 = NULL;
	bool                  restore_current_element = false;

	PHONGO_BSON_INIT_STATE(state);

	intern = Z_CURSOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY_OR_NULL(typemap)
	PHONGO_PARSE_PARAMETERS_END();

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

		if (!php_phongo_bson_to_zval_ex(doc, &intern->visitor_data)) {
			php_phongo_cursor_free_current(intern);
		}
	}
}

static int php_phongo_cursor_to_array_apply(zend_object_iterator* iter, void* puser)
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
}

static void php_phongo_cursor_id_new_from_id(zval* object, int64_t cursorid)
{
	php_phongo_cursorid_t* intern;

	object_init_ex(object, php_phongo_cursorid_ce);

	intern              = Z_CURSORID_OBJ_P(object);
	intern->id          = cursorid;
	intern->initialized = true;
}

/* Returns an array of all result documents for this cursor */
static PHP_METHOD(MongoDB_Driver_Cursor, toArray)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	array_init(return_value);

	if (spl_iterator_apply(getThis(), php_phongo_cursor_to_array_apply, (void*) return_value) != SUCCESS) {
		zval_dtor(return_value);
		RETURN_NULL();
	}
}

/* Returns the CursorId for this cursor */
static PHP_METHOD(MongoDB_Driver_Cursor, getId)
{
	php_phongo_cursor_t* intern;
	zend_bool            asInt64 = false;

	intern = Z_CURSOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_BOOL(asInt64)
	PHONGO_PARSE_PARAMETERS_END();

	if (asInt64) {
		phongo_int64_new(return_value, mongoc_cursor_get_id(intern->cursor));
	} else {
		php_error_docref(NULL, E_DEPRECATED, "The method \"MongoDB\\Driver\\Cursor::getId\" will no longer return a \"MongoDB\\Driver\\CursorId\" instance in the future. Pass \"true\" as argument to change to the new behavior and receive a \"MongoDB\\BSON\\Int64\" instance instead.");

		php_phongo_cursor_id_new_from_id(return_value, mongoc_cursor_get_id(intern->cursor));
	}
}

/* Returns the Server object to which this cursor is attached */
static PHP_METHOD(MongoDB_Driver_Cursor, getServer)
{
	php_phongo_cursor_t* intern;

	intern = Z_CURSOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_server_init(return_value, &intern->manager, intern->server_id);
}

/* Checks if a cursor is still alive */
static PHP_METHOD(MongoDB_Driver_Cursor, isDead)
{
	php_phongo_cursor_t* intern;

	intern = Z_CURSOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(!mongoc_cursor_more(intern->cursor));
}

static PHP_METHOD(MongoDB_Driver_Cursor, current)
{
	php_phongo_cursor_t* intern = Z_CURSOR_OBJ_P(getThis());
	zval*                data;

	PHONGO_PARSE_PARAMETERS_NONE();

	data = &intern->visitor_data.zchild;

	if (Z_ISUNDEF_P(data)) {
		RETURN_NULL();
	} else {
		ZVAL_COPY_DEREF(return_value, data);
	}
}

static PHP_METHOD(MongoDB_Driver_Cursor, key)
{
	php_phongo_cursor_t* intern = Z_CURSOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (Z_ISUNDEF(intern->visitor_data.zchild)) {
		RETURN_NULL();
	}

	RETURN_LONG(intern->current);
}

static PHP_METHOD(MongoDB_Driver_Cursor, next)
{
	php_phongo_cursor_t* intern = Z_CURSOR_OBJ_P(getThis());
	const bson_t*        doc;

	PHONGO_PARSE_PARAMETERS_NONE();

	php_phongo_cursor_free_current(intern);

	/* If the intern has already advanced, increment its position. Otherwise,
	 * the first call to mongoc_cursor_next() will be made below and we should
	 * leave its position at zero. */
	if (intern->advanced) {
		intern->current++;
	} else {
		intern->advanced = true;
	}

	if (mongoc_cursor_next(intern->cursor, &doc)) {
		if (!php_phongo_bson_to_zval_ex(doc, &intern->visitor_data)) {
			/* Free invalid result, but don't return as we want to free the
			 * session if the intern is exhausted. */
			php_phongo_cursor_free_current(intern);
		}
	} else {
		bson_error_t  error = { 0 };
		const bson_t* doc   = NULL;

		if (mongoc_cursor_error_document(intern->cursor, &error, &doc)) {
			/* Intentionally not destroying the intern as it will happen
			 * naturally now that there are no more results */
			phongo_throw_exception_from_bson_error_t_and_reply(&error, doc);
		}
	}

	php_phongo_cursor_free_session_if_exhausted(intern);
}

static PHP_METHOD(MongoDB_Driver_Cursor, valid)
{
	php_phongo_cursor_t* intern = Z_CURSOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(!Z_ISUNDEF(intern->visitor_data.zchild));
}

static PHP_METHOD(MongoDB_Driver_Cursor, rewind)
{
	php_phongo_cursor_t* intern = Z_CURSOR_OBJ_P(getThis());
	const bson_t*        doc;

	PHONGO_PARSE_PARAMETERS_NONE();

	/* If the cursor was never advanced (e.g. command cursor), do so now */
	if (!intern->advanced) {
		intern->advanced = true;

		if (!phongo_cursor_advance_and_check_for_error(intern->cursor)) {
			/* Exception should already have been thrown */
			return;
		}
	}

	if (intern->current > 0) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cursors cannot rewind after starting iteration");
		return;
	}

	php_phongo_cursor_free_current(intern);

	doc = mongoc_cursor_current(intern->cursor);

	if (doc) {
		if (!php_phongo_bson_to_zval_ex(doc, &intern->visitor_data)) {
			/* Free invalid result, but don't return as we want to free the
			 * session if the intern is exhausted. */
			php_phongo_cursor_free_current(intern);
		}
	}

	php_phongo_cursor_free_session_if_exhausted(intern);
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Cursor)

/* MongoDB\Driver\Cursor object handlers */
static zend_object_handlers php_phongo_handler_cursor;

static void php_phongo_cursor_free_object(zend_object* object)
{
	php_phongo_cursor_t* intern = Z_OBJ_CURSOR(object);

	zend_object_std_dtor(&intern->std);

	/* If this Cursor was created in a different process, reset the client so
	 * that mongoc_cursor_destroy does not issue a killCursors command for an
	 * active cursor owned by a parent process. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

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

	if (!Z_ISUNDEF(intern->manager)) {
		zval_ptr_dtor(&intern->manager);
	}

	php_phongo_bson_typemap_dtor(&intern->visitor_data.map);

	php_phongo_cursor_free_current(intern);
}

static zend_object* php_phongo_cursor_create_object(zend_class_entry* class_type)
{
	php_phongo_cursor_t* intern = zend_object_alloc(sizeof(php_phongo_cursor_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	PHONGO_SET_CREATED_BY_PID(intern);

	intern->std.handlers = &php_phongo_handler_cursor;

	return &intern->std;
}

static HashTable* php_phongo_cursor_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_cursor_t* intern;
	zval                 retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_OBJ_CURSOR(object);

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

		phongo_server_init(&server, &intern->manager, intern->server_id);
		ADD_ASSOC_ZVAL_EX(&retval, "server", &server);
	}

	return Z_ARRVAL(retval);
}

void php_phongo_cursor_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_cursor_ce                = register_class_MongoDB_Driver_Cursor(zend_ce_iterator, php_phongo_cursor_interface_ce);
	php_phongo_cursor_ce->create_object = php_phongo_cursor_create_object;

	memcpy(&php_phongo_handler_cursor, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_cursor.get_debug_info = php_phongo_cursor_get_debug_info;
	php_phongo_handler_cursor.free_obj       = php_phongo_cursor_free_object;
	php_phongo_handler_cursor.offset         = XtOffsetOf(php_phongo_cursor_t, std);
}

static void phongo_cursor_init(zval* return_value, zval* manager, mongoc_cursor_t* cursor, zval* readPreference, zval* session)
{
	php_phongo_cursor_t* intern;

	object_init_ex(return_value, php_phongo_cursor_ce);

	intern            = Z_CURSOR_OBJ_P(return_value);
	intern->cursor    = cursor;
	intern->server_id = mongoc_cursor_get_server_id(cursor);
	intern->advanced  = false;
	intern->current   = 0;

	ZVAL_ZVAL(&intern->manager, manager, 1, 0);

	if (readPreference) {
		ZVAL_ZVAL(&intern->read_preference, readPreference, 1, 0);
	}

	if (session) {
		ZVAL_ZVAL(&intern->session, session, 1, 0);
	}
}

/* Initialize the cursor for a query and return whether there is an error. This
 * function always returns true. */
bool phongo_cursor_init_for_command(zval* return_value, zval* manager, mongoc_cursor_t* cursor, const char* db, zval* command, zval* readPreference, zval* session)
{
	php_phongo_cursor_t* intern;

	phongo_cursor_init(return_value, manager, cursor, readPreference, session);

	intern           = Z_CURSOR_OBJ_P(return_value);
	intern->database = estrdup(db);

	ZVAL_ZVAL(&intern->command, command, 1, 0);

	return true;
}

/* Initialize the cursor for a query and return whether there is an error. The
 * libmongoc cursor will be advanced once. On error, false is returned and an
 * exception is thrown. */
bool phongo_cursor_init_for_query(zval* return_value, zval* manager, mongoc_cursor_t* cursor, const char* namespace, zval* query, zval* readPreference, zval* session)
{
	php_phongo_cursor_t* intern;

	/* Advancing the cursor before phongo_cursor_init ensures that a server
	 * stream is obtained before mongoc_cursor_get_server_id() is called. */
	if (!phongo_cursor_advance_and_check_for_error(cursor)) {
		/* Exception should already have been thrown */
		return false;
	}

	phongo_cursor_init(return_value, manager, cursor, readPreference, session);

	intern           = Z_CURSOR_OBJ_P(return_value);
	intern->advanced = true;

	/* The namespace should already have been validated, but we'll still check
	 * for an error and throw accordingly. */
	if (!phongo_split_namespace(namespace, &intern->database, &intern->collection)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Cannot initialize cursor with invalid namespace: %s", namespace);
		zval_ptr_dtor(return_value);

		return false;
	}

	ZVAL_ZVAL(&intern->query, query, 1, 0);

	return true;
}

/* Advance the cursor and return whether there is an error. On error, false is
 * returned and an exception is thrown. */
bool phongo_cursor_advance_and_check_for_error(mongoc_cursor_t* cursor)
{
	const bson_t* doc = NULL;

	if (!mongoc_cursor_next(cursor, &doc)) {
		bson_error_t error = { 0 };

		/* Check for connection related exceptions */
		if (EG(exception)) {
			return false;
		}

		/* Could simply be no docs, which is not an error */
		if (mongoc_cursor_error_document(cursor, &error, &doc)) {
			phongo_throw_exception_from_bson_error_t_and_reply(&error, doc);
			return false;
		}
	}

	return true;
}
