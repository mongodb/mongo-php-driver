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

#define CLASS_ENTRY_DECL(name) extern zend_class_entry* phongo_##name##_ce

#define CE_INIT_FUNC_DECL(name) extern void phongo_##name##_init_ce(INIT_FUNC_ARGS)

#define Z_OBJ_P_FETCH_DECL(ucname, name)                                            \
	static zend_always_inline phongo_##name##_t* Z_##ucname##_OBJ_P(const zval* zv) \
	{                                                                               \
		return php_##name##_fetch_object(Z_OBJ_P(zv));                              \
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

Z_OBJ_P_FETCH_DECL(CLIENTENCRYPTION, clientencryption)
Z_OBJ_P_FETCH_DECL(COMMAND, command)
Z_OBJ_P_FETCH_DECL(CURSOR, cursor)
Z_OBJ_P_FETCH_DECL(MANAGER, manager)
Z_OBJ_P_FETCH_DECL(QUERY, query)
Z_OBJ_P_FETCH_DECL(READCONCERN, readconcern)
Z_OBJ_P_FETCH_DECL(READPREFERENCE, readpreference)
Z_OBJ_P_FETCH_DECL(SERVER, server)
Z_OBJ_P_FETCH_DECL(SERVERAPI, serverapi)
Z_OBJ_P_FETCH_DECL(SERVERDESCRIPTION, serverdescription)
Z_OBJ_P_FETCH_DECL(SESSION, session)
Z_OBJ_P_FETCH_DECL(TOPOLOGYDESCRIPTION, topologydescription)
Z_OBJ_P_FETCH_DECL(BULKWRITE, bulkwrite)
Z_OBJ_P_FETCH_DECL(BULKWRITECOMMAND, bulkwritecommand)
Z_OBJ_P_FETCH_DECL(BULKWRITECOMMANDRESULT, bulkwritecommandresult)
Z_OBJ_P_FETCH_DECL(WRITECONCERN, writeconcern)
Z_OBJ_P_FETCH_DECL(WRITECONCERNERROR, writeconcernerror)
Z_OBJ_P_FETCH_DECL(WRITEERROR, writeerror)
Z_OBJ_P_FETCH_DECL(WRITERESULT, writeresult)
Z_OBJ_P_FETCH_DECL(BINARY, binary)
Z_OBJ_P_FETCH_DECL(DOCUMENT, document)
Z_OBJ_P_FETCH_DECL(ITERATOR, iterator)
Z_OBJ_P_FETCH_DECL(DBPOINTER, dbpointer)
Z_OBJ_P_FETCH_DECL(DECIMAL128, decimal128)
Z_OBJ_P_FETCH_DECL(INT64, int64)
Z_OBJ_P_FETCH_DECL(JAVASCRIPT, javascript)
Z_OBJ_P_FETCH_DECL(MAXKEY, maxkey)
Z_OBJ_P_FETCH_DECL(MINKEY, minkey)
Z_OBJ_P_FETCH_DECL(OBJECTID, objectid)
Z_OBJ_P_FETCH_DECL(PACKEDARRAY, packedarray)
Z_OBJ_P_FETCH_DECL(REGEX, regex)
Z_OBJ_P_FETCH_DECL(SYMBOL, symbol)
Z_OBJ_P_FETCH_DECL(TIMESTAMP, timestamp)
Z_OBJ_P_FETCH_DECL(UNDEFINED, undefined)
Z_OBJ_P_FETCH_DECL(UTCDATETIME, utcdatetime)
Z_OBJ_P_FETCH_DECL(COMMANDFAILEDEVENT, commandfailedevent)
Z_OBJ_P_FETCH_DECL(COMMANDSTARTEDEVENT, commandstartedevent)
Z_OBJ_P_FETCH_DECL(COMMANDSUCCEEDEDEVENT, commandsucceededevent)
Z_OBJ_P_FETCH_DECL(SERVERCHANGEDEVENT, serverchangedevent)
Z_OBJ_P_FETCH_DECL(SERVERCLOSEDEVENT, serverclosedevent)
Z_OBJ_P_FETCH_DECL(SERVERHEARTBEATFAILEDEVENT, serverheartbeatfailedevent)
Z_OBJ_P_FETCH_DECL(SERVERHEARTBEATSTARTEDEVENT, serverheartbeatstartedevent)
Z_OBJ_P_FETCH_DECL(SERVERHEARTBEATSUCCEEDEDEVENT, serverheartbeatsucceededevent)
Z_OBJ_P_FETCH_DECL(SERVEROPENINGEVENT, serveropeningevent)
Z_OBJ_P_FETCH_DECL(TOPOLOGYCHANGEDEVENT, topologychangedevent)
Z_OBJ_P_FETCH_DECL(TOPOLOGYCLOSEDEVENT, topologyclosedevent)
Z_OBJ_P_FETCH_DECL(TOPOLOGYOPENINGEVENT, topologyopeningevent)

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

CLASS_ENTRY_DECL(clientencryption);
CLASS_ENTRY_DECL(command);
CLASS_ENTRY_DECL(cursor);
CLASS_ENTRY_DECL(manager);
CLASS_ENTRY_DECL(query);
CLASS_ENTRY_DECL(readconcern);
CLASS_ENTRY_DECL(readpreference);
CLASS_ENTRY_DECL(server);
CLASS_ENTRY_DECL(serverapi);
CLASS_ENTRY_DECL(serverdescription);
CLASS_ENTRY_DECL(session);
CLASS_ENTRY_DECL(topologydescription);
CLASS_ENTRY_DECL(bulkwrite);
CLASS_ENTRY_DECL(bulkwritecommand);
CLASS_ENTRY_DECL(bulkwritecommandresult);
CLASS_ENTRY_DECL(writeconcern);
CLASS_ENTRY_DECL(writeconcernerror);
CLASS_ENTRY_DECL(writeerror);
CLASS_ENTRY_DECL(writeresult);

CLASS_ENTRY_DECL(cursor_interface);

CLASS_ENTRY_DECL(exception);
CLASS_ENTRY_DECL(logicexception);
CLASS_ENTRY_DECL(runtimeexception);
CLASS_ENTRY_DECL(serverexception);
CLASS_ENTRY_DECL(commandexception);
CLASS_ENTRY_DECL(unexpectedvalueexception);
CLASS_ENTRY_DECL(invalidargumentexception);
CLASS_ENTRY_DECL(connectionexception);
CLASS_ENTRY_DECL(authenticationexception);
CLASS_ENTRY_DECL(encryptionexception);
CLASS_ENTRY_DECL(executiontimeoutexception);
CLASS_ENTRY_DECL(connectiontimeoutexception);
CLASS_ENTRY_DECL(bulkwriteexception);
CLASS_ENTRY_DECL(bulkwritecommandexception);

CLASS_ENTRY_DECL(type);
CLASS_ENTRY_DECL(persistable);
CLASS_ENTRY_DECL(unserializable);
CLASS_ENTRY_DECL(serializable);
CLASS_ENTRY_DECL(binary);
CLASS_ENTRY_DECL(document);
CLASS_ENTRY_DECL(iterator);
CLASS_ENTRY_DECL(dbpointer);
CLASS_ENTRY_DECL(decimal128);
CLASS_ENTRY_DECL(int64);
CLASS_ENTRY_DECL(javascript);
CLASS_ENTRY_DECL(maxkey);
CLASS_ENTRY_DECL(minkey);
CLASS_ENTRY_DECL(objectid);
CLASS_ENTRY_DECL(packedarray);
CLASS_ENTRY_DECL(regex);
CLASS_ENTRY_DECL(symbol);
CLASS_ENTRY_DECL(timestamp);
CLASS_ENTRY_DECL(undefined);
CLASS_ENTRY_DECL(utcdatetime);
CLASS_ENTRY_DECL(vectortype);

CLASS_ENTRY_DECL(binary_interface);
CLASS_ENTRY_DECL(decimal128_interface);
CLASS_ENTRY_DECL(javascript_interface);
CLASS_ENTRY_DECL(maxkey_interface);
CLASS_ENTRY_DECL(minkey_interface);
CLASS_ENTRY_DECL(objectid_interface);
CLASS_ENTRY_DECL(regex_interface);
CLASS_ENTRY_DECL(timestamp_interface);
CLASS_ENTRY_DECL(utcdatetime_interface);

CLASS_ENTRY_DECL(commandfailedevent);
CLASS_ENTRY_DECL(commandstartedevent);
CLASS_ENTRY_DECL(commandsubscriber);
CLASS_ENTRY_DECL(commandsucceededevent);
CLASS_ENTRY_DECL(logsubscriber);
CLASS_ENTRY_DECL(sdamsubscriber);
CLASS_ENTRY_DECL(subscriber);
CLASS_ENTRY_DECL(serverchangedevent);
CLASS_ENTRY_DECL(serverclosedevent);
CLASS_ENTRY_DECL(serverheartbeatfailedevent);
CLASS_ENTRY_DECL(serverheartbeatstartedevent);
CLASS_ENTRY_DECL(serverheartbeatsucceededevent);
CLASS_ENTRY_DECL(serveropeningevent);
CLASS_ENTRY_DECL(topologychangedevent);
CLASS_ENTRY_DECL(topologyclosedevent);
CLASS_ENTRY_DECL(topologyopeningevent);

CE_INIT_FUNC_DECL(binary);
CE_INIT_FUNC_DECL(packedarray);
CE_INIT_FUNC_DECL(document);
CE_INIT_FUNC_DECL(iterator);
CE_INIT_FUNC_DECL(dbpointer);
CE_INIT_FUNC_DECL(decimal128);
CE_INIT_FUNC_DECL(int64);
CE_INIT_FUNC_DECL(javascript);
CE_INIT_FUNC_DECL(maxkey);
CE_INIT_FUNC_DECL(minkey);
CE_INIT_FUNC_DECL(objectid);
CE_INIT_FUNC_DECL(persistable);
CE_INIT_FUNC_DECL(regex);
CE_INIT_FUNC_DECL(serializable);
CE_INIT_FUNC_DECL(symbol);
CE_INIT_FUNC_DECL(timestamp);
CE_INIT_FUNC_DECL(type);
CE_INIT_FUNC_DECL(undefined);
CE_INIT_FUNC_DECL(unserializable);
CE_INIT_FUNC_DECL(utcdatetime);
CE_INIT_FUNC_DECL(vectortype);

CE_INIT_FUNC_DECL(binary_interface);
CE_INIT_FUNC_DECL(decimal128_interface);
CE_INIT_FUNC_DECL(javascript_interface);
CE_INIT_FUNC_DECL(maxkey_interface);
CE_INIT_FUNC_DECL(minkey_interface);
CE_INIT_FUNC_DECL(objectid_interface);
CE_INIT_FUNC_DECL(regex_interface);
CE_INIT_FUNC_DECL(timestamp_interface);
CE_INIT_FUNC_DECL(utcdatetime_interface);

CE_INIT_FUNC_DECL(bulkwrite);
CE_INIT_FUNC_DECL(bulkwritecommand);
CE_INIT_FUNC_DECL(bulkwritecommandresult);
CE_INIT_FUNC_DECL(clientencryption);
CE_INIT_FUNC_DECL(command);
CE_INIT_FUNC_DECL(cursor);
CE_INIT_FUNC_DECL(manager);
CE_INIT_FUNC_DECL(query);
CE_INIT_FUNC_DECL(readconcern);
CE_INIT_FUNC_DECL(readpreference);
CE_INIT_FUNC_DECL(server);
CE_INIT_FUNC_DECL(serverapi);
CE_INIT_FUNC_DECL(serverdescription);
CE_INIT_FUNC_DECL(session);
CE_INIT_FUNC_DECL(topologydescription);
CE_INIT_FUNC_DECL(writeconcern);
CE_INIT_FUNC_DECL(writeconcernerror);
CE_INIT_FUNC_DECL(writeerror);
CE_INIT_FUNC_DECL(writeresult);

CE_INIT_FUNC_DECL(cursor_interface);

CE_INIT_FUNC_DECL(authenticationexception);
CE_INIT_FUNC_DECL(bulkwriteexception);
CE_INIT_FUNC_DECL(bulkwritecommandexception);
CE_INIT_FUNC_DECL(commandexception);
CE_INIT_FUNC_DECL(connectionexception);
CE_INIT_FUNC_DECL(connectiontimeoutexception);
CE_INIT_FUNC_DECL(encryptionexception);
CE_INIT_FUNC_DECL(exception);
CE_INIT_FUNC_DECL(executiontimeoutexception);
CE_INIT_FUNC_DECL(invalidargumentexception);
CE_INIT_FUNC_DECL(logicexception);
CE_INIT_FUNC_DECL(runtimeexception);
CE_INIT_FUNC_DECL(serverexception);
CE_INIT_FUNC_DECL(sslconnectionexception);
CE_INIT_FUNC_DECL(unexpectedvalueexception);

CE_INIT_FUNC_DECL(commandfailedevent);
CE_INIT_FUNC_DECL(commandstartedevent);
CE_INIT_FUNC_DECL(commandsubscriber);
CE_INIT_FUNC_DECL(commandsucceededevent);
CE_INIT_FUNC_DECL(logsubscriber);
CE_INIT_FUNC_DECL(sdamsubscriber);
CE_INIT_FUNC_DECL(subscriber);
CE_INIT_FUNC_DECL(serverchangedevent);
CE_INIT_FUNC_DECL(serverclosedevent);
CE_INIT_FUNC_DECL(serverheartbeatfailedevent);
CE_INIT_FUNC_DECL(serverheartbeatstartedevent);
CE_INIT_FUNC_DECL(serverheartbeatsucceededevent);
CE_INIT_FUNC_DECL(serveropeningevent);
CE_INIT_FUNC_DECL(topologychangedevent);
CE_INIT_FUNC_DECL(topologyclosedevent);
CE_INIT_FUNC_DECL(topologyopeningevent);

#endif /* PHONGO_CLASSES_H */
