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
#include "ServerHeartbeatStartedEvent_arginfo.h"

zend_class_entry* phongo_serverheartbeatstartedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent)

/* Returns this event's host */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, getHost)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatstartedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
}

/* Returns this event's port */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, getPort)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatstartedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
}

/* Returns whether this event came from an awaitable hello */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, isAwaited)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatstartedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(intern->awaited);
}

/* MongoDB\Driver\Monitoring\ServerHeartbeatStartedEvent object handlers */
static zend_object_handlers phongo_handler_serverheartbeatstartedevent;

static void phongo_serverheartbeatstartedevent_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverheartbeatstartedevent, object);

	zend_object_std_dtor(&intern->std);
}

static zend_object* phongo_serverheartbeatstartedevent_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(serverheartbeatstartedevent, class_type);

	intern->std.handlers = &phongo_handler_serverheartbeatstartedevent;

	return &intern->std;
}

static HashTable* phongo_serverheartbeatstartedevent_get_debug_info(zend_object* object, int* is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverheartbeatstartedevent, object);

	zval retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	array_init_size(&retval, 4);

	ADD_ASSOC_STRING(&retval, "host", intern->host.host);
	ADD_ASSOC_LONG_EX(&retval, "port", intern->host.port);
	ADD_ASSOC_BOOL_EX(&retval, "awaited", intern->awaited);

	return Z_ARRVAL(retval);
}

void phongo_serverheartbeatstartedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverheartbeatstartedevent_ce                = register_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent();
	phongo_serverheartbeatstartedevent_ce->create_object = phongo_serverheartbeatstartedevent_create_object;

	memcpy(&phongo_handler_serverheartbeatstartedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_serverheartbeatstartedevent.get_debug_info = phongo_serverheartbeatstartedevent_get_debug_info;
	phongo_handler_serverheartbeatstartedevent.free_obj       = phongo_serverheartbeatstartedevent_free_object;
	phongo_handler_serverheartbeatstartedevent.offset         = XtOffsetOf(phongo_serverheartbeatstartedevent_t, std);

	return;
}
