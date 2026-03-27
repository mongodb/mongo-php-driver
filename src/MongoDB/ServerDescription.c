/*
 * Copyright 2021-present MongoDB, Inc.
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
#include <zend_smart_str.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_error.h"

#include "MongoDB/ServerDescription.h"
#include "ServerDescription_arginfo.h"

zend_class_entry* phongo_serverdescription_ce;

phongo_server_description_type_map_t
	phongo_server_description_type_map[PHONGO_SERVER_DESCRIPTION_TYPES] = {
		{ PHONGO_SERVER_UNKNOWN, PHONGO_SERVER_TYPE_UNKNOWN },
		{ PHONGO_SERVER_STANDALONE, PHONGO_SERVER_TYPE_STANDALONE },
		{ PHONGO_SERVER_MONGOS, PHONGO_SERVER_TYPE_MONGOS },
		{ PHONGO_SERVER_POSSIBLE_PRIMARY, PHONGO_SERVER_TYPE_POSSIBLE_PRIMARY },
		{ PHONGO_SERVER_RS_PRIMARY, PHONGO_SERVER_TYPE_RS_PRIMARY },
		{ PHONGO_SERVER_RS_SECONDARY, PHONGO_SERVER_TYPE_RS_SECONDARY },
		{ PHONGO_SERVER_RS_ARBITER, PHONGO_SERVER_TYPE_RS_ARBITER },
		{ PHONGO_SERVER_RS_OTHER, PHONGO_SERVER_TYPE_RS_OTHER },
		{ PHONGO_SERVER_RS_GHOST, PHONGO_SERVER_TYPE_RS_GHOST },
		{ PHONGO_SERVER_LOAD_BALANCER, PHONGO_SERVER_TYPE_LOAD_BALANCER },
	};

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_ServerDescription)

/* Returns the most recent "hello" response */
static PHP_METHOD(MongoDB_Driver_ServerDescription, getHelloResponse)
{
	PHONGO_INTERN_FROM_THIS(serverdescription);

	const bson_t*     helloResponse;
	phongo_bson_state state;

	PHONGO_PARSE_PARAMETERS_NONE();

	/* Note: the hello response will be empty for load balancers since they are
	 * not monitored. Unlike Server::getInfo(), we do not attempt to fetch the
	 * corresponding handshake description, as that would require holding a
	 * reference to the libmongoc client (and likely a Manager object) on the
	 * ServerDescription and TopologyDescription classes. */
	helloResponse = mongoc_server_description_hello_response(intern->server_description);

	PHONGO_BSON_INIT_DEBUG_STATE(state);

	if (!phongo_bson_to_zval_ex(helloResponse, &state)) {
		/* Exception should already have been thrown */
		zval_ptr_dtor(&state.zchild);
		return;
	}

	RETURN_ZVAL(&state.zchild, 0, 1);
}

/* Returns the server's hostname */
static PHP_METHOD(MongoDB_Driver_ServerDescription, getHost)
{
	PHONGO_INTERN_FROM_THIS(serverdescription);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(mongoc_server_description_host(intern->server_description)->host);
}

/* Returns the server's last update time, in microseconds */
static PHP_METHOD(MongoDB_Driver_ServerDescription, getLastUpdateTime)
{
	PHONGO_INTERN_FROM_THIS(serverdescription);

	int64_t last_update_time;

	PHONGO_PARSE_PARAMETERS_NONE();

	last_update_time = mongoc_server_description_last_update_time(intern->server_description);

#if SIZEOF_ZEND_LONG == 4
	if (last_update_time > INT32_MAX || last_update_time < INT32_MIN) {
		zend_error(E_WARNING, "Truncating 64-bit value for lastUpdateTime");
	}
#endif

	RETVAL_LONG(last_update_time);
}

/* Returns the server's port */
static PHP_METHOD(MongoDB_Driver_ServerDescription, getPort)
{
	PHONGO_INTERN_FROM_THIS(serverdescription);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(mongoc_server_description_host(intern->server_description)->port);
}

/* Returns the server's round trip time, in milliseconds */
static PHP_METHOD(MongoDB_Driver_ServerDescription, getRoundTripTime)
{
	PHONGO_INTERN_FROM_THIS(serverdescription);

	PHONGO_PARSE_PARAMETERS_NONE();

	/* TODO: Use MONGOC_RTT_UNSET once it is added to libmongoc's public API (CDRIVER-4176) */
	if (mongoc_server_description_round_trip_time(intern->server_description) == -1) {
		RETVAL_NULL();
	} else {
		RETVAL_LONG(mongoc_server_description_round_trip_time(intern->server_description));
	}
}

/* Returns the server's node type */
static PHP_METHOD(MongoDB_Driver_ServerDescription, getType)
{
	PHONGO_INTERN_FROM_THIS(serverdescription);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(mongoc_server_description_type(intern->server_description));
}

/* MongoDB\Driver\ServerDescription object handlers */
static zend_object_handlers phongo_handler_serverdescription;

static void phongo_serverdescription_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverdescription, object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->server_description) {
		mongoc_server_description_destroy(intern->server_description);
	}
}

static zend_object* phongo_serverdescription_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(serverdescription, class_type);

	intern->std.handlers = &phongo_handler_serverdescription;

	return &intern->std;
}

HashTable* phongo_serverdescription_get_properties_hash(zend_object* object, bool is_debug)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverdescription, object);

	HashTable* props;

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 6);

	if (!intern->server_description) {
		return props;
	}

	{
		zval                      host, port, type;
		const mongoc_host_list_t* host_list = mongoc_server_description_host(intern->server_description);

		ZVAL_STRING(&host, host_list->host);
		zend_hash_str_update(props, "host", sizeof("host") - 1, &host);

		ZVAL_LONG(&port, host_list->port);
		zend_hash_str_update(props, "port", sizeof("port") - 1, &port);

		ZVAL_STRING(&type, mongoc_server_description_type(intern->server_description));
		zend_hash_str_update(props, "type", sizeof("type") - 1, &type);
	}

	{
		const bson_t*     hello_response = mongoc_server_description_hello_response(intern->server_description);
		phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!phongo_bson_to_zval_ex(hello_response, &state)) {
			zval_ptr_dtor(&state.zchild);
			goto done;
		}
		zend_hash_str_update(props, "hello_response", sizeof("hello_response") - 1, &state.zchild);
	}

	{
		int64_t last_update_time;
		zval    z_last_update_time;

		last_update_time = mongoc_server_description_last_update_time(intern->server_description);

#if SIZEOF_ZEND_LONG == 4
		if (last_update_time > INT32_MAX || last_update_time < INT32_MIN) {
			ZVAL_INT64_STRING(&z_last_update_time, last_update_time);
		} else {
			ZVAL_LONG(&z_last_update_time, last_update_time);
		}
#else
		ZVAL_LONG(&z_last_update_time, last_update_time);
#endif

		zend_hash_str_update(props, "last_update_time", sizeof("last_update_time") - 1, &z_last_update_time);
	}

	{
		zval round_trip_time;

		/* TODO: Use MONGOC_RTT_UNSET once it is added to libmongoc's public API (CDRIVER-4176) */
		if (mongoc_server_description_round_trip_time(intern->server_description) == -1) {
			ZVAL_NULL(&round_trip_time);
		} else {
			ZVAL_LONG(&round_trip_time, mongoc_server_description_round_trip_time(intern->server_description));
		}

		zend_hash_str_update(props, "round_trip_time", sizeof("round_trip_time") - 1, &round_trip_time);
	}

done:
	return props;
}

static HashTable* phongo_serverdescription_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return phongo_serverdescription_get_properties_hash(object, true);
}

static HashTable* phongo_serverdescription_get_properties(zend_object* object)
{
	return phongo_serverdescription_get_properties_hash(object, false);
}

void phongo_serverdescription_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverdescription_ce                = register_class_MongoDB_Driver_ServerDescription();
	phongo_serverdescription_ce->create_object = phongo_serverdescription_create_object;

	memcpy(&phongo_handler_serverdescription, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_serverdescription.get_debug_info = phongo_serverdescription_get_debug_info;
	phongo_handler_serverdescription.get_properties = phongo_serverdescription_get_properties;
	phongo_handler_serverdescription.free_obj       = phongo_serverdescription_free_object;
	phongo_handler_serverdescription.offset         = XtOffsetOf(phongo_serverdescription_t, std);
}

void phongo_serverdescription_init_ex(zval* return_value, mongoc_server_description_t* server_description, bool copy)
{
	PHONGO_INTERN_INIT_EX(serverdescription, return_value);
	intern->server_description = copy ? mongoc_server_description_new_copy(server_description) : server_description;
}

phongo_server_description_type_t phongo_server_description_type(mongoc_server_description_t* sd)
{
	const char* name = mongoc_server_description_type(sd);
	int         i;

	for (i = 0; i < PHONGO_SERVER_DESCRIPTION_TYPES; i++) {
		if (!strcmp(name, phongo_server_description_type_map[i].name)) {
			return phongo_server_description_type_map[i].type;
		}
	}

	return PHONGO_SERVER_UNKNOWN;
}
