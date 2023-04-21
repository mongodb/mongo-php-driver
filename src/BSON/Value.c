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
#include <ext/standard/base64.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_operators.h>
#include <ext/standard/php_var.h>
#include <zend_smart_str.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_bson_encode.h"
#include "BSON/Value_arginfo.h"

#define PHONGO_VALUE_TYPE_ISSER(name, type)                              \
	static PHP_METHOD(MongoDB_BSON_Value, name)                          \
	{                                                                    \
		PHONGO_PARSE_PARAMETERS_NONE();                                  \
		RETURN_BOOL(Z_VALUE_OBJ_P(getThis())->value.value_type == type); \
	}

zend_class_entry* php_phongo_value_ce;

static HashTable* php_phongo_value_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_value_t* intern;
	HashTable*          props;

	intern = Z_OBJ_VALUE(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 1);

	// TODO: Print value (somehow)
	//{
	//	zval data;
	//
	//	ZVAL_STR(&data, php_base64_encode((const unsigned char*) bson_get_data(intern->bson), intern->bson->len));
	//	zend_hash_str_update(props, "data", sizeof("data") - 1, &data);
	//}

	return props;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_BSON_Value)

static PHP_METHOD(MongoDB_BSON_Value, fromPHP)
{
	php_phongo_value_t* intern;
	zval                zv;
	zval*               data;
	zend_long           lvalue;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ZVAL(data);
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(&zv, php_phongo_value_ce);
	intern = Z_VALUE_OBJ_P(&zv);

	switch (Z_TYPE_P(data)) {
		case IS_UNDEF:
		case IS_NULL:
			intern->value.value_type = BSON_TYPE_NULL;
			break;

		case IS_FALSE:
			intern->value.value_type   = BSON_TYPE_BOOL;
			intern->value.value.v_bool = false;
			break;

		case IS_TRUE:
			intern->value.value_type   = BSON_TYPE_BOOL;
			intern->value.value.v_bool = true;
			break;

		case IS_LONG:
			lvalue = Z_LVAL_P(data);

			if (lvalue > INT32_MAX || lvalue < INT32_MIN) {
				intern->value.value_type    = BSON_TYPE_INT64;
				intern->value.value.v_int64 = lvalue;
			} else {
				intern->value.value_type    = BSON_TYPE_INT32;
				intern->value.value.v_int32 = (int32_t) lvalue;
			}

			break;

		case IS_DOUBLE:
			intern->value.value_type     = BSON_TYPE_DOUBLE;
			intern->value.value.v_double = Z_DVAL_P(data);
			break;

		case IS_STRING:
			intern->value.value_type       = BSON_TYPE_UTF8;
			intern->value.value.v_utf8.len = Z_STRLEN_P(data);

			// Duplicate string as bson_value_t is expected to own values
			intern->value.value.v_utf8.str = bson_malloc(intern->value.value.v_utf8.len + 1);
			memcpy(intern->value.value.v_utf8.str, Z_STRVAL_P(data), intern->value.value.v_utf8.len);
			intern->value.value.v_utf8.str[intern->value.value.v_utf8.len] = '\0';
			break;

		case IS_ARRAY:
		case IS_OBJECT:
			/*
			 * TODO: This will handle the following BSON types:
			 * BSON_TYPE_DOCUMENT
			 * BSON_TYPE_ARRAY
			 * BSON_TYPE_BINARY
			 * BSON_TYPE_UNDEFINED
			 * BSON_TYPE_OID
			 * BSON_TYPE_DATE_TIME
			 * BSON_TYPE_REGEX
			 * BSON_TYPE_DBPOINTER
			 * BSON_TYPE_CODE
			 * BSON_TYPE_SYMBOL
			 * BSON_TYPE_CODEWSCOPE
			 * BSON_TYPE_TIMESTAMP
			 * BSON_TYPE_DECIMAL128
			 * BSON_TYPE_MAXKEY
			 * BSON_TYPE_MINKEY
			 */
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Array and objects are not supported for Value");
			break;

		default:
			// Unsupported
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Received unsupported type %s for initialiser", zend_zval_type_name(data));
	}

	RETURN_ZVAL(&zv, 1, 1);
}

static PHP_METHOD(MongoDB_BSON_Value, getType)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	RETURN_LONG(intern->value.value_type);
}

static PHP_METHOD(MongoDB_BSON_Value, getValue)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	php_phongo_bson_value_to_zval(&intern->value, return_value);
}

PHONGO_VALUE_TYPE_ISSER(isArray, BSON_TYPE_ARRAY);
PHONGO_VALUE_TYPE_ISSER(isBinary, BSON_TYPE_BINARY);
PHONGO_VALUE_TYPE_ISSER(isBool, BSON_TYPE_BOOL);
PHONGO_VALUE_TYPE_ISSER(isDBPointer, BSON_TYPE_DBPOINTER);
PHONGO_VALUE_TYPE_ISSER(isDecimal128, BSON_TYPE_DECIMAL128);
PHONGO_VALUE_TYPE_ISSER(isDouble, BSON_TYPE_DOUBLE);
PHONGO_VALUE_TYPE_ISSER(isDocument, BSON_TYPE_DOCUMENT);
PHONGO_VALUE_TYPE_ISSER(isInt32, BSON_TYPE_INT32);
PHONGO_VALUE_TYPE_ISSER(isInt64, BSON_TYPE_INT64);
PHONGO_VALUE_TYPE_ISSER(isMaxKey, BSON_TYPE_MAXKEY);
PHONGO_VALUE_TYPE_ISSER(isMinKey, BSON_TYPE_MINKEY);
PHONGO_VALUE_TYPE_ISSER(isNull, BSON_TYPE_NULL);
PHONGO_VALUE_TYPE_ISSER(isObjectId, BSON_TYPE_OID);
PHONGO_VALUE_TYPE_ISSER(isRegex, BSON_TYPE_REGEX);
PHONGO_VALUE_TYPE_ISSER(isSymbol, BSON_TYPE_SYMBOL);
PHONGO_VALUE_TYPE_ISSER(isTimestamp, BSON_TYPE_TIMESTAMP);
PHONGO_VALUE_TYPE_ISSER(isUndefined, BSON_TYPE_UNDEFINED);
PHONGO_VALUE_TYPE_ISSER(isUTCDateTime, BSON_TYPE_DATE_TIME);
PHONGO_VALUE_TYPE_ISSER(isUtf8, BSON_TYPE_UTF8);

static PHP_METHOD(MongoDB_BSON_Value, isCode)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	RETURN_BOOL(intern->value.value_type == BSON_TYPE_CODE || intern->value.value_type == BSON_TYPE_CODEWSCOPE);
}

static PHP_METHOD(MongoDB_BSON_Value, isInt)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	RETURN_BOOL(intern->value.value_type == BSON_TYPE_INT32 || intern->value.value_type == BSON_TYPE_INT64);
}

static PHP_METHOD(MongoDB_BSON_Value, isNumber)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	RETURN_BOOL(intern->value.value_type == BSON_TYPE_INT32 || intern->value.value_type == BSON_TYPE_INT64 || intern->value.value_type == BSON_TYPE_DOUBLE);
}

/* MongoDB\BSON\Value object handlers */
static zend_object_handlers php_phongo_handler_value;

static void php_phongo_value_free_object(zend_object* object)
{
	php_phongo_value_t* intern = Z_OBJ_VALUE(object);

	zend_object_std_dtor(&intern->std);

	bson_value_destroy(&intern->value);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* php_phongo_value_create_object(zend_class_entry* class_type)
{
	php_phongo_value_t* intern = zend_object_alloc(sizeof(php_phongo_value_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_value;

	return &intern->std;
}

static zend_object* php_phongo_value_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_value_t* intern;
	php_phongo_value_t* new_intern;
	zend_object*        new_object;

	intern     = Z_OBJ_VALUE(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_value_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_VALUE(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	bson_value_copy(&intern->value, &new_intern->value);

	return new_object;
}

// TODO: can we even compare values?
static int php_phongo_value_compare_objects(zval* o1, zval* o2)
{
	//	php_phongo_value_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	//	intern1 = Z_VALUE_OBJ_P(o1);
	//	intern2 = Z_VALUE_OBJ_P(o2);
	//
	//	return bson_compare(intern1->bson, intern2->bson);
	return 0;
}

static HashTable* php_phongo_value_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_value_get_properties_hash(object, true);
}

static HashTable* php_phongo_value_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_value_get_properties_hash(object, false);
}

void php_phongo_value_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_value_ce                = register_class_MongoDB_BSON_Value();
	php_phongo_value_ce->create_object = php_phongo_value_create_object;

	memcpy(&php_phongo_handler_value, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(value);
	php_phongo_handler_value.clone_obj      = php_phongo_value_clone_object;
	php_phongo_handler_value.get_debug_info = php_phongo_value_get_debug_info;
	php_phongo_handler_value.get_properties = php_phongo_value_get_properties;
	php_phongo_handler_value.free_obj       = php_phongo_value_free_object;
	php_phongo_handler_value.offset         = XtOffsetOf(php_phongo_value_t, std);
}
