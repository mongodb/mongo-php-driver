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

#include <php.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_exceptions.h>

#include "phongo.h"
#include "phongo_error.h"
#include "ServerHeartbeatFailedEvent_arginfo.h"

zend_class_entry* phongo_serverheartbeatfailedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent)

/* Returns this event's duration in microseconds */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getDurationMicros)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->duration_micros);
}

/* Returns the error associated with the event */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getError)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ZVAL(&intern->z_error, 1, 0);
}

/* Returns this event's host */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getHost)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
}

/* Returns this event's port */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getPort)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
}

/* Returns whether this event came from an awaitable hello */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, isAwaited)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatfailedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(intern->awaited);
}

static void phongo_serverheartbeatfailedevent_update_properties(phongo_serverheartbeatfailedevent_t* intern)
{
	zend_update_property_string(phongo_serverheartbeatfailedevent_ce, &intern->std, ZEND_STRL("host"), intern->host.host);
	zend_update_property_long(phongo_serverheartbeatfailedevent_ce, &intern->std, ZEND_STRL("port"), intern->host.port);
	zend_update_property_bool(phongo_serverheartbeatfailedevent_ce, &intern->std, ZEND_STRL("awaited"), intern->awaited);
	zend_update_property_long(phongo_serverheartbeatfailedevent_ce, &intern->std, ZEND_STRL("durationMicros"), intern->duration_micros);
	zend_update_property(phongo_serverheartbeatfailedevent_ce, &intern->std, ZEND_STRL("error"), &intern->z_error);
}

/* MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent object handlers */
static zend_object_handlers phongo_handler_serverheartbeatfailedevent;

static void phongo_serverheartbeatfailedevent_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverheartbeatfailedevent, object);

	zend_object_std_dtor(&intern->std);

	if (!Z_ISUNDEF(intern->z_error)) {
		zval_ptr_dtor(&intern->z_error);
	}
}

static zend_object* phongo_serverheartbeatfailedevent_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(serverheartbeatfailedevent, class_type);

	intern->std.handlers = &phongo_handler_serverheartbeatfailedevent;

	return &intern->std;
}

void phongo_serverheartbeatfailedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverheartbeatfailedevent_ce                = register_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent();
	phongo_serverheartbeatfailedevent_ce->create_object = phongo_serverheartbeatfailedevent_create_object;

	memcpy(&phongo_handler_serverheartbeatfailedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_serverheartbeatfailedevent.free_obj = phongo_serverheartbeatfailedevent_free_object;
	phongo_handler_serverheartbeatfailedevent.offset   = XtOffsetOf(phongo_serverheartbeatfailedevent_t, std);
}

void phongo_serverheartbeatfailedevent_init(zval* return_value, const mongoc_apm_server_heartbeat_failed_t* event)
{
	PHONGO_INTERN_INIT_EX(serverheartbeatfailedevent, return_value);

	bson_error_t tmp_error = { 0 };

	memcpy(&intern->host, mongoc_apm_server_heartbeat_failed_get_host(event), sizeof(mongoc_host_list_t));
	intern->awaited         = mongoc_apm_server_heartbeat_failed_get_awaited(event);
	intern->duration_micros = mongoc_apm_server_heartbeat_failed_get_duration(event);

	/* We need to process and convert the error right here, otherwise
	 * debug_info will turn into a recursive loop, and with the wrong trace
	 * locations */
	mongoc_apm_server_heartbeat_failed_get_error(event, &tmp_error);

	object_init_ex(&intern->z_error, phongo_exception_from_mongoc_domain(tmp_error.domain, tmp_error.code));
	zend_update_property_string(zend_ce_exception, Z_OBJ_P(&intern->z_error), ZEND_STRL("message"), tmp_error.message);
	zend_update_property_long(zend_ce_exception, Z_OBJ_P(&intern->z_error), ZEND_STRL("code"), tmp_error.code);

	phongo_serverheartbeatfailedevent_update_properties(intern);
}
