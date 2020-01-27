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

#ifndef PHONGO_CLASSES_H
#define PHONGO_CLASSES_H

#include "php_phongo_structs.h"

/* Export zend_class_entry dependencies, which are initialized in MINIT */
extern zend_class_entry* php_phongo_date_immutable_ce;
extern zend_class_entry* php_phongo_json_serializable_ce;

#if PHP_VERSION_ID >= 70000

static inline php_phongo_bulkwrite_t* php_bulkwrite_fetch_object(zend_object* obj)
{
	return (php_phongo_bulkwrite_t*) ((char*) obj - XtOffsetOf(php_phongo_bulkwrite_t, std));
}
static inline php_phongo_clientencryption_t* php_clientencryption_fetch_object(zend_object* obj)
{
	return (php_phongo_clientencryption_t*) ((char*) obj - XtOffsetOf(php_phongo_clientencryption_t, std));
}
static inline php_phongo_command_t* php_command_fetch_object(zend_object* obj)
{
	return (php_phongo_command_t*) ((char*) obj - XtOffsetOf(php_phongo_command_t, std));
}
static inline php_phongo_cursor_t* php_cursor_fetch_object(zend_object* obj)
{
	return (php_phongo_cursor_t*) ((char*) obj - XtOffsetOf(php_phongo_cursor_t, std));
}
static inline php_phongo_cursorid_t* php_cursorid_fetch_object(zend_object* obj)
{
	return (php_phongo_cursorid_t*) ((char*) obj - XtOffsetOf(php_phongo_cursorid_t, std));
}
static inline php_phongo_manager_t* php_manager_fetch_object(zend_object* obj)
{
	return (php_phongo_manager_t*) ((char*) obj - XtOffsetOf(php_phongo_manager_t, std));
}
static inline php_phongo_query_t* php_query_fetch_object(zend_object* obj)
{
	return (php_phongo_query_t*) ((char*) obj - XtOffsetOf(php_phongo_query_t, std));
}
static inline php_phongo_readconcern_t* php_readconcern_fetch_object(zend_object* obj)
{
	return (php_phongo_readconcern_t*) ((char*) obj - XtOffsetOf(php_phongo_readconcern_t, std));
}
static inline php_phongo_readpreference_t* php_readpreference_fetch_object(zend_object* obj)
{
	return (php_phongo_readpreference_t*) ((char*) obj - XtOffsetOf(php_phongo_readpreference_t, std));
}
static inline php_phongo_server_t* php_server_fetch_object(zend_object* obj)
{
	return (php_phongo_server_t*) ((char*) obj - XtOffsetOf(php_phongo_server_t, std));
}
static inline php_phongo_session_t* php_session_fetch_object(zend_object* obj)
{
	return (php_phongo_session_t*) ((char*) obj - XtOffsetOf(php_phongo_session_t, std));
}
static inline php_phongo_writeconcern_t* php_writeconcern_fetch_object(zend_object* obj)
{
	return (php_phongo_writeconcern_t*) ((char*) obj - XtOffsetOf(php_phongo_writeconcern_t, std));
}
static inline php_phongo_writeconcernerror_t* php_writeconcernerror_fetch_object(zend_object* obj)
{
	return (php_phongo_writeconcernerror_t*) ((char*) obj - XtOffsetOf(php_phongo_writeconcernerror_t, std));
}
static inline php_phongo_writeerror_t* php_writeerror_fetch_object(zend_object* obj)
{
	return (php_phongo_writeerror_t*) ((char*) obj - XtOffsetOf(php_phongo_writeerror_t, std));
}
static inline php_phongo_writeresult_t* php_writeresult_fetch_object(zend_object* obj)
{
	return (php_phongo_writeresult_t*) ((char*) obj - XtOffsetOf(php_phongo_writeresult_t, std));
}
static inline php_phongo_binary_t* php_binary_fetch_object(zend_object* obj)
{
	return (php_phongo_binary_t*) ((char*) obj - XtOffsetOf(php_phongo_binary_t, std));
}
static inline php_phongo_dbpointer_t* php_dbpointer_fetch_object(zend_object* obj)
{
	return (php_phongo_dbpointer_t*) ((char*) obj - XtOffsetOf(php_phongo_dbpointer_t, std));
}
static inline php_phongo_decimal128_t* php_decimal128_fetch_object(zend_object* obj)
{
	return (php_phongo_decimal128_t*) ((char*) obj - XtOffsetOf(php_phongo_decimal128_t, std));
}
static inline php_phongo_int64_t* php_int64_fetch_object(zend_object* obj)
{
	return (php_phongo_int64_t*) ((char*) obj - XtOffsetOf(php_phongo_int64_t, std));
}
static inline php_phongo_javascript_t* php_javascript_fetch_object(zend_object* obj)
{
	return (php_phongo_javascript_t*) ((char*) obj - XtOffsetOf(php_phongo_javascript_t, std));
}
static inline php_phongo_maxkey_t* php_maxkey_fetch_object(zend_object* obj)
{
	return (php_phongo_maxkey_t*) ((char*) obj - XtOffsetOf(php_phongo_maxkey_t, std));
}
static inline php_phongo_minkey_t* php_minkey_fetch_object(zend_object* obj)
{
	return (php_phongo_minkey_t*) ((char*) obj - XtOffsetOf(php_phongo_minkey_t, std));
}
static inline php_phongo_objectid_t* php_objectid_fetch_object(zend_object* obj)
{
	return (php_phongo_objectid_t*) ((char*) obj - XtOffsetOf(php_phongo_objectid_t, std));
}
static inline php_phongo_regex_t* php_regex_fetch_object(zend_object* obj)
{
	return (php_phongo_regex_t*) ((char*) obj - XtOffsetOf(php_phongo_regex_t, std));
}
static inline php_phongo_symbol_t* php_symbol_fetch_object(zend_object* obj)
{
	return (php_phongo_symbol_t*) ((char*) obj - XtOffsetOf(php_phongo_symbol_t, std));
}
static inline php_phongo_timestamp_t* php_timestamp_fetch_object(zend_object* obj)
{
	return (php_phongo_timestamp_t*) ((char*) obj - XtOffsetOf(php_phongo_timestamp_t, std));
}
static inline php_phongo_undefined_t* php_undefined_fetch_object(zend_object* obj)
{
	return (php_phongo_undefined_t*) ((char*) obj - XtOffsetOf(php_phongo_undefined_t, std));
}
static inline php_phongo_utcdatetime_t* php_utcdatetime_fetch_object(zend_object* obj)
{
	return (php_phongo_utcdatetime_t*) ((char*) obj - XtOffsetOf(php_phongo_utcdatetime_t, std));
}
static inline php_phongo_commandfailedevent_t* php_commandfailedevent_fetch_object(zend_object* obj)
{
	return (php_phongo_commandfailedevent_t*) ((char*) obj - XtOffsetOf(php_phongo_commandfailedevent_t, std));
}
static inline php_phongo_commandstartedevent_t* php_commandstartedevent_fetch_object(zend_object* obj)
{
	return (php_phongo_commandstartedevent_t*) ((char*) obj - XtOffsetOf(php_phongo_commandstartedevent_t, std));
}
static inline php_phongo_commandsucceededevent_t* php_commandsucceededevent_fetch_object(zend_object* obj)
{
	return (php_phongo_commandsucceededevent_t*) ((char*) obj - XtOffsetOf(php_phongo_commandsucceededevent_t, std));
}

#define Z_CLIENTENCRYPTION_OBJ_P(zv) (php_clientencryption_fetch_object(Z_OBJ_P(zv)))
#define Z_COMMAND_OBJ_P(zv) (php_command_fetch_object(Z_OBJ_P(zv)))
#define Z_CURSOR_OBJ_P(zv) (php_cursor_fetch_object(Z_OBJ_P(zv)))
#define Z_CURSORID_OBJ_P(zv) (php_cursorid_fetch_object(Z_OBJ_P(zv)))
#define Z_MANAGER_OBJ_P(zv) (php_manager_fetch_object(Z_OBJ_P(zv)))
#define Z_QUERY_OBJ_P(zv) (php_query_fetch_object(Z_OBJ_P(zv)))
#define Z_READCONCERN_OBJ_P(zv) (php_readconcern_fetch_object(Z_OBJ_P(zv)))
#define Z_READPREFERENCE_OBJ_P(zv) (php_readpreference_fetch_object(Z_OBJ_P(zv)))
#define Z_SERVER_OBJ_P(zv) (php_server_fetch_object(Z_OBJ_P(zv)))
#define Z_SESSION_OBJ_P(zv) (php_session_fetch_object(Z_OBJ_P(zv)))
#define Z_BULKWRITE_OBJ_P(zv) (php_bulkwrite_fetch_object(Z_OBJ_P(zv)))
#define Z_WRITECONCERN_OBJ_P(zv) (php_writeconcern_fetch_object(Z_OBJ_P(zv)))
#define Z_WRITECONCERNERROR_OBJ_P(zv) (php_writeconcernerror_fetch_object(Z_OBJ_P(zv)))
#define Z_WRITEERROR_OBJ_P(zv) (php_writeerror_fetch_object(Z_OBJ_P(zv)))
#define Z_WRITERESULT_OBJ_P(zv) (php_writeresult_fetch_object(Z_OBJ_P(zv)))
#define Z_BINARY_OBJ_P(zv) (php_binary_fetch_object(Z_OBJ_P(zv)))
#define Z_DBPOINTER_OBJ_P(zv) (php_dbpointer_fetch_object(Z_OBJ_P(zv)))
#define Z_DECIMAL128_OBJ_P(zv) (php_decimal128_fetch_object(Z_OBJ_P(zv)))
#define Z_INT64_OBJ_P(zv) (php_int64_fetch_object(Z_OBJ_P(zv)))
#define Z_JAVASCRIPT_OBJ_P(zv) (php_javascript_fetch_object(Z_OBJ_P(zv)))
#define Z_MAXKEY_OBJ_P(zv) (php_maxkey_fetch_object(Z_OBJ_P(zv)))
#define Z_MINKEY_OBJ_P(zv) (php_minkey_fetch_object(Z_OBJ_P(zv)))
#define Z_OBJECTID_OBJ_P(zv) (php_objectid_fetch_object(Z_OBJ_P(zv)))
#define Z_REGEX_OBJ_P(zv) (php_regex_fetch_object(Z_OBJ_P(zv)))
#define Z_SYMBOL_OBJ_P(zv) (php_symbol_fetch_object(Z_OBJ_P(zv)))
#define Z_TIMESTAMP_OBJ_P(zv) (php_timestamp_fetch_object(Z_OBJ_P(zv)))
#define Z_UNDEFINED_OBJ_P(zv) (php_undefined_fetch_object(Z_OBJ_P(zv)))
#define Z_UTCDATETIME_OBJ_P(zv) (php_utcdatetime_fetch_object(Z_OBJ_P(zv)))
#define Z_COMMANDFAILEDEVENT_OBJ_P(zv) (php_commandfailedevent_fetch_object(Z_OBJ_P(zv)))
#define Z_COMMANDSTARTEDEVENT_OBJ_P(zv) (php_commandstartedevent_fetch_object(Z_OBJ_P(zv)))
#define Z_COMMANDSUCCEEDEDEVENT_OBJ_P(zv) (php_commandsucceededevent_fetch_object(Z_OBJ_P(zv)))

#define Z_OBJ_CLIENTENCRYPTION(zo) (php_clientencryption_fetch_object(zo))
#define Z_OBJ_COMMAND(zo) (php_command_fetch_object(zo))
#define Z_OBJ_CURSOR(zo) (php_cursor_fetch_object(zo))
#define Z_OBJ_CURSORID(zo) (php_cursorid_fetch_object(zo))
#define Z_OBJ_MANAGER(zo) (php_manager_fetch_object(zo))
#define Z_OBJ_QUERY(zo) (php_query_fetch_object(zo))
#define Z_OBJ_READCONCERN(zo) (php_readconcern_fetch_object(zo))
#define Z_OBJ_READPREFERENCE(zo) (php_readpreference_fetch_object(zo))
#define Z_OBJ_SERVER(zo) (php_server_fetch_object(zo))
#define Z_OBJ_SESSION(zo) (php_session_fetch_object(zo))
#define Z_OBJ_BULKWRITE(zo) (php_bulkwrite_fetch_object(zo))
#define Z_OBJ_WRITECONCERN(zo) (php_writeconcern_fetch_object(zo))
#define Z_OBJ_WRITECONCERNERROR(zo) (php_writeconcernerror_fetch_object(zo))
#define Z_OBJ_WRITEERROR(zo) (php_writeerror_fetch_object(zo))
#define Z_OBJ_WRITERESULT(zo) (php_writeresult_fetch_object(zo))
#define Z_OBJ_BINARY(zo) (php_binary_fetch_object(zo))
#define Z_OBJ_DBPOINTER(zo) (php_dbpointer_fetch_object(zo))
#define Z_OBJ_DECIMAL128(zo) (php_decimal128_fetch_object(zo))
#define Z_OBJ_INT64(zo) (php_int64_fetch_object(zo))
#define Z_OBJ_JAVASCRIPT(zo) (php_javascript_fetch_object(zo))
#define Z_OBJ_MAXKEY(zo) (php_maxkey_fetch_object(zo))
#define Z_OBJ_MINKEY(zo) (php_minkey_fetch_object(zo))
#define Z_OBJ_OBJECTID(zo) (php_objectid_fetch_object(zo))
#define Z_OBJ_REGEX(zo) (php_regex_fetch_object(zo))
#define Z_OBJ_SYMBOL(zo) (php_symbol_fetch_object(zo))
#define Z_OBJ_TIMESTAMP(zo) (php_timestamp_fetch_object(zo))
#define Z_OBJ_UNDEFINED(zo) (php_undefined_fetch_object(zo))
#define Z_OBJ_UTCDATETIME(zo) (php_utcdatetime_fetch_object(zo))
#define Z_OBJ_COMMANDFAILEDEVENT(zo) (php_commandfailedevent_fetch_object(zo))
#define Z_OBJ_COMMANDSTARTEDEVENT(zo) (php_commandstartedevent_fetch_object(zo))
#define Z_OBJ_COMMANDSUCCEEDEDEVENT(zo) (php_commandsucceededevent_fetch_object(zo))

#else /* PHP_VERSION_ID >= 70000 */

#define Z_CLIENTENCRYPTION_OBJ_P(zv) ((php_phongo_clientencryption_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_COMMAND_OBJ_P(zv) ((php_phongo_command_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_CURSOR_OBJ_P(zv) ((php_phongo_cursor_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_CURSORID_OBJ_P(zv) ((php_phongo_cursorid_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_MANAGER_OBJ_P(zv) ((php_phongo_manager_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_QUERY_OBJ_P(zv) ((php_phongo_query_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_READCONCERN_OBJ_P(zv) ((php_phongo_readconcern_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_READPREFERENCE_OBJ_P(zv) ((php_phongo_readpreference_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_SERVER_OBJ_P(zv) ((php_phongo_server_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_SESSION_OBJ_P(zv) ((php_phongo_session_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_BULKWRITE_OBJ_P(zv) ((php_phongo_bulkwrite_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_WRITECONCERN_OBJ_P(zv) ((php_phongo_writeconcern_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_WRITECONCERNERROR_OBJ_P(zv) ((php_phongo_writeconcernerror_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_WRITEERROR_OBJ_P(zv) ((php_phongo_writeerror_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_WRITERESULT_OBJ_P(zv) ((php_phongo_writeresult_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_BINARY_OBJ_P(zv) ((php_phongo_binary_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_DBPOINTER_OBJ_P(zv) ((php_phongo_dbpointer_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_DECIMAL128_OBJ_P(zv) ((php_phongo_decimal128_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_INT64_OBJ_P(zv) ((php_phongo_int64_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_JAVASCRIPT_OBJ_P(zv) ((php_phongo_javascript_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_MAXKEY_OBJ_P(zv) ((php_phongo_maxkey_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_MINKEY_OBJ_P(zv) ((php_phongo_minkey_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_OBJECTID_OBJ_P(zv) ((php_phongo_objectid_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_REGEX_OBJ_P(zv) ((php_phongo_regex_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_SYMBOL_OBJ_P(zv) ((php_phongo_symbol_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_TIMESTAMP_OBJ_P(zv) ((php_phongo_timestamp_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_UNDEFINED_OBJ_P(zv) ((php_phongo_undefined_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_UTCDATETIME_OBJ_P(zv) ((php_phongo_utcdatetime_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_COMMANDFAILEDEVENT_OBJ_P(zv) ((php_phongo_commandfailedevent_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_COMMANDSTARTEDEVENT_OBJ_P(zv) ((php_phongo_commandstartedevent_t*) zend_object_store_get_object(zv TSRMLS_CC))
#define Z_COMMANDSUCCEEDEDEVENT_OBJ_P(zv) ((php_phongo_commandsucceededevent_t*) zend_object_store_get_object(zv TSRMLS_CC))

#define Z_OBJ_CLIENTENCRYPTION(zo) ((php_phongo_clientencryption_t*) zo)
#define Z_OBJ_COMMAND(zo) ((php_phongo_command_t*) zo)
#define Z_OBJ_CURSOR(zo) ((php_phongo_cursor_t*) zo)
#define Z_OBJ_CURSORID(zo) ((php_phongo_cursorid_t*) zo)
#define Z_OBJ_MANAGER(zo) ((php_phongo_manager_t*) zo)
#define Z_OBJ_QUERY(zo) ((php_phongo_query_t*) zo)
#define Z_OBJ_READCONCERN(zo) ((php_phongo_readconcern_t*) zo)
#define Z_OBJ_READPREFERENCE(zo) ((php_phongo_readpreference_t*) zo)
#define Z_OBJ_SERVER(zo) ((php_phongo_server_t*) zo)
#define Z_OBJ_SESSION(zo) ((php_phongo_session_t*) zo)
#define Z_OBJ_BULKWRITE(zo) ((php_phongo_bulkwrite_t*) zo)
#define Z_OBJ_WRITECONCERN(zo) ((php_phongo_writeconcern_t*) zo)
#define Z_OBJ_WRITECONCERNERROR(zo) ((php_phongo_writeconcernerror_t*) zo)
#define Z_OBJ_WRITEERROR(zo) ((php_phongo_writeerror_t*) zo)
#define Z_OBJ_WRITERESULT(zo) ((php_phongo_writeresult_t*) zo)
#define Z_OBJ_BINARY(zo) ((php_phongo_binary_t*) zo)
#define Z_OBJ_DBPOINTER(zo) ((php_phongo_dbpointer_t*) zo)
#define Z_OBJ_DECIMAL128(zo) ((php_phongo_decimal128_t*) zo)
#define Z_OBJ_INT64(zo) ((php_phongo_int64_t*) zo)
#define Z_OBJ_JAVASCRIPT(zo) ((php_phongo_javascript_t*) zo)
#define Z_OBJ_MAXKEY(zo) ((php_phongo_maxkey_t*) zo)
#define Z_OBJ_MINKEY(zo) ((php_phongo_minkey_t*) zo)
#define Z_OBJ_OBJECTID(zo) ((php_phongo_objectid_t*) zo)
#define Z_OBJ_REGEX(zo) ((php_phongo_regex_t*) zo)
#define Z_OBJ_SYMBOL(zo) ((php_phongo_symbol_t*) zo)
#define Z_OBJ_TIMESTAMP(zo) ((php_phongo_timestamp_t*) zo)
#define Z_OBJ_UNDEFINED(zo) ((php_phongo_undefined_t*) zo)
#define Z_OBJ_UTCDATETIME(zo) ((php_phongo_utcdatetime_t*) zo)
#define Z_OBJ_COMMANDFAILEDEVENT(zo) ((php_phongo_commandfailedevent_t*) zo)
#define Z_OBJ_COMMANDSTARTEDEVENT(zo) ((php_phongo_commandstartedevent_t*) zo)
#define Z_OBJ_COMMANDSUCCEEDEDEVENT(zo) ((php_phongo_commandsucceededevent_t*) zo)

#endif /* PHP_VERSION_ID >= 70000 */

typedef struct {
	zend_object_iterator intern;
	php_phongo_cursor_t* cursor;
} php_phongo_cursor_iterator;

extern zend_class_entry* php_phongo_clientencryption_ce;
extern zend_class_entry* php_phongo_command_ce;
extern zend_class_entry* php_phongo_cursor_ce;
extern zend_class_entry* php_phongo_cursorid_ce;
extern zend_class_entry* php_phongo_manager_ce;
extern zend_class_entry* php_phongo_query_ce;
extern zend_class_entry* php_phongo_readconcern_ce;
extern zend_class_entry* php_phongo_readpreference_ce;
extern zend_class_entry* php_phongo_server_ce;
extern zend_class_entry* php_phongo_session_ce;
extern zend_class_entry* php_phongo_bulkwrite_ce;
extern zend_class_entry* php_phongo_writeconcern_ce;
extern zend_class_entry* php_phongo_writeconcernerror_ce;
extern zend_class_entry* php_phongo_writeerror_ce;
extern zend_class_entry* php_phongo_writeresult_ce;

extern zend_class_entry* php_phongo_cursor_interface_ce;

extern zend_class_entry* php_phongo_exception_ce;
extern zend_class_entry* php_phongo_logicexception_ce;
extern zend_class_entry* php_phongo_runtimeexception_ce;
extern zend_class_entry* php_phongo_serverexception_ce;
extern zend_class_entry* php_phongo_commandexception_ce;
extern zend_class_entry* php_phongo_unexpectedvalueexception_ce;
extern zend_class_entry* php_phongo_invalidargumentexception_ce;
extern zend_class_entry* php_phongo_connectionexception_ce;
extern zend_class_entry* php_phongo_authenticationexception_ce;
extern zend_class_entry* php_phongo_sslconnectionexception_ce;
extern zend_class_entry* php_phongo_encryptionexception_ce;
extern zend_class_entry* php_phongo_executiontimeoutexception_ce;
extern zend_class_entry* php_phongo_connectiontimeoutexception_ce;
extern zend_class_entry* php_phongo_writeexception_ce;
extern zend_class_entry* php_phongo_bulkwriteexception_ce;

extern zend_class_entry* php_phongo_type_ce;
extern zend_class_entry* php_phongo_persistable_ce;
extern zend_class_entry* php_phongo_unserializable_ce;
extern zend_class_entry* php_phongo_serializable_ce;
extern zend_class_entry* php_phongo_binary_ce;
extern zend_class_entry* php_phongo_dbpointer_ce;
extern zend_class_entry* php_phongo_decimal128_ce;
extern zend_class_entry* php_phongo_int64_ce;
extern zend_class_entry* php_phongo_javascript_ce;
extern zend_class_entry* php_phongo_maxkey_ce;
extern zend_class_entry* php_phongo_minkey_ce;
extern zend_class_entry* php_phongo_objectid_ce;
extern zend_class_entry* php_phongo_regex_ce;
extern zend_class_entry* php_phongo_symbol_ce;
extern zend_class_entry* php_phongo_timestamp_ce;
extern zend_class_entry* php_phongo_undefined_ce;
extern zend_class_entry* php_phongo_utcdatetime_ce;

extern zend_class_entry* php_phongo_binary_interface_ce;
extern zend_class_entry* php_phongo_decimal128_interface_ce;
extern zend_class_entry* php_phongo_javascript_interface_ce;
extern zend_class_entry* php_phongo_maxkey_interface_ce;
extern zend_class_entry* php_phongo_minkey_interface_ce;
extern zend_class_entry* php_phongo_objectid_interface_ce;
extern zend_class_entry* php_phongo_regex_interface_ce;
extern zend_class_entry* php_phongo_timestamp_interface_ce;
extern zend_class_entry* php_phongo_utcdatetime_interface_ce;

extern zend_class_entry* php_phongo_commandfailedevent_ce;
extern zend_class_entry* php_phongo_commandstartedevent_ce;
extern zend_class_entry* php_phongo_commandsubscriber_ce;
extern zend_class_entry* php_phongo_commandsucceededevent_ce;
extern zend_class_entry* php_phongo_subscriber_ce;

extern void php_phongo_binary_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_dbpointer_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_decimal128_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_int64_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_javascript_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_maxkey_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_minkey_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_objectid_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_persistable_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_regex_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_serializable_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_symbol_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_timestamp_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_type_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_undefined_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_unserializable_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_utcdatetime_init_ce(INIT_FUNC_ARGS);

extern void php_phongo_binary_interface_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_decimal128_interface_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_javascript_interface_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_maxkey_interface_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_minkey_interface_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_objectid_interface_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_regex_interface_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_timestamp_interface_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_utcdatetime_interface_init_ce(INIT_FUNC_ARGS);

extern void php_phongo_bulkwrite_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_clientencryption_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_command_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_cursor_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_cursorid_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_manager_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_query_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_readconcern_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_readpreference_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_server_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_session_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_writeconcern_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_writeconcernerror_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_writeerror_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_writeresult_init_ce(INIT_FUNC_ARGS);

extern void php_phongo_cursor_interface_init_ce(INIT_FUNC_ARGS);

extern void php_phongo_authenticationexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_bulkwriteexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_commandexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_connectionexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_connectiontimeoutexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_encryptionexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_exception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_executiontimeoutexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_invalidargumentexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_logicexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_runtimeexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_serverexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_sslconnectionexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_unexpectedvalueexception_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_writeexception_init_ce(INIT_FUNC_ARGS);

extern void php_phongo_commandfailedevent_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_commandstartedevent_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_commandsubscriber_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_commandsucceededevent_init_ce(INIT_FUNC_ARGS);
extern void php_phongo_subscriber_init_ce(INIT_FUNC_ARGS);

/* Shared function entries for disabling constructors and unserialize() */
PHP_FUNCTION(MongoDB_disabled___construct);
PHP_FUNCTION(MongoDB_disabled___wakeup);

#endif /* PHONGO_CLASSES_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
