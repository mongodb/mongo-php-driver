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
#include "ServerHeartbeatSucceededEvent_arginfo.h"

zend_class_entry* phongo_serverheartbeatsucceededevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent)

PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getDurationMicros, serverheartbeatsucceededevent, "duration")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getHost, serverheartbeatsucceededevent, "host")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getPort, serverheartbeatsucceededevent, "port")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getReply, serverheartbeatsucceededevent, "reply")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, isAwaited, serverheartbeatsucceededevent, "awaited")

static void phongo_serverheartbeatsucceededevent_update_properties(zend_object* object, const mongoc_apm_server_heartbeat_succeeded_t* event)
{
	phongo_bson_state reply_state;

	PHONGO_BSON_INIT_STATE(reply_state);

	zend_update_property_string(phongo_serverheartbeatsucceededevent_ce, object, ZEND_STRL("host"), mongoc_apm_server_heartbeat_succeeded_get_host(event)->host);
	zend_update_property_long(phongo_serverheartbeatsucceededevent_ce, object, ZEND_STRL("port"), mongoc_apm_server_heartbeat_succeeded_get_host(event)->port);
	zend_update_property_bool(phongo_serverheartbeatsucceededevent_ce, object, ZEND_STRL("awaited"), mongoc_apm_server_heartbeat_succeeded_get_awaited(event));
	zend_update_property_long(phongo_serverheartbeatsucceededevent_ce, object, ZEND_STRL("duration"), mongoc_apm_server_heartbeat_succeeded_get_duration(event));

	if (phongo_bson_to_zval_ex(mongoc_apm_server_heartbeat_succeeded_get_reply(event), &reply_state)) {
		zend_update_property(phongo_serverheartbeatsucceededevent_ce, object, ZEND_STRL("reply"), &reply_state.zchild);
	}
	zval_ptr_dtor(&reply_state.zchild);
}

void phongo_serverheartbeatsucceededevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverheartbeatsucceededevent_ce = register_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent();
}

void phongo_serverheartbeatsucceededevent_init(zval* return_value, const mongoc_apm_server_heartbeat_succeeded_t* event)
{
	PHONGO_OBJECT_INIT_EX(serverheartbeatsucceededevent, return_value);

	phongo_serverheartbeatsucceededevent_update_properties(object, event);
}
