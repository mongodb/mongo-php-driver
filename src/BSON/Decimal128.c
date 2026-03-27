/*
 * Copyright 2015-present MongoDB, Inc.
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

#include "phongo.h"
#include "phongo_error.h"
#include "Decimal128_arginfo.h"

zend_class_entry* phongo_decimal128_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool phongo_decimal128_init(phongo_decimal128_t* intern, const char* value)
{
	if (!bson_decimal128_from_string(value, &intern->decimal)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing Decimal128 string: %s", value);
		return false;
	}

	intern->initialized = true;

	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_decimal128_init_from_hash(phongo_decimal128_t* intern, HashTable* props)
{
	zval* dec;

	if ((dec = zend_hash_str_find(props, "dec", sizeof("dec") - 1)) && Z_TYPE_P(dec) == IS_STRING) {
		return phongo_decimal128_init(intern, Z_STRVAL_P(dec));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"dec\" string field", ZSTR_VAL(phongo_decimal128_ce->name));
	return false;
}

static HashTable* phongo_decimal128_get_properties_hash(zend_object* object, bool is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(decimal128, object);

	HashTable* props;
	char       outbuf[BSON_DECIMAL128_STRING] = "";

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 1);

	if (!intern->initialized) {
		return props;
	}

	bson_decimal128_to_string(&intern->decimal, outbuf);

	{
		zval dec;

		ZVAL_STRING(&dec, outbuf);
		zend_hash_str_update(props, "dec", sizeof("dec") - 1, &dec);
	}

	return props;
}

/* Construct a new BSON Decimal128 type */
static PHP_METHOD(MongoDB_BSON_Decimal128, __construct)
{
	PHONGO_INTERN_FROM_THIS(decimal128);
	char*  value;
	size_t value_len;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(value, value_len)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_decimal128_init(intern, value);
}

static PHP_METHOD(MongoDB_BSON_Decimal128, __set_state)
{
	HashTable* props;
	zval*      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(decimal128, return_value);
	props = Z_ARRVAL_P(array);

	phongo_decimal128_init_from_hash(intern, props);
}

static PHP_METHOD(MongoDB_BSON_Decimal128, __toString)
{
	PHONGO_INTERN_FROM_THIS(decimal128);
	char outbuf[BSON_DECIMAL128_STRING];

	PHONGO_PARSE_PARAMETERS_NONE();

	bson_decimal128_to_string(&intern->decimal, outbuf);

	RETURN_STRING(outbuf);
}

static PHP_METHOD(MongoDB_BSON_Decimal128, jsonSerialize)
{
	PHONGO_INTERN_FROM_THIS(decimal128);
	char outbuf[BSON_DECIMAL128_STRING] = "";

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 1);
	bson_decimal128_to_string(&intern->decimal, outbuf);
	ADD_ASSOC_STRING(return_value, "$numberDecimal", outbuf);
}

static PHP_METHOD(MongoDB_BSON_Decimal128, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(phongo_decimal128_get_properties_hash(Z_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_Decimal128, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_decimal128_init_from_hash(Z_DECIMAL128_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\Decimal128 object handlers */
static zend_object_handlers phongo_handler_decimal128;

static void phongo_decimal128_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(decimal128, object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* phongo_decimal128_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(decimal128, class_type);

	intern->std.handlers = &phongo_handler_decimal128;

	return &intern->std;
}

static zend_object* phongo_decimal128_clone_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(decimal128, object);

	phongo_decimal128_t* new_intern;
	zend_object*         new_object;

	new_object = phongo_decimal128_create_object(object->ce);

	new_intern = Z_OBJ_DECIMAL128(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	// Use memcpy to copy bson value to avoid converting to string and back
	memcpy(&new_intern->decimal, &intern->decimal, sizeof(bson_decimal128_t));
	new_intern->initialized = true;

	return new_object;
}

static HashTable* phongo_decimal128_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return phongo_decimal128_get_properties_hash(object, true);
}

static HashTable* phongo_decimal128_get_properties(zend_object* object)
{
	return phongo_decimal128_get_properties_hash(object, false);
}

void phongo_decimal128_init_ce(INIT_FUNC_ARGS)
{
	phongo_decimal128_ce                = register_class_MongoDB_BSON_Decimal128(phongo_decimal128_interface_ce, phongo_json_serializable_ce, phongo_type_ce, zend_ce_stringable);
	phongo_decimal128_ce->create_object = phongo_decimal128_create_object;

	memcpy(&phongo_handler_decimal128, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_decimal128.clone_obj      = phongo_decimal128_clone_object;
	phongo_handler_decimal128.get_debug_info = phongo_decimal128_get_debug_info;
	phongo_handler_decimal128.get_properties = phongo_decimal128_get_properties;
	phongo_handler_decimal128.free_obj       = phongo_decimal128_free_object;
	phongo_handler_decimal128.offset         = XtOffsetOf(phongo_decimal128_t, std);
}

bool phongo_decimal128_new(zval* object, const bson_decimal128_t* decimal)
{
	PHONGO_INTERN_INIT_EX(decimal128, object);
	memcpy(&intern->decimal, decimal, sizeof(bson_decimal128_t));
	intern->initialized = true;

	return true;
}
