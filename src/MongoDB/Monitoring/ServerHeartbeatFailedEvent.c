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
	phongo_serverheartbeatfailedevent_t* intern = zend_object_alloc(sizeof(phongo_serverheartbeatfailedevent_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &phongo_handler_serverheartbeatfailedevent;

	return &intern->std;
}

static HashTable* phongo_serverheartbeatfailedevent_get_debug_info(zend_object* object, int* is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverheartbeatfailedevent, object);

	zval retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	array_init_size(&retval, 5);

	ADD_ASSOC_STRING(&retval, "host", intern->host.host);
	ADD_ASSOC_LONG_EX(&retval, "port", intern->host.port);
	ADD_ASSOC_BOOL_EX(&retval, "awaited", intern->awaited);
	ADD_ASSOC_INT64(&retval, "durationMicros", intern->duration_micros);

	ADD_ASSOC_ZVAL_EX(&retval, "error", &intern->z_error);
	Z_ADDREF(intern->z_error);

	return Z_ARRVAL(retval);
}

void phongo_serverheartbeatfailedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverheartbeatfailedevent_ce                = register_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent();
	phongo_serverheartbeatfailedevent_ce->create_object = phongo_serverheartbeatfailedevent_create_object;

	memcpy(&phongo_handler_serverheartbeatfailedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_serverheartbeatfailedevent.get_debug_info = phongo_serverheartbeatfailedevent_get_debug_info;
	phongo_handler_serverheartbeatfailedevent.free_obj       = phongo_serverheartbeatfailedevent_free_object;
	phongo_handler_serverheartbeatfailedevent.offset         = XtOffsetOf(phongo_serverheartbeatfailedevent_t, std);
}
