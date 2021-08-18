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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_topologychangedevent_ce;

/* {{{ proto MongoDB\Driver\TopologyDescription TopologyChangedEvent::getNewDescription()
   Returns this event's new description */
static PHP_METHOD(TopologyChangedEvent, getNewDescription)
{
	mongoc_topology_description_t*     topology_description;
	php_phongo_topologychangedevent_t* intern = Z_TOPOLOGYCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	topology_description = mongoc_topology_description_new_copy(intern->new_topology_description);

	phongo_topologydescription_init(return_value, topology_description);
} /* }}} */

/* {{{ proto MongoDB\Driver\TopologyDescription TopologyChangedEvent::getPreviousDescription()
   Returns this event's previous description */
static PHP_METHOD(TopologyChangedEvent, getPreviousDescription)
{
	mongoc_topology_description_t*     topology_description;
	php_phongo_topologychangedevent_t* intern = Z_TOPOLOGYCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	topology_description = mongoc_topology_description_new_copy(intern->old_topology_description);

	phongo_topologydescription_init(return_value, topology_description);
} /* }}} */

/* {{{ proto MongoDB\BSON\ObjectId TopologyChangedEvent::getTopologyId()
   Returns this event's topology id */
static PHP_METHOD(TopologyChangedEvent, getTopologyId)
{
	php_phongo_objectid_t*             topology_id;
	php_phongo_topologychangedevent_t* intern = Z_TOPOLOGYCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	object_init_ex(return_value, php_phongo_objectid_ce);

	topology_id = Z_OBJECTID_OBJ_P(return_value);
	bson_oid_to_string(&intern->topology_id, topology_id->oid);
	topology_id->initialized = true;
} /* }}} */

/* {{{ MongoDB\Driver\Monitoring\TopologyChangedEvent function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_TopologyChangedEvent_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_topologychangedevent_me[] = {
	/* clang-format off */
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_TopologyChangedEvent_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_ME(TopologyChangedEvent, getNewDescription, ai_TopologyChangedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(TopologyChangedEvent, getPreviousDescription, ai_TopologyChangedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(TopologyChangedEvent, getTopologyId, ai_TopologyChangedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_TopologyChangedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Monitoring\TopologyChangedEvent object handlers */
static zend_object_handlers php_phongo_handler_topologychangedevent;

static void php_phongo_topologychangedevent_free_object(zend_object* object) /* {{{ */
{
	php_phongo_topologychangedevent_t* intern = Z_OBJ_TOPOLOGYCHANGEDEVENT(object);

	zend_object_std_dtor(&intern->std);

	if (intern->new_topology_description) {
		mongoc_topology_description_destroy(intern->new_topology_description);
	}

	if (intern->old_topology_description) {
		mongoc_topology_description_destroy(intern->old_topology_description);
	}
} /* }}} */

static zend_object* php_phongo_topologychangedevent_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_topologychangedevent_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_topologychangedevent_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_topologychangedevent;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_topologychangedevent_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_topologychangedevent_t* intern;
	zval                               retval = ZVAL_STATIC_INIT;
	char                               topology_id[25];

	intern   = Z_OBJ_TOPOLOGYCHANGEDEVENT(PHONGO_COMPAT_GET_OBJ(object));
	*is_temp = 1;
	array_init_size(&retval, 3);

	bson_oid_to_string(&intern->topology_id, topology_id);
	ADD_ASSOC_STRING(&retval, "topologyId", topology_id);

	{
		zval new_topology_description;
		php_phongo_topology_description_to_zval(&new_topology_description, intern->new_topology_description);
		ADD_ASSOC_ZVAL_EX(&retval, "newTopologyDescription", &new_topology_description);
	}

	{
		zval old_topology_description;
		php_phongo_topology_description_to_zval(&old_topology_description, intern->old_topology_description);
		ADD_ASSOC_ZVAL_EX(&retval, "oldTopologyDescription", &old_topology_description);
	}

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_topologychangedevent_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;
	(void) type;
	(void) module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "TopologyChangedEvent", php_phongo_topologychangedevent_me);
	php_phongo_topologychangedevent_ce                = zend_register_internal_class(&ce);
	php_phongo_topologychangedevent_ce->create_object = php_phongo_topologychangedevent_create_object;
	PHONGO_CE_FINAL(php_phongo_topologychangedevent_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_topologychangedevent_ce);

	memcpy(&php_phongo_handler_topologychangedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_topologychangedevent.get_debug_info = php_phongo_topologychangedevent_get_debug_info;
	php_phongo_handler_topologychangedevent.free_obj       = php_phongo_topologychangedevent_free_object;
	php_phongo_handler_topologychangedevent.offset         = XtOffsetOf(php_phongo_topologychangedevent_t, std);

	return;
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
