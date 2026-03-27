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

/* Returns this event's duration in microseconds */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getDurationMicros)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatsucceededevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->duration_micros);
}

/* Returns this event's host */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getHost)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatsucceededevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
}

/* Returns this event's port */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getPort)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatsucceededevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
}

/* Returns this event's reply */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, getReply)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatsucceededevent);

	phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!phongo_bson_to_zval_ex(intern->reply, &state)) {
		zval_ptr_dtor(&state.zchild);
		return;
	}

	RETURN_ZVAL(&state.zchild, 0, 1);
}

/* Returns whether this event came from an awaitable hello */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent, isAwaited)
{
	PHONGO_INTERN_FROM_THIS(serverheartbeatsucceededevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(intern->awaited);
}

/* MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent object handlers */
static zend_object_handlers phongo_handler_serverheartbeatsucceededevent;

static void phongo_serverheartbeatsucceededevent_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverheartbeatsucceededevent, object);

	zend_object_std_dtor(&intern->std);

	if (intern->reply) {
		bson_destroy(intern->reply);
	}
}

static zend_object* phongo_serverheartbeatsucceededevent_create_object(zend_class_entry* class_type)
{
	phongo_serverheartbeatsucceededevent_t* intern = zend_object_alloc(sizeof(phongo_serverheartbeatsucceededevent_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &phongo_handler_serverheartbeatsucceededevent;

	return &intern->std;
}

static HashTable* phongo_serverheartbeatsucceededevent_get_debug_info(zend_object* object, int* is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverheartbeatsucceededevent, object);

	zval              retval = ZVAL_STATIC_INIT;
	phongo_bson_state reply_state;

	PHONGO_BSON_INIT_STATE(reply_state);

	*is_temp = 1;
	array_init_size(&retval, 4);

	ADD_ASSOC_STRING(&retval, "host", intern->host.host);
	ADD_ASSOC_LONG_EX(&retval, "port", intern->host.port);
	ADD_ASSOC_BOOL_EX(&retval, "awaited", intern->awaited);
	ADD_ASSOC_INT64(&retval, "durationMicros", intern->duration_micros);

	if (!phongo_bson_to_zval_ex(intern->reply, &reply_state)) {
		zval_ptr_dtor(&reply_state.zchild);
		goto done;
	}

	ADD_ASSOC_ZVAL(&retval, "reply", &reply_state.zchild);

done:
	return Z_ARRVAL(retval);
}

void phongo_serverheartbeatsucceededevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverheartbeatsucceededevent_ce                = register_class_MongoDB_Driver_Monitoring_ServerHeartbeatSucceededEvent();
	phongo_serverheartbeatsucceededevent_ce->create_object = phongo_serverheartbeatsucceededevent_create_object;

	memcpy(&phongo_handler_serverheartbeatsucceededevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_serverheartbeatsucceededevent.get_debug_info = phongo_serverheartbeatsucceededevent_get_debug_info;
	phongo_handler_serverheartbeatsucceededevent.free_obj       = phongo_serverheartbeatsucceededevent_free_object;
	phongo_handler_serverheartbeatsucceededevent.offset         = XtOffsetOf(phongo_serverheartbeatsucceededevent_t, std);

	return;
}
