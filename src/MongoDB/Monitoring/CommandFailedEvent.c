/*
 * Copyright 2016-present MongoDB, Inc.
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

#include "bson/bson.h"

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"

#include "BSON/ObjectId.h"
#include "MongoDB/Server.h"

zend_class_entry* php_phongo_commandfailedevent_ce;

/* {{{ proto string CommandFailedEvent::getCommandName()
   Returns the command name for this event */
PHP_METHOD(CommandFailedEvent, getCommandName)
{
	php_phongo_commandfailedevent_t* intern;

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->command_name);
} /* }}} */

/* {{{ proto int CommandFailedEvent::getDurationMicros()
   Returns the event's duration in microseconds */
PHP_METHOD(CommandFailedEvent, getDurationMicros)
{
	php_phongo_commandfailedevent_t* intern;

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->duration_micros);
} /* }}} */

/* {{{ proto Exception CommandFailedEvent::getError()
   Returns the error document associated with the event */
PHP_METHOD(CommandFailedEvent, getError)
{
	php_phongo_commandfailedevent_t* intern;

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ZVAL(&intern->z_error, 1, 0);
} /* }}} */

/* {{{ proto string CommandFailedEvent::getOperationId()
   Returns the event's operation ID */
PHP_METHOD(CommandFailedEvent, getOperationId)
{
	php_phongo_commandfailedevent_t* intern;
	char                             int_as_string[20];

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	sprintf(int_as_string, "%" PRIu64, intern->operation_id);
	RETVAL_STRING(int_as_string);
} /* }}} */

/* {{{ proto stdClass CommandFailedEvent::getReply()
   Returns the reply document associated with the event */
PHP_METHOD(CommandFailedEvent, getReply)
{
	php_phongo_commandfailedevent_t* intern;
	php_phongo_bson_state            state;

	PHONGO_BSON_INIT_STATE(state);

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!php_phongo_bson_to_zval_ex(bson_get_data(intern->reply), intern->reply->len, &state)) {
		zval_ptr_dtor(&state.zchild);
		return;
	}

	RETURN_ZVAL(&state.zchild, 0, 1);
} /* }}} */

/* {{{ proto string CommandFailedEvent::getRequestId()
   Returns the event's request ID */
PHP_METHOD(CommandFailedEvent, getRequestId)
{
	php_phongo_commandfailedevent_t* intern;
	char                             int_as_string[20];

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	sprintf(int_as_string, "%" PRIu64, intern->request_id);
	RETVAL_STRING(int_as_string);
} /* }}} */

/* {{{ proto MongoDB\Driver\Server CommandFailedEvent::getServer()
   Returns the Server from which the event originated */
PHP_METHOD(CommandFailedEvent, getServer)
{
	php_phongo_commandfailedevent_t* intern;

	intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_server_init(return_value, &intern->manager, intern->server_id);
} /* }}} */

/* {{{ proto MongoDB\BSON\ObjectId|null CommandFailedEvent::getServiceId()
   Returns the event's service ID */
PHP_METHOD(CommandFailedEvent, getServiceId)
{
	php_phongo_commandfailedevent_t* intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->has_service_id) {
		RETURN_NULL();
	}

	phongo_objectid_init(return_value, &intern->service_id);
} /* }}} */

/* {{{ proto int|null CommandFailedEvent::getServerConnectionId()
   Returns the event's server connection ID */
PHP_METHOD(CommandFailedEvent, getServerConnectionId)
{
	php_phongo_commandfailedevent_t* intern = Z_COMMANDFAILEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	/* TODO: Use MONGOC_NO_SERVER_CONNECTION_ID once it is added to libmongoc's public API (CDRIVER-4176) */
	if (intern->server_connection_id == -1) {
		RETURN_NULL();
	}

	RETURN_LONG(intern->server_connection_id);
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
	PHP_ME(CommandFailedEvent, getServiceId, ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CommandFailedEvent, getServerConnectionId, ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_CommandFailedEvent_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Monitoring\CommandFailedEvent object handlers */
static zend_object_handlers php_phongo_handler_commandfailedevent;

static void php_phongo_commandfailedevent_free_object(zend_object* object) /* {{{ */
{
	php_phongo_commandfailedevent_t* intern = Z_OBJ_COMMANDFAILEDEVENT(object);

	zend_object_std_dtor(&intern->std);

	if (!Z_ISUNDEF(intern->z_error)) {
		zval_ptr_dtor(&intern->z_error);
	}

	if (!Z_ISUNDEF(intern->manager)) {
		zval_ptr_dtor(&intern->manager);
	}

	if (intern->reply) {
		bson_destroy(intern->reply);
	}

	if (intern->command_name) {
		efree(intern->command_name);
	}
} /* }}} */

static zend_object* php_phongo_commandfailedevent_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_commandfailedevent_t* intern = zend_object_alloc(sizeof(php_phongo_commandfailedevent_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_commandfailedevent;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_commandfailedevent_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_commandfailedevent_t* intern;
	zval                             retval = ZVAL_STATIC_INIT;
	char                             operation_id[20], request_id[20];
	php_phongo_bson_state            reply_state;

	PHONGO_BSON_INIT_STATE(reply_state);

	intern   = Z_OBJ_COMMANDFAILEDEVENT(PHONGO_COMPAT_GET_OBJ(object));
	*is_temp = 1;
	array_init_size(&retval, 6);

	ADD_ASSOC_STRING(&retval, "commandName", intern->command_name);
	ADD_ASSOC_INT64(&retval, "durationMicros", (int64_t) intern->duration_micros);

	ADD_ASSOC_ZVAL_EX(&retval, "error", &intern->z_error);
	Z_ADDREF(intern->z_error);

	sprintf(operation_id, "%" PRIu64, intern->operation_id);
	ADD_ASSOC_STRING(&retval, "operationId", operation_id);

	if (!php_phongo_bson_to_zval_ex(bson_get_data(intern->reply), intern->reply->len, &reply_state)) {
		zval_ptr_dtor(&reply_state.zchild);
		goto done;
	}

	ADD_ASSOC_ZVAL(&retval, "reply", &reply_state.zchild);

	sprintf(request_id, "%" PRIu64, intern->request_id);
	ADD_ASSOC_STRING(&retval, "requestId", request_id);

	{
		zval server;

		phongo_server_init(&server, &intern->manager, intern->server_id);
		ADD_ASSOC_ZVAL_EX(&retval, "server", &server);
	}

	if (intern->has_service_id) {
		zval service_id;

		phongo_objectid_init(&service_id, &intern->service_id);
		ADD_ASSOC_ZVAL_EX(&retval, "serviceId", &service_id);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "serviceId");
	}

	/* TODO: Use MONGOC_NO_SERVER_CONNECTION_ID once it is added to libmongoc's public API (CDRIVER-4176) */
	if (intern->server_connection_id == -1) {
		ADD_ASSOC_NULL_EX(&retval, "serverConnectionId");
	} else {
		ADD_ASSOC_LONG_EX(&retval, "serverConnectionId", intern->server_connection_id);
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
	php_phongo_commandfailedevent_ce                = zend_register_internal_class(&ce);
	php_phongo_commandfailedevent_ce->create_object = php_phongo_commandfailedevent_create_object;
	PHONGO_CE_FINAL(php_phongo_commandfailedevent_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_commandfailedevent_ce);

	memcpy(&php_phongo_handler_commandfailedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_commandfailedevent.get_debug_info = php_phongo_commandfailedevent_get_debug_info;
	php_phongo_handler_commandfailedevent.free_obj       = php_phongo_commandfailedevent_free_object;
	php_phongo_handler_commandfailedevent.offset         = XtOffsetOf(php_phongo_commandfailedevent_t, std);
} /* }}} */
