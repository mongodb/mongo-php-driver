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

zend_class_entry* php_phongo_serverheartbeatsucceededevent_ce;

/* {{{ proto integer ServerHeartbeatSucceededEvent::getDurationMicros()
   Returns this event's duration in microseconds */
static PHP_METHOD(ServerHeartbeatSucceededEvent, getDurationMicros)
{
	php_phongo_serverheartbeatsucceededevent_t* intern = Z_SERVERHEARTBEATSUCCEEDEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->duration_micros);
} /* }}} */

/* {{{ proto string ServerHeartbeatSucceededEvent::getHost()
   Returns this event's host */
static PHP_METHOD(ServerHeartbeatSucceededEvent, getHost)
{
	php_phongo_serverheartbeatsucceededevent_t* intern = Z_SERVERHEARTBEATSUCCEEDEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
} /* }}} */

/* {{{ proto integer ServerHeartbeatSucceededEvent::getPort()
   Returns this event's port */
static PHP_METHOD(ServerHeartbeatSucceededEvent, getPort)
{
	php_phongo_serverheartbeatsucceededevent_t* intern = Z_SERVERHEARTBEATSUCCEEDEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
} /* }}} */

/* {{{ proto integer ServerHeartbeatSucceededEvent::getReply()
   Returns this event's reply */
static PHP_METHOD(ServerHeartbeatSucceededEvent, getReply)
{
	php_phongo_serverheartbeatsucceededevent_t* intern = Z_SERVERHEARTBEATSUCCEEDEDEVENT_OBJ_P(getThis());
	php_phongo_bson_state                       state;

	PHONGO_BSON_INIT_STATE(state);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!php_phongo_bson_to_zval_ex(bson_get_data(intern->reply), intern->reply->len, &state)) {
		zval_ptr_dtor(&state.zchild);
		return;
	}

	RETURN_ZVAL(&state.zchild, 0, 1);
} /* }}} */

/* {{{ proto boolean ServerHeartbeatSucceededEvent::isAwaited()
   Returns whether this event came from an awaitable hello */
static PHP_METHOD(ServerHeartbeatSucceededEvent, isAwaited)
{
	php_phongo_serverheartbeatsucceededevent_t* intern = Z_SERVERHEARTBEATSUCCEEDEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(intern->awaited);
} /* }}} */

/* {{{ MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_ServerHeartbeatSucceededEvent_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_serverheartbeatsucceededevent_me[] = {
	/* clang-format off */
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_ServerHeartbeatSucceededEvent_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_ME(ServerHeartbeatSucceededEvent, getDurationMicros, ai_ServerHeartbeatSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerHeartbeatSucceededEvent, getHost, ai_ServerHeartbeatSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerHeartbeatSucceededEvent, getPort, ai_ServerHeartbeatSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerHeartbeatSucceededEvent, getReply, ai_ServerHeartbeatSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerHeartbeatSucceededEvent, isAwaited, ai_ServerHeartbeatSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_ServerHeartbeatSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Monitoring\ServerHeartbeatSucceededEvent object handlers */
static zend_object_handlers php_phongo_handler_serverheartbeatsucceededevent;

static void php_phongo_serverheartbeatsucceededevent_free_object(zend_object* object) /* {{{ */
{
	php_phongo_serverheartbeatsucceededevent_t* intern = Z_OBJ_SERVERHEARTBEATSUCCEEDEDEVENT(object);

	zend_object_std_dtor(&intern->std);

	if (intern->reply) {
		bson_destroy(intern->reply);
	}
} /* }}} */

static zend_object* php_phongo_serverheartbeatsucceededevent_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_serverheartbeatsucceededevent_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_serverheartbeatsucceededevent_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_serverheartbeatsucceededevent;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_serverheartbeatsucceededevent_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_serverheartbeatsucceededevent_t* intern;
	zval                                        retval = ZVAL_STATIC_INIT;
	php_phongo_bson_state                       reply_state;

	PHONGO_BSON_INIT_STATE(reply_state);

	intern   = Z_OBJ_SERVERHEARTBEATSUCCEEDEDEVENT(PHONGO_COMPAT_GET_OBJ(object));
	*is_temp = 1;
	array_init_size(&retval, 4);

	ADD_ASSOC_STRING(&retval, "host", intern->host.host);
	ADD_ASSOC_LONG_EX(&retval, "port", intern->host.port);
	ADD_ASSOC_BOOL_EX(&retval, "awaited", intern->awaited);

	if (!php_phongo_bson_to_zval_ex(bson_get_data(intern->reply), intern->reply->len, &reply_state)) {
		zval_ptr_dtor(&reply_state.zchild);
		goto done;
	}

	ADD_ASSOC_ZVAL(&retval, "reply", &reply_state.zchild);

done:
	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_serverheartbeatsucceededevent_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;
	(void) type;
	(void) module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "ServerHeartbeatSucceededEvent", php_phongo_serverheartbeatsucceededevent_me);
	php_phongo_serverheartbeatsucceededevent_ce                = zend_register_internal_class(&ce);
	php_phongo_serverheartbeatsucceededevent_ce->create_object = php_phongo_serverheartbeatsucceededevent_create_object;
	PHONGO_CE_FINAL(php_phongo_serverheartbeatsucceededevent_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_serverheartbeatsucceededevent_ce);

	memcpy(&php_phongo_handler_serverheartbeatsucceededevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_serverheartbeatsucceededevent.get_debug_info = php_phongo_serverheartbeatsucceededevent_get_debug_info;
	php_phongo_handler_serverheartbeatsucceededevent.free_obj       = php_phongo_serverheartbeatsucceededevent_free_object;
	php_phongo_handler_serverheartbeatsucceededevent.offset         = XtOffsetOf(php_phongo_serverheartbeatsucceededevent_t, std);

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
