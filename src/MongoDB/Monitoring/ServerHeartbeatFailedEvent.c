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

PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getDurationMicros, serverheartbeatfailedevent, "duration")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getError, serverheartbeatfailedevent, "error")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getHost, serverheartbeatfailedevent, "host")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getPort, serverheartbeatfailedevent, "port")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, isAwaited, serverheartbeatfailedevent, "awaited")

static void phongo_serverheartbeatfailedevent_update_properties(zend_object* object, const mongoc_apm_server_heartbeat_failed_t* event)
{
	zend_update_property_string(phongo_serverheartbeatfailedevent_ce, object, ZEND_STRL("host"), mongoc_apm_server_heartbeat_failed_get_host(event)->host);
	zend_update_property_long(phongo_serverheartbeatfailedevent_ce, object, ZEND_STRL("port"), mongoc_apm_server_heartbeat_failed_get_host(event)->port);
	zend_update_property_bool(phongo_serverheartbeatfailedevent_ce, object, ZEND_STRL("awaited"), mongoc_apm_server_heartbeat_failed_get_awaited(event));
	zend_update_property_long(phongo_serverheartbeatfailedevent_ce, object, ZEND_STRL("duration"), mongoc_apm_server_heartbeat_failed_get_duration(event));

	{
		bson_error_t tmp_error = { 0 };
		zval         zerror;

		/* We need to process and convert the error right here, otherwise
		 * debug_info will turn into a recursive loop, and with the wrong trace
		 * locations */
		mongoc_apm_server_heartbeat_failed_get_error(event, &tmp_error);

		object_init_ex(&zerror, phongo_exception_from_mongoc_domain(tmp_error.domain, tmp_error.code));
		zend_update_property_string(zend_ce_exception, Z_OBJ_P(&zerror), ZEND_STRL("message"), tmp_error.message);
		zend_update_property_long(zend_ce_exception, Z_OBJ_P(&zerror), ZEND_STRL("code"), tmp_error.code);

		zend_update_property(phongo_serverheartbeatfailedevent_ce, object, ZEND_STRL("error"), &zerror);
		zval_ptr_dtor(&zerror);
	}
}

void phongo_serverheartbeatfailedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverheartbeatfailedevent_ce = register_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent();
}

void phongo_serverheartbeatfailedevent_init(zval* return_value, const mongoc_apm_server_heartbeat_failed_t* event)
{
	PHONGO_OBJECT_INIT_EX(serverheartbeatfailedevent, return_value);

	phongo_serverheartbeatfailedevent_update_properties(object, event);
}
