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

#include "php_phongo.h"
#include "phongo_error.h"

#include "BSON/ObjectId.h"
#include "MongoDB/TopologyDescription.h"
#include "TopologyChangedEvent_arginfo.h"

zend_class_entry* php_phongo_topologychangedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_TopologyChangedEvent)

/* Returns this event's new description */
static PHP_METHOD(MongoDB_Driver_Monitoring_TopologyChangedEvent, getNewDescription)
{
	php_phongo_topologychangedevent_t* intern = Z_TOPOLOGYCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_topologydescription_init(return_value, intern->new_topology_description);
}

/* Returns this event's previous description */
static PHP_METHOD(MongoDB_Driver_Monitoring_TopologyChangedEvent, getPreviousDescription)
{
	php_phongo_topologychangedevent_t* intern = Z_TOPOLOGYCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_topologydescription_init(return_value, intern->old_topology_description);
}

/* Returns this event's topology id */
static PHP_METHOD(MongoDB_Driver_Monitoring_TopologyChangedEvent, getTopologyId)
{
	php_phongo_topologychangedevent_t* intern = Z_TOPOLOGYCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_objectid_new(return_value, &intern->topology_id);
}

/* MongoDB\Driver\Monitoring\TopologyChangedEvent object handlers */
static zend_object_handlers php_phongo_handler_topologychangedevent;

static void php_phongo_topologychangedevent_free_object(zend_object* object)
{
	php_phongo_topologychangedevent_t* intern = Z_OBJ_TOPOLOGYCHANGEDEVENT(object);

	zend_object_std_dtor(&intern->std);

	if (intern->new_topology_description) {
		mongoc_topology_description_destroy(intern->new_topology_description);
	}

	if (intern->old_topology_description) {
		mongoc_topology_description_destroy(intern->old_topology_description);
	}
}

static zend_object* php_phongo_topologychangedevent_create_object(zend_class_entry* class_type)
{
	php_phongo_topologychangedevent_t* intern = zend_object_alloc(sizeof(php_phongo_topologychangedevent_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_topologychangedevent;

	return &intern->std;
}

static HashTable* php_phongo_topologychangedevent_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_topologychangedevent_t* intern;
	zval                               retval = ZVAL_STATIC_INIT;

	intern   = Z_OBJ_TOPOLOGYCHANGEDEVENT(object);
	*is_temp = 1;
	array_init_size(&retval, 3);

	{
		zval topology_id;

		if (!phongo_objectid_new(&topology_id, &intern->topology_id)) {
			/* Exception should already have been thrown */
			goto done;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "topologyId", &topology_id);
	}

	{
		zval new_td;
		phongo_topologydescription_init(&new_td, intern->new_topology_description);
		ADD_ASSOC_ZVAL_EX(&retval, "newDescription", &new_td);
	}

	{
		zval old_td;
		phongo_topologydescription_init(&old_td, intern->old_topology_description);
		ADD_ASSOC_ZVAL_EX(&retval, "oldDescription", &old_td);
	}

done:
	return Z_ARRVAL(retval);
}

void php_phongo_topologychangedevent_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_topologychangedevent_ce                = register_class_MongoDB_Driver_Monitoring_TopologyChangedEvent();
	php_phongo_topologychangedevent_ce->create_object = php_phongo_topologychangedevent_create_object;

	memcpy(&php_phongo_handler_topologychangedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_topologychangedevent.get_debug_info = php_phongo_topologychangedevent_get_debug_info;
	php_phongo_handler_topologychangedevent.free_obj       = php_phongo_topologychangedevent_free_object;
	php_phongo_handler_topologychangedevent.offset         = XtOffsetOf(php_phongo_topologychangedevent_t, std);
}
