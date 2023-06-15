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

#include <math.h>

#include <php.h>
#include <zend_smart_str.h>
#include <ext/date/php_date.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_util.h"
#include "UTCDateTime_arginfo.h"

zend_class_entry* php_phongo_utcdatetime_ce;

/* Initialize the object and return whether it was successful. */
static bool php_phongo_utcdatetime_init(php_phongo_utcdatetime_t* intern, int64_t milliseconds)
{
	intern->milliseconds = milliseconds;
	intern->initialized  = true;

	return true;
}

/* Initialize the object from a numeric string and return whether it was
 * successful. An exception will be thrown on error. */
static bool php_phongo_utcdatetime_init_from_string(php_phongo_utcdatetime_t* intern, const char* s_milliseconds, size_t s_milliseconds_len)
{
	int64_t milliseconds;

	if (!php_phongo_parse_int64(&milliseconds, s_milliseconds, s_milliseconds_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit integer for %s initialization", s_milliseconds, ZSTR_VAL(php_phongo_utcdatetime_ce->name));
		return false;
	}

	return php_phongo_utcdatetime_init(intern, milliseconds);
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_utcdatetime_init_from_hash(php_phongo_utcdatetime_t* intern, HashTable* props)
{
	zval* milliseconds;

	if ((milliseconds = zend_hash_str_find(props, "milliseconds", sizeof("milliseconds") - 1)) && Z_TYPE_P(milliseconds) == IS_LONG) {
		return php_phongo_utcdatetime_init(intern, Z_LVAL_P(milliseconds));
	}

	if ((milliseconds = zend_hash_str_find(props, "milliseconds", sizeof("milliseconds") - 1)) && Z_TYPE_P(milliseconds) == IS_STRING) {
		return php_phongo_utcdatetime_init_from_string(intern, Z_STRVAL_P(milliseconds), Z_STRLEN_P(milliseconds));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"milliseconds\" integer or numeric string field", ZSTR_VAL(php_phongo_utcdatetime_ce->name));
	return false;
}

/* Initialize the object from the current time and return whether it was
 * successful. */
static bool php_phongo_utcdatetime_init_from_current_time(php_phongo_utcdatetime_t* intern)
{
	int64_t        sec, usec;
	struct timeval cur_time;

	bson_gettimeofday(&cur_time);
	sec  = cur_time.tv_sec;
	usec = cur_time.tv_usec;

	intern->milliseconds = (sec * 1000) + (usec / 1000);
	intern->initialized  = true;

	return true;
}

/* Initialize the object from a DateTime object and return whether it was
 * successful. */
static bool php_phongo_utcdatetime_init_from_date(php_phongo_utcdatetime_t* intern, php_date_obj* datetime_obj)
{
	int64_t sec, usec;

	/* The following assignments use the same logic as date_format() in php_date.c */
	sec  = datetime_obj->time->sse;
	usec = (int64_t) floor(datetime_obj->time->us);

	intern->milliseconds = (sec * 1000) + (usec / 1000);
	intern->initialized  = true;

	return true;
}

static HashTable* php_phongo_utcdatetime_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_utcdatetime_t* intern;
	HashTable*                props;

	intern = Z_OBJ_UTCDATETIME(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 1);

	if (!intern->initialized) {
		return props;
	}

	{
		zval milliseconds;

		ZVAL_INT64_STRING(&milliseconds, intern->milliseconds);
		zend_hash_str_update(props, "milliseconds", sizeof("milliseconds") - 1, &milliseconds);
	}

	return props;
}

/* Construct a new BSON UTCDateTime type from either the current time,
   milliseconds since the epoch, or a DateTimeInterface object. Defaults to the
   current time. */
static PHP_METHOD(MongoDB_BSON_UTCDateTime, __construct)
{
	php_phongo_utcdatetime_t* intern;
	zval*                     milliseconds = NULL;

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_ZVAL_OR_NULL(milliseconds)
	PHONGO_PARSE_PARAMETERS_END();

	if (milliseconds == NULL) {
		php_phongo_utcdatetime_init_from_current_time(intern);
		return;
	}

	if (Z_TYPE_P(milliseconds) == IS_OBJECT) {
		if (instanceof_function(Z_OBJCE_P(milliseconds), php_date_get_interface_ce())) {
			php_phongo_utcdatetime_init_from_date(intern, Z_PHPDATE_P(milliseconds));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected instance of DateTimeInterface, %s given", ZSTR_VAL(Z_OBJCE_P(milliseconds)->name));
		}
		return;
	}

	if (Z_TYPE_P(milliseconds) == IS_LONG) {
		php_phongo_utcdatetime_init(intern, Z_LVAL_P(milliseconds));
		return;
	}

	if (Z_TYPE_P(milliseconds) == IS_DOUBLE) {
		char tmp[24];
		int  tmp_len;

		tmp_len = snprintf(tmp, sizeof(tmp), "%.0f", Z_DVAL_P(milliseconds) > 0 ? floor(Z_DVAL_P(milliseconds)) : ceil(Z_DVAL_P(milliseconds)));

		php_phongo_utcdatetime_init_from_string(intern, tmp, tmp_len);
		return;
	}

	if (Z_TYPE_P(milliseconds) != IS_STRING) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected integer or string, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(milliseconds));
		return;
	}

	php_phongo_utcdatetime_init_from_string(intern, Z_STRVAL_P(milliseconds), Z_STRLEN_P(milliseconds));
}

static PHP_METHOD(MongoDB_BSON_UTCDateTime, __set_state)
{
	php_phongo_utcdatetime_t* intern;
	HashTable*                props;
	zval*                     array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_utcdatetime_ce);

	intern = Z_UTCDATETIME_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_utcdatetime_init_from_hash(intern, props);
}

/* Returns the UTCDateTime's milliseconds as a string */
static PHP_METHOD(MongoDB_BSON_UTCDateTime, __toString)
{
	php_phongo_utcdatetime_t* intern;

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	ZVAL_INT64_STRING(return_value, intern->milliseconds);
}

/* Returns a DateTime object representing this UTCDateTime */
static PHP_METHOD(MongoDB_BSON_UTCDateTime, toDateTime)
{
	php_phongo_utcdatetime_t* intern;
	php_date_obj*             datetime_obj;
	char*                     sec;
	size_t                    sec_len;

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	object_init_ex(return_value, php_date_get_date_ce());
	datetime_obj = Z_PHPDATE_P(return_value);

	sec_len = spprintf(&sec, 0, "@%" PRId64, intern->milliseconds / 1000);
	php_date_initialize(datetime_obj, sec, sec_len, NULL, NULL, 0);
	efree(sec);

	datetime_obj->time->us = (intern->milliseconds % 1000) * 1000;
}

static PHP_METHOD(MongoDB_BSON_UTCDateTime, jsonSerialize)
{
	php_phongo_utcdatetime_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	array_init_size(return_value, 1);

	{
		zval udt;

		array_init_size(&udt, 1);
		ADD_ASSOC_INT64_AS_STRING(&udt, "$numberLong", intern->milliseconds);
		ADD_ASSOC_ZVAL_EX(return_value, "$date", &udt);
	}
}

static PHP_METHOD(MongoDB_BSON_UTCDateTime, serialize)
{
	php_phongo_utcdatetime_t* intern;
	zval                      retval;
	php_serialize_data_t      var_hash;
	smart_str                 buf = { 0 };

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(&retval, 1);
	ADD_ASSOC_INT64_AS_STRING(&retval, "milliseconds", intern->milliseconds);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_BSON_UTCDateTime, unserialize)
{
	php_phongo_utcdatetime_t* intern;
	char*                     serialized;
	size_t                    serialized_len;
	zval                      props;
	php_unserialize_data_t    var_hash;

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_utcdatetime_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_utcdatetime_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_BSON_UTCDateTime, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_utcdatetime_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_UTCDateTime, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_utcdatetime_init_from_hash(Z_UTCDATETIME_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\UTCDateTime object handlers */
static zend_object_handlers php_phongo_handler_utcdatetime;

static void php_phongo_utcdatetime_free_object(zend_object* object)
{
	php_phongo_utcdatetime_t* intern = Z_OBJ_UTCDATETIME(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* php_phongo_utcdatetime_create_object(zend_class_entry* class_type)
{
	php_phongo_utcdatetime_t* intern = zend_object_alloc(sizeof(php_phongo_utcdatetime_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_utcdatetime;

	return &intern->std;
}

static zend_object* php_phongo_utcdatetime_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_utcdatetime_t* intern;
	php_phongo_utcdatetime_t* new_intern;
	zend_object*              new_object;

	intern     = Z_OBJ_UTCDATETIME(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_utcdatetime_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_UTCDATETIME(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_utcdatetime_init(new_intern, intern->milliseconds);

	return new_object;
}

static int php_phongo_utcdatetime_compare_objects(zval* o1, zval* o2)
{
	php_phongo_utcdatetime_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_UTCDATETIME_OBJ_P(o1);
	intern2 = Z_UTCDATETIME_OBJ_P(o2);

	if (intern1->milliseconds != intern2->milliseconds) {
		return intern1->milliseconds < intern2->milliseconds ? -1 : 1;
	}

	return 0;
}

static HashTable* php_phongo_utcdatetime_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_utcdatetime_get_properties_hash(object, true);
}

static HashTable* php_phongo_utcdatetime_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_utcdatetime_get_properties_hash(object, false);
}

void php_phongo_utcdatetime_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_utcdatetime_ce                = register_class_MongoDB_BSON_UTCDateTime(php_phongo_utcdatetime_interface_ce, php_phongo_json_serializable_ce, php_phongo_type_ce, zend_ce_serializable);
	php_phongo_utcdatetime_ce->create_object = php_phongo_utcdatetime_create_object;

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_utcdatetime_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_utcdatetime, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(utcdatetime);
	php_phongo_handler_utcdatetime.clone_obj      = php_phongo_utcdatetime_clone_object;
	php_phongo_handler_utcdatetime.get_debug_info = php_phongo_utcdatetime_get_debug_info;
	php_phongo_handler_utcdatetime.get_properties = php_phongo_utcdatetime_get_properties;
	php_phongo_handler_utcdatetime.free_obj       = php_phongo_utcdatetime_free_object;
	php_phongo_handler_utcdatetime.offset         = XtOffsetOf(php_phongo_utcdatetime_t, std);
}

bool phongo_utcdatetime_new(zval* object, int64_t msec_since_epoch)
{
	php_phongo_utcdatetime_t* intern;

	object_init_ex(object, php_phongo_utcdatetime_ce);

	intern               = Z_UTCDATETIME_OBJ_P(object);
	intern->milliseconds = msec_since_epoch;
	intern->initialized  = true;

	return true;
}
