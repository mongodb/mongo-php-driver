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
#include "CommandStartedEvent_arginfo.h"

zend_class_entry* php_phongo_commandstartedevent_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_Monitoring_CommandStartedEvent)

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getCommand)
{
	php_phongo_commandstartedevent_t* intern;
	php_phongo_bson_state             state;

	PHONGO_BSON_INIT_STATE(state);

	intern = Z_COMMANDSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!php_phongo_bson_to_zval_ex(intern->command, &state)) {
		zval_ptr_dtor(&state.zchild);
		return;
	}

	RETURN_ZVAL(&state.zchild, 0, 1);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getCommandName)
{
	php_phongo_commandstartedevent_t* intern;

	intern = Z_COMMANDSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->command_name);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getDatabaseName)
{
	php_phongo_commandstartedevent_t* intern;

	intern = Z_COMMANDSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->database_name);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getHost)
{
	php_phongo_commandstartedevent_t* intern = Z_COMMANDSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(intern->host.host);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getOperationId)
{
	php_phongo_commandstartedevent_t* intern;
	char                              operation_id[24];

	intern = Z_COMMANDSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	snprintf(operation_id, sizeof(operation_id), "%" PRId64, intern->operation_id);
	RETVAL_STRING(operation_id);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getPort)
{
	php_phongo_commandstartedevent_t* intern = Z_COMMANDSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->host.port);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getRequestId)
{
	php_phongo_commandstartedevent_t* intern;
	char                              request_id[24];

	intern = Z_COMMANDSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	snprintf(request_id, sizeof(request_id), "%" PRId64, intern->request_id);
	RETVAL_STRING(request_id);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getServer)
{
	php_phongo_commandstartedevent_t* intern;

	intern = Z_COMMANDSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_server_init(return_value, &intern->manager, intern->server_id);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getServiceId)
{
	php_phongo_commandstartedevent_t* intern = Z_COMMANDSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->has_service_id) {
		RETURN_NULL();
	}

	phongo_objectid_new(return_value, &intern->service_id);
}

static PHP_METHOD(MongoDB_Driver_Monitoring_CommandStartedEvent, getServerConnectionId)
{
	php_phongo_commandstartedevent_t* intern = Z_COMMANDSTARTEDEVENT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	/* TODO: Use MONGOC_NO_SERVER_CONNECTION_ID once it is added to libmongoc's public API (CDRIVER-4176) */
	if (intern->server_connection_id == -1) {
		RETURN_NULL();
	}

#if SIZEOF_ZEND_LONG == 4
	if (intern->server_connection_id > INT32_MAX || intern->server_connection_id < INT32_MIN) {
		zend_error(E_WARNING, "Truncating 64-bit value %" PRId64 " for serverConnectionId", intern->server_connection_id);
	}
#endif

	RETURN_LONG(intern->server_connection_id);
}

/* MongoDB\Driver\Monitoring\CommandStartedEvent object handlers */
static zend_object_handlers php_phongo_handler_commandstartedevent;

static void php_phongo_commandstartedevent_free_object(zend_object* object)
{
	php_phongo_commandstartedevent_t* intern = Z_OBJ_COMMANDSTARTEDEVENT(object);

	zend_object_std_dtor(&intern->std);

	if (!Z_ISUNDEF(intern->manager)) {
		zval_ptr_dtor(&intern->manager);
	}

	if (intern->command) {
		bson_destroy(intern->command);
	}

	if (intern->command_name) {
		efree(intern->command_name);
	}

	if (intern->database_name) {
		efree(intern->database_name);
	}
}

static zend_object* php_phongo_commandstartedevent_create_object(zend_class_entry* class_type)
{
	php_phongo_commandstartedevent_t* intern = zend_object_alloc(sizeof(php_phongo_commandstartedevent_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_commandstartedevent;

	return &intern->std;
}

static HashTable* php_phongo_commandstartedevent_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_commandstartedevent_t* intern;
	zval                              retval = ZVAL_STATIC_INIT;
	char                              operation_id[24], request_id[24];
	php_phongo_bson_state             command_state;

	PHONGO_BSON_INIT_STATE(command_state);

	intern   = Z_OBJ_COMMANDSTARTEDEVENT(object);
	*is_temp = 1;
	array_init_size(&retval, 10);

	ADD_ASSOC_STRING(&retval, "host", intern->host.host);
	ADD_ASSOC_LONG_EX(&retval, "port", intern->host.port);
	ADD_ASSOC_STRING(&retval, "commandName", intern->command_name);
	ADD_ASSOC_STRING(&retval, "databaseName", intern->database_name);

	if (!php_phongo_bson_to_zval_ex(intern->command, &command_state)) {
		zval_ptr_dtor(&command_state.zchild);
		goto done;
	}

	ADD_ASSOC_ZVAL(&retval, "command", &command_state.zchild);

	snprintf(operation_id, sizeof(operation_id), "%" PRId64, intern->operation_id);
	ADD_ASSOC_STRING(&retval, "operationId", operation_id);

	snprintf(request_id, sizeof(request_id), "%" PRId64, intern->request_id);
	ADD_ASSOC_STRING(&retval, "requestId", request_id);

	{
		zval server;

		phongo_server_init(&server, &intern->manager, intern->server_id);
		ADD_ASSOC_ZVAL_EX(&retval, "server", &server);
	}

	if (intern->has_service_id) {
		zval service_id;

		if (!phongo_objectid_new(&service_id, &intern->service_id)) {
			/* Exception should already have been thrown */
			goto done;
		}

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
}

void php_phongo_commandstartedevent_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_commandstartedevent_ce                = register_class_MongoDB_Driver_Monitoring_CommandStartedEvent();
	php_phongo_commandstartedevent_ce->create_object = php_phongo_commandstartedevent_create_object;

	memcpy(&php_phongo_handler_commandstartedevent, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_commandstartedevent.get_debug_info = php_phongo_commandstartedevent_get_debug_info;
	php_phongo_handler_commandstartedevent.free_obj       = php_phongo_commandstartedevent_free_object;
	php_phongo_handler_commandstartedevent.offset         = XtOffsetOf(php_phongo_commandstartedevent_t, std);
}
