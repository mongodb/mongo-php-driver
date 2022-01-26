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

#include "php_phongo.h"
#include "phongo_error.h"

#include "MongoDB/ReadPreference.h"
#include "MongoDB/ServerDescription.h"
#include "MongoDB/TopologyDescription.h"

zend_class_entry* php_phongo_topologydescription_ce;

/* Note: these constants are derived from _mongoc_topology_description_type,
 * since mongoc_topology_description_t is private. */
#define PHONGO_TOPOLOGY_UNKNOWN "Unknown"
#define PHONGO_TOPOLOGY_SINGLE "Single"
#define PHONGO_TOPOLOGY_SHARDED "Sharded"
#define PHONGO_TOPOLOGY_REPLICA_SET_NO_PRIMARY "ReplicaSetNoPrimary"
#define PHONGO_TOPOLOGY_REPLICA_SET_WITH_PRIMARY "ReplicaSetWithPrimary"
#define PHONGO_TOPOLOGY_LOAD_BALANCED "LoadBalanced"

/* {{{ proto array MongoDB\Driver\TopologyDescription::getServers()
   Returns an array of ServerDescription objects for all known servers in the topology */
static PHP_METHOD(TopologyDescription, getServers)
{
	php_phongo_topologydescription_t* intern;
	mongoc_server_description_t**     sds;
	size_t                            i, n = 0;

	intern = Z_TOPOLOGYDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	sds = mongoc_topology_description_get_servers(intern->topology_description, &n);

	array_init_size(return_value, n);

	for (i = 0; i < n; i++) {
		zval sd;
		phongo_serverdescription_init(&sd, sds[i]);
		add_next_index_zval(return_value, &sd);
	}

	mongoc_server_descriptions_destroy_all(sds, n);
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\TopologyDescription::hasReadableServer([MongoDB\Driver\ReadPreference $readPreference])
    Returns whether the topology has a readable server available */
static PHP_METHOD(TopologyDescription, hasReadableServer)
{
	php_phongo_topologydescription_t* intern;
	const mongoc_read_prefs_t*        read_preference   = NULL;
	zval*                             z_read_preference = NULL;

	intern = Z_TOPOLOGYDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_OBJECT_OF_CLASS(z_read_preference, php_phongo_readpreference_ce)
	PHONGO_PARSE_PARAMETERS_END();

	if (z_read_preference) {
		read_preference = phongo_read_preference_from_zval(z_read_preference);
	}

	RETVAL_BOOL(mongoc_topology_description_has_readable_server(intern->topology_description, read_preference));
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\TopologyDescription::hasWritableServer()
    Returns whether the topology has a writable server available */
static PHP_METHOD(TopologyDescription, hasWritableServer)
{
	php_phongo_topologydescription_t* intern = Z_TOPOLOGYDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(mongoc_topology_description_has_writable_server(intern->topology_description));
} /* }}} */

/* {{{ proto string MongoDB\Driver\TopologyDescription::getType()
    Returns the topology type */
static PHP_METHOD(TopologyDescription, getType)
{
	php_phongo_topologydescription_t* intern = Z_TOPOLOGYDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(mongoc_topology_description_type(intern->topology_description));
} /* }}} */

/* {{{ MongoDB\Driver\TopologyDescription function entries */
/* clang-format off */
ZEND_BEGIN_ARG_INFO_EX(ai_TopologyDescription_hasReadableServer, 0, 0, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\Driver\\ReadPreference, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_TopologyDescription_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_topologydescription_me[] = {
	PHP_ME(TopologyDescription, getServers, ai_TopologyDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(TopologyDescription, hasReadableServer, ai_TopologyDescription_hasReadableServer, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(TopologyDescription, hasWritableServer, ai_TopologyDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(TopologyDescription, getType, ai_TopologyDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_TopologyDescription_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_FE_END
};
/* clang-format on */
/* }}} */

/* {{{ MongoDB\Driver\TopologyDescription object handlers */
static zend_object_handlers php_phongo_handler_topologydescription;

static void php_phongo_topologydescription_free_object(zend_object* object) /* {{{ */
{
	php_phongo_topologydescription_t* intern = Z_OBJ_TOPOLOGYDESCRIPTION(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->topology_description) {
		mongoc_topology_description_destroy(intern->topology_description);
	}
}

static zend_object* php_phongo_topologydescription_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_topologydescription_t* intern = zend_object_alloc(sizeof(php_phongo_topologydescription_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_topologydescription;

	return &intern->std;
} /* }}} */

HashTable* php_phongo_topologydescription_get_properties_hash(phongo_compat_object_handler_type* object, bool is_debug) /* {{{ */
{
	php_phongo_topologydescription_t* intern = NULL;
	HashTable*                        props;

	intern = Z_OBJ_TOPOLOGYDESCRIPTION(PHONGO_COMPAT_GET_OBJ(object));

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
} /* }}} */

static HashTable* php_phongo_topologydescription_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_topologydescription_get_properties_hash(object, true);
} /* }}} */

static HashTable* php_phongo_topologydescription_get_properties(phongo_compat_object_handler_type* object) /* {{{ */
{
	return php_phongo_topologydescription_get_properties_hash(object, false);
} /* }}} */
/* }}} */

void php_phongo_topologydescription_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "TopologyDescription", php_phongo_topologydescription_me);
	php_phongo_topologydescription_ce                = zend_register_internal_class(&ce);
	php_phongo_topologydescription_ce->create_object = php_phongo_topologydescription_create_object;
	PHONGO_CE_FINAL(php_phongo_topologydescription_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_topologydescription_ce);

	memcpy(&php_phongo_handler_topologydescription, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_topologydescription.get_debug_info = php_phongo_topologydescription_get_debug_info;
	php_phongo_handler_topologydescription.get_properties = php_phongo_topologydescription_get_properties;
	php_phongo_handler_topologydescription.free_obj       = php_phongo_topologydescription_free_object;
	php_phongo_handler_topologydescription.offset         = XtOffsetOf(php_phongo_topologydescription_t, std);

	zend_declare_class_constant_string(php_phongo_topologydescription_ce, ZEND_STRL("TYPE_UNKNOWN"), PHONGO_TOPOLOGY_UNKNOWN);
	zend_declare_class_constant_string(php_phongo_topologydescription_ce, ZEND_STRL("TYPE_SINGLE"), PHONGO_TOPOLOGY_SINGLE);
	zend_declare_class_constant_string(php_phongo_topologydescription_ce, ZEND_STRL("TYPE_SHARDED"), PHONGO_TOPOLOGY_SHARDED);
	zend_declare_class_constant_string(php_phongo_topologydescription_ce, ZEND_STRL("TYPE_REPLICA_SET_NO_PRIMARY"), PHONGO_TOPOLOGY_REPLICA_SET_NO_PRIMARY);
	zend_declare_class_constant_string(php_phongo_topologydescription_ce, ZEND_STRL("TYPE_REPLICA_SET_WITH_PRIMARY"), PHONGO_TOPOLOGY_REPLICA_SET_WITH_PRIMARY);
	zend_declare_class_constant_string(php_phongo_topologydescription_ce, ZEND_STRL("TYPE_LOAD_BALANCED"), PHONGO_TOPOLOGY_LOAD_BALANCED);
} /* }}} */

void phongo_topologydescription_init(zval* return_value, mongoc_topology_description_t* topology_description) /* {{{ */
{
	php_phongo_topologydescription_t* intern;

	object_init_ex(return_value, php_phongo_topologydescription_ce);

	intern                       = Z_TOPOLOGYDESCRIPTION_OBJ_P(return_value);
	intern->topology_description = mongoc_topology_description_new_copy(topology_description);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
