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

PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_CommandStartedEvent, getCommand, commandstartedevent, "command")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_CommandStartedEvent, getCommandName, commandstartedevent, "commandName")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_CommandStartedEvent, getDatabaseName, commandstartedevent, "databaseName")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_CommandStartedEvent, getHost, commandstartedevent, "host")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_CommandStartedEvent, getOperationId, commandstartedevent, "operationId")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_CommandStartedEvent, getPort, commandstartedevent, "port")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_CommandStartedEvent, getRequestId, commandstartedevent, "requestId")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_CommandStartedEvent, getServiceId, commandstartedevent, "serviceId")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_CommandStartedEvent, getServerConnectionId, commandstartedevent, "serverConnectionId")

static void phongo_commandstartedevent_update_properties(zend_object* object, const mongoc_apm_command_started_t* event)
{
	char              operation_id[24], request_id[24];
	phongo_bson_state command_state;
	int64_t           server_connection_id = mongoc_apm_command_started_get_server_connection_id_int64(event);

	PHONGO_BSON_INIT_STATE(command_state);

	zend_update_property_string(phongo_commandstartedevent_ce, object, ZEND_STRL("host"), mongoc_apm_command_started_get_host(event)->host);
	zend_update_property_long(phongo_commandstartedevent_ce, object, ZEND_STRL("port"), mongoc_apm_command_started_get_host(event)->port);
	zend_update_property_string(phongo_commandstartedevent_ce, object, ZEND_STRL("commandName"), mongoc_apm_command_started_get_command_name(event));
	zend_update_property_string(phongo_commandstartedevent_ce, object, ZEND_STRL("databaseName"), mongoc_apm_command_started_get_database_name(event));

	if (phongo_bson_to_zval_ex(mongoc_apm_command_started_get_command(event), &command_state)) {
		zend_update_property(phongo_commandstartedevent_ce, object, ZEND_STRL("command"), &command_state.zchild);
	}
	zval_ptr_dtor(&command_state.zchild);

	snprintf(operation_id, sizeof(operation_id), "%" PRId64, mongoc_apm_command_started_get_operation_id(event));
	zend_update_property_string(phongo_commandstartedevent_ce, object, ZEND_STRL("operationId"), operation_id);

	snprintf(request_id, sizeof(request_id), "%" PRId64, mongoc_apm_command_started_get_request_id(event));
	zend_update_property_string(phongo_commandstartedevent_ce, object, ZEND_STRL("requestId"), request_id);

	if (mongoc_apm_command_started_get_service_id(event)) {
		zval service_id;

		if (phongo_objectid_new(&service_id, mongoc_apm_command_started_get_service_id(event))) {
			zend_update_property(phongo_commandstartedevent_ce, object, ZEND_STRL("serviceId"), &service_id);
			zval_ptr_dtor(&service_id);
		}
	} else {
		zend_update_property_null(phongo_commandstartedevent_ce, object, ZEND_STRL("serviceId"));
	}

	/* TODO: Use MONGOC_NO_SERVER_CONNECTION_ID once it is added to libmongoc's public API (CDRIVER-4176) */
	if (server_connection_id == -1) {
		zend_update_property_null(phongo_commandstartedevent_ce, object, ZEND_STRL("serverConnectionId"));
	} else {
#if SIZEOF_ZEND_LONG == 4
		if (server_connection_id > INT32_MAX || server_connection_id < INT32_MIN) {
			zend_error(E_WARNING, "Truncating 64-bit value %" PRId64 " for serverConnectionId", server_connection_id);
		}
#endif

		zend_update_property_long(phongo_commandstartedevent_ce, object, ZEND_STRL("serverConnectionId"), server_connection_id);
	}
}

void phongo_commandstartedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_commandstartedevent_ce = register_class_MongoDB_Driver_Monitoring_CommandStartedEvent();
}

void phongo_commandstartedevent_init(zval* return_value, const mongoc_apm_command_started_t* event)
{
	PHONGO_OBJECT_INIT_EX(commandstartedevent, return_value);

	phongo_commandstartedevent_update_properties(object, event);
}
