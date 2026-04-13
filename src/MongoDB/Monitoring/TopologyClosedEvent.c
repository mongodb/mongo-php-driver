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
#include "TopologyClosedEvent_arginfo.h"

zend_class_entry* phongo_topologyclosedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_TopologyClosedEvent)

/* Returns this event's topology id */
static PHP_METHOD(MongoDB_Driver_Monitoring_TopologyClosedEvent, getTopologyId)
{
	PHONGO_INTERN_FROM_THIS(topologyclosedevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_objectid_new(return_value, &intern->topology_id);
}

static void phongo_topologyclosedevent_update_properties(phongo_topologyclosedevent_t* intern)
{
	zval topology_id;

	if (phongo_objectid_new(&topology_id, &intern->topology_id)) {
		zend_update_property(phongo_topologyclosedevent_ce, &intern->std, ZEND_STRL("topologyId"), &topology_id);
		zval_ptr_dtor(&topology_id);
	}
}

/* MongoDB\Driver\Monitoring\TopologyClosedEvent object handlers */
static zend_object_handlers phongo_handler_topologyclosedevent;

static void phongo_topologyclosedevent_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(topologyclosedevent, object);

	zend_object_std_dtor(&intern->std);
}

static zend_object* phongo_topologyclosedevent_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(topologyclosedevent, class_type);

	intern->std.handlers = &phongo_handler_topologyclosedevent;

	return &intern->std;
}

void phongo_topologyclosedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_topologyclosedevent_ce                = register_class_MongoDB_Driver_Monitoring_TopologyClosedEvent();
	phongo_topologyclosedevent_ce->create_object = phongo_topologyclosedevent_create_object;

	memcpy(&phongo_handler_topologyclosedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_topologyclosedevent.free_obj = phongo_topologyclosedevent_free_object;
	phongo_handler_topologyclosedevent.offset   = XtOffsetOf(phongo_topologyclosedevent_t, std);
}

void phongo_topologyclosedevent_init(zval* return_value, const mongoc_apm_topology_closed_t* event)
{
	PHONGO_INTERN_INIT_EX(topologyclosedevent, return_value);

	mongoc_apm_topology_closed_get_topology_id(event, &intern->topology_id);

	phongo_topologyclosedevent_update_properties(intern);
}
