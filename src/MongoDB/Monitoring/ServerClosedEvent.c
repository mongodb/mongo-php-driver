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

#include "BSON/ObjectId.h"
#include "ServerClosedEvent_arginfo.h"

zend_class_entry* php_phongo_serverclosedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_ServerClosedEvent)

/* Returns this event's host */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerClosedEvent, getHost)
{
	php_phongo_serverclosedevent_t* intern = Z_SERVERCLOSEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
}

/* Returns this event's port */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerClosedEvent, getPort)
{
	php_phongo_serverclosedevent_t* intern = Z_SERVERCLOSEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
}

/* Returns this event's topology id */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerClosedEvent, getTopologyId)
{
	php_phongo_serverclosedevent_t* intern = Z_SERVERCLOSEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_objectid_new(return_value, &intern->topology_id);
}

/* MongoDB\Driver\Monitoring\ServerClosedEvent object handlers */
static zend_object_handlers php_phongo_handler_serverclosedevent;

static void php_phongo_serverclosedevent_free_object(zend_object* object)
{
	php_phongo_serverclosedevent_t* intern = Z_OBJ_SERVERCLOSEDEVENT(object);

	zend_object_std_dtor(&intern->std);
}

static zend_object* php_phongo_serverclosedevent_create_object(zend_class_entry* class_type)
{
	php_phongo_serverclosedevent_t* intern = zend_object_alloc(sizeof(php_phongo_serverclosedevent_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_serverclosedevent;

	return &intern->std;
}

static HashTable* php_phongo_serverclosedevent_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_serverclosedevent_t* intern;
	zval                            retval = ZVAL_STATIC_INIT;

	intern   = Z_OBJ_SERVERCLOSEDEVENT(object);
	*is_temp = 1;
	array_init_size(&retval, 3);

	ADD_ASSOC_STRING(&retval, "host", intern->host.host);
	ADD_ASSOC_LONG_EX(&retval, "port", intern->host.port);

	{
		zval topology_id;

		if (!phongo_objectid_new(&topology_id, &intern->topology_id)) {
			/* Exception should already have been thrown */
			goto done;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "topologyId", &topology_id);
	}

done:
	return Z_ARRVAL(retval);
}

void php_phongo_serverclosedevent_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_serverclosedevent_ce                = register_class_MongoDB_Driver_Monitoring_ServerClosedEvent();
	php_phongo_serverclosedevent_ce->create_object = php_phongo_serverclosedevent_create_object;

	memcpy(&php_phongo_handler_serverclosedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_serverclosedevent.get_debug_info = php_phongo_serverclosedevent_get_debug_info;
	php_phongo_handler_serverclosedevent.free_obj       = php_phongo_serverclosedevent_free_object;
	php_phongo_handler_serverclosedevent.offset         = XtOffsetOf(php_phongo_serverclosedevent_t, std);
}
