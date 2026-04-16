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
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->command_name);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getDatabaseName)
{
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->database_name);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getDurationMicros)
{
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->duration_micros);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getError)
{
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ZVAL(&intern->z_error, 1, 0);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getHost)
{
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getOperationId)
{
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

	char operation_id[24];

	PHONGO_PARSE_PARAMETERS_NONE();

	snprintf(operation_id, sizeof(operation_id), "%" PRId64, intern->operation_id);
	RETVAL_STRING(operation_id);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getPort)
{
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getReply)
{
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

	phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!phongo_bson_to_zval_ex(intern->reply, &state)) {
		zval_ptr_dtor(&state.zchild);
		return;
	}

	RETURN_ZVAL(&state.zchild, 0, 1);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getRequestId)
{
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

	char request_id[24];

	PHONGO_PARSE_PARAMETERS_NONE();

	snprintf(request_id, sizeof(request_id), "%" PRId64, intern->request_id);
	RETVAL_STRING(request_id);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getServiceId)
{
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->has_service_id) {
		RETURN_NULL();
	}

	phongo_objectid_new(return_value, &intern->service_id);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandFailedEvent, getServerConnectionId)
{
	PHONGO_INTERN_FROM_THIS(commandfailedevent);

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

static void phongo_commandfailedevent_update_properties(phongo_commandfailedevent_t* intern)
{
	char              operation_id[24], request_id[24];
	phongo_bson_state reply_state;

	PHONGO_BSON_INIT_STATE(reply_state);

	zend_update_property_string(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("host"), intern->host.host);
	zend_update_property_long(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("port"), intern->host.port);
	zend_update_property_string(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("commandName"), intern->command_name);
	zend_update_property_string(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("databaseName"), intern->database_name);
	zend_update_property_long(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("duration"), intern->duration_micros);
	zend_update_property(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("error"), &intern->z_error);

	if (phongo_bson_to_zval_ex(intern->reply, &reply_state)) {
		zend_update_property(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("reply"), &reply_state.zchild);
	}
	zval_ptr_dtor(&reply_state.zchild);

	snprintf(operation_id, sizeof(operation_id), "%" PRId64, intern->operation_id);
	zend_update_property_string(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("operationId"), operation_id);

	snprintf(request_id, sizeof(request_id), "%" PRId64, intern->request_id);
	zend_update_property_string(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("requestId"), request_id);

	if (intern->has_service_id) {
		zval service_id;

		if (phongo_objectid_new(&service_id, &intern->service_id)) {
			zend_update_property(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("serviceId"), &service_id);
			zval_ptr_dtor(&service_id);
		}
	} else {
		zend_update_property_null(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("serviceId"));
	}

	/* TODO: Use MONGOC_NO_SERVER_CONNECTION_ID once it is added to libmongoc's public API (CDRIVER-4176) */
	if (intern->server_connection_id == -1) {
		zend_update_property_null(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("serverConnectionId"));
	} else {
		zend_update_property_long(phongo_commandfailedevent_ce, &intern->std, ZEND_STRL("serverConnectionId"), intern->server_connection_id);
	}
}

/* MongoDB\Driver\Monitoring\CommandFailedEvent object handlers */
static zend_object_handlers phongo_handler_commandfailedevent;

static void phongo_commandfailedevent_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(commandfailedevent, object);

	zend_object_std_dtor(&intern->std);

	if (!Z_ISUNDEF(intern->z_error)) {
		zval_ptr_dtor(&intern->z_error);
	}

	if (intern->reply) {
		bson_destroy(intern->reply);
	}

	if (intern->command_name) {
		efree(intern->command_name);
	}

	if (intern->database_name) {
		efree(intern->database_name);
	}
}

static zend_object* phongo_commandfailedevent_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(commandfailedevent, class_type);

	intern->std.handlers = &phongo_handler_commandfailedevent;

	return &intern->std;
}

void phongo_commandfailedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_commandfailedevent_ce                = register_class_MongoDB_Driver_Monitoring_CommandFailedEvent();
	phongo_commandfailedevent_ce->create_object = phongo_commandfailedevent_create_object;

	memcpy(&phongo_handler_commandfailedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_commandfailedevent.free_obj = phongo_commandfailedevent_free_object;
	phongo_handler_commandfailedevent.offset   = XtOffsetOf(phongo_commandfailedevent_t, std);
}

void phongo_commandfailedevent_init(zval* return_value, const mongoc_apm_command_failed_t* event)
{
	PHONGO_INTERN_INIT_EX(commandfailedevent, return_value);

	bson_error_t tmp_error = { 0 };

	memcpy(&intern->host, mongoc_apm_command_failed_get_host(event), sizeof(mongoc_host_list_t));

	intern->command_name         = estrdup(mongoc_apm_command_failed_get_command_name(event));
	intern->database_name        = estrdup(mongoc_apm_command_failed_get_database_name(event));
	intern->server_id            = mongoc_apm_command_failed_get_server_id(event);
	intern->operation_id         = mongoc_apm_command_failed_get_operation_id(event);
	intern->request_id           = mongoc_apm_command_failed_get_request_id(event);
	intern->duration_micros      = mongoc_apm_command_failed_get_duration(event);
	intern->reply                = bson_copy(mongoc_apm_command_failed_get_reply(event));
	intern->server_connection_id = mongoc_apm_command_failed_get_server_connection_id_int64(event);
	intern->has_service_id       = mongoc_apm_command_failed_get_service_id(event) != NULL;

	if (intern->has_service_id) {
		bson_oid_copy(mongoc_apm_command_failed_get_service_id(event), &intern->service_id);
	}

	/* We need to process and convert the error right here, otherwise
	 * debug_info will turn into a recursive loop, and with the wrong trace
	 * locations */
	mongoc_apm_command_failed_get_error(event, &tmp_error);

	object_init_ex(&intern->z_error, phongo_exception_from_mongoc_domain(tmp_error.domain, tmp_error.code));
	zend_update_property_string(zend_ce_exception, Z_OBJ_P(&intern->z_error), ZEND_STRL("message"), tmp_error.message);
	zend_update_property_long(zend_ce_exception, Z_OBJ_P(&intern->z_error), ZEND_STRL("code"), tmp_error.code);

	phongo_commandfailedevent_update_properties(intern);
}