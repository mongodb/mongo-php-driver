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

/* {{{ proto string|null MongoDB\Driver\ServerDescription::getHelloResponse()
   Returns the most recent “hello” response */
static PHP_METHOD(ServerDescription, getHelloResponse)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;
	const bson_t*                   helloResponse;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	helloResponse = mongoc_server_description_hello_response(intern->server_description);

	if (helloResponse->len) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!php_phongo_bson_to_zval_ex(bson_get_data(helloResponse), helloResponse->len, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		RETURN_ZVAL(&state.zchild, 0, 1);
	} else {
		RETURN_NULL();
	}
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::getHost()
   Returns the server’s hostname */
static PHP_METHOD(ServerDescription, getHost)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRING(mongoc_server_description_host(intern->server_description)->host);
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::getId()
   Returns the server’s id */
static PHP_METHOD(ServerDescription, getId)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG((zend_long) mongoc_server_description_id(intern->server_description));
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::getLastUpdateTime()
   Returns the server’s last update time, in microseconds */
static PHP_METHOD(ServerDescription, getLastUpdateTime)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG((zend_long) mongoc_server_description_last_update_time(intern->server_description));
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::getPort()
   Returns the server’s port */
static PHP_METHOD(ServerDescription, getPort)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(mongoc_server_description_host(intern->server_description)->port);
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::getRoundTripTime()
   Returns the server’s round trip time, in milliseconds */
static PHP_METHOD(ServerDescription, getRoundTripTime)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG((zend_long) mongoc_server_description_round_trip_time(intern->server_description));
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::getTags()
   Returns the server's currently configured tags */
static PHP_METHOD(ServerDescription, getTags)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;
	const bson_t*                   helloResponse;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	helloResponse = mongoc_server_description_hello_response(intern->server_description);

	if (helloResponse->len) {
		bson_iter_t iter;

		if (bson_iter_init_find(&iter, helloResponse, "tags") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
			const uint8_t*        bytes;
			uint32_t              len;
			php_phongo_bson_state state;

			PHONGO_BSON_INIT_DEBUG_STATE(state);
			bson_iter_document(&iter, &len, &bytes);

			if (!php_phongo_bson_to_zval_ex(bytes, len, &state)) {
				zval_ptr_dtor(&state.zchild);
				return;
			}

			RETURN_ZVAL(&state.zchild, 0, 1);
		}

		array_init(return_value);
	}
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::getType()
   Returns the server’s node type */
static PHP_METHOD(ServerDescription, getType)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(php_phongo_server_description_type(intern->server_description));
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::isArbiter()
   Returns whether the server is an arbiter member of a replica set */
static PHP_METHOD(ServerDescription, isArbiter)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(!strcmp(mongoc_server_description_type(intern->server_description), php_phongo_server_description_type_map[PHONGO_SERVER_RS_ARBITER].name));
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::isHidden()
   Returns whether the server is a hidden member of a replica set */
static PHP_METHOD(ServerDescription, isHidden)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	bson_iter_t iter;
	RETVAL_BOOL(bson_iter_init_find_case(&iter, mongoc_server_description_hello_response(intern->server_description), "hidden") && bson_iter_as_bool(&iter));
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::isPassive()
   Returns whether the server is a passive member of a replica set */
static PHP_METHOD(ServerDescription, isPassive)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	bson_iter_t iter;
	RETVAL_BOOL(bson_iter_init_find_case(&iter, mongoc_server_description_hello_response(intern->server_description), "passive") && bson_iter_as_bool(&iter));
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::isPrimary()
   Returns whether the server is a primary member of a replica set */
static PHP_METHOD(ServerDescription, isPrimary)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(!strcmp(mongoc_server_description_type(intern->server_description), php_phongo_server_description_type_map[PHONGO_SERVER_RS_PRIMARY].name));
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ServerDescription::isSecondary()
   Returns whether the server is a secondary member of a replica set */
static PHP_METHOD(ServerDescription, isSecondary)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_BOOL(!strcmp(mongoc_server_description_type(intern->server_description), php_phongo_server_description_type_map[PHONGO_SERVER_RS_SECONDARY].name));
} /* }}} */

/* {{{ MongoDB\Driver\ServerDescription function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_ServerDescription_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_serverdescription_me[] = {
	/* clang-format off */
	PHP_ME(ServerDescription, getHelloResponse, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getHost, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getId, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getLastUpdateTime, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getPort, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getRoundTripTime, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getTags, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, getType, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, isArbiter, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, isHidden, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, isPassive, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, isPrimary, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerDescription, isSecondary, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
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

static HashTable* php_phongo_serverdescription_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_serverdescription_t* intern = NULL;
	zval                            retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_OBJ_SERVERDESCRIPTION(PHONGO_COMPAT_GET_OBJ(object));

	php_phongo_server_to_zval(&retval, intern->server_description);

	return Z_ARRVAL(retval);
} /* }}} */

static HashTable* php_phongo_serverdescription_get_properties(phongo_compat_object_handler_type* object) /* {{{ */
{
	// return php_phongo_serverdescription_get_properties_hash(object, false);
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

	zend_class_implements(php_phongo_serverdescription_ce, 1, zend_ce_iterator);

	memcpy(&php_phongo_handler_serverdescription, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_serverdescription.get_debug_info = php_phongo_serverdescription_get_debug_info;
	php_phongo_handler_serverdescription.get_properties = php_phongo_serverdescription_get_properties;
	php_phongo_handler_serverdescription.free_obj       = php_phongo_serverdescription_free_object;
	php_phongo_handler_serverdescription.offset         = XtOffsetOf(php_phongo_serverdescription_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
