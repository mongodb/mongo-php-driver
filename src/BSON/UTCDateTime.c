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
#include <inttypes.h>

#include <php.h>
#include <ext/date/php_date.h>
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_error.h"
#include "phongo_util.h"
#include "UTCDateTime_arginfo.h"

zend_class_entry* phongo_utcdatetime_ce;

/* Initialize the object and return whether it was successful. */
static bool phongo_utcdatetime_init(phongo_utcdatetime_t* intern, int64_t milliseconds)
{
	intern->milliseconds = milliseconds;
	intern->initialized  = true;

	return true;
}

/* Initialize the object from a numeric string and return whether it was
 * successful. An exception will be thrown on error. */
static bool phongo_utcdatetime_init_from_string(phongo_utcdatetime_t* intern, const char* s_milliseconds, size_t s_milliseconds_len)
{
	int64_t milliseconds;

	if (!phongo_parse_int64(&milliseconds, s_milliseconds, s_milliseconds_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit integer for %s initialization", s_milliseconds, ZSTR_VAL(phongo_utcdatetime_ce->name));
		return false;
	}

	return phongo_utcdatetime_init(intern, milliseconds);
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_utcdatetime_init_from_hash(phongo_utcdatetime_t* intern, HashTable* props)
{
	zval* milliseconds;

	if ((milliseconds = zend_hash_str_find(props, "milliseconds", sizeof("milliseconds") - 1)) && Z_TYPE_P(milliseconds) == IS_LONG) {
		return phongo_utcdatetime_init(intern, Z_LVAL_P(milliseconds));
	}

	if ((milliseconds = zend_hash_str_find(props, "milliseconds", sizeof("milliseconds") - 1)) && Z_TYPE_P(milliseconds) == IS_STRING) {
		return phongo_utcdatetime_init_from_string(intern, Z_STRVAL_P(milliseconds), Z_STRLEN_P(milliseconds));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"milliseconds\" integer or numeric string field", ZSTR_VAL(phongo_utcdatetime_ce->name));
	return false;
}

/* Initialize the object from the current time and return whether it was
 * successful. */
static bool phongo_utcdatetime_init_from_current_time(phongo_utcdatetime_t* intern)
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
static bool phongo_utcdatetime_init_from_date(phongo_utcdatetime_t* intern, php_date_obj* datetime_obj)
{
	int64_t sec, usec;

	/* The following assignments use the same logic as date_format() in php_date.c */
	sec  = datetime_obj->time->sse;
	usec = (int64_t) floor(datetime_obj->time->us);

	intern->milliseconds = (sec * 1000) + (usec / 1000);
	intern->initialized  = true;

	return true;
}

static bool phongo_utcdatetime_init_from_object(phongo_utcdatetime_t* intern, zend_object* object)
{
	if (instanceof_function(object->ce, php_date_get_interface_ce())) {
		phongo_utcdatetime_init_from_date(intern, php_date_obj_from_obj(object));

		return true;
	}

	if (instanceof_function(object->ce, phongo_int64_ce)) {
		phongo_utcdatetime_init(intern, php_int64_fetch_object(object)->integer);

		return true;
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected instance of %s or %s, %s given", ZSTR_VAL(php_date_get_interface_ce()->name), ZSTR_VAL(phongo_int64_ce->name), ZSTR_VAL(object->ce->name));

	return false;
}

static HashTable* phongo_utcdatetime_get_properties_hash(zend_object* object, bool is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(utcdatetime, object);

	HashTable* props;

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

static void phongo_utcdatetime_to_php_date(zval* return_value, const zval* this, zend_class_entry* ce)
{
	PHONGO_INTERN_FROM_ZVAL(utcdatetime, this);

	php_date_obj* datetime_obj;
	char*         sec_str;
	size_t        sec_len;
	int64_t       sec, usec;

	object_init_ex(return_value, ce);
	datetime_obj = Z_PHPDATE_P(return_value);

	sec  = intern->milliseconds / 1000;
	usec = (llabs(intern->milliseconds) % 1000) * 1000;
	if (intern->milliseconds < 0 && usec != 0) {
		/* For dates before the unix epoch, we need to subtract the microseconds from the timestamp.
		 * Since we can't directly pass microseconds when calling php_date_initialize due to a bug in PHP,
		 * we manually decrement the timestamp and subtract the number of microseconds from a full seconds
		 * to store in the us field. */
		sec--;
		usec = 1000000 - usec;
	}

	/* TODO PHP 8.1.7+: microseconds can be included in the format string */
	sec_len = spprintf(&sec_str, 0, "@%" PRId64, sec);
	php_date_initialize(datetime_obj, sec_str, sec_len, NULL, NULL, 0);
	efree(sec_str);

	datetime_obj->time->us = usec;
}

/* Construct a new BSON UTCDateTime type from either the current time,
   milliseconds since the epoch, or a DateTimeInterface object. Defaults to the
   current time. */
static PHP_METHOD(MongoDB_BSON_UTCDateTime, __construct)
{
	PHONGO_INTERN_FROM_THIS(utcdatetime);

	zval* milliseconds = NULL;

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_ZVAL_OR_NULL(milliseconds)
	PHONGO_PARSE_PARAMETERS_END();

	if (milliseconds == NULL) {
		phongo_utcdatetime_init_from_current_time(intern);
		return;
	}

	switch (Z_TYPE_P(milliseconds)) {
		case IS_OBJECT:
			phongo_utcdatetime_init_from_object(intern, Z_OBJ_P(milliseconds));
			return;

		case IS_LONG:
			phongo_utcdatetime_init(intern, Z_LVAL_P(milliseconds));
			return;
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected integer or object, %s given", zend_zval_type_name(milliseconds));
}

static PHP_METHOD(MongoDB_BSON_UTCDateTime, __set_state)
{
	HashTable* props;
	zval*      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(utcdatetime, return_value);
	props = Z_ARRVAL_P(array);

	phongo_utcdatetime_init_from_hash(intern, props);
}

/* Returns the UTCDateTime's milliseconds as a string */
static PHP_METHOD(MongoDB_BSON_UTCDateTime, __toString)
{
	PHONGO_INTERN_FROM_THIS(utcdatetime);

	PHONGO_PARSE_PARAMETERS_NONE();

	ZVAL_INT64_STRING(return_value, intern->milliseconds);
}

/* Returns a DateTime object representing this UTCDateTime */
static PHP_METHOD(MongoDB_BSON_UTCDateTime, toDateTime)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_utcdatetime_to_php_date(return_value, getThis(), php_date_get_date_ce());
}

/* Returns a DateTimeImmutable object representing this UTCDateTime */
static PHP_METHOD(MongoDB_BSON_UTCDateTime, toDateTimeImmutable)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_utcdatetime_to_php_date(return_value, getThis(), php_date_get_immutable_ce());
}

static PHP_METHOD(MongoDB_BSON_UTCDateTime, jsonSerialize)
{
	PHONGO_INTERN_FROM_THIS(utcdatetime);

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 1);

	{
		zval udt;

		array_init_size(&udt, 1);
		ADD_ASSOC_INT64_AS_STRING(&udt, "$numberLong", intern->milliseconds);
		ADD_ASSOC_ZVAL_EX(return_value, "$date", &udt);
	}
}

static PHP_METHOD(MongoDB_BSON_UTCDateTime, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(phongo_utcdatetime_get_properties_hash(Z_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_UTCDateTime, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_utcdatetime_init_from_hash(Z_UTCDATETIME_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\UTCDateTime object handlers */
static zend_object_handlers phongo_handler_utcdatetime;

static void phongo_utcdatetime_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(utcdatetime, object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* phongo_utcdatetime_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(utcdatetime, class_type);

	intern->std.handlers = &phongo_handler_utcdatetime;

	return &intern->std;
}

static zend_object* phongo_utcdatetime_clone_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(utcdatetime, object);

	phongo_utcdatetime_t* new_intern;
	zend_object*          new_object;

	new_object = phongo_utcdatetime_create_object(object->ce);

	new_intern = Z_OBJ_UTCDATETIME(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	phongo_utcdatetime_init(new_intern, intern->milliseconds);

	return new_object;
}

static int phongo_utcdatetime_compare_objects(zval* o1, zval* o2)
{
	phongo_utcdatetime_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_UTCDATETIME_OBJ_P(o1);
	intern2 = Z_UTCDATETIME_OBJ_P(o2);

	if (intern1->milliseconds != intern2->milliseconds) {
		return intern1->milliseconds < intern2->milliseconds ? -1 : 1;
	}

	return 0;
}

static HashTable* phongo_utcdatetime_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return phongo_utcdatetime_get_properties_hash(object, true);
}

static HashTable* phongo_utcdatetime_get_properties(zend_object* object)
{
	return phongo_utcdatetime_get_properties_hash(object, false);
}

void phongo_utcdatetime_init_ce(INIT_FUNC_ARGS)
{
	phongo_utcdatetime_ce                = register_class_MongoDB_BSON_UTCDateTime(phongo_utcdatetime_interface_ce, phongo_json_serializable_ce, phongo_type_ce, zend_ce_stringable);
	phongo_utcdatetime_ce->create_object = phongo_utcdatetime_create_object;

	memcpy(&phongo_handler_utcdatetime, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_utcdatetime.compare        = phongo_utcdatetime_compare_objects;
	phongo_handler_utcdatetime.clone_obj      = phongo_utcdatetime_clone_object;
	phongo_handler_utcdatetime.get_debug_info = phongo_utcdatetime_get_debug_info;
	phongo_handler_utcdatetime.get_properties = phongo_utcdatetime_get_properties;
	phongo_handler_utcdatetime.free_obj       = phongo_utcdatetime_free_object;
	phongo_handler_utcdatetime.offset         = XtOffsetOf(phongo_utcdatetime_t, std);
}

bool phongo_utcdatetime_new(zval* object, int64_t msec_since_epoch)
{
	PHONGO_INTERN_INIT_EX(utcdatetime, object);
	intern->milliseconds = msec_since_epoch;
	intern->initialized  = true;

	return true;
}
