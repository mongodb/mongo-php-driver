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
#include <Zend/zend_exceptions.h>

#include "phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"

#include "BSON/ObjectId.h"
#include "MongoDB/Server.h"
#include "CommandFailedEvent_arginfo.h"

zend_class_entry* phongo_commandfailedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_CommandFailedEvent)

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getCommandName)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "commandName");
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getDatabaseName)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "databaseName");
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getDurationMicros)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "duration");
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getError)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "error");
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getHost)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "host");
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getOperationId)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "operationId");
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getPort)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "port");
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getReply)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "reply");
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getRequestId)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "requestId");
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getServiceId)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "serviceId");
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getServerConnectionId)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_RETURN_PROPERTY(commandfailedevent, "serverConnectionId");
}

static void phongo_commandfailedevent_update_properties(zend_object* object, const mongoc_apm_command_failed_t* event)
{
	char              operation_id[24], request_id[24];
	phongo_bson_state reply_state;

	PHONGO_BSON_INIT_STATE(reply_state);

	zend_update_property_string(phongo_commandfailedevent_ce, object, ZEND_STRL("host"), mongoc_apm_command_failed_get_host(event)->host);
	zend_update_property_long(phongo_commandfailedevent_ce, object, ZEND_STRL("port"), mongoc_apm_command_failed_get_host(event)->port);
	zend_update_property_string(phongo_commandfailedevent_ce, object, ZEND_STRL("commandName"), mongoc_apm_command_failed_get_command_name(event));
	zend_update_property_string(phongo_commandfailedevent_ce, object, ZEND_STRL("databaseName"), mongoc_apm_command_failed_get_database_name(event));
	zend_update_property_long(phongo_commandfailedevent_ce, object, ZEND_STRL("duration"), mongoc_apm_command_failed_get_duration(event));

	{
		bson_error_t tmp_error = { 0 };
		zval         z_error;

		mongoc_apm_command_failed_get_error(event, &tmp_error);

		object_init_ex(&z_error, phongo_exception_from_mongoc_domain(tmp_error.domain, tmp_error.code));
		zend_update_property_string(zend_ce_exception, Z_OBJ_P(&z_error), ZEND_STRL("message"), tmp_error.message);
		zend_update_property_long(zend_ce_exception, Z_OBJ_P(&z_error), ZEND_STRL("code"), tmp_error.code);

		zend_update_property(phongo_commandfailedevent_ce, object, ZEND_STRL("error"), &z_error);
		zval_ptr_dtor(&z_error);
	}

	if (phongo_bson_to_zval_ex(mongoc_apm_command_failed_get_reply(event), &reply_state)) {
		zend_update_property(phongo_commandfailedevent_ce, object, ZEND_STRL("reply"), &reply_state.zchild);
	}
	zval_ptr_dtor(&reply_state.zchild);

	snprintf(operation_id, sizeof(operation_id), "%" PRId64, mongoc_apm_command_failed_get_operation_id(event));
	zend_update_property_string(phongo_commandfailedevent_ce, object, ZEND_STRL("operationId"), operation_id);

	snprintf(request_id, sizeof(request_id), "%" PRId64, mongoc_apm_command_failed_get_request_id(event));
	zend_update_property_string(phongo_commandfailedevent_ce, object, ZEND_STRL("requestId"), request_id);

	if (mongoc_apm_command_failed_get_service_id(event)) {
		zval service_id;

		if (phongo_objectid_new(&service_id, mongoc_apm_command_failed_get_service_id(event))) {
			zend_update_property(phongo_commandfailedevent_ce, object, ZEND_STRL("serviceId"), &service_id);
			zval_ptr_dtor(&service_id);
		}
	} else {
		zend_update_property_null(phongo_commandfailedevent_ce, object, ZEND_STRL("serviceId"));
	}

	/* TODO: Use MONGOC_NO_SERVER_CONNECTION_ID once it is added to libmongoc's public API (CDRIVER-4176) */
	if (mongoc_apm_command_failed_get_server_connection_id_int64(event) == -1) {
		zend_update_property_null(phongo_commandfailedevent_ce, object, ZEND_STRL("serverConnectionId"));
	} else {
		zend_update_property_long(phongo_commandfailedevent_ce, object, ZEND_STRL("serverConnectionId"), mongoc_apm_command_failed_get_server_connection_id_int64(event));
	}
}

void phongo_commandfailedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_commandfailedevent_ce = register_class_MongoDB_Driver_Monitoring_CommandFailedEvent();
}

void phongo_commandfailedevent_init(zval* return_value, const mongoc_apm_command_failed_t* event)
{
	PHONGO_OBJECT_INIT_EX(commandfailedevent, return_value);

	phongo_commandfailedevent_update_properties(object, event);
}