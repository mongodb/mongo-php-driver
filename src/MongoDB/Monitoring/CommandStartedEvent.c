/*
 * Copyright 2016-present MongoDB, Inc.
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

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"

#include "BSON/ObjectId.h"
#include "MongoDB/Server.h"
#include "CommandStartedEvent_arginfo.h"

zend_class_entry* phongo_commandstartedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_CommandStartedEvent)

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getCommand)
{
	PHONGO_INTERN_FROM_THIS(commandstartedevent);

	phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!phongo_bson_to_zval_ex(intern->command, &state)) {
		zval_ptr_dtor(&state.zchild);
		return;
	}

	RETURN_ZVAL(&state.zchild, 0, 1);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getCommandName)
{
	PHONGO_INTERN_FROM_THIS(commandstartedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->command_name);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getDatabaseName)
{
	PHONGO_INTERN_FROM_THIS(commandstartedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->database_name);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getHost)
{
	PHONGO_INTERN_FROM_THIS(commandstartedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getOperationId)
{
	PHONGO_INTERN_FROM_THIS(commandstartedevent);

	char operation_id[24];

	PHONGO_PARSE_PARAMETERS_NONE();

	snprintf(operation_id, sizeof(operation_id), "%" PRId64, intern->operation_id);
	RETVAL_STRING(operation_id);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getPort)
{
	PHONGO_INTERN_FROM_THIS(commandstartedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getRequestId)
{
	PHONGO_INTERN_FROM_THIS(commandstartedevent);

	char request_id[24];

	PHONGO_PARSE_PARAMETERS_NONE();

	snprintf(request_id, sizeof(request_id), "%" PRId64, intern->request_id);
	RETVAL_STRING(request_id);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getServiceId)
{
	PHONGO_INTERN_FROM_THIS(commandstartedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->has_service_id) {
		RETURN_NULL();
	}

	phongo_objectid_new(return_value, &intern->service_id);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getServerConnectionId)
{
	PHONGO_INTERN_FROM_THIS(commandstartedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	/* TODO: Use MONGOC_NO_SERVER_CONNECTION_ID once it is added to libmongoc's public API (CDRIVER-4176) */
	if (intern->server_connection_id == -1) {
		RETURN_NULL();
	}

#if SIZEOF_ZEND_LONG == 4
	if (intern->server_connection_id > INT32_MAX || intern->server_connection_id < INT32_MIN) {
		zend_error(E_WARNING, "Truncating 64-bit value %" PRId64 " for serverConnectionId", intern->server_connection_id);
	}
#endif

	RETURN_LONG(intern->server_connection_id);
}

static void phongo_commandstartedevent_update_properties(phongo_commandstartedevent_t* intern)
{
	char              operation_id[24], request_id[24];
	phongo_bson_state command_state;

	PHONGO_BSON_INIT_STATE(command_state);

	zend_update_property_string(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("host"), intern->host.host);
	zend_update_property_long(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("port"), intern->host.port);
	zend_update_property_string(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("commandName"), intern->command_name);
	zend_update_property_string(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("databaseName"), intern->database_name);

	if (phongo_bson_to_zval_ex(intern->command, &command_state)) {
		zend_update_property(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("command"), &command_state.zchild);
	}
	zval_ptr_dtor(&command_state.zchild);

	snprintf(operation_id, sizeof(operation_id), "%" PRId64, intern->operation_id);
	zend_update_property_string(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("operationId"), operation_id);

	snprintf(request_id, sizeof(request_id), "%" PRId64, intern->request_id);
	zend_update_property_string(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("requestId"), request_id);

	if (intern->has_service_id) {
		zval service_id;

		if (phongo_objectid_new(&service_id, &intern->service_id)) {
			zend_update_property(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("serviceId"), &service_id);
			zval_ptr_dtor(&service_id);
		}
	} else {
		zend_update_property_null(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("serviceId"));
	}

	/* TODO: Use MONGOC_NO_SERVER_CONNECTION_ID once it is added to libmongoc's public API (CDRIVER-4176) */
	if (intern->server_connection_id == -1) {
		zend_update_property_null(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("serverConnectionId"));
	} else {
		zend_update_property_long(phongo_commandstartedevent_ce, &intern->std, ZEND_STRL("serverConnectionId"), intern->server_connection_id);
	}
}

/* MongoDB\Driver\Monitoring\CommandStartedEvent object handlers */
static zend_object_handlers phongo_handler_commandstartedevent;

static void phongo_commandstartedevent_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(commandstartedevent, object);

	zend_object_std_dtor(&intern->std);

	if (intern->command) {
		bson_destroy(intern->command);
	}

	if (intern->command_name) {
		efree(intern->command_name);
	}

	if (intern->database_name) {
		efree(intern->database_name);
	}
}

static zend_object* phongo_commandstartedevent_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(commandstartedevent, class_type);

	intern->std.handlers = &phongo_handler_commandstartedevent;

	return &intern->std;
}

void phongo_commandstartedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_commandstartedevent_ce                = register_class_MongoDB_Driver_Monitoring_CommandStartedEvent();
	phongo_commandstartedevent_ce->create_object = phongo_commandstartedevent_create_object;

	memcpy(&phongo_handler_commandstartedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_commandstartedevent.free_obj = phongo_commandstartedevent_free_object;
	phongo_handler_commandstartedevent.offset   = XtOffsetOf(phongo_commandstartedevent_t, std);
}

void phongo_commandstartedevent_init(zval* return_value, const mongoc_apm_command_started_t* event)
{
	PHONGO_INTERN_INIT_EX(commandstartedevent, return_value);

	memcpy(&intern->host, mongoc_apm_command_started_get_host(event), sizeof(mongoc_host_list_t));

	intern->command_name         = estrdup(mongoc_apm_command_started_get_command_name(event));
	intern->database_name        = estrdup(mongoc_apm_command_started_get_database_name(event));
	intern->server_id            = mongoc_apm_command_started_get_server_id(event);
	intern->operation_id         = mongoc_apm_command_started_get_operation_id(event);
	intern->request_id           = mongoc_apm_command_started_get_request_id(event);
	intern->command              = bson_copy(mongoc_apm_command_started_get_command(event));
	intern->server_connection_id = mongoc_apm_command_started_get_server_connection_id_int64(event);
	intern->has_service_id       = mongoc_apm_command_started_get_service_id(event) != NULL;

	if (intern->has_service_id) {
		bson_oid_copy(mongoc_apm_command_started_get_service_id(event), &intern->service_id);
	}

	phongo_commandstartedevent_update_properties(intern);
}
