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

#include "php_phongo.h"
#include "phongo_error.h"
#include "ServerHeartbeatFailedEvent_arginfo.h"

zend_class_entry* php_phongo_serverheartbeatfailedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent)

/* Returns this event's duration in microseconds */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getDurationMicros)
{
	php_phongo_serverheartbeatfailedevent_t* intern = Z_SERVERHEARTBEATFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->duration_micros);
}

/* Returns the error associated with the event */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getError)
{
	php_phongo_serverheartbeatfailedevent_t* intern = Z_SERVERHEARTBEATFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ZVAL(&intern->z_error, 1, 0);
}

/* Returns this event's host */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getHost)
{
	php_phongo_serverheartbeatfailedevent_t* intern = Z_SERVERHEARTBEATFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
}

/* Returns this event's port */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, getPort)
{
	php_phongo_serverheartbeatfailedevent_t* intern = Z_SERVERHEARTBEATFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
}

/* Returns whether this event came from an awaitable hello */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent, isAwaited)
{
	php_phongo_serverheartbeatfailedevent_t* intern = Z_SERVERHEARTBEATFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(intern->awaited);
}

/* MongoDB\Driver\Monitoring\ServerHeartbeatFailedEvent object handlers */
static zend_object_handlers php_phongo_handler_serverheartbeatfailedevent;

static void php_phongo_serverheartbeatfailedevent_free_object(zend_object* object)
{
	php_phongo_serverheartbeatfailedevent_t* intern = Z_OBJ_SERVERHEARTBEATFAILEDEVENT(object);

	zend_object_std_dtor(&intern->std);

	if (!Z_ISUNDEF(intern->z_error)) {
		zval_ptr_dtor(&intern->z_error);
	}
}

static zend_object* php_phongo_serverheartbeatfailedevent_create_object(zend_class_entry* class_type)
{
	php_phongo_serverheartbeatfailedevent_t* intern = zend_object_alloc(sizeof(php_phongo_serverheartbeatfailedevent_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_serverheartbeatfailedevent;

	return &intern->std;
}

static HashTable* php_phongo_serverheartbeatfailedevent_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_serverheartbeatfailedevent_t* intern;
	zval                                     retval = ZVAL_STATIC_INIT;

	intern   = Z_OBJ_SERVERHEARTBEATFAILEDEVENT(object);
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

void php_phongo_serverheartbeatfailedevent_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_serverheartbeatfailedevent_ce                = register_class_MongoDB_Driver_Monitoring_ServerHeartbeatFailedEvent();
	php_phongo_serverheartbeatfailedevent_ce->create_object = php_phongo_serverheartbeatfailedevent_create_object;

	memcpy(&php_phongo_handler_serverheartbeatfailedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_serverheartbeatfailedevent.get_debug_info = php_phongo_serverheartbeatfailedevent_get_debug_info;
	php_phongo_handler_serverheartbeatfailedevent.free_obj       = php_phongo_serverheartbeatfailedevent_free_object;
	php_phongo_handler_serverheartbeatfailedevent.offset         = XtOffsetOf(php_phongo_serverheartbeatfailedevent_t, std);
}
