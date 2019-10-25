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

zend_class_entry* php_phongo_commandfailedevent_ce;

/* {{{ proto string CommandFailedEvent::getCommandName()
   Returns the command name for this event */
PHP_METHOD(CommandFailedEvent, getCommandName)
{
	php_phongo_commandfailedevent_t* intern;

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	PHONGO_RETVAL_STRING(intern->command_name);
} /* }}} */

/* {{{ proto int CommandFailedEvent::getDurationMicros()
   Returns the event's duration in microseconds */
PHP_METHOD(CommandFailedEvent, getDurationMicros)
{
	php_phongo_commandfailedevent_t* intern;

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(intern->duration_micros);
} /* }}} */

/* {{{ proto Exception CommandFailedEvent::getError()
   Returns the error document associated with the event */
PHP_METHOD(CommandFailedEvent, getError)
{
	php_phongo_commandfailedevent_t* intern;

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

#if PHP_VERSION_ID >= 70000
	RETURN_ZVAL(&intern->z_error, 1, 0);
#else
	RETURN_ZVAL(intern->z_error, 1, 0);
#endif
} /* }}} */

/* {{{ proto string CommandFailedEvent::getOperationId()
   Returns the event's operation ID */
PHP_METHOD(CommandFailedEvent, getOperationId)
{
	php_phongo_commandfailedevent_t* intern;
	char                             int_as_string[20];

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	sprintf(int_as_string, "%" PRIu64, intern->operation_id);
	PHONGO_RETVAL_STRING(int_as_string);
} /* }}} */

/* {{{ proto stdClass CommandFailedEvent::getReply()
   Returns the reply document associated with the event */
PHP_METHOD(CommandFailedEvent, getReply)
{
	php_phongo_commandfailedevent_t* intern;
	php_phongo_bson_state            state;

	PHONGO_BSON_INIT_STATE(state);

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

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

/* {{{ proto string CommandFailedEvent::getRequestId()
   Returns the event's request ID */
PHP_METHOD(CommandFailedEvent, getRequestId)
{
	php_phongo_commandfailedevent_t* intern;
	char                             int_as_string[20];

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	sprintf(int_as_string, "%" PRIu64, intern->request_id);
	PHONGO_RETVAL_STRING(int_as_string);
} /* }}} */

/* {{{ proto MongoDB\Driver\Server CommandFailedEvent::getServer()
   Returns the Server from which the event originated */
PHP_METHOD(CommandFailedEvent, getServer)
{
	php_phongo_commandfailedevent_t* intern;

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	phongo_server_init(return_value, intern->client, intern->server_id TSRMLS_CC);
} /* }}} */

/**
 * Event thrown when a command has failed to execute.
 *
 * This class is only constructed internally.
 */

/* {{{ MongoDB\Driver\Monitoring\CommandFailedEvent function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_CommandFailedEvent_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_commandfailedevent_me[] = {
	/* clang-format off */
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_CommandFailedEvent_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_ME(CommandFailedEvent, getCommandName, ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandFailedEvent, getError, ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandFailedEvent, getDurationMicros, ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandFailedEvent, getOperationId, ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandFailedEvent, getReply, ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandFailedEvent, getRequestId, ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandFailedEvent, getServer, ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Monitoring\CommandFailedEvent object handlers */
static zend_object_handlers php_phongo_handler_commandfailedevent;

static void php_phongo_commandfailedevent_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_commandfailedevent_t* intern = Z_OBJ_COMMANDFAILEDEVENT(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (!Z_ISUNDEF(intern->z_error)) {
		zval_ptr_dtor(&intern->z_error);
	}

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

static phongo_create_object_retval php_phongo_commandfailedevent_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_commandfailedevent_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_commandfailedevent_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_commandfailedevent;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_commandfailedevent_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_commandfailedevent;

		return retval;
	}
#endif
} /* }}} */

static HashTable* php_phongo_commandfailedevent_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_commandfailedevent_t* intern;
	zval                             retval = ZVAL_STATIC_INIT;
	char                             operation_id[20], request_id[20];
	php_phongo_bson_state            reply_state;

	PHONGO_BSON_INIT_STATE(reply_state);

	intern   = Z_COMMANDFAILEDEVENT_OBJ_P(object);
	*is_temp = 1;
	array_init_size(&retval, 6);

	ADD_ASSOC_STRING(&retval, "commandName", intern->command_name);
	ADD_ASSOC_INT64(&retval, "durationMicros", (int64_t) intern->duration_micros);

#if PHP_VERSION_ID >= 70000
	ADD_ASSOC_ZVAL_EX(&retval, "error", &intern->z_error);
	Z_ADDREF(intern->z_error);
#else
	ADD_ASSOC_ZVAL_EX(&retval, "error", intern->z_error);
	Z_ADDREF_P(intern->z_error);
#endif

	sprintf(operation_id, "%" PRIu64, intern->operation_id);
	ADD_ASSOC_STRING(&retval, "operationId", operation_id);

	if (!php_phongo_bson_to_zval_ex(bson_get_data(intern->reply), intern->reply->len, &reply_state)) {
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

void php_phongo_commandfailedevent_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;
	(void) type;
	(void) module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "CommandFailedEvent", php_phongo_commandfailedevent_me);
	php_phongo_commandfailedevent_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_commandfailedevent_ce->create_object = php_phongo_commandfailedevent_create_object;
	PHONGO_CE_FINAL(php_phongo_commandfailedevent_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_commandfailedevent_ce);

	memcpy(&php_phongo_handler_commandfailedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_commandfailedevent.get_debug_info = php_phongo_commandfailedevent_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_commandfailedevent.free_obj = php_phongo_commandfailedevent_free_object;
	php_phongo_handler_commandfailedevent.offset   = XtOffsetOf(php_phongo_commandfailedevent_t, std);
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
