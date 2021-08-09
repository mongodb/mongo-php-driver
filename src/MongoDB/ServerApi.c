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

zend_class_entry* php_phongo_serverapi_ce;

static bool php_phongo_serverapi_create_libmongoc_object(mongoc_server_api_t** server_api, zend_string* version, bool strict_set, bool strict, bool deprecation_errors_set, bool deprecation_errors) /* {{{ */
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
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_serverapi_init_from_hash(php_phongo_serverapi_t* intern, HashTable* props) /* {{{ */
{
	zval* version;
	zval* strict;
	zval* deprecation_errors;

	version = zend_hash_str_find(props, ZEND_STRL("version"));
	if (!version || Z_TYPE_P(version) != IS_STRING) {
		// Exception
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"version\" field to be string", ZSTR_VAL(php_phongo_serverapi_ce->name));
		return false;
	}

	if ((strict = zend_hash_str_find(props, ZEND_STRL("strict"))) && !ZVAL_IS_NULL(strict)) {
		if (Z_TYPE_P(strict) != IS_TRUE && Z_TYPE_P(strict) != IS_FALSE) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"strict\" field to be bool or null", ZSTR_VAL(php_phongo_serverapi_ce->name));
			return false;
		}
	}

	if ((deprecation_errors = zend_hash_str_find(props, ZEND_STRL("deprecationErrors"))) && !ZVAL_IS_NULL(deprecation_errors)) {
		if (Z_TYPE_P(deprecation_errors) != IS_TRUE && Z_TYPE_P(deprecation_errors) != IS_FALSE) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"deprecationErrors\" field to be bool or null", ZSTR_VAL(php_phongo_serverapi_ce->name));
			return false;
		}
	}

	return php_phongo_serverapi_create_libmongoc_object(
		&intern->server_api,
		Z_STR_P(version),
		strict && !ZVAL_IS_NULL(strict),
		strict && zval_is_true(strict),
		deprecation_errors && !ZVAL_IS_NULL(deprecation_errors),
		deprecation_errors && zval_is_true(deprecation_errors));
} /* }}} */

/* {{{ proto void MongoDB\Driver\ServerApi::__construct(string $version, [?bool $strict], [?bool $deprecationErrors])
   Constructs a new ServerApi object */
static PHP_METHOD(ServerApi, __construct)
{
	php_phongo_serverapi_t* intern;
	zend_string*            version;
	zend_bool               strict                  = 0;
	zend_bool               strict_null             = 1;
	zend_bool               deprecation_errors      = 0;
	zend_bool               deprecation_errors_null = 1;

	intern = Z_SERVERAPI_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 3)
	Z_PARAM_STR(version)
	Z_PARAM_OPTIONAL
	Z_PARAM_BOOL_EX(strict, strict_null, 1, 0)
	Z_PARAM_BOOL_EX(deprecation_errors, deprecation_errors_null, 1, 0)
	PHONGO_PARSE_PARAMETERS_END();

	// Will throw on failure
	php_phongo_serverapi_create_libmongoc_object(
		&intern->server_api,
		version,
		(bool) !strict_null,
		(bool) strict,
		(bool) !deprecation_errors_null,
		(bool) deprecation_errors);
} /* }}} */

/* {{{ proto MongoDB\Driver\ServerApi MongoDB\Driver\ServerApi::__set_state(array $properties)
*/
static PHP_METHOD(ServerApi, __set_state)
{
	php_phongo_serverapi_t* intern;
	HashTable*              props;
	zval*                   array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_serverapi_ce);

	intern = Z_SERVERAPI_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_serverapi_init_from_hash(intern, props);
} /* }}} */

static HashTable* php_phongo_serverapi_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp, bool include_null) /* {{{ */
{
	php_phongo_serverapi_t* intern;
	HashTable*              props;
	zval                    version, strict, deprecation_errors;
	bool                    is_set;

	intern = Z_OBJ_SERVERAPI(PHONGO_COMPAT_GET_OBJ(object));

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
} /* }}} */

/* {{{ proto array MongoDB\Driver\ServerApi::bsonSerialize()
*/
static PHP_METHOD(ServerApi, bsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	ZVAL_ARR(return_value, php_phongo_serverapi_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true, false));
	convert_to_object(return_value);
} /* }}} */

/* {{{ proto string MongoDB\Driver\ServerApi::serialize()
*/
static PHP_METHOD(ServerApi, serialize)
{
	php_phongo_serverapi_t* intern;
	zval                    retval;
	php_serialize_data_t    var_hash;
	smart_str               buf = { 0 };

	intern = Z_SERVERAPI_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(&retval, 3);

	ADD_ASSOC_STRING(&retval, "version", mongoc_server_api_version_to_string(mongoc_server_api_get_version(intern->server_api)));

	if (mongoc_optional_is_set(mongoc_server_api_get_strict(intern->server_api))) {
		ADD_ASSOC_BOOL_EX(&retval, "strict", mongoc_optional_value(mongoc_server_api_get_strict(intern->server_api)));
	} else {
		ADD_ASSOC_NULL_EX(&retval, "strict");
	}

	if (mongoc_optional_is_set(mongoc_server_api_get_deprecation_errors(intern->server_api))) {
		ADD_ASSOC_BOOL_EX(&retval, "deprecationErrors", mongoc_optional_value(mongoc_server_api_get_deprecation_errors(intern->server_api)));
	} else {
		ADD_ASSOC_NULL_EX(&retval, "deprecationErrors");
	}

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\Driver\ServerApi::unserialize(string $serialized)
*/
static PHP_METHOD(ServerApi, unserialize)
{
	php_phongo_serverapi_t* intern;
	char*                   serialized;
	size_t                  serialized_len;
	zval                    props;
	php_unserialize_data_t  var_hash;

	intern = Z_SERVERAPI_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	if (!serialized_len) {
		return;
	}

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_serverapi_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_serverapi_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ proto array MongoDB\Driver\ServerApi::__serialize()
*/
static PHP_METHOD(ServerApi, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_serverapi_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true, true));
} /* }}} */

/* {{{ proto array MongoDB\Driver\ServerApi::__unserialize()
*/
static PHP_METHOD(ServerApi, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_serverapi_init_from_hash(Z_SERVERAPI_OBJ_P(getThis()), Z_ARRVAL_P(data));
} /* }}} */

/* {{{ MongoDB\Driver\ServerApi function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_ServerApi___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, strict, _IS_BOOL, 1)
	ZEND_ARG_TYPE_INFO(0, deprecationErrors, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ServerApi___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ServerApi___unserialize, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ServerApi_unserialize, 0, 0, 1)
#if PHP_VERSION_ID >= 80000
	ZEND_ARG_TYPE_INFO(0, serialized, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, serialized)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ServerApi_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_serverapi_me[] = {
	/* clang-format off */
	PHP_ME(ServerApi, __construct, ai_ServerApi___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerApi, __serialize, ai_ServerApi_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerApi, __set_state, ai_ServerApi___set_state, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(ServerApi, __unserialize, ai_ServerApi___unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerApi, bsonSerialize, ai_ServerApi_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerApi, serialize, ai_ServerApi_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ServerApi, unserialize, ai_ServerApi_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\ServerApi object handlers */
static zend_object_handlers php_phongo_handler_serverapi;

static void php_phongo_serverapi_free_object(zend_object* object) /* {{{ */
{
	php_phongo_serverapi_t* intern = Z_OBJ_SERVERAPI(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->server_api) {
		mongoc_server_api_destroy(intern->server_api);
	}
}

static zend_object* php_phongo_serverapi_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_serverapi_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_serverapi_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_serverapi;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_serverapi_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_serverapi_get_properties_hash(object, true, true);
} /* }}} */

static HashTable* php_phongo_serverapi_get_properties(phongo_compat_object_handler_type* object) /* {{{ */
{
	return php_phongo_serverapi_get_properties_hash(object, false, true);
} /* }}} */

void php_phongo_serverapi_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ServerApi", php_phongo_serverapi_me);
	php_phongo_serverapi_ce                = zend_register_internal_class(&ce);
	php_phongo_serverapi_ce->create_object = php_phongo_serverapi_create_object;
	PHONGO_CE_FINAL(php_phongo_serverapi_ce);

	zend_class_implements(php_phongo_serverapi_ce, 1, php_phongo_serializable_ce);
	zend_class_implements(php_phongo_serverapi_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_serverapi, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_serverapi.get_debug_info = php_phongo_serverapi_get_debug_info;
	php_phongo_handler_serverapi.get_properties = php_phongo_serverapi_get_properties;
	php_phongo_handler_serverapi.free_obj       = php_phongo_serverapi_free_object;
	php_phongo_handler_serverapi.offset         = XtOffsetOf(php_phongo_serverapi_t, std);

	zend_declare_class_constant_stringl(php_phongo_serverapi_ce, ZEND_STRL("V1"), ZEND_STRL("1"));
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
