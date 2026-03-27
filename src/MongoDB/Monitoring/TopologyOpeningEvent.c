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
#include "TopologyOpeningEvent_arginfo.h"

zend_class_entry* phongo_topologyopeningevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_TopologyOpeningEvent)

/* Returns this event's topology id */
static PHP_METHOD(MongoDB_Driver_Monitoring_TopologyOpeningEvent, getTopologyId)
{
	PHONGO_INTERN_FROM_THIS(topologyopeningevent);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_objectid_new(return_value, &intern->topology_id);
}

/* MongoDB\Driver\Monitoring\TopologyOpeningEvent object handlers */
static zend_object_handlers phongo_handler_topologyopeningevent;

static void phongo_topologyopeningevent_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(topologyopeningevent, object);

	zend_object_std_dtor(&intern->std);
}

static zend_object* phongo_topologyopeningevent_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(topologyopeningevent, class_type);

	intern->std.handlers = &phongo_handler_topologyopeningevent;

	return &intern->std;
}

static HashTable* phongo_topologyopeningevent_get_debug_info(zend_object* object, int* is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(topologyopeningevent, object);

	zval retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	array_init_size(&retval, 1);

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

void phongo_topologyopeningevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_topologyopeningevent_ce                = register_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent();
	phongo_topologyopeningevent_ce->create_object = phongo_topologyopeningevent_create_object;

	memcpy(&phongo_handler_topologyopeningevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_topologyopeningevent.get_debug_info = phongo_topologyopeningevent_get_debug_info;
	phongo_handler_topologyopeningevent.free_obj       = phongo_topologyopeningevent_free_object;
	phongo_handler_topologyopeningevent.offset         = XtOffsetOf(phongo_topologyopeningevent_t, std);
}
