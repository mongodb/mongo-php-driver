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

#ifndef PHONGO_CLASSES_H
#define PHONGO_CLASSES_H

#include "phongo_structs.h"

#define CLASS_FETCH_OBJ_DECL(name)                                                                 \
	static zend_always_inline phongo_##name##_t* php_##name##_fetch_object(const zend_object* obj) \
	{                                                                                              \
		return (phongo_##name##_t*) ((char*) obj - XtOffsetOf(phongo_##name##_t, std));            \
	}

/* Export zend_class_entry dependencies, which are initialized in MINIT */
extern zend_class_entry* phongo_json_serializable_ce;

CLASS_FETCH_OBJ_DECL(bulkwrite)
CLASS_FETCH_OBJ_DECL(bulkwritecommand)
CLASS_FETCH_OBJ_DECL(bulkwritecommandresult)
CLASS_FETCH_OBJ_DECL(clientencryption)
CLASS_FETCH_OBJ_DECL(command)
CLASS_FETCH_OBJ_DECL(cursor)
CLASS_FETCH_OBJ_DECL(manager)
CLASS_FETCH_OBJ_DECL(query)
CLASS_FETCH_OBJ_DECL(readconcern)
CLASS_FETCH_OBJ_DECL(readpreference)
CLASS_FETCH_OBJ_DECL(server)
CLASS_FETCH_OBJ_DECL(serverdescription)
CLASS_FETCH_OBJ_DECL(topologydescription)
CLASS_FETCH_OBJ_DECL(serverapi)
CLASS_FETCH_OBJ_DECL(session)
CLASS_FETCH_OBJ_DECL(writeconcern)
CLASS_FETCH_OBJ_DECL(writeconcernerror)
CLASS_FETCH_OBJ_DECL(writeerror)
CLASS_FETCH_OBJ_DECL(writeresult)
CLASS_FETCH_OBJ_DECL(binary)
CLASS_FETCH_OBJ_DECL(document)
CLASS_FETCH_OBJ_DECL(iterator)
CLASS_FETCH_OBJ_DECL(dbpointer)
CLASS_FETCH_OBJ_DECL(decimal128)
CLASS_FETCH_OBJ_DECL(int64)
CLASS_FETCH_OBJ_DECL(javascript)
CLASS_FETCH_OBJ_DECL(maxkey)
CLASS_FETCH_OBJ_DECL(minkey)
CLASS_FETCH_OBJ_DECL(objectid)
CLASS_FETCH_OBJ_DECL(packedarray)
CLASS_FETCH_OBJ_DECL(regex)
CLASS_FETCH_OBJ_DECL(symbol)
CLASS_FETCH_OBJ_DECL(timestamp)
CLASS_FETCH_OBJ_DECL(undefined)
CLASS_FETCH_OBJ_DECL(utcdatetime)
CLASS_FETCH_OBJ_DECL(commandfailedevent)
CLASS_FETCH_OBJ_DECL(commandstartedevent)
CLASS_FETCH_OBJ_DECL(commandsucceededevent)
CLASS_FETCH_OBJ_DECL(serverchangedevent)
CLASS_FETCH_OBJ_DECL(serverclosedevent)
CLASS_FETCH_OBJ_DECL(serverheartbeatfailedevent)
CLASS_FETCH_OBJ_DECL(serverheartbeatstartedevent)
CLASS_FETCH_OBJ_DECL(serverheartbeatsucceededevent)
CLASS_FETCH_OBJ_DECL(serveropeningevent)
CLASS_FETCH_OBJ_DECL(topologychangedevent)
CLASS_FETCH_OBJ_DECL(topologyclosedevent)
CLASS_FETCH_OBJ_DECL(topologyopeningevent)

#define Z_CLIENTENCRYPTION_OBJ_P(zv) (php_clientencryption_fetch_object(Z_OBJ_P(zv)))
#define Z_COMMAND_OBJ_P(zv) (php_command_fetch_object(Z_OBJ_P(zv)))
#define Z_CURSOR_OBJ_P(zv) (php_cursor_fetch_object(Z_OBJ_P(zv)))
#define Z_MANAGER_OBJ_P(zv) (php_manager_fetch_object(Z_OBJ_P(zv)))
#define Z_QUERY_OBJ_P(zv) (php_query_fetch_object(Z_OBJ_P(zv)))
#define Z_READCONCERN_OBJ_P(zv) (php_readconcern_fetch_object(Z_OBJ_P(zv)))
#define Z_READPREFERENCE_OBJ_P(zv) (php_readpreference_fetch_object(Z_OBJ_P(zv)))
#define Z_SERVER_OBJ_P(zv) (php_server_fetch_object(Z_OBJ_P(zv)))
#define Z_SERVERAPI_OBJ_P(zv) (php_serverapi_fetch_object(Z_OBJ_P(zv)))
#define Z_SERVERDESCRIPTION_OBJ_P(zv) (php_serverdescription_fetch_object(Z_OBJ_P(zv)))
#define Z_SESSION_OBJ_P(zv) (php_session_fetch_object(Z_OBJ_P(zv)))
#define Z_TOPOLOGYDESCRIPTION_OBJ_P(zv) (php_topologydescription_fetch_object(Z_OBJ_P(zv)))
#define Z_BULKWRITE_OBJ_P(zv) (php_bulkwrite_fetch_object(Z_OBJ_P(zv)))
#define Z_BULKWRITECOMMAND_OBJ_P(zv) (php_bulkwritecommand_fetch_object(Z_OBJ_P(zv)))
#define Z_BULKWRITECOMMANDRESULT_OBJ_P(zv) (php_bulkwritecommandresult_fetch_object(Z_OBJ_P(zv)))
#define Z_WRITECONCERN_OBJ_P(zv) (php_writeconcern_fetch_object(Z_OBJ_P(zv)))
#define Z_WRITECONCERNERROR_OBJ_P(zv) (php_writeconcernerror_fetch_object(Z_OBJ_P(zv)))
#define Z_WRITEERROR_OBJ_P(zv) (php_writeerror_fetch_object(Z_OBJ_P(zv)))
#define Z_WRITERESULT_OBJ_P(zv) (php_writeresult_fetch_object(Z_OBJ_P(zv)))
#define Z_BINARY_OBJ_P(zv) (php_binary_fetch_object(Z_OBJ_P(zv)))
#define Z_DOCUMENT_OBJ_P(zv) (php_document_fetch_object(Z_OBJ_P(zv)))
#define Z_ITERATOR_OBJ_P(zv) (php_iterator_fetch_object(Z_OBJ_P(zv)))
#define Z_DBPOINTER_OBJ_P(zv) (php_dbpointer_fetch_object(Z_OBJ_P(zv)))
#define Z_DECIMAL128_OBJ_P(zv) (php_decimal128_fetch_object(Z_OBJ_P(zv)))
#define Z_INT64_OBJ_P(zv) (php_int64_fetch_object(Z_OBJ_P(zv)))
#define Z_JAVASCRIPT_OBJ_P(zv) (php_javascript_fetch_object(Z_OBJ_P(zv)))
#define Z_MAXKEY_OBJ_P(zv) (php_maxkey_fetch_object(Z_OBJ_P(zv)))
#define Z_MINKEY_OBJ_P(zv) (php_minkey_fetch_object(Z_OBJ_P(zv)))
#define Z_OBJECTID_OBJ_P(zv) (php_objectid_fetch_object(Z_OBJ_P(zv)))
#define Z_PACKEDARRAY_OBJ_P(zv) (php_packedarray_fetch_object(Z_OBJ_P(zv)))
#define Z_REGEX_OBJ_P(zv) (php_regex_fetch_object(Z_OBJ_P(zv)))
#define Z_SYMBOL_OBJ_P(zv) (php_symbol_fetch_object(Z_OBJ_P(zv)))
#define Z_TIMESTAMP_OBJ_P(zv) (php_timestamp_fetch_object(Z_OBJ_P(zv)))
#define Z_UNDEFINED_OBJ_P(zv) (php_undefined_fetch_object(Z_OBJ_P(zv)))
#define Z_UTCDATETIME_OBJ_P(zv) (php_utcdatetime_fetch_object(Z_OBJ_P(zv)))
#define Z_COMMANDFAILEDEVENT_OBJ_P(zv) (php_commandfailedevent_fetch_object(Z_OBJ_P(zv)))
#define Z_COMMANDSTARTEDEVENT_OBJ_P(zv) (php_commandstartedevent_fetch_object(Z_OBJ_P(zv)))
#define Z_COMMANDSUCCEEDEDEVENT_OBJ_P(zv) (php_commandsucceededevent_fetch_object(Z_OBJ_P(zv)))
#define Z_SERVERCHANGEDEVENT_OBJ_P(zv) (php_serverchangedevent_fetch_object(Z_OBJ_P(zv)))
#define Z_SERVERCLOSEDEVENT_OBJ_P(zv) (php_serverclosedevent_fetch_object(Z_OBJ_P(zv)))
#define Z_SERVERHEARTBEATFAILEDEVENT_OBJ_P(zv) (php_serverheartbeatfailedevent_fetch_object(Z_OBJ_P(zv)))
#define Z_SERVERHEARTBEATSTARTEDEVENT_OBJ_P(zv) (php_serverheartbeatstartedevent_fetch_object(Z_OBJ_P(zv)))
#define Z_SERVERHEARTBEATSUCCEEDEDEVENT_OBJ_P(zv) (php_serverheartbeatsucceededevent_fetch_object(Z_OBJ_P(zv)))
#define Z_SERVEROPENINGEVENT_OBJ_P(zv) (php_serveropeningevent_fetch_object(Z_OBJ_P(zv)))
#define Z_TOPOLOGYCHANGEDEVENT_OBJ_P(zv) (php_topologychangedevent_fetch_object(Z_OBJ_P(zv)))
#define Z_TOPOLOGYCLOSEDEVENT_OBJ_P(zv) (php_topologyclosedevent_fetch_object(Z_OBJ_P(zv)))
#define Z_TOPOLOGYOPENINGEVENT_OBJ_P(zv) (php_topologyopeningevent_fetch_object(Z_OBJ_P(zv)))

#define Z_OBJ_CLIENTENCRYPTION(zo) (php_clientencryption_fetch_object(zo))
#define Z_OBJ_COMMAND(zo) (php_command_fetch_object(zo))
#define Z_OBJ_CURSOR(zo) (php_cursor_fetch_object(zo))
#define Z_OBJ_MANAGER(zo) (php_manager_fetch_object(zo))
#define Z_OBJ_QUERY(zo) (php_query_fetch_object(zo))
#define Z_OBJ_READCONCERN(zo) (php_readconcern_fetch_object(zo))
#define Z_OBJ_READPREFERENCE(zo) (php_readpreference_fetch_object(zo))
#define Z_OBJ_SERVER(zo) (php_server_fetch_object(zo))
#define Z_OBJ_SERVERAPI(zo) (php_serverapi_fetch_object(zo))
#define Z_OBJ_SERVERDESCRIPTION(zo) (php_serverdescription_fetch_object(zo))
#define Z_OBJ_SESSION(zo) (php_session_fetch_object(zo))
#define Z_OBJ_TOPOLOGYDESCRIPTION(zo) (php_topologydescription_fetch_object(zo))
#define Z_OBJ_BULKWRITE(zo) (php_bulkwrite_fetch_object(zo))
#define Z_OBJ_BULKWRITECOMMAND(zo) (php_bulkwritecommand_fetch_object(zo))
#define Z_OBJ_BULKWRITECOMMANDRESULT(zo) (php_bulkwritecommandresult_fetch_object(zo))
#define Z_OBJ_WRITECONCERN(zo) (php_writeconcern_fetch_object(zo))
#define Z_OBJ_WRITECONCERNERROR(zo) (php_writeconcernerror_fetch_object(zo))
#define Z_OBJ_WRITEERROR(zo) (php_writeerror_fetch_object(zo))
#define Z_OBJ_WRITERESULT(zo) (php_writeresult_fetch_object(zo))
#define Z_OBJ_BINARY(zo) (php_binary_fetch_object(zo))
#define Z_OBJ_DOCUMENT(zo) (php_document_fetch_object(zo))
#define Z_OBJ_ITERATOR(zo) (php_iterator_fetch_object(zo))
#define Z_OBJ_DBPOINTER(zo) (php_dbpointer_fetch_object(zo))
#define Z_OBJ_DECIMAL128(zo) (php_decimal128_fetch_object(zo))
#define Z_OBJ_INT64(zo) (php_int64_fetch_object(zo))
#define Z_OBJ_JAVASCRIPT(zo) (php_javascript_fetch_object(zo))
#define Z_OBJ_MAXKEY(zo) (php_maxkey_fetch_object(zo))
#define Z_OBJ_MINKEY(zo) (php_minkey_fetch_object(zo))
#define Z_OBJ_OBJECTID(zo) (php_objectid_fetch_object(zo))
#define Z_OBJ_PACKEDARRAY(zo) (php_packedarray_fetch_object(zo))
#define Z_OBJ_REGEX(zo) (php_regex_fetch_object(zo))
#define Z_OBJ_SYMBOL(zo) (php_symbol_fetch_object(zo))
#define Z_OBJ_TIMESTAMP(zo) (php_timestamp_fetch_object(zo))
#define Z_OBJ_UNDEFINED(zo) (php_undefined_fetch_object(zo))
#define Z_OBJ_UTCDATETIME(zo) (php_utcdatetime_fetch_object(zo))
#define Z_OBJ_COMMANDFAILEDEVENT(zo) (php_commandfailedevent_fetch_object(zo))
#define Z_OBJ_COMMANDSTARTEDEVENT(zo) (php_commandstartedevent_fetch_object(zo))
#define Z_OBJ_COMMANDSUCCEEDEDEVENT(zo) (php_commandsucceededevent_fetch_object(zo))
#define Z_OBJ_SERVERCHANGEDEVENT(zo) (php_serverchangedevent_fetch_object(zo))
#define Z_OBJ_SERVERCLOSEDEVENT(zo) (php_serverclosedevent_fetch_object(zo))
#define Z_OBJ_SERVERHEARTBEATFAILEDEVENT(zo) (php_serverheartbeatfailedevent_fetch_object(zo))
#define Z_OBJ_SERVERHEARTBEATSTARTEDEVENT(zo) (php_serverheartbeatstartedevent_fetch_object(zo))
#define Z_OBJ_SERVERHEARTBEATSUCCEEDEDEVENT(zo) (php_serverheartbeatsucceededevent_fetch_object(zo))
#define Z_OBJ_SERVEROPENINGEVENT(zo) (php_serveropeningevent_fetch_object(zo))
#define Z_OBJ_TOPOLOGYCHANGEDEVENT(zo) (php_topologychangedevent_fetch_object(zo))
#define Z_OBJ_TOPOLOGYCLOSEDEVENT(zo) (php_topologyclosedevent_fetch_object(zo))
#define Z_OBJ_TOPOLOGYOPENINGEVENT(zo) (php_topologyopeningevent_fetch_object(zo))

extern zend_class_entry* phongo_clientencryption_ce;
extern zend_class_entry* phongo_command_ce;
extern zend_class_entry* phongo_cursor_ce;
extern zend_class_entry* phongo_manager_ce;
extern zend_class_entry* phongo_query_ce;
extern zend_class_entry* phongo_readconcern_ce;
extern zend_class_entry* phongo_readpreference_ce;
extern zend_class_entry* phongo_server_ce;
extern zend_class_entry* phongo_serverapi_ce;
extern zend_class_entry* phongo_serverdescription_ce;
extern zend_class_entry* phongo_session_ce;
extern zend_class_entry* phongo_topologydescription_ce;
extern zend_class_entry* phongo_bulkwrite_ce;
extern zend_class_entry* phongo_bulkwritecommand_ce;
extern zend_class_entry* phongo_bulkwritecommandresult_ce;
extern zend_class_entry* phongo_writeconcern_ce;
extern zend_class_entry* phongo_writeconcernerror_ce;
extern zend_class_entry* phongo_writeerror_ce;
extern zend_class_entry* phongo_writeresult_ce;

extern zend_class_entry* phongo_cursor_interface_ce;

extern zend_class_entry* phongo_exception_ce;
extern zend_class_entry* phongo_logicexception_ce;
extern zend_class_entry* phongo_runtimeexception_ce;
extern zend_class_entry* phongo_serverexception_ce;
extern zend_class_entry* phongo_commandexception_ce;
extern zend_class_entry* phongo_unexpectedvalueexception_ce;
extern zend_class_entry* phongo_invalidargumentexception_ce;
extern zend_class_entry* phongo_connectionexception_ce;
extern zend_class_entry* phongo_authenticationexception_ce;
extern zend_class_entry* phongo_encryptionexception_ce;
extern zend_class_entry* phongo_executiontimeoutexception_ce;
extern zend_class_entry* phongo_connectiontimeoutexception_ce;
extern zend_class_entry* phongo_bulkwriteexception_ce;
extern zend_class_entry* phongo_bulkwritecommandexception_ce;

extern zend_class_entry* phongo_type_ce;
extern zend_class_entry* phongo_persistable_ce;
extern zend_class_entry* phongo_unserializable_ce;
extern zend_class_entry* phongo_serializable_ce;
extern zend_class_entry* phongo_binary_ce;
extern zend_class_entry* phongo_document_ce;
extern zend_class_entry* phongo_iterator_ce;
extern zend_class_entry* phongo_dbpointer_ce;
extern zend_class_entry* phongo_decimal128_ce;
extern zend_class_entry* phongo_int64_ce;
extern zend_class_entry* phongo_javascript_ce;
extern zend_class_entry* phongo_maxkey_ce;
extern zend_class_entry* phongo_minkey_ce;
extern zend_class_entry* phongo_objectid_ce;
extern zend_class_entry* phongo_packedarray_ce;
extern zend_class_entry* phongo_regex_ce;
extern zend_class_entry* phongo_symbol_ce;
extern zend_class_entry* phongo_timestamp_ce;
extern zend_class_entry* phongo_undefined_ce;
extern zend_class_entry* phongo_utcdatetime_ce;
extern zend_class_entry* phongo_vectortype_ce;

extern zend_class_entry* phongo_binary_interface_ce;
extern zend_class_entry* phongo_decimal128_interface_ce;
extern zend_class_entry* phongo_javascript_interface_ce;
extern zend_class_entry* phongo_maxkey_interface_ce;
extern zend_class_entry* phongo_minkey_interface_ce;
extern zend_class_entry* phongo_objectid_interface_ce;
extern zend_class_entry* phongo_regex_interface_ce;
extern zend_class_entry* phongo_timestamp_interface_ce;
extern zend_class_entry* phongo_utcdatetime_interface_ce;

extern zend_class_entry* phongo_commandfailedevent_ce;
extern zend_class_entry* phongo_commandstartedevent_ce;
extern zend_class_entry* phongo_commandsubscriber_ce;
extern zend_class_entry* phongo_commandsucceededevent_ce;
extern zend_class_entry* phongo_logsubscriber_ce;
extern zend_class_entry* phongo_sdamsubscriber_ce;
extern zend_class_entry* phongo_subscriber_ce;
extern zend_class_entry* phongo_serverchangedevent_ce;
extern zend_class_entry* phongo_serverclosedevent_ce;
extern zend_class_entry* phongo_serverheartbeatfailedevent_ce;
extern zend_class_entry* phongo_serverheartbeatstartedevent_ce;
extern zend_class_entry* phongo_serverheartbeatsucceededevent_ce;
extern zend_class_entry* phongo_serveropeningevent_ce;
extern zend_class_entry* phongo_topologychangedevent_ce;
extern zend_class_entry* phongo_topologyclosedevent_ce;
extern zend_class_entry* phongo_topologyopeningevent_ce;

extern void phongo_binary_init_ce(INIT_FUNC_ARGS);
extern void phongo_packedarray_init_ce(INIT_FUNC_ARGS);
extern void phongo_document_init_ce(INIT_FUNC_ARGS);
extern void phongo_iterator_init_ce(INIT_FUNC_ARGS);
extern void phongo_dbpointer_init_ce(INIT_FUNC_ARGS);
extern void phongo_decimal128_init_ce(INIT_FUNC_ARGS);
extern void phongo_int64_init_ce(INIT_FUNC_ARGS);
extern void phongo_javascript_init_ce(INIT_FUNC_ARGS);
extern void phongo_maxkey_init_ce(INIT_FUNC_ARGS);
extern void phongo_minkey_init_ce(INIT_FUNC_ARGS);
extern void phongo_objectid_init_ce(INIT_FUNC_ARGS);
extern void phongo_persistable_init_ce(INIT_FUNC_ARGS);
extern void phongo_regex_init_ce(INIT_FUNC_ARGS);
extern void phongo_serializable_init_ce(INIT_FUNC_ARGS);
extern void phongo_symbol_init_ce(INIT_FUNC_ARGS);
extern void phongo_timestamp_init_ce(INIT_FUNC_ARGS);
extern void phongo_type_init_ce(INIT_FUNC_ARGS);
extern void phongo_undefined_init_ce(INIT_FUNC_ARGS);
extern void phongo_unserializable_init_ce(INIT_FUNC_ARGS);
extern void phongo_utcdatetime_init_ce(INIT_FUNC_ARGS);
extern void phongo_vectortype_init_ce(INIT_FUNC_ARGS);

extern void phongo_binary_interface_init_ce(INIT_FUNC_ARGS);
extern void phongo_decimal128_interface_init_ce(INIT_FUNC_ARGS);
extern void phongo_javascript_interface_init_ce(INIT_FUNC_ARGS);
extern void phongo_maxkey_interface_init_ce(INIT_FUNC_ARGS);
extern void phongo_minkey_interface_init_ce(INIT_FUNC_ARGS);
extern void phongo_objectid_interface_init_ce(INIT_FUNC_ARGS);
extern void phongo_regex_interface_init_ce(INIT_FUNC_ARGS);
extern void phongo_timestamp_interface_init_ce(INIT_FUNC_ARGS);
extern void phongo_utcdatetime_interface_init_ce(INIT_FUNC_ARGS);

extern void phongo_bulkwrite_init_ce(INIT_FUNC_ARGS);
extern void phongo_bulkwritecommand_init_ce(INIT_FUNC_ARGS);
extern void phongo_bulkwritecommandresult_init_ce(INIT_FUNC_ARGS);
extern void phongo_clientencryption_init_ce(INIT_FUNC_ARGS);
extern void phongo_command_init_ce(INIT_FUNC_ARGS);
extern void phongo_cursor_init_ce(INIT_FUNC_ARGS);
extern void phongo_manager_init_ce(INIT_FUNC_ARGS);
extern void phongo_query_init_ce(INIT_FUNC_ARGS);
extern void phongo_readconcern_init_ce(INIT_FUNC_ARGS);
extern void phongo_readpreference_init_ce(INIT_FUNC_ARGS);
extern void phongo_server_init_ce(INIT_FUNC_ARGS);
extern void phongo_serverapi_init_ce(INIT_FUNC_ARGS);
extern void phongo_serverdescription_init_ce(INIT_FUNC_ARGS);
extern void phongo_session_init_ce(INIT_FUNC_ARGS);
extern void phongo_topologydescription_init_ce(INIT_FUNC_ARGS);
extern void phongo_writeconcern_init_ce(INIT_FUNC_ARGS);
extern void phongo_writeconcernerror_init_ce(INIT_FUNC_ARGS);
extern void phongo_writeerror_init_ce(INIT_FUNC_ARGS);
extern void phongo_writeresult_init_ce(INIT_FUNC_ARGS);

extern void phongo_cursor_interface_init_ce(INIT_FUNC_ARGS);

extern void phongo_authenticationexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_bulkwriteexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_bulkwritecommandexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_commandexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_connectionexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_connectiontimeoutexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_encryptionexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_exception_init_ce(INIT_FUNC_ARGS);
extern void phongo_executiontimeoutexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_invalidargumentexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_logicexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_runtimeexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_serverexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_sslconnectionexception_init_ce(INIT_FUNC_ARGS);
extern void phongo_unexpectedvalueexception_init_ce(INIT_FUNC_ARGS);

extern void phongo_commandfailedevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_commandstartedevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_commandsubscriber_init_ce(INIT_FUNC_ARGS);
extern void phongo_commandsucceededevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_logsubscriber_init_ce(INIT_FUNC_ARGS);
extern void phongo_sdamsubscriber_init_ce(INIT_FUNC_ARGS);
extern void phongo_subscriber_init_ce(INIT_FUNC_ARGS);
extern void phongo_serverchangedevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_serverclosedevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_serverheartbeatfailedevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_serverheartbeatstartedevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_serverheartbeatsucceededevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_serveropeningevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_topologychangedevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_topologyclosedevent_init_ce(INIT_FUNC_ARGS);
extern void phongo_topologyopeningevent_init_ce(INIT_FUNC_ARGS);

#endif /* PHONGO_CLASSES_H */
