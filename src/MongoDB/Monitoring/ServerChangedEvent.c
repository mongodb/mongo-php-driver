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

zend_class_entry* php_phongo_serverchangedevent_ce;

/* {{{ proto string ServerChangedEvent::getHost()
   Returns this event's host */
static PHP_METHOD(ServerChangedEvent, getHost)
{
	php_phongo_serverchangedevent_t* intern = Z_SERVERCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
} /* }}} */

/* {{{ proto integer ServerChangedEvent::getPort()
   Returns this event's port */
static PHP_METHOD(ServerChangedEvent, getPort)
{
	php_phongo_serverchangedevent_t* intern = Z_SERVERCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
} /* }}} */

/* {{{ proto MongoDB\Driver\ServerDescription ServerChangedEvent::getNewDescription()
   Returns this event's new description */
static PHP_METHOD(ServerChangedEvent, getNewDescription)
{
	php_phongo_serverchangedevent_t* intern = Z_SERVERCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_serverdescription_init(return_value, intern->new_server_description);
} /* }}} */

/* {{{ proto MongoDB\Driver\ServerDescription ServerChangedEvent::getPreviousDescription()
   Returns this event's previous description */
static PHP_METHOD(ServerChangedEvent, getPreviousDescription)
{
	php_phongo_serverchangedevent_t* intern = Z_SERVERCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_serverdescription_init(return_value, intern->old_server_description);
} /* }}} */

/* {{{ proto MongoDB\BSON\ObjectId ServerChangedEvent::getTopologyId()
   Returns this event's topology id */
static PHP_METHOD(ServerChangedEvent, getTopologyId)
{
	php_phongo_serverchangedevent_t* intern = Z_SERVERCHANGEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_objectid_init(return_value, &intern->topology_id);
} /* }}} */

/* {{{ MongoDB\Driver\Monitoring\ServerChangedEvent function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_ServerChangedEvent_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_serverchangedevent_me[] = {
	/* clang-format off */
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_ServerChangedEvent_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_ME(ServerChangedEvent, getHost, ai_ServerChangedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerChangedEvent, getPort, ai_ServerChangedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerChangedEvent, getNewDescription, ai_ServerChangedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerChangedEvent, getPreviousDescription, ai_ServerChangedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerChangedEvent, getTopologyId, ai_ServerChangedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_ServerChangedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Monitoring\ServerChangedEvent object handlers */
static zend_object_handlers php_phongo_handler_serverchangedevent;

static void php_phongo_serverchangedevent_free_object(zend_object* object) /* {{{ */
{
	php_phongo_serverchangedevent_t* intern = Z_OBJ_SERVERCHANGEDEVENT(object);

	zend_object_std_dtor(&intern->std);

	if (intern->new_server_description) {
		mongoc_server_description_destroy(intern->new_server_description);
	}

	if (intern->old_server_description) {
		mongoc_server_description_destroy(intern->old_server_description);
	}
} /* }}} */

static zend_object* php_phongo_serverchangedevent_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_serverchangedevent_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_serverchangedevent_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_serverchangedevent;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_serverchangedevent_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_serverchangedevent_t* intern;
	zval                             retval = ZVAL_STATIC_INIT;

	intern   = Z_OBJ_SERVERCHANGEDEVENT(PHONGO_COMPAT_GET_OBJ(object));
	*is_temp = 1;
	array_init_size(&retval, 4);

	ADD_ASSOC_STRING(&retval, "host", intern->host.host);
	ADD_ASSOC_LONG_EX(&retval, "port", intern->host.port);

	{
		zval topology_id;
		phongo_objectid_init(&topology_id, &intern->topology_id);
		ADD_ASSOC_ZVAL_EX(&retval, "topologyId", &topology_id);
	}

	{
		zval new_sd;
		phongo_serverdescription_init(&new_sd, intern->new_server_description);
		ADD_ASSOC_ZVAL_EX(&retval, "newDescription", &new_sd);
	}

	{
		zval old_sd;
		phongo_serverdescription_init(&old_sd, intern->old_server_description);
		ADD_ASSOC_ZVAL_EX(&retval, "oldDescription", &old_sd);
	}

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_serverchangedevent_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;
	(void) type;
	(void) module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "ServerChangedEvent", php_phongo_serverchangedevent_me);
	php_phongo_serverchangedevent_ce                = zend_register_internal_class(&ce);
	php_phongo_serverchangedevent_ce->create_object = php_phongo_serverchangedevent_create_object;
	PHONGO_CE_FINAL(php_phongo_serverchangedevent_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_serverchangedevent_ce);

	memcpy(&php_phongo_handler_serverchangedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_serverchangedevent.get_debug_info = php_phongo_serverchangedevent_get_debug_info;
	php_phongo_handler_serverchangedevent.free_obj       = php_phongo_serverchangedevent_free_object;
	php_phongo_handler_serverchangedevent.offset         = XtOffsetOf(php_phongo_serverchangedevent_t, std);

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
