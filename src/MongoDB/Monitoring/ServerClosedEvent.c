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

#include "BSON/ObjectId.h"
#include "ServerClosedEvent_arginfo.h"

zend_class_entry* phongo_serverclosedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_ServerClosedEvent)

/* Returns this event's host */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerClosedEvent, getHost)
{
	PHONGO_INTERN_FROM_THIS(serverclosedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
}

/* Returns this event's port */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerClosedEvent, getPort)
{
	PHONGO_INTERN_FROM_THIS(serverclosedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
}

/* Returns this event's topology id */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerClosedEvent, getTopologyId)
{
	PHONGO_INTERN_FROM_THIS(serverclosedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_objectid_new(return_value, &intern->topology_id);
}

static void phongo_serverclosedevent_update_properties(phongo_serverclosedevent_t* intern)
{
	zval topology_id;

	zend_update_property_string(phongo_serverclosedevent_ce, &intern->std, ZEND_STRL("host"), intern->host.host);
	zend_update_property_long(phongo_serverclosedevent_ce, &intern->std, ZEND_STRL("port"), intern->host.port);

	if (phongo_objectid_new(&topology_id, &intern->topology_id)) {
		zend_update_property(phongo_serverclosedevent_ce, &intern->std, ZEND_STRL("topologyId"), &topology_id);
		zval_ptr_dtor(&topology_id);
	}
}

/* MongoDB\Driver\Monitoring\ServerClosedEvent object handlers */
static zend_object_handlers phongo_handler_serverclosedevent;

static void phongo_serverclosedevent_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverclosedevent, object);

	zend_object_std_dtor(&intern->std);
}

static zend_object* phongo_serverclosedevent_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(serverclosedevent, class_type);

	intern->std.handlers = &phongo_handler_serverclosedevent;

	return &intern->std;
}

void phongo_serverclosedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverclosedevent_ce                = register_class_MongoDB_Driver_Monitoring_ServerClosedEvent();
	phongo_serverclosedevent_ce->create_object = phongo_serverclosedevent_create_object;

	memcpy(&phongo_handler_serverclosedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_serverclosedevent.free_obj = phongo_serverclosedevent_free_object;
	phongo_handler_serverclosedevent.offset   = XtOffsetOf(phongo_serverclosedevent_t, std);
}

void phongo_serverclosedevent_init(zval* return_value, const mongoc_apm_server_closed_t* event)
{
	PHONGO_INTERN_INIT_EX(serverclosedevent, return_value);

	memcpy(&intern->host, mongoc_apm_server_closed_get_host(event), sizeof(mongoc_host_list_t));
	mongoc_apm_server_closed_get_topology_id(event, &intern->topology_id);

	phongo_serverclosedevent_update_properties(intern);
}
