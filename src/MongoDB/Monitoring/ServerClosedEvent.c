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

PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerClosedEvent, getHost, serverclosedevent, "host")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerClosedEvent, getPort, serverclosedevent, "port")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_Monitoring_ServerClosedEvent, getTopologyId, serverclosedevent, "topologyId")

static void phongo_serverclosedevent_update_properties(zend_object* object, const mongoc_apm_server_closed_t* event)
{
	zval       ztopology_id;
	bson_oid_t topology_id;

	zend_update_property_string(phongo_serverclosedevent_ce, object, ZEND_STRL("host"), mongoc_apm_server_closed_get_host(event)->host);
	zend_update_property_long(phongo_serverclosedevent_ce, object, ZEND_STRL("port"), mongoc_apm_server_closed_get_host(event)->port);

	mongoc_apm_server_closed_get_topology_id(event, &topology_id);

	if (phongo_objectid_new(&ztopology_id, &topology_id)) {
		zend_update_property(phongo_serverclosedevent_ce, object, ZEND_STRL("topologyId"), &ztopology_id);
		zval_ptr_dtor(&ztopology_id);
	}
}

void phongo_serverclosedevent_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverclosedevent_ce = register_class_MongoDB_Driver_Monitoring_ServerClosedEvent();
}

void phongo_serverclosedevent_init(zval* return_value, const mongoc_apm_server_closed_t* event)
{
	PHONGO_OBJECT_INIT_EX(serverclosedevent, return_value);

	phongo_serverclosedevent_update_properties(object, event);
}
