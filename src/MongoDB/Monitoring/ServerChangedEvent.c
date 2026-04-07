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

#include "mongoc/mongoc.h"

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_error.h"

#include "BSON/ObjectId.h"
#include "MongoDB/Server.h"
#include "MongoDB/ServerDescription.h"
#include "ServerChangedEvent_arginfo.h"

zend_class_entry* phongo_serverchangedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_ServerChangedEvent)

/* Returns this event's host */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, getHost)
{
	PHONGO_INTERN_FROM_THIS(serverchangedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
}

/* Returns this event's port */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, getPort)
{
	PHONGO_INTERN_FROM_THIS(serverchangedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
}

/* Returns this event's new description */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, getNewDescription)
{
	PHONGO_INTERN_FROM_THIS(serverchangedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_serverdescription_init(return_value, intern->new_server_description);
}

/* Returns this event's previous description */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, getPreviousDescription)
{
	PHONGO_INTERN_FROM_THIS(serverchangedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_serverdescription_init(return_value, intern->old_server_description);
}

/* Returns this event's topology id */
static PHP_METHOD(MongoDB_Driver_Monitoring_ServerChangedEvent, getTopologyId)
{
	PHONGO_INTERN_FROM_THIS(serverchangedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_objectid_new(return_value, &intern->topology_id);
}

static void phongo_serverchangedevent_update_properties(phongo_serverchangedevent_t* intern)
{
	zval topology_id, new_sd, old_sd;

	zend_update_property_string(phongo_serverchangedevent_ce, &intern->std, ZEND_STRL("host"), intern->host.host);
	zend_update_property_long(phongo_serverchangedevent_ce, &intern->std, ZEND_STRL("port"), intern->host.port);

	if (phongo_objectid_new(&topology_id, &intern->topology_id)) {
		zend_update_property(phongo_serverchangedevent_ce, &intern->std, ZEND_STRL("topologyId"), &topology_id);
		zval_ptr_dtor(&topology_id);
	}

	phongo_serverdescription_init(&new_sd, intern->new_server_description);
	zend_update_property(phongo_serverchangedevent_ce, &intern->std, ZEND_STRL("newDescription"), &new_sd);
	zval_ptr_dtor(&new_sd);

	phongo_serverdescription_init(&old_sd, intern->old_server_description);
	zend_update_property(phongo_serverchangedevent_ce, &intern->std, ZEND_STRL("previousDescription"), &old_sd);
	zval_ptr_dtor(&old_sd);
}

/* MongoDB\Driver\Monitoring\ServerChangedEvent object handlers */
static zend_object_handlers phongo_handler_serverchangedevent;

static void phongo_serverchangedevent_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverchangedevent, object);

	zend_object_std_dtor(&intern->std);

	if (intern->new_server_description) {
		mongoc_server_description_destroy(intern->new_server_description);
	}

	if (intern->old_server_description) {
		mongoc_server_description_destroy(intern->old_server_description);
	}
}

static zend_object* phongo_serverchangedevent_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(serverchangedevent, class_type);

	intern->std.handlers = &phongo_handler_serverchangedevent;

	return &intern->std;
}

void phongo_serverchangedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverchangedevent_ce                = register_class_MongoDB_Driver_Monitoring_ServerChangedEvent();
	phongo_serverchangedevent_ce->create_object = phongo_serverchangedevent_create_object;

	memcpy(&phongo_handler_serverchangedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_serverchangedevent.free_obj = phongo_serverchangedevent_free_object;
	phongo_handler_serverchangedevent.offset   = XtOffsetOf(phongo_serverchangedevent_t, std);
}

void phongo_serverchangedevent_init(zval* return_value, const mongoc_apm_server_changed_t* event)
{
	PHONGO_INTERN_INIT_EX(serverchangedevent, return_value);

	memcpy(&intern->host, mongoc_apm_server_changed_get_host(event), sizeof(mongoc_host_list_t));
	mongoc_apm_server_changed_get_topology_id(event, &intern->topology_id);
	intern->new_server_description = mongoc_server_description_new_copy(mongoc_apm_server_changed_get_new_description(event));
	intern->old_server_description = mongoc_server_description_new_copy(mongoc_apm_server_changed_get_previous_description(event));

	phongo_serverchangedevent_update_properties(intern);
}
