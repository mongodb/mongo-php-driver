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

PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_TopologyOpeningEvent, getTopologyId, topologyopeningevent, "topologyId")

static void phongo_topologyopeningevent_update_properties(zend_object* object, const mongoc_apm_topology_opening_t* event)
{
	zval       ztopology_id;
	bson_oid_t topology_id;

	mongoc_apm_topology_opening_get_topology_id(event, &topology_id);

	if (phongo_objectid_new(&ztopology_id, &topology_id)) {
		zend_update_property(phongo_topologyopeningevent_ce, object, ZEND_STRL("topologyId"), &ztopology_id);
		zval_ptr_dtor(&ztopology_id);
	}
}

void phongo_topologyopeningevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_topologyopeningevent_ce = register_class_MongoDB_Driver_Monitoring_TopologyOpeningEvent();
}

void phongo_topologyopeningevent_init(zval* return_value, const mongoc_apm_topology_opening_t* event)
{
	PHONGO_OBJECT_INIT_EX(topologyopeningevent, return_value);

	phongo_topologyopeningevent_update_properties(object, event);
}
