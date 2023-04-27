/*
 * Copyright 2023-present MongoDB, Inc.
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
#include "phongo_util.h"
#include "BSON/Value_arginfo.h"
#include "BSON/Binary.h"
#include "BSON/DBPointer.h"
#include "BSON/Decimal128.h"
#include "BSON/Document.h"
#include "BSON/Int64.h"
#include "BSON/Javascript.h"
#include "BSON/ObjectId.h"
#include "BSON/PackedArray.h"
#include "BSON/Regex.h"
#include "BSON/Symbol.h"
#include "BSON/Timestamp.h"
#include "BSON/UTCDateTime.h"

zend_class_entry* php_phongo_value_ce;

#define UNEXPECTED_BSON_TYPE_EXCEPTION(type) phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Unexpected BSON type 0x%02hhx", (type));

static HashTable* php_phongo_value_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_value_t* intern;
	HashTable*          props;

	intern = Z_OBJ_VALUE(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	{
		zval type;

		ZVAL_LONG(&type, intern->value.value_type);

		zend_hash_str_update(props, "type", sizeof("type") - 1, &type);
	}

	{
		zval value;

		phongo_bson_value_to_zval(&intern->value, &value);

		zend_hash_str_update(props, "value", sizeof("value") - 1, &value);
	}

	return props;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_BSON_Value)

static PHP_METHOD(MongoDB_BSON_Value, fromPHP)
{
	php_phongo_value_t* intern;
	zval                zv;
	zval*               data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ZVAL(data);
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(&zv, php_phongo_value_ce);
	intern = Z_VALUE_OBJ_P(&zv);

	if (!phongo_zval_to_bson_value(data, &intern->value)) {
		/* Exception already thrown */
		zval_ptr_dtor(&zv);
		return;
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

	phongo_bson_value_to_zval(&intern->value, return_value);
}

#define PHONGO_VALUE_CHECK_TYPE(bson_value, expected_type)     \
	if (bson_value.value_type != expected_type) {              \
		UNEXPECTED_BSON_TYPE_EXCEPTION(bson_value.value_type); \
		return;                                                \
	}

static PHP_METHOD(MongoDB_BSON_Value, getArray)
{
	php_phongo_value_t* intern;
	bson_t*             bson;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_ARRAY);

	bson = bson_new_from_data(intern->value.value.v_doc.data, intern->value.value.v_doc.data_len);

	phongo_packedarray_new(return_value, bson, false);
}

static PHP_METHOD(MongoDB_BSON_Value, getBinary)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_BINARY);

	phongo_binary_new(return_value, (const char*) intern->value.value.v_binary.data, intern->value.value.v_binary.data_len, intern->value.value.v_binary.subtype);
}

static PHP_METHOD(MongoDB_BSON_Value, getBool)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_BOOL);

	RETURN_BOOL(intern->value.value.v_bool);
}

static PHP_METHOD(MongoDB_BSON_Value, getCode)
{
	php_phongo_value_t* intern;
	bson_t*             scope;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	switch (intern->value.value_type) {
		case BSON_TYPE_CODE:
			phongo_javascript_new(return_value, intern->value.value.v_code.code, intern->value.value.v_code.code_len, NULL);
			return;

		case BSON_TYPE_CODEWSCOPE:
			scope = bson_new_from_data(intern->value.value.v_codewscope.scope_data, intern->value.value.v_codewscope.scope_len);
			phongo_javascript_new(return_value, intern->value.value.v_codewscope.code, intern->value.value.v_codewscope.code_len, scope);
			return;

		default:
			UNEXPECTED_BSON_TYPE_EXCEPTION(intern->value.value_type);
	}
}

static PHP_METHOD(MongoDB_BSON_Value, getDateTime)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_DATE_TIME);

	phongo_utcdatetime_new(return_value, intern->value.value.v_datetime);
}

static PHP_METHOD(MongoDB_BSON_Value, getDBPointer)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_DBPOINTER);

	phongo_dbpointer_new(return_value, intern->value.value.v_dbpointer.collection, intern->value.value.v_dbpointer.collection_len, &intern->value.value.v_dbpointer.oid);
}

static PHP_METHOD(MongoDB_BSON_Value, getDecimal128)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_DECIMAL128);

	phongo_decimal128_new(return_value, &intern->value.value.v_decimal128);
}

static PHP_METHOD(MongoDB_BSON_Value, getDocument)
{
	php_phongo_value_t* intern;
	bson_t*             bson;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_DOCUMENT);

	bson = bson_new_from_data(intern->value.value.v_doc.data, intern->value.value.v_doc.data_len);

	phongo_document_new(return_value, bson, false);
}

static PHP_METHOD(MongoDB_BSON_Value, getDouble)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_DOUBLE);

	RETURN_DOUBLE(intern->value.value.v_double);
}

static PHP_METHOD(MongoDB_BSON_Value, getInt)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	switch (intern->value.value_type) {
		case BSON_TYPE_INT32:
			RETURN_LONG(intern->value.value.v_int32);

		case BSON_TYPE_INT64:
			RETURN_INT64(intern->value.value.v_int64);

		default:
			UNEXPECTED_BSON_TYPE_EXCEPTION(intern->value.value_type);
	}
}

static PHP_METHOD(MongoDB_BSON_Value, getInt32)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_INT32);

	RETURN_LONG(intern->value.value.v_int32);
}

static PHP_METHOD(MongoDB_BSON_Value, getInt64)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_INT64);

	RETURN_INT64(intern->value.value.v_int64);
}

static PHP_METHOD(MongoDB_BSON_Value, getMaxKey)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_MAXKEY);

	object_init_ex(return_value, php_phongo_maxkey_ce);
}

static PHP_METHOD(MongoDB_BSON_Value, getMinKey)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_MINKEY);

	object_init_ex(return_value, php_phongo_minkey_ce);
}

static PHP_METHOD(MongoDB_BSON_Value, getNull)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_NULL);

	RETURN_NULL();
}

static PHP_METHOD(MongoDB_BSON_Value, getObjectId)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_OID);

	phongo_objectid_new(return_value, &intern->value.value.v_oid);
}

static PHP_METHOD(MongoDB_BSON_Value, getRegex)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_REGEX);

	phongo_regex_new(return_value, intern->value.value.v_regex.regex, intern->value.value.v_regex.options);
}

static PHP_METHOD(MongoDB_BSON_Value, getSymbol)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_SYMBOL);

	phongo_symbol_new(return_value, intern->value.value.v_symbol.symbol, intern->value.value.v_symbol.len);
}

static PHP_METHOD(MongoDB_BSON_Value, getTimestamp)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_TIMESTAMP);

	phongo_timestamp_new(return_value, intern->value.value.v_timestamp.increment, intern->value.value.v_timestamp.timestamp);
}

static PHP_METHOD(MongoDB_BSON_Value, getUndefined)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_UNDEFINED);

	object_init_ex(return_value, php_phongo_undefined_ce);
}

static PHP_METHOD(MongoDB_BSON_Value, getUtf8)
{
	php_phongo_value_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_VALUE_OBJ_P(getThis());

	PHONGO_VALUE_CHECK_TYPE(intern->value, BSON_TYPE_UTF8);

	RETURN_STRINGL(intern->value.value.v_utf8.str, intern->value.value.v_utf8.len);
}

#undef PHONGO_VALUE_CHECK_TYPE

#define PHONGO_VALUE_IS_TYPE(name, type)                                 \
	static PHP_METHOD(MongoDB_BSON_Value, name)                          \
	{                                                                    \
		PHONGO_PARSE_PARAMETERS_NONE();                                  \
		RETURN_BOOL(Z_VALUE_OBJ_P(getThis())->value.value_type == type); \
	}

PHONGO_VALUE_IS_TYPE(isArray, BSON_TYPE_ARRAY);
PHONGO_VALUE_IS_TYPE(isBinary, BSON_TYPE_BINARY);
PHONGO_VALUE_IS_TYPE(isBool, BSON_TYPE_BOOL);
PHONGO_VALUE_IS_TYPE(isDBPointer, BSON_TYPE_DBPOINTER);
PHONGO_VALUE_IS_TYPE(isDateTime, BSON_TYPE_DATE_TIME);
PHONGO_VALUE_IS_TYPE(isDecimal128, BSON_TYPE_DECIMAL128);
PHONGO_VALUE_IS_TYPE(isDouble, BSON_TYPE_DOUBLE);
PHONGO_VALUE_IS_TYPE(isDocument, BSON_TYPE_DOCUMENT);
PHONGO_VALUE_IS_TYPE(isInt32, BSON_TYPE_INT32);
PHONGO_VALUE_IS_TYPE(isInt64, BSON_TYPE_INT64);
PHONGO_VALUE_IS_TYPE(isMaxKey, BSON_TYPE_MAXKEY);
PHONGO_VALUE_IS_TYPE(isMinKey, BSON_TYPE_MINKEY);
PHONGO_VALUE_IS_TYPE(isNull, BSON_TYPE_NULL);
PHONGO_VALUE_IS_TYPE(isObjectId, BSON_TYPE_OID);
PHONGO_VALUE_IS_TYPE(isRegex, BSON_TYPE_REGEX);
PHONGO_VALUE_IS_TYPE(isSymbol, BSON_TYPE_SYMBOL);
PHONGO_VALUE_IS_TYPE(isTimestamp, BSON_TYPE_TIMESTAMP);
PHONGO_VALUE_IS_TYPE(isUndefined, BSON_TYPE_UNDEFINED);
PHONGO_VALUE_IS_TYPE(isUtf8, BSON_TYPE_UTF8);

#undef PHONGO_VALUE_IS_TYPE

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

#undef UNEXPECTED_BSON_TYPE_EXCEPTION

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

static HashTable* php_phongo_value_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	php_phongo_value_t* intern;
	HashTable*          props;

	*is_temp = 1;

	intern = Z_OBJ_VALUE(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(*is_temp, intern, props, 2);

	{
		zval type;

		ZVAL_STRING(&type, php_phongo_bson_type_to_string(intern->value.value_type));

		zend_hash_str_update(props, "type", sizeof("type") - 1, &type);
	}

	{
		zval value;

		phongo_bson_value_to_zval_legacy(&intern->value, &value);

		zend_hash_str_update(props, "value", sizeof("value") - 1, &value);
	}

	return props;
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
	php_phongo_handler_value.clone_obj      = php_phongo_value_clone_object;
	php_phongo_handler_value.get_debug_info = php_phongo_value_get_debug_info;
	php_phongo_handler_value.get_properties = php_phongo_value_get_properties;
	php_phongo_handler_value.free_obj       = php_phongo_value_free_object;
	php_phongo_handler_value.offset         = XtOffsetOf(php_phongo_value_t, std);
}

bool phongo_value_new(zval* object, const bson_value_t* value)
{
	php_phongo_value_t* intern;

	object_init_ex(object, php_phongo_value_ce);

	intern = Z_VALUE_OBJ_P(object);
	bson_value_copy(value, &intern->value);

	return true;
}
