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
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_util.h"
#include "Timestamp_arginfo.h"

zend_class_entry* php_phongo_timestamp_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_timestamp_init(php_phongo_timestamp_t* intern, int64_t increment, int64_t timestamp)
{
	if (increment < 0 || increment > UINT32_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected increment to be an unsigned 32-bit integer, %" PRId64 " given", increment);
		return false;
	}

	if (timestamp < 0 || timestamp > UINT32_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected timestamp to be an unsigned 32-bit integer, %" PRId64 " given", timestamp);
		return false;
	}

	intern->increment   = (uint32_t) increment;
	intern->timestamp   = (uint32_t) timestamp;
	intern->initialized = true;

	return true;
}

/* Initialize the object from numeric strings and return whether it was
 * successful. An exception will be thrown on error. */
static bool php_phongo_timestamp_init_from_string(php_phongo_timestamp_t* intern, const char* s_increment, size_t s_increment_len, const char* s_timestamp, size_t s_timestamp_len)
{
	int64_t increment, timestamp;

	if (!php_phongo_parse_int64(&increment, s_increment, s_increment_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit integer increment for %s initialization", s_increment, ZSTR_VAL(php_phongo_timestamp_ce->name));
		return false;
	}

	if (!php_phongo_parse_int64(&timestamp, s_timestamp, s_timestamp_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit integer timestamp for %s initialization", s_timestamp, ZSTR_VAL(php_phongo_timestamp_ce->name));
		return false;
	}

	return php_phongo_timestamp_init(intern, increment, timestamp);
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_timestamp_init_from_hash(php_phongo_timestamp_t* intern, HashTable* props)
{
	zval *increment, *timestamp;

	if ((increment = zend_hash_str_find(props, "increment", sizeof("increment") - 1)) && Z_TYPE_P(increment) == IS_LONG &&
		(timestamp = zend_hash_str_find(props, "timestamp", sizeof("timestamp") - 1)) && Z_TYPE_P(timestamp) == IS_LONG) {

		return php_phongo_timestamp_init(intern, Z_LVAL_P(increment), Z_LVAL_P(timestamp));
	}
	if ((increment = zend_hash_str_find(props, "increment", sizeof("increment") - 1)) && Z_TYPE_P(increment) == IS_STRING &&
		(timestamp = zend_hash_str_find(props, "timestamp", sizeof("timestamp") - 1)) && Z_TYPE_P(timestamp) == IS_STRING) {

		return php_phongo_timestamp_init_from_string(intern, Z_STRVAL_P(increment), Z_STRLEN_P(increment), Z_STRVAL_P(timestamp), Z_STRLEN_P(timestamp));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"increment\" and \"timestamp\" integer or numeric string fields", ZSTR_VAL(php_phongo_timestamp_ce->name));
	return false;
}

static HashTable* php_phongo_timestamp_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_timestamp_t* intern;
	HashTable*              props;
	char                    s_increment[24];
	char                    s_timestamp[24];
	int                     s_increment_len;
	int                     s_timestamp_len;

	intern = Z_OBJ_TIMESTAMP(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->initialized) {
		return props;
	}

	s_increment_len = snprintf(s_increment, sizeof(s_increment), "%" PRIu32, intern->increment);
	s_timestamp_len = snprintf(s_timestamp, sizeof(s_timestamp), "%" PRIu32, intern->timestamp);

	{
		zval increment, timestamp;

		ZVAL_STRINGL(&increment, s_increment, s_increment_len);
		zend_hash_str_update(props, "increment", sizeof("increment") - 1, &increment);

		ZVAL_STRINGL(&timestamp, s_timestamp, s_timestamp_len);
		zend_hash_str_update(props, "timestamp", sizeof("timestamp") - 1, &timestamp);
	}

	return props;
}

/* Construct a new BSON timestamp type, which consists of a 4-byte increment and
   4-byte timestamp. */
static PHP_METHOD(MongoDB_BSON_Timestamp, __construct)
{
	php_phongo_timestamp_t* intern;
	zval *                  increment = NULL, *timestamp = NULL;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(2, 2)
	Z_PARAM_ZVAL(increment)
	Z_PARAM_ZVAL(timestamp)
	PHONGO_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(increment) == IS_LONG && Z_TYPE_P(timestamp) == IS_LONG) {
		php_phongo_timestamp_init(intern, Z_LVAL_P(increment), Z_LVAL_P(timestamp));
		return;
	}

	if (Z_TYPE_P(increment) == IS_LONG) {
		convert_to_string(increment);
	}

	if (Z_TYPE_P(increment) != IS_STRING) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected increment to be an unsigned 32-bit integer or string, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(increment));
		return;
	}

	if (Z_TYPE_P(timestamp) == IS_LONG) {
		convert_to_string(timestamp);
	}

	if (Z_TYPE_P(timestamp) != IS_STRING) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected timestamp to be an unsigned 32-bit integer or string, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(timestamp));
		return;
	}

	php_phongo_timestamp_init_from_string(intern, Z_STRVAL_P(increment), Z_STRLEN_P(increment), Z_STRVAL_P(timestamp), Z_STRLEN_P(timestamp));
}

static PHP_METHOD(MongoDB_BSON_Timestamp, getIncrement)
{
	php_phongo_timestamp_t* intern;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->increment);
}

static PHP_METHOD(MongoDB_BSON_Timestamp, getTimestamp)
{
	php_phongo_timestamp_t* intern;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_LONG(intern->timestamp);
}

static PHP_METHOD(MongoDB_BSON_Timestamp, __set_state)
{
	php_phongo_timestamp_t* intern;
	HashTable*              props;
	zval*                   array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_timestamp_ce);

	intern = Z_TIMESTAMP_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_timestamp_init_from_hash(intern, props);
}

/* Returns a string in the form: [increment:timestamp] */
static PHP_METHOD(MongoDB_BSON_Timestamp, __toString)
{
	php_phongo_timestamp_t* intern;
	char*                   retval;
	int                     retval_len;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	retval_len = spprintf(&retval, 0, "[%" PRIu32 ":%" PRIu32 "]", intern->increment, intern->timestamp);
	RETVAL_STRINGL(retval, retval_len);
	efree(retval);
}

static PHP_METHOD(MongoDB_BSON_Timestamp, jsonSerialize)
{
	php_phongo_timestamp_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	array_init_size(return_value, 1);

	{
		zval ts;

		array_init_size(&ts, 2);
		ADD_ASSOC_LONG_EX(&ts, "t", intern->timestamp);
		ADD_ASSOC_LONG_EX(&ts, "i", intern->increment);
		ADD_ASSOC_ZVAL_EX(return_value, "$timestamp", &ts);
	}
}

static PHP_METHOD(MongoDB_BSON_Timestamp, serialize)
{
	php_phongo_timestamp_t* intern;
	zval                    retval;
	php_serialize_data_t    var_hash;
	smart_str               buf = { 0 };
	char                    s_increment[12];
	char                    s_timestamp[12];
	int                     s_increment_len;
	int                     s_timestamp_len;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	s_increment_len = snprintf(s_increment, sizeof(s_increment), "%" PRIu32, intern->increment);
	s_timestamp_len = snprintf(s_timestamp, sizeof(s_timestamp), "%" PRIu32, intern->timestamp);

	array_init_size(&retval, 2);
	ADD_ASSOC_STRINGL(&retval, "increment", s_increment, s_increment_len);
	ADD_ASSOC_STRINGL(&retval, "timestamp", s_timestamp, s_timestamp_len);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_BSON_Timestamp, unserialize)
{
	php_phongo_timestamp_t* intern;
	char*                   serialized;
	size_t                  serialized_len;
	zval                    props;
	php_unserialize_data_t  var_hash;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_timestamp_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_timestamp_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_BSON_Timestamp, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_timestamp_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_Timestamp, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_timestamp_init_from_hash(Z_TIMESTAMP_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\Timestamp object handlers */
static zend_object_handlers php_phongo_handler_timestamp;

static void php_phongo_timestamp_free_object(zend_object* object)
{
	php_phongo_timestamp_t* intern = Z_OBJ_TIMESTAMP(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* php_phongo_timestamp_create_object(zend_class_entry* class_type)
{
	php_phongo_timestamp_t* intern = zend_object_alloc(sizeof(php_phongo_timestamp_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_timestamp;

	return &intern->std;
}

static zend_object* php_phongo_timestamp_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_timestamp_t* intern;
	php_phongo_timestamp_t* new_intern;
	zend_object*            new_object;

	intern     = Z_OBJ_TIMESTAMP(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_timestamp_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_TIMESTAMP(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_timestamp_init(new_intern, intern->increment, intern->timestamp);

	return new_object;
}

static int php_phongo_timestamp_compare_objects(zval* o1, zval* o2)
{
	php_phongo_timestamp_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_TIMESTAMP_OBJ_P(o1);
	intern2 = Z_TIMESTAMP_OBJ_P(o2);

	/* MongoDB compares the timestamp before the increment. */
	if (intern1->timestamp != intern2->timestamp) {
		return intern1->timestamp < intern2->timestamp ? -1 : 1;
	}

	if (intern1->increment != intern2->increment) {
		return intern1->increment < intern2->increment ? -1 : 1;
	}

	return 0;
}

static HashTable* php_phongo_timestamp_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_timestamp_get_properties_hash(object, true);
}

static HashTable* php_phongo_timestamp_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_timestamp_get_properties_hash(object, false);
}

void php_phongo_timestamp_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_timestamp_ce                = register_class_MongoDB_BSON_Timestamp(php_phongo_timestamp_interface_ce, php_phongo_json_serializable_ce, php_phongo_type_ce, zend_ce_serializable);
	php_phongo_timestamp_ce->create_object = php_phongo_timestamp_create_object;

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_timestamp_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_timestamp, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(timestamp);
	php_phongo_handler_timestamp.clone_obj      = php_phongo_timestamp_clone_object;
	php_phongo_handler_timestamp.get_debug_info = php_phongo_timestamp_get_debug_info;
	php_phongo_handler_timestamp.get_properties = php_phongo_timestamp_get_properties;
	php_phongo_handler_timestamp.free_obj       = php_phongo_timestamp_free_object;
	php_phongo_handler_timestamp.offset         = XtOffsetOf(php_phongo_timestamp_t, std);
}

bool phongo_timestamp_new(zval* object, uint32_t increment, uint32_t timestamp)
{
	php_phongo_timestamp_t* intern;

	object_init_ex(object, php_phongo_timestamp_ce);

	intern              = Z_TIMESTAMP_OBJ_P(object);
	intern->increment   = increment;
	intern->timestamp   = timestamp;
	intern->initialized = true;

	return true;
}
