/*
 * Copyright 2016-2017 MongoDB, Inc.
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

zend_class_entry* php_phongo_commandsucceededevent_ce;

/* {{{ proto string CommandSucceededEvent::getCommandName()
   Returns the command name for this event */
PHP_METHOD(CommandSucceededEvent, getCommandName)
{
	php_phongo_commandsucceededevent_t* intern;

	intern = Z_COMMANDSUCCEEDEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	PHONGO_RETVAL_STRING(intern->command_name);
} /* }}} */

/* {{{ proto int CommandSucceededEvent::getDurationMicros()
   Returns the event's duration in microseconds */
PHP_METHOD(CommandSucceededEvent, getDurationMicros)
{
	php_phongo_commandsucceededevent_t* intern;

	intern = Z_COMMANDSUCCEEDEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(intern->duration_micros);
} /* }}} */

/* {{{ proto string CommandSucceededEvent::getOperationId()
   Returns the event's operation ID */
PHP_METHOD(CommandSucceededEvent, getOperationId)
{
	php_phongo_commandsucceededevent_t* intern;
	char                                int_as_string[20];

	intern = Z_COMMANDSUCCEEDEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	sprintf(int_as_string, "%" PRIu64, intern->operation_id);
	PHONGO_RETVAL_STRING(int_as_string);
} /* }}} */

/* {{{ proto stdClass CommandSucceededEvent::getReply()
   Returns the reply document associated with the event */
PHP_METHOD(CommandSucceededEvent, getReply)
{
	php_phongo_commandsucceededevent_t* intern;
	php_phongo_bson_state               state;

	PHONGO_BSON_INIT_STATE(state);

	intern = Z_COMMANDSUCCEEDEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!php_phongo_bson_to_zval_ex(bson_get_data(intern->reply), intern->reply->len, &state)) {
		zval_ptr_dtor(&state.zchild);
		return;
	}

#if PHP_VERSION_ID >= 70000
	RETURN_ZVAL(&state.zchild, 0, 1);
#else
	RETURN_ZVAL(state.zchild, 0, 1);
#endif
} /* }}} */

/* {{{ proto string CommandsucceededEvent::getRequestId()
   Returns the event's request ID */
PHP_METHOD(CommandSucceededEvent, getRequestId)
{
	php_phongo_commandsucceededevent_t* intern;
	char                                int_as_string[20];

	intern = Z_COMMANDSUCCEEDEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	sprintf(int_as_string, "%" PRIu64, intern->request_id);
	PHONGO_RETVAL_STRING(int_as_string);
} /* }}} */

/* {{{ proto MongoDB\Driver\Server CommandSucceededEvent::getServer()
   Returns the Server from which the event originated */
PHP_METHOD(CommandSucceededEvent, getServer)
{
	php_phongo_commandsucceededevent_t* intern;

	intern = Z_COMMANDSUCCEEDEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	phongo_server_init(return_value, intern->client, intern->server_id TSRMLS_CC);
} /* }}} */

/**
 * Event thrown when a command has succeeded to execute.
 *
 * This class is only constructed internally.
 */

/* {{{ MongoDB\Driver\Monitoring\CommandSucceededEvent function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_CommandSucceededEvent_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_commandsucceededevent_me[] = {
	/* clang-format off */
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_CommandSucceededEvent_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_ME(CommandSucceededEvent, getCommandName, ai_CommandSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandSucceededEvent, getDurationMicros, ai_CommandSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandSucceededEvent, getOperationId, ai_CommandSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandSucceededEvent, getReply, ai_CommandSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandSucceededEvent, getRequestId, ai_CommandSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandSucceededEvent, getServer, ai_CommandSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_CommandSucceededEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Monitoring\CommandSucceededEvent object handlers */
static zend_object_handlers php_phongo_handler_commandsucceededevent;

static void php_phongo_commandsucceededevent_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_commandsucceededevent_t* intern = Z_OBJ_COMMANDSUCCEEDEDEVENT(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->reply) {
		bson_destroy(intern->reply);
	}
	if (intern->command_name) {
		efree(intern->command_name);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_commandsucceededevent_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_commandsucceededevent_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_commandsucceededevent_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_commandsucceededevent;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_commandsucceededevent_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_commandsucceededevent;

		return retval;
	}
#endif
} /* }}} */

static HashTable* php_phongo_commandsucceededevent_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_commandsucceededevent_t* intern;
	zval                                retval = ZVAL_STATIC_INIT;
	char                                operation_id[20], request_id[20];
	php_phongo_bson_state               reply_state;

	PHONGO_BSON_INIT_STATE(reply_state);

	intern   = Z_COMMANDSUCCEEDEDEVENT_OBJ_P(object);
	*is_temp = 1;
	array_init_size(&retval, 6);

	ADD_ASSOC_STRING(&retval, "commandName", intern->command_name);
	ADD_ASSOC_INT64(&retval, "durationMicros", (int64_t) intern->duration_micros);

	sprintf(operation_id, "%" PRIu64, intern->operation_id);
	ADD_ASSOC_STRING(&retval, "operationId", operation_id);

	if (php_phongo_bson_to_zval_ex(bson_get_data(intern->reply), intern->reply->len, &reply_state)) {
		zval_ptr_dtor(&reply_state.zchild);
		goto done;
	}

#if PHP_VERSION_ID >= 70000
	ADD_ASSOC_ZVAL(&retval, "reply", &reply_state.zchild);
#else
	ADD_ASSOC_ZVAL(&retval, "reply", reply_state.zchild);
#endif

	sprintf(request_id, "%" PRIu64, intern->request_id);
	ADD_ASSOC_STRING(&retval, "requestId", request_id);

	{
#if PHP_VERSION_ID >= 70000
		zval server;

		phongo_server_init(&server, intern->client, intern->server_id TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(&retval, "server", &server);
#else
		zval* server = NULL;

		MAKE_STD_ZVAL(server);
		phongo_server_init(server, intern->client, intern->server_id TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(&retval, "server", server);
#endif
	}

done:
	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_commandsucceededevent_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;
	(void) type;
	(void) module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "CommandSucceededEvent", php_phongo_commandsucceededevent_me);
	php_phongo_commandsucceededevent_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_commandsucceededevent_ce->create_object = php_phongo_commandsucceededevent_create_object;
	PHONGO_CE_FINAL(php_phongo_commandsucceededevent_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_commandsucceededevent_ce);

	memcpy(&php_phongo_handler_commandsucceededevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_commandsucceededevent.get_debug_info = php_phongo_commandsucceededevent_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_commandsucceededevent.free_obj = php_phongo_commandsucceededevent_free_object;
	php_phongo_handler_commandsucceededevent.offset   = XtOffsetOf(php_phongo_commandsucceededevent_t, std);
#endif

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
