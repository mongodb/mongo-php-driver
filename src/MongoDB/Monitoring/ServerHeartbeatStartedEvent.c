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

PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, getHost, serverheartbeatstartedevent, "host")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, getPort, serverheartbeatstartedevent, "port")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent, isAwaited, serverheartbeatstartedevent, "awaited")

static void phongo_serverheartbeatstartedevent_update_properties(zend_object* object, const mongoc_apm_server_heartbeat_started_t* event)
{
	zend_update_property_string(phongo_serverheartbeatstartedevent_ce, object, ZEND_STRL("host"), mongoc_apm_server_heartbeat_started_get_host(event)->host);
	zend_update_property_long(phongo_serverheartbeatstartedevent_ce, object, ZEND_STRL("port"), mongoc_apm_server_heartbeat_started_get_host(event)->port);
	zend_update_property_bool(phongo_serverheartbeatstartedevent_ce, object, ZEND_STRL("awaited"), mongoc_apm_server_heartbeat_started_get_awaited(event));
}

void phongo_serverheartbeatstartedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverheartbeatstartedevent_ce = register_class_MongoDB_Driver_Monitoring_ServerHeartbeatStartedEvent();
}

void phongo_serverheartbeatstartedevent_init(zval* return_value, const mongoc_apm_server_heartbeat_started_t* event)
{
	PHONGO_OBJECT_INIT_EX(serverheartbeatstartedevent, return_value);

	phongo_serverheartbeatstartedevent_update_properties(object, event);
}
