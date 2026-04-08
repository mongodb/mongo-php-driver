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
#include <zend_smart_str.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_error.h"

#include "MongoDB/ReadPreference.h"
#include "MongoDB/ServerDescription.h"
#include "MongoDB/TopologyDescription.h"
#include "TopologyDescription_arginfo.h"

zend_class_entry* phongo_topologydescription_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_TopologyDescription)

/* Returns an array of ServerDescription objects for all known servers in the topology */
static PHP_METHOD(MongoDB_Driver_TopologyDescription, getServers)
{
	PHONGO_INTERN_FROM_THIS(topologydescription);

	mongoc_server_description_t** sds;
	size_t                        i, n = 0;

	PHONGO_PARSE_PARAMETERS_NONE();

	sds = mongoc_topology_description_get_servers(intern->topology_description, &n);

	array_init_size(return_value, n);

	for (i = 0; i < n; i++) {
		zval sd;
		phongo_serverdescription_init(&sd, sds[i]);
		add_next_index_zval(return_value, &sd);
	}

	mongoc_server_descriptions_destroy_all(sds, n);
}

/* Returns whether the topology has a readable server available */
static PHP_METHOD(MongoDB_Driver_TopologyDescription, hasReadableServer)
{
	PHONGO_INTERN_FROM_THIS(topologydescription);

	const mongoc_read_prefs_t* read_preference   = NULL;
	zval*                      z_read_preference = NULL;

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_OBJECT_OF_CLASS(z_read_preference, phongo_readpreference_ce)
	PHONGO_PARSE_PARAMETERS_END();

	if (z_read_preference) {
		read_preference = phongo_read_preference_from_zval(z_read_preference);
	}

	RETVAL_BOOL(mongoc_topology_description_has_readable_server(intern->topology_description, read_preference));
}

/* Returns whether the topology has a writable server available */
static PHP_METHOD(MongoDB_Driver_TopologyDescription, hasWritableServer)
{
	PHONGO_INTERN_FROM_THIS(topologydescription);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(mongoc_topology_description_has_writable_server(intern->topology_description));
}

/* Returns the topology type */
static PHP_METHOD(MongoDB_Driver_TopologyDescription, getType)
{
	PHONGO_INTERN_FROM_THIS(topologydescription);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(mongoc_topology_description_type(intern->topology_description));
}

/* MongoDB\Driver\TopologyDescription object handlers */
static zend_object_handlers phongo_handler_topologydescription;

static void phongo_topologydescription_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(topologydescription, object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->topology_description) {
		mongoc_topology_description_destroy(intern->topology_description);
	}
}

static zend_object* phongo_topologydescription_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(topologydescription, class_type);

	intern->std.handlers = &phongo_handler_topologydescription;

	return &intern->std;
}

HashTable* phongo_topologydescription_get_properties_hash(zend_object* object, bool is_debug)
{
	PHONGO_INTERN_FROM_Z_OBJ(topologydescription, object);

	HashTable* props;

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 2);

	if (!intern->topology_description) {
		return props;
	}

	{
		zval                          servers;
		size_t                        i, n = 0;
		mongoc_server_description_t** sds = mongoc_topology_description_get_servers(intern->topology_description, &n);

		array_init_size(&servers, n);

		for (i = 0; i < n; i++) {
			zval sd;
			phongo_serverdescription_init(&sd, sds[i]);
			add_next_index_zval(&servers, &sd);
		}

		zend_hash_str_update(props, "servers", sizeof("servers") - 1, &servers);
		mongoc_server_descriptions_destroy_all(sds, n);
	}

	{
		zval type;

		ZVAL_STRING(&type, mongoc_topology_description_type(intern->topology_description));
		zend_hash_str_update(props, "type", sizeof("type") - 1, &type);
	}

	return props;
}

static HashTable* phongo_topologydescription_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return phongo_topologydescription_get_properties_hash(object, true);
}

static HashTable* phongo_topologydescription_get_properties(zend_object* object)
{
	return phongo_topologydescription_get_properties_hash(object, false);
}

void phongo_topologydescription_init_ce(INIT_FUNC_ARGS)
{
	phongo_topologydescription_ce                = register_class_MongoDB_Driver_TopologyDescription();
	phongo_topologydescription_ce->create_object = phongo_topologydescription_create_object;

	memcpy(&phongo_handler_topologydescription, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_topologydescription.get_debug_info = phongo_topologydescription_get_debug_info;
	phongo_handler_topologydescription.get_properties = phongo_topologydescription_get_properties;
	phongo_handler_topologydescription.free_obj       = phongo_topologydescription_free_object;
	phongo_handler_topologydescription.offset         = XtOffsetOf(phongo_topologydescription_t, std);
}

void phongo_topologydescription_init(zval* return_value, mongoc_topology_description_t* topology_description)
{
	PHONGO_INTERN_INIT_EX(topologydescription, return_value);
	intern->topology_description = mongoc_topology_description_new_copy(topology_description);
}
