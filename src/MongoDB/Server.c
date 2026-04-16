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

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_client.h"
#include "phongo_error.h"
#include "phongo_execute.h"
#include "phongo_util.h"

#include "MongoDB/Server.h"
#include "MongoDB/ServerDescription.h"
#include "Server_arginfo.h"

zend_class_entry* phongo_server_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Server)

/* Executes a Command on this Server */
static PHP_METHOD(MongoDB_Driver_Server, executeCommand)
{
	PHONGO_INTERN_FROM_THIS(server);

	char*  db;
	size_t db_len;
	zval*  command;
	zval*  options = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING(db, db_len)
	Z_PARAM_OBJECT_OF_CLASS(command, phongo_command_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ZVAL_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_command(&intern->manager, PHONGO_COMMAND_RAW, db, command, options, intern->server_id, return_value);
}

/* Executes a ReadCommand on this Server */
static PHP_METHOD(MongoDB_Driver_Server, executeReadCommand)
{
	PHONGO_INTERN_FROM_THIS(server);

	char*  db;
	size_t db_len;
	zval*  command;
	zval*  options = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING(db, db_len)
	Z_PARAM_OBJECT_OF_CLASS(command, phongo_command_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_command(&intern->manager, PHONGO_COMMAND_READ, db, command, options, intern->server_id, return_value);
}

/* Executes a WriteCommand on this Server */
static PHP_METHOD(MongoDB_Driver_Server, executeWriteCommand)
{
	PHONGO_INTERN_FROM_THIS(server);

	char*  db;
	size_t db_len;
	zval*  command;
	zval*  options = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING(db, db_len)
	Z_PARAM_OBJECT_OF_CLASS(command, phongo_command_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated. and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_command(&intern->manager, PHONGO_COMMAND_WRITE, db, command, options, intern->server_id, return_value);
}

/* Executes a ReadWriteCommand on this Server */
static PHP_METHOD(MongoDB_Driver_Server, executeReadWriteCommand)
{
	PHONGO_INTERN_FROM_THIS(server);

	char*  db;
	size_t db_len;
	zval*  command;
	zval*  options = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING(db, db_len)
	Z_PARAM_OBJECT_OF_CLASS(command, phongo_command_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_command(&intern->manager, PHONGO_COMMAND_READ_WRITE, db, command, options, intern->server_id, return_value);
}

/* Executes a Query on this Server */
static PHP_METHOD(MongoDB_Driver_Server, executeQuery)
{
	PHONGO_INTERN_FROM_THIS(server);

	char* namespace;
	size_t namespace_len;
	zval*  query;
	zval*  options = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING(namespace, namespace_len)
	Z_PARAM_OBJECT_OF_CLASS(query, phongo_query_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ZVAL_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_query(&intern->manager, namespace, query, options, intern->server_id, return_value);
}

/* Executes a BulkWrite (i.e. any number of insert, update, and delete ops) on
   this Server */
static PHP_METHOD(MongoDB_Driver_Server, executeBulkWrite)
{
	PHONGO_INTERN_FROM_THIS(server);

	char* namespace;
	size_t              namespace_len;
	zval*               zbulk;
	phongo_bulkwrite_t* bulk;
	zval*               options = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING(namespace, namespace_len)
	Z_PARAM_OBJECT_OF_CLASS(zbulk, phongo_bulkwrite_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ZVAL_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	bulk = Z_BULKWRITE_OBJ_P(zbulk);

	/* If the Server was created in a different process, reset the client so
	 * that its session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_bulk_write(&intern->manager, namespace, bulk, options, intern->server_id, return_value);
}

/* Executes a BulkWriteCommand (i.e. bulkWrite command for MongoDB 8.0+) */
static PHP_METHOD(MongoDB_Driver_Server, executeBulkWriteCommand)
{
	PHONGO_INTERN_FROM_THIS(server);

	zval*                      zbwc;
	phongo_bulkwritecommand_t* bwc;
	zval*                      zoptions = NULL;

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_OBJECT_OF_CLASS(zbwc, phongo_bulkwritecommand_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ZVAL_OR_NULL(zoptions)
	PHONGO_PARSE_PARAMETERS_END();

	bwc = Z_BULKWRITECOMMAND_OBJ_P(zbwc);

	/* If the Server was created in a different process, reset the client so
	 * that its session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_bulkwritecommand(&intern->manager, bwc, zoptions, intern->server_id, return_value);
}

/* Returns the hostname for this Server */
static PHP_METHOD(MongoDB_Driver_Server, getHost)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_STRING(mongoc_server_description_host(sd)->host);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
}

/* Returns the currently configured tags for this Server */
static PHP_METHOD(MongoDB_Driver_Server, getTags)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		const bson_t* hello_response = mongoc_server_description_hello_response(sd);
		bson_iter_t   iter;

		if (bson_iter_init_find(&iter, hello_response, "tags") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
			const uint8_t*    bytes;
			uint32_t          len;
			phongo_bson_state state;

			PHONGO_BSON_INIT_DEBUG_STATE(state);
			bson_iter_document(&iter, &len, &bytes);

			if (!phongo_bson_data_to_zval_ex(bytes, len, &state)) {
				/* Exception should already have been thrown */
				zval_ptr_dtor(&state.zchild);
				mongoc_server_description_destroy(sd);
				return;
			}

			mongoc_server_description_destroy(sd);

			RETURN_ZVAL(&state.zchild, 0, 1);
		}

		array_init(return_value);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
}

/* Returns the last hello response for this Server or, in the case of a load
   balancer, the initial handshake response. */
static PHP_METHOD(MongoDB_Driver_Server, getInfo)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_client_t*             client;
	mongoc_server_description_t* sd;
	mongoc_server_description_t* handshake_sd = NULL;
	const bson_t*                hello_response;
	phongo_bson_state            state;

	PHONGO_PARSE_PARAMETERS_NONE();

	client = Z_MANAGER_OBJ_P(&intern->manager)->client;

	if (!(sd = mongoc_client_get_server_description(client, intern->server_id))) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
		return;
	}

	hello_response = mongoc_server_description_hello_response(sd);

	/* If the server description is a load balancer, its hello_response will be
	 * empty. Instead, report the hello_response from the handshake description
	 * (i.e. backing server). */
	if (!strcmp(mongoc_server_description_type(sd), phongo_server_description_type_map[PHONGO_SERVER_LOAD_BALANCER].name)) {
		bson_error_t error = { 0 };

		if (!(handshake_sd = mongoc_client_get_handshake_description(client, intern->server_id, NULL, &error))) {
			phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get handshake server description: %s", error.message);
			goto cleanup;
		}

		hello_response = mongoc_server_description_hello_response(handshake_sd);
	}

	PHONGO_BSON_INIT_DEBUG_STATE(state);

	if (!phongo_bson_to_zval_ex(hello_response, &state)) {
		/* Exception should already have been thrown */
		zval_ptr_dtor(&state.zchild);
		goto cleanup;
	}

	RETVAL_ZVAL(&state.zchild, 0, 1);

cleanup:
	if (handshake_sd) {
		mongoc_server_description_destroy(handshake_sd);
	}

	mongoc_server_description_destroy(sd);
}

/* Returns the measured latency (i.e. round trip time in milliseconds) for
   this Server, or null if unset. */
static PHP_METHOD(MongoDB_Driver_Server, getLatency)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!(sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
		return;
	}

	/* TODO: Use MONGOC_RTT_UNSET once it is added to libmongoc's public API (CDRIVER-4176) */
	if (mongoc_server_description_round_trip_time(sd) == -1) {
		RETVAL_NULL();
	} else {
		RETVAL_LONG(mongoc_server_description_round_trip_time(sd));
	}

	mongoc_server_description_destroy(sd);
}

/* Returns the port for this Server */
static PHP_METHOD(MongoDB_Driver_Server, getPort)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_LONG(mongoc_server_description_host(sd)->port);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
}

/* Returns the server description for this Server */
static PHP_METHOD(MongoDB_Driver_Server, getServerDescription)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* server_description;

	PHONGO_PARSE_PARAMETERS_NONE();

	server_description = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id);

	/* Avoid making another copy in phongo_serverdescription_init */
	phongo_serverdescription_init_ex(return_value, server_description, false);
}

/* Returns the node type of this Server */
static PHP_METHOD(MongoDB_Driver_Server, getType)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_LONG(phongo_server_description_type(sd));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
}

/* Returns whether this Server is a primary member of a replica set */
static PHP_METHOD(MongoDB_Driver_Server, isPrimary)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), phongo_server_description_type_map[PHONGO_SERVER_RS_PRIMARY].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
}

/* Returns whether this Server is a secondary member of a replica set */
static PHP_METHOD(MongoDB_Driver_Server, isSecondary)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), phongo_server_description_type_map[PHONGO_SERVER_RS_SECONDARY].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
}

/* Returns whether this Server is an arbiter member of a replica set */
static PHP_METHOD(MongoDB_Driver_Server, isArbiter)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), phongo_server_description_type_map[PHONGO_SERVER_RS_ARBITER].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
}

/* Returns whether this Server is a hidden member of a replica set */
static PHP_METHOD(MongoDB_Driver_Server, isHidden)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		bson_iter_t iter;

		RETVAL_BOOL(bson_iter_init_find_case(&iter, mongoc_server_description_hello_response(sd), "hidden") && bson_iter_as_bool(&iter));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
}

/* Returns whether this Server is a passive member of a replica set */
static PHP_METHOD(MongoDB_Driver_Server, isPassive)
{
	PHONGO_INTERN_FROM_THIS(server);

	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		bson_iter_t iter;

		RETVAL_BOOL(bson_iter_init_find_case(&iter, mongoc_server_description_hello_response(sd), "passive") && bson_iter_as_bool(&iter));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
}

/* MongoDB\Driver\Server object handlers */
static zend_object_handlers phongo_handler_server;

static int phongo_server_compare_objects(zval* o1, zval* o2)
{
	phongo_server_t*             intern1;
	phongo_server_t*             intern2;
	mongoc_server_description_t *sd1, *sd2;
	int                          retval = 0;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_SERVER_OBJ_P(o1);
	intern2 = Z_SERVER_OBJ_P(o2);

	sd1 = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern1->manager)->client, intern1->server_id);
	sd2 = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern2->manager)->client, intern2->server_id);

	if (sd1 && sd2) {
		retval = strcasecmp(mongoc_server_description_host(sd1)->host_and_port, mongoc_server_description_host(sd2)->host_and_port);
	} else {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description(s)");
	}

	if (sd1) {
		mongoc_server_description_destroy(sd1);
	}

	if (sd2) {
		mongoc_server_description_destroy(sd2);
	}

	return retval;
}

static void phongo_server_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(server, object);

	zend_object_std_dtor(&intern->std);

	if (!Z_ISUNDEF(intern->manager)) {
		zval_ptr_dtor(&intern->manager);
	}
}

static zend_object* phongo_server_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(server, class_type);

	PHONGO_SET_CREATED_BY_PID(intern);

	intern->std.handlers = &phongo_handler_server;

	return &intern->std;
}

static HashTable* phongo_server_get_debug_info(zend_object* object, int* is_temp)
{
	phongo_server_t*             intern = NULL;
	zval                         retval = ZVAL_STATIC_INIT;
	mongoc_client_t*             client;
	mongoc_server_description_t* sd;
	const mongoc_host_list_t*    host;
	const bson_t*                hello_response;
	bson_iter_t                  iter;

	*is_temp = 1;
	intern   = Z_OBJ_SERVER(object);
	client   = Z_MANAGER_OBJ_P(&intern->manager)->client;

	if (!(sd = mongoc_client_get_server_description(client, intern->server_id))) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
		return NULL;
	}

	host           = mongoc_server_description_host(sd);
	hello_response = mongoc_server_description_hello_response(sd);

	array_init(&retval);

	ADD_ASSOC_STRING(&retval, "host", host->host);
	ADD_ASSOC_LONG_EX(&retval, "port", host->port);
	ADD_ASSOC_LONG_EX(&retval, "type", phongo_server_description_type(sd));
	ADD_ASSOC_BOOL_EX(&retval, "is_primary", !strcmp(mongoc_server_description_type(sd), phongo_server_description_type_map[PHONGO_SERVER_RS_PRIMARY].name));
	ADD_ASSOC_BOOL_EX(&retval, "is_secondary", !strcmp(mongoc_server_description_type(sd), phongo_server_description_type_map[PHONGO_SERVER_RS_SECONDARY].name));
	ADD_ASSOC_BOOL_EX(&retval, "is_arbiter", !strcmp(mongoc_server_description_type(sd), phongo_server_description_type_map[PHONGO_SERVER_RS_ARBITER].name));
	ADD_ASSOC_BOOL_EX(&retval, "is_hidden", bson_iter_init_find_case(&iter, hello_response, "hidden") && bson_iter_as_bool(&iter));
	ADD_ASSOC_BOOL_EX(&retval, "is_passive", bson_iter_init_find_case(&iter, hello_response, "passive") && bson_iter_as_bool(&iter));

	if (bson_iter_init_find(&iter, hello_response, "tags") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
		const uint8_t*    bytes;
		uint32_t          len;
		phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);
		bson_iter_document(&iter, &len, &bytes);
		if (!phongo_bson_data_to_zval_ex(bytes, len, &state)) {
			/* Exception already thrown */
			zval_ptr_dtor(&state.zchild);
			mongoc_server_description_destroy(sd);
			zval_ptr_dtor(&retval);
			return NULL;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "tags", &state.zchild);
	}

	/* If the server description is a load balancer, its hello_response will be
	 * empty. Instead, report the hello_response from the handshake description
	 * (i.e. backing server). */
	if (!strcmp(mongoc_server_description_type(sd), phongo_server_description_type_map[PHONGO_SERVER_LOAD_BALANCER].name)) {
		const bson_t*                handshake_response;
		mongoc_server_description_t* handshake_sd;
		bson_error_t                 error = { 0 };
		phongo_bson_state            state;

		if (!(handshake_sd = mongoc_client_get_handshake_description(client, mongoc_server_description_id(sd), NULL, &error))) {
			phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get handshake server description: %s", error.message);
			mongoc_server_description_destroy(sd);
			zval_ptr_dtor(&retval);
			return NULL;
		}

		PHONGO_BSON_INIT_DEBUG_STATE(state);
		handshake_response = mongoc_server_description_hello_response(handshake_sd);

		if (!phongo_bson_to_zval_ex(handshake_response, &state)) {
			/* Exception already thrown */
			mongoc_server_description_destroy(handshake_sd);
			zval_ptr_dtor(&state.zchild);
			mongoc_server_description_destroy(sd);
			zval_ptr_dtor(&retval);
			return NULL;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "last_hello_response", &state.zchild);
		mongoc_server_description_destroy(handshake_sd);
	} else {
		phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!phongo_bson_to_zval_ex(hello_response, &state)) {
			/* Exception already thrown */
			zval_ptr_dtor(&state.zchild);
			mongoc_server_description_destroy(sd);
			zval_ptr_dtor(&retval);
			return NULL;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "last_hello_response", &state.zchild);
	}

	/* TODO: Use MONGOC_RTT_UNSET once it is added to libmongoc's public API (CDRIVER-4176) */
	if (mongoc_server_description_round_trip_time(sd) == -1) {
		ADD_ASSOC_NULL_EX(&retval, "round_trip_time");
	} else {
		ADD_ASSOC_LONG_EX(&retval, "round_trip_time", mongoc_server_description_round_trip_time(sd));
	}

	mongoc_server_description_destroy(sd);

	return Z_ARRVAL(retval);
}

void phongo_server_init_ce(INIT_FUNC_ARGS)
{
	phongo_server_ce                = register_class_MongoDB_Driver_Server();
	phongo_server_ce->create_object = phongo_server_create_object;

	memcpy(&phongo_handler_server, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_server.compare        = phongo_server_compare_objects;
	phongo_handler_server.get_debug_info = phongo_server_get_debug_info;
	phongo_handler_server.free_obj       = phongo_server_free_object;
	phongo_handler_server.offset         = XtOffsetOf(phongo_server_t, std);
}

void phongo_server_init(zval* return_value, zval* manager, uint32_t server_id)
{
	phongo_server_t* server;

	object_init_ex(return_value, phongo_server_ce);

	server            = Z_SERVER_OBJ_P(return_value);
	server->server_id = server_id;

	ZVAL_ZVAL(&server->manager, manager, 1, 0);
}
