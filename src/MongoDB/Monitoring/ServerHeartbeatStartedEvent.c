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

#include "php_phongo.h"
#include "phongo_error.h"

zend_class_entry* php_phongo_serverheartbeatstartedevent_ce;

/* {{{ proto string ServerHeartbeatStartedEvent::getHost()
   Returns this event's host */
static PHP_METHOD(ServerHeartbeatStartedEvent, getHost)
{
	php_phongo_serverheartbeatstartedevent_t* intern = Z_SERVERHEARTBEATSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
} /* }}} */

/* {{{ proto integer ServerHeartbeatStartedEvent::getPort()
   Returns this event's port */
static PHP_METHOD(ServerHeartbeatStartedEvent, getPort)
{
	php_phongo_serverheartbeatstartedevent_t* intern = Z_SERVERHEARTBEATSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
} /* }}} */

/* {{{ proto boolean ServerHeartbeatStartedEvent::isAwaited()
   Returns whether this event came from an awaitable hello */
static PHP_METHOD(ServerHeartbeatStartedEvent, isAwaited)
{
	php_phongo_serverheartbeatstartedevent_t* intern = Z_SERVERHEARTBEATSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(intern->awaited);
} /* }}} */

/* {{{ MongoDB\Driver\Monitoring\ServerHeartbeatStartedEvent function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_ServerHeartbeatStartedEvent_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_serverheartbeatstartedevent_me[] = {
	/* clang-format off */
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_ServerHeartbeatStartedEvent_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_ME(ServerHeartbeatStartedEvent, getHost, ai_ServerHeartbeatStartedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerHeartbeatStartedEvent, getPort, ai_ServerHeartbeatStartedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerHeartbeatStartedEvent, isAwaited, ai_ServerHeartbeatStartedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_ServerHeartbeatStartedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Monitoring\ServerHeartbeatStartedEvent object handlers */
static zend_object_handlers php_phongo_handler_serverheartbeatstartedevent;

static void php_phongo_serverheartbeatstartedevent_free_object(zend_object* object) /* {{{ */
{
	php_phongo_serverheartbeatstartedevent_t* intern = Z_OBJ_SERVERHEARTBEATSTARTEDEVENT(object);

	zend_object_std_dtor(&intern->std);
} /* }}} */

static zend_object* php_phongo_serverheartbeatstartedevent_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_serverheartbeatstartedevent_t* intern = zend_object_alloc(sizeof(php_phongo_serverheartbeatstartedevent_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_serverheartbeatstartedevent;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_serverheartbeatstartedevent_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_serverheartbeatstartedevent_t* intern;
	zval                                      retval = ZVAL_STATIC_INIT;

	intern   = Z_OBJ_SERVERHEARTBEATSTARTEDEVENT(PHONGO_COMPAT_GET_OBJ(object));
	*is_temp = 1;
	array_init_size(&retval, 4);

	ADD_ASSOC_STRING(&retval, "host", intern->host.host);
	ADD_ASSOC_LONG_EX(&retval, "port", intern->host.port);
	ADD_ASSOC_BOOL_EX(&retval, "awaited", intern->awaited);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_serverheartbeatstartedevent_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;
	(void) type;
	(void) module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "ServerHeartbeatStartedEvent", php_phongo_serverheartbeatstartedevent_me);
	php_phongo_serverheartbeatstartedevent_ce                = zend_register_internal_class(&ce);
	php_phongo_serverheartbeatstartedevent_ce->create_object = php_phongo_serverheartbeatstartedevent_create_object;
	PHONGO_CE_FINAL(php_phongo_serverheartbeatstartedevent_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_serverheartbeatstartedevent_ce);

	memcpy(&php_phongo_handler_serverheartbeatstartedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_serverheartbeatstartedevent.get_debug_info = php_phongo_serverheartbeatstartedevent_get_debug_info;
	php_phongo_handler_serverheartbeatstartedevent.free_obj       = php_phongo_serverheartbeatstartedevent_free_object;
	php_phongo_handler_serverheartbeatstartedevent.offset         = XtOffsetOf(php_phongo_serverheartbeatstartedevent_t, std);

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
