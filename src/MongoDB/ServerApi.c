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

#include "mongoc/mongoc.h"

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_error.h"
#include "ServerApi_arginfo.h"

zend_class_entry* phongo_serverapi_ce;

static bool phongo_serverapi_create_libmongoc_object(mongoc_server_api_t** server_api, zend_string* version, bool strict_set, bool strict, bool deprecation_errors_set, bool deprecation_errors)
{
	mongoc_server_api_version_t server_api_version;

	if (!mongoc_server_api_version_from_string(ZSTR_VAL(version), &server_api_version)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Server API version \"%s\" is not supported in this driver version", ZSTR_VAL(version));
		return false;
	}

	if (*server_api) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Server API object already initialized. Please file a bug report as this should not happen.");
		return false;
	}

	*server_api = mongoc_server_api_new(server_api_version);

	if (strict_set) {
		mongoc_server_api_strict(*server_api, strict);
	}

	if (deprecation_errors_set) {
		mongoc_server_api_deprecation_errors(*server_api, deprecation_errors);
	}

	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_serverapi_init_from_hash(phongo_serverapi_t* intern, HashTable* props)
{
	zval* version;
	zval* strict;
	zval* deprecation_errors;

	version = zend_hash_str_find(props, ZEND_STRL("version"));
	if (!version || Z_TYPE_P(version) != IS_STRING) {
		// Exception
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"version\" field to be string", ZSTR_VAL(phongo_serverapi_ce->name));
		return false;
	}

	if ((strict = zend_hash_str_find(props, ZEND_STRL("strict"))) && !Z_ISNULL_P(strict)) {
		if (Z_TYPE_P(strict) != IS_TRUE && Z_TYPE_P(strict) != IS_FALSE) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"strict\" field to be bool or null", ZSTR_VAL(phongo_serverapi_ce->name));
			return false;
		}
	}

	if ((deprecation_errors = zend_hash_str_find(props, ZEND_STRL("deprecationErrors"))) && !Z_ISNULL_P(deprecation_errors)) {
		if (Z_TYPE_P(deprecation_errors) != IS_TRUE && Z_TYPE_P(deprecation_errors) != IS_FALSE) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"deprecationErrors\" field to be bool or null", ZSTR_VAL(phongo_serverapi_ce->name));
			return false;
		}
	}

	return phongo_serverapi_create_libmongoc_object(
		&intern->server_api,
		Z_STR_P(version),
		strict && !Z_ISNULL_P(strict),
		strict && zend_is_true(strict),
		deprecation_errors && !Z_ISNULL_P(deprecation_errors),
		deprecation_errors && zend_is_true(deprecation_errors));
}

/* Constructs a new ServerApi object */
static PHP_METHOD(MongoDB_Driver_ServerApi, __construct)
{
	PHONGO_INTERN_FROM_THIS(serverapi);

	zend_string* version;
	zend_bool    strict                  = 0;
	zend_bool    strict_null             = 1;
	zend_bool    deprecation_errors      = 0;
	zend_bool    deprecation_errors_null = 1;

	PHONGO_PARSE_PARAMETERS_START(1, 3)
	Z_PARAM_STR(version)
	Z_PARAM_OPTIONAL
	Z_PARAM_BOOL_EX(strict, strict_null, 1, 0)
	Z_PARAM_BOOL_EX(deprecation_errors, deprecation_errors_null, 1, 0)
	PHONGO_PARSE_PARAMETERS_END();

	// Will throw on failure
	phongo_serverapi_create_libmongoc_object(
		&intern->server_api,
		version,
		(bool) !strict_null,
		(bool) strict,
		(bool) !deprecation_errors_null,
		(bool) deprecation_errors);
}

static PHP_METHOD(MongoDB_Driver_ServerApi, __set_state)
{
	HashTable* props;
	zval*      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(serverapi, return_value);
	props = Z_ARRVAL_P(array);

	phongo_serverapi_init_from_hash(intern, props);
}

static HashTable* phongo_serverapi_get_properties_hash(zend_object* object, bool is_temp, bool include_null)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverapi, object);

	HashTable* props;
	zval       version, strict, deprecation_errors;
	bool       is_set;

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 1);

	ZVAL_STRING(&version, mongoc_server_api_version_to_string(mongoc_server_api_get_version(intern->server_api)));
	zend_hash_str_add(props, "version", sizeof("version") - 1, &version);

	is_set = mongoc_optional_is_set(mongoc_server_api_get_strict(intern->server_api));
	if (is_set) {
		ZVAL_BOOL(&strict, mongoc_optional_value(mongoc_server_api_get_strict(intern->server_api)));
	} else {
		ZVAL_NULL(&strict);
	}

	if (include_null || is_set) {
		zend_hash_str_add(props, "strict", sizeof("strict") - 1, &strict);
	}

	is_set = mongoc_optional_is_set(mongoc_server_api_get_deprecation_errors(intern->server_api));
	if (is_set) {
		ZVAL_BOOL(&deprecation_errors, mongoc_optional_value(mongoc_server_api_get_deprecation_errors(intern->server_api)));
	} else {
		ZVAL_NULL(&deprecation_errors);
	}

	if (include_null || is_set) {
		zend_hash_str_add(props, "deprecationErrors", sizeof("deprecationErrors") - 1, &deprecation_errors);
	}

	return props;
}

static PHP_METHOD(MongoDB_Driver_ServerApi, bsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	ZVAL_ARR(return_value, phongo_serverapi_get_properties_hash(Z_OBJ_P(getThis()), true, false));
	convert_to_object(return_value);
}

static PHP_METHOD(MongoDB_Driver_ServerApi, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(phongo_serverapi_get_properties_hash(Z_OBJ_P(getThis()), true, true));
}

static PHP_METHOD(MongoDB_Driver_ServerApi, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_serverapi_init_from_hash(Z_SERVERAPI_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\Driver\ServerApi object handlers */
static zend_object_handlers phongo_handler_serverapi;

static void phongo_serverapi_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(serverapi, object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->server_api) {
		mongoc_server_api_destroy(intern->server_api);
	}
}

static zend_object* phongo_serverapi_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(serverapi, class_type);

	intern->std.handlers = &phongo_handler_serverapi;

	return &intern->std;
}

static HashTable* phongo_serverapi_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return phongo_serverapi_get_properties_hash(object, true, true);
}

static HashTable* phongo_serverapi_get_properties(zend_object* object)
{
	return phongo_serverapi_get_properties_hash(object, false, true);
}

void phongo_serverapi_init_ce(INIT_FUNC_ARGS)
{
	phongo_serverapi_ce                = register_class_MongoDB_Driver_ServerApi(phongo_serializable_ce);
	phongo_serverapi_ce->create_object = phongo_serverapi_create_object;

	memcpy(&phongo_handler_serverapi, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_serverapi.get_debug_info = phongo_serverapi_get_debug_info;
	phongo_handler_serverapi.get_properties = phongo_serverapi_get_properties;
	phongo_handler_serverapi.free_obj       = phongo_serverapi_free_object;
	phongo_handler_serverapi.offset         = XtOffsetOf(phongo_serverapi_t, std);
}
