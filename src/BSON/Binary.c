/*
 * Copyright 2014-present MongoDB, Inc.
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
#include <zend_smart_str.h>
#include <ext/standard/base64.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "Binary_arginfo.h"

#define PHONGO_BINARY_UUID_SIZE 16

zend_class_entry* php_phongo_binary_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_binary_init(php_phongo_binary_t* intern, const char* data, size_t data_len, zend_long type)
{
	if (type < 0 || type > UINT8_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected type to be an unsigned 8-bit integer, %" PHONGO_LONG_FORMAT " given", type);
		return false;
	}

	if ((type == BSON_SUBTYPE_UUID_DEPRECATED || type == BSON_SUBTYPE_UUID) && data_len != PHONGO_BINARY_UUID_SIZE) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected UUID length to be %d bytes, %d given", PHONGO_BINARY_UUID_SIZE, data_len);
		return false;
	}

	intern->data     = estrndup(data, data_len);
	intern->data_len = data_len;
	intern->type     = (uint8_t) type;

	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_binary_init_from_hash(php_phongo_binary_t* intern, HashTable* props)
{
	zval *data, *type;

	if ((data = zend_hash_str_find(props, "data", sizeof("data") - 1)) && Z_TYPE_P(data) == IS_STRING &&
		(type = zend_hash_str_find(props, "type", sizeof("type") - 1)) && Z_TYPE_P(type) == IS_LONG) {

		return php_phongo_binary_init(intern, Z_STRVAL_P(data), Z_STRLEN_P(data), Z_LVAL_P(type));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"data\" string and \"type\" integer fields", ZSTR_VAL(php_phongo_binary_ce->name));
	return false;
}

static HashTable* php_phongo_binary_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_binary_t* intern;
	HashTable*           props;

	intern = Z_OBJ_BINARY(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->data) {
		return props;
	}

	{
		zval data, type;

		ZVAL_STRINGL(&data, intern->data, intern->data_len);
		zend_hash_str_update(props, "data", sizeof("data") - 1, &data);

		ZVAL_LONG(&type, intern->type);
		zend_hash_str_update(props, "type", sizeof("type") - 1, &type);
	}

	return props;
}

/* Construct a new BSON binary type */
static PHP_METHOD(MongoDB_BSON_Binary, __construct)
{
	php_phongo_binary_t* intern;
	char*                data;
	size_t               data_len;
	zend_long            type = BSON_SUBTYPE_BINARY;

	intern = Z_BINARY_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_STRING(data, data_len)
	Z_PARAM_OPTIONAL
	Z_PARAM_LONG(type)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_binary_init(intern, data, data_len, type);
}

static PHP_METHOD(MongoDB_BSON_Binary, __set_state)
{
	php_phongo_binary_t* intern;
	HashTable*           props;
	zval*                array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_binary_ce);

	intern = Z_BINARY_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_binary_init_from_hash(intern, props);
}

/* Return the Binary's data string. */
static PHP_METHOD(MongoDB_BSON_Binary, __toString)
{
	php_phongo_binary_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_BINARY_OBJ_P(getThis());

	RETURN_STRINGL(intern->data, intern->data_len);
}

static PHP_METHOD(MongoDB_BSON_Binary, getData)
{
	php_phongo_binary_t* intern;

	intern = Z_BINARY_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRINGL(intern->data, intern->data_len);
}

static PHP_METHOD(MongoDB_BSON_Binary, getType)
{
	php_phongo_binary_t* intern;

	intern = Z_BINARY_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->type);
}

static PHP_METHOD(MongoDB_BSON_Binary, jsonSerialize)
{
	php_phongo_binary_t* intern;
	char                 type[3];
	int                  type_len;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_BINARY_OBJ_P(getThis());

	array_init_size(return_value, 2);

	{
		zend_string* data = php_base64_encode((unsigned char*) intern->data, intern->data_len);
		ADD_ASSOC_STRINGL(return_value, "$binary", ZSTR_VAL(data), ZSTR_LEN(data));
		zend_string_free(data);
	}

	type_len = snprintf(type, sizeof(type), "%02x", intern->type);
	ADD_ASSOC_STRINGL(return_value, "$type", type, type_len);
}

static PHP_METHOD(MongoDB_BSON_Binary, serialize)
{
	php_phongo_binary_t* intern;
	zval                 retval;
	php_serialize_data_t var_hash;
	smart_str            buf = { 0 };

	intern = Z_BINARY_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(&retval, 2);
	ADD_ASSOC_STRINGL(&retval, "data", intern->data, intern->data_len);
	ADD_ASSOC_LONG_EX(&retval, "type", intern->type);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_BSON_Binary, unserialize)
{
	php_phongo_binary_t*   intern;
	char*                  serialized;
	size_t                 serialized_len;
	zval                   props;
	php_unserialize_data_t var_hash;

	intern = Z_BINARY_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_binary_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_binary_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_BSON_Binary, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_binary_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_Binary, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_binary_init_from_hash(Z_BINARY_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\Binary object handlers */
static zend_object_handlers php_phongo_handler_binary;

static void php_phongo_binary_free_object(zend_object* object)
{
	php_phongo_binary_t* intern = Z_OBJ_BINARY(object);

	zend_object_std_dtor(&intern->std);

	if (intern->data) {
		efree(intern->data);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* php_phongo_binary_create_object(zend_class_entry* class_type)
{
	php_phongo_binary_t* intern = zend_object_alloc(sizeof(php_phongo_binary_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_binary;

	return &intern->std;
}

static zend_object* php_phongo_binary_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_binary_t* intern;
	php_phongo_binary_t* new_intern;
	zend_object*         new_object;

	intern     = Z_OBJ_BINARY(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_binary_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_BINARY(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_binary_init(new_intern, intern->data, intern->data_len, intern->type);

	return new_object;
}

static int php_phongo_binary_compare_objects(zval* o1, zval* o2)
{
	php_phongo_binary_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_BINARY_OBJ_P(o1);
	intern2 = Z_BINARY_OBJ_P(o2);

	/* MongoDB compares binary types first by the data length, then by the type
	 * byte, and finally by the binary data itself. */
	if (intern1->data_len != intern2->data_len) {
		return intern1->data_len < intern2->data_len ? -1 : 1;
	}

	if (intern1->type != intern2->type) {
		return intern1->type < intern2->type ? -1 : 1;
	}

	return zend_binary_strcmp(intern1->data, intern1->data_len, intern2->data, intern2->data_len);
}

static HashTable* php_phongo_binary_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_binary_get_properties_hash(object, true);
}

static HashTable* php_phongo_binary_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_binary_get_properties_hash(object, false);
}

void php_phongo_binary_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_binary_ce                = register_class_MongoDB_BSON_Binary(php_phongo_binary_interface_ce, php_phongo_json_serializable_ce, php_phongo_type_ce, zend_ce_serializable);
	php_phongo_binary_ce->create_object = php_phongo_binary_create_object;

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_binary_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_binary, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(binary);
	php_phongo_handler_binary.clone_obj      = php_phongo_binary_clone_object;
	php_phongo_handler_binary.get_debug_info = php_phongo_binary_get_debug_info;
	php_phongo_handler_binary.get_properties = php_phongo_binary_get_properties;
	php_phongo_handler_binary.free_obj       = php_phongo_binary_free_object;
	php_phongo_handler_binary.offset         = XtOffsetOf(php_phongo_binary_t, std);
}

bool phongo_binary_new(zval* object, const char* data, size_t data_len, bson_subtype_t type)
{
	php_phongo_binary_t* intern;

	object_init_ex(object, php_phongo_binary_ce);

	intern           = Z_BINARY_OBJ_P(object);
	intern->data     = estrndup(data, data_len);
	intern->data_len = data_len;
	intern->type     = (uint8_t) type;

	return true;
}
