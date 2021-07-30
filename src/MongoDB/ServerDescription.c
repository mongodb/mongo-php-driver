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
#include <ext/standard/php_var.h>
#include <zend_smart_str.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_serverdescription_ce;

/* {{{ proto array MongoDB\Driver\ServerDescription::getHelloResponse()
   Returns the most recent "hello" response */
static PHP_METHOD(ServerDescription, getHelloResponse)
{
	php_phongo_serverdescription_t* intern;
	const bson_t*                   helloResponse;
	php_phongo_bson_state           state;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	helloResponse = mongoc_server_description_hello_response(intern->server_description);

	PHONGO_BSON_INIT_DEBUG_STATE(state);

	if (!php_phongo_bson_to_zval_ex(bson_get_data(helloResponse), helloResponse->len, &state)) {
		/* Exception should already have been thrown */
		zval_ptr_dtor(&state.zchild);
		return;
	}

	RETURN_ZVAL(&state.zchild, 0, 1);
} /* }}} */

/* {{{ proto string MongoDB\Driver\ServerDescription::getHost()
   Returns the server's hostname */
static PHP_METHOD(ServerDescription, getHost)
{
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(mongoc_server_description_host(intern->server_description)->host);
} /* }}} */

/* {{{ proto integer MongoDB\Driver\ServerDescription::getLastUpdateTime()
   Returns the server’s last update time, in microseconds */
static PHP_METHOD(ServerDescription, getLastUpdateTime)
{
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(mongoc_server_description_last_update_time(intern->server_description));
} /* }}} */

/* {{{ proto integer MongoDB\Driver\ServerDescription::getPort()
   Returns the server’s port */
static PHP_METHOD(ServerDescription, getPort)
{
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(mongoc_server_description_host(intern->server_description)->port);
} /* }}} */

/* {{{ proto integer MongoDB\Driver\ServerDescription::getRoundTripTime()
   Returns the server’s round trip time, in milliseconds */
static PHP_METHOD(ServerDescription, getRoundTripTime)
{
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(mongoc_server_description_round_trip_time(intern->server_description));
} /* }}} */

/* {{{ proto integer MongoDB\Driver\ServerDescription::getType()
   Returns the server’s node type */
static PHP_METHOD(ServerDescription, getType)
{
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(php_phongo_server_description_type(intern->server_description));
} /* }}} */

/* {{{ MongoDB\Driver\ServerDescription function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_ServerDescription_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_serverdescription_me[] = {
	/* clang-format off */
	PHP_ME(ServerDescription, getHelloResponse, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getHost, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getLastUpdateTime, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getPort, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getRoundTripTime, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getType, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_ServerDescription_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\ServerDescription object handlers */
static zend_object_handlers php_phongo_handler_serverdescription;

static void php_phongo_serverdescription_free_object(zend_object* object) /* {{{ */
{
	php_phongo_serverdescription_t* intern = Z_OBJ_SERVERDESCRIPTION(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->server_description) {
		mongoc_server_description_destroy(intern->server_description);
	}
}

static zend_object* php_phongo_serverdescription_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_serverdescription_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_serverdescription_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_serverdescription;

	return &intern->std;
} /* }}} */

HashTable* php_phongo_serverdescription_get_properties_hash(phongo_compat_object_handler_type* object, bool is_debug) /* {{{ */
{
	php_phongo_serverdescription_t* intern = NULL;
	HashTable*                      props;

	intern = Z_OBJ_SERVERDESCRIPTION(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 6);

	{
		zval                host, port, type;
		mongoc_host_list_t* host_list = mongoc_server_description_host(intern->server_description);

		ZVAL_STRING(&host, host_list->host);
		zend_hash_str_update(props, "host", sizeof("host") - 1, &host);

		ZVAL_LONG(&port, host_list->port);
		zend_hash_str_update(props, "port", sizeof("port") - 1, &port);

		ZVAL_LONG(&type, php_phongo_server_description_type(intern->server_description));
		zend_hash_str_update(props, "type", sizeof("type") - 1, &type);
	}

	{
		const bson_t*         hello_response = mongoc_server_description_hello_response(intern->server_description);
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!php_phongo_bson_to_zval_ex(bson_get_data(hello_response), hello_response->len, &state)) {
			zval_ptr_dtor(&state.zchild);
			goto done;
		}
		zend_hash_str_update(props, "hello_response", sizeof("hello_response") - 1, &state.zchild);
	}

	{
		zval last_update_time, round_trip_time;

		ZVAL_LONG(&last_update_time, mongoc_server_description_last_update_time(intern->server_description));
		zend_hash_str_update(props, "last_update_time", sizeof("last_update_time") - 1, &last_update_time);
		ZVAL_LONG(&round_trip_time, mongoc_server_description_round_trip_time(intern->server_description));
		zend_hash_str_update(props, "round_trip_time", sizeof("round_trip_time") - 1, &round_trip_time);
	}

done:
	return props;
} /* }}} */

static HashTable* php_phongo_serverdescription_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_serverdescription_get_properties_hash(object, true);
} /* }}} */

static HashTable* php_phongo_serverdescription_get_properties(phongo_compat_object_handler_type* object) /* {{{ */
{
	return php_phongo_serverdescription_get_properties_hash(object, false);
} /* }}} */
/* }}} */

void php_phongo_serverdescription_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ServerDescription", php_phongo_serverdescription_me);
	php_phongo_serverdescription_ce                = zend_register_internal_class(&ce);
	php_phongo_serverdescription_ce->create_object = php_phongo_serverdescription_create_object;
	PHONGO_CE_FINAL(php_phongo_serverdescription_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_serverdescription_ce);

	memcpy(&php_phongo_handler_serverdescription, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_serverdescription.get_debug_info = php_phongo_serverdescription_get_debug_info;
	php_phongo_handler_serverdescription.get_properties = php_phongo_serverdescription_get_properties;
	php_phongo_handler_serverdescription.free_obj       = php_phongo_serverdescription_free_object;
	php_phongo_handler_serverdescription.offset         = XtOffsetOf(php_phongo_serverdescription_t, std);

	zend_declare_class_constant_long(php_phongo_serverdescription_ce, ZEND_STRL("TYPE_UNKNOWN"), PHONGO_SERVER_UNKNOWN);
	zend_declare_class_constant_long(php_phongo_serverdescription_ce, ZEND_STRL("TYPE_STANDALONE"), PHONGO_SERVER_STANDALONE);
	zend_declare_class_constant_long(php_phongo_serverdescription_ce, ZEND_STRL("TYPE_MONGOS"), PHONGO_SERVER_MONGOS);
	zend_declare_class_constant_long(php_phongo_serverdescription_ce, ZEND_STRL("TYPE_POSSIBLE_PRIMARY"), PHONGO_SERVER_POSSIBLE_PRIMARY);
	zend_declare_class_constant_long(php_phongo_serverdescription_ce, ZEND_STRL("TYPE_RS_PRIMARY"), PHONGO_SERVER_RS_PRIMARY);
	zend_declare_class_constant_long(php_phongo_serverdescription_ce, ZEND_STRL("TYPE_RS_SECONDARY"), PHONGO_SERVER_RS_SECONDARY);
	zend_declare_class_constant_long(php_phongo_serverdescription_ce, ZEND_STRL("TYPE_RS_ARBITER"), PHONGO_SERVER_RS_ARBITER);
	zend_declare_class_constant_long(php_phongo_serverdescription_ce, ZEND_STRL("TYPE_RS_OTHER"), PHONGO_SERVER_RS_OTHER);
	zend_declare_class_constant_long(php_phongo_serverdescription_ce, ZEND_STRL("TYPE_RS_GHOST"), PHONGO_SERVER_RS_GHOST);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
