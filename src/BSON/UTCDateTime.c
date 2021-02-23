/*
 * Copyright 2014-2017 MongoDB, Inc.
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
#include <Zend/zend_interfaces.h>
#include <ext/date/php_date.h>
#include <ext/standard/php_var.h>
#include <zend_smart_str.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef PHP_WIN32
#include "win32/time.h"
#endif
#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_utcdatetime_ce;

/* Initialize the object and return whether it was successful. */
static bool php_phongo_utcdatetime_init(php_phongo_utcdatetime_t* intern, int64_t milliseconds) /* {{{ */
{
	intern->milliseconds = milliseconds;
	intern->initialized  = true;

	return true;
} /* }}} */

/* Initialize the object from a numeric string and return whether it was
 * successful. An exception will be thrown on error. */
static bool php_phongo_utcdatetime_init_from_string(php_phongo_utcdatetime_t* intern, const char* s_milliseconds, size_t s_milliseconds_len) /* {{{ */
{
	int64_t milliseconds;

	if (!php_phongo_parse_int64(&milliseconds, s_milliseconds, s_milliseconds_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit integer for %s initialization", s_milliseconds, ZSTR_VAL(php_phongo_utcdatetime_ce->name));
		return false;
	}

	return php_phongo_utcdatetime_init(intern, milliseconds);
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_utcdatetime_init_from_hash(php_phongo_utcdatetime_t* intern, HashTable* props) /* {{{ */
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
} /* }}} */

/* Initialize the object from the current time and return whether it was
 * successful. */
static bool php_phongo_utcdatetime_init_from_current_time(php_phongo_utcdatetime_t* intern) /* {{{ */
{
	int64_t        sec, usec;
	struct timeval cur_time;

	gettimeofday(&cur_time, NULL);
	sec  = cur_time.tv_sec;
	usec = cur_time.tv_usec;

	intern->milliseconds = (sec * 1000) + (usec / 1000);
	intern->initialized  = true;

	return true;
} /* }}} */

/* Initialize the object from a DateTime object and return whether it was
 * successful. */
static bool php_phongo_utcdatetime_init_from_date(php_phongo_utcdatetime_t* intern, php_date_obj* datetime_obj) /* {{{ */
{
	int64_t sec, usec;

	/* The following assignments use the same logic as date_format() in php_date.c */
	sec = datetime_obj->time->sse;
#if PHP_VERSION_ID >= 70200
	usec = (int64_t) floor(datetime_obj->time->us);
#else
	usec                  = (int64_t) floor(datetime_obj->time->f * 1000000 + 0.5);
#endif

	intern->milliseconds = (sec * 1000) + (usec / 1000);
	intern->initialized  = true;

	return true;
} /* }}} */

/* {{{ proto void MongoDB\BSON\UTCDateTime::__construct([int|float|string|DateTimeInterface $milliseconds = null])
   Construct a new BSON UTCDateTime type from either the current time,
   milliseconds since the epoch, or a DateTimeInterface object. Defaults to the
   current time. */
static PHP_METHOD(UTCDateTime, __construct)
{
	zend_error_handling       error_handling;
	php_phongo_utcdatetime_t* intern;
	zval*                     milliseconds = NULL;

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "|z!", &milliseconds) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if (milliseconds == NULL) {
		php_phongo_utcdatetime_init_from_current_time(intern);
		return;
	}

	if (Z_TYPE_P(milliseconds) == IS_OBJECT) {
		if (instanceof_function(Z_OBJCE_P(milliseconds), php_date_get_date_ce()) ||
			(php_phongo_date_immutable_ce && instanceof_function(Z_OBJCE_P(milliseconds), php_phongo_date_immutable_ce))) {

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
} /* }}} */

/* {{{ proto void MongoDB\BSON\UTCDateTime::__set_state(array $properties)
*/
static PHP_METHOD(UTCDateTime, __set_state)
{
	zend_error_handling       error_handling;
	php_phongo_utcdatetime_t* intern;
	HashTable*                props;
	zval*                     array;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &array) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	object_init_ex(return_value, php_phongo_utcdatetime_ce);

	intern = Z_UTCDATETIME_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_utcdatetime_init_from_hash(intern, props);
} /* }}} */

/* {{{ proto string MongoDB\BSON\UTCDateTime::__toString()
   Returns the UTCDateTime's milliseconds as a string */
static PHP_METHOD(UTCDateTime, __toString)
{
	zend_error_handling       error_handling;
	php_phongo_utcdatetime_t* intern;

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	ZVAL_INT64_STRING(return_value, intern->milliseconds);
} /* }}} */

/* {{{ proto DateTime MongoDB\BSON\UTCDateTime::toDateTime()
   Returns a DateTime object representing this UTCDateTime */
static PHP_METHOD(UTCDateTime, toDateTime)
{
	zend_error_handling       error_handling;
	php_phongo_utcdatetime_t* intern;
	php_date_obj*             datetime_obj;
	char*                     sec;
	size_t                    sec_len;

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	object_init_ex(return_value, php_date_get_date_ce());
	datetime_obj = Z_PHPDATE_P(return_value);

	sec_len = spprintf(&sec, 0, "@%" PRId64, intern->milliseconds / 1000);
	php_date_initialize(datetime_obj, sec, sec_len, NULL, NULL, 0);
	efree(sec);

#if PHP_VERSION_ID >= 70200
	datetime_obj->time->us = (intern->milliseconds % 1000) * 1000;
#else
	datetime_obj->time->f = (double) (intern->milliseconds % 1000) / 1000;
#endif
}
/* }}} */

/* {{{ proto array MongoDB\BSON\UTCDateTime::jsonSerialize()
*/
static PHP_METHOD(UTCDateTime, jsonSerialize)
{
	zend_error_handling       error_handling;
	php_phongo_utcdatetime_t* intern;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	array_init_size(return_value, 1);

	{
		zval udt;

		array_init_size(&udt, 1);
		ADD_ASSOC_INT64_AS_STRING(&udt, "$numberLong", intern->milliseconds);
		ADD_ASSOC_ZVAL_EX(return_value, "$date", &udt);
	}
} /* }}} */

/* {{{ proto string MongoDB\BSON\UTCDateTime::serialize()
*/
static PHP_METHOD(UTCDateTime, serialize)
{
	zend_error_handling       error_handling;
	php_phongo_utcdatetime_t* intern;
	zval                      retval;
	php_serialize_data_t      var_hash;
	smart_str                 buf = { 0 };

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	array_init_size(&retval, 1);
	ADD_ASSOC_INT64_AS_STRING(&retval, "milliseconds", intern->milliseconds);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\BSON\UTCDateTime::unserialize(string $serialized)
*/
static PHP_METHOD(UTCDateTime, unserialize)
{
	zend_error_handling       error_handling;
	php_phongo_utcdatetime_t* intern;
	char*                     serialized;
	size_t                    serialized_len;
	zval                      props;
	php_unserialize_data_t    var_hash;

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

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
} /* }}} */

/* {{{ MongoDB\BSON\UTCDateTime function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_UTCDateTime___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, milliseconds)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_UTCDateTime___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_UTCDateTime_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_UTCDateTime_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_utcdatetime_me[] = {
	/* clang-format off */
	PHP_ME(UTCDateTime, __construct, ai_UTCDateTime___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, __set_state, ai_UTCDateTime___set_state, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(UTCDateTime, __toString, ai_UTCDateTime_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, jsonSerialize, ai_UTCDateTime_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, serialize, ai_UTCDateTime_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, unserialize, ai_UTCDateTime_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, toDateTime, ai_UTCDateTime_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\BSON\UTCDateTime object handlers */
static zend_object_handlers php_phongo_handler_utcdatetime;

static void php_phongo_utcdatetime_free_object(zend_object* object) /* {{{ */
{
	php_phongo_utcdatetime_t* intern = Z_OBJ_UTCDATETIME(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
} /* }}} */

static zend_object* php_phongo_utcdatetime_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_utcdatetime_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_utcdatetime_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_utcdatetime;

	return &intern->std;
} /* }}} */

static zend_object* php_phongo_utcdatetime_clone_object(phongo_compat_object_handler_type* object) /* {{{ */
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
} /* }}} */

static int php_phongo_utcdatetime_compare_objects(zval* o1, zval* o2) /* {{{ */
{
	php_phongo_utcdatetime_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_UTCDATETIME_OBJ_P(o1);
	intern2 = Z_UTCDATETIME_OBJ_P(o2);

	if (intern1->milliseconds != intern2->milliseconds) {
		return intern1->milliseconds < intern2->milliseconds ? -1 : 1;
	}

	return 0;
} /* }}} */

static HashTable* php_phongo_utcdatetime_get_properties_hash(phongo_compat_object_handler_type* object, bool is_debug) /* {{{ */
{
	php_phongo_utcdatetime_t* intern;
	HashTable*                props;

	intern = Z_OBJ_UTCDATETIME(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 1);

	if (!intern->initialized) {
		return props;
	}

	{
		zval milliseconds;

		ZVAL_INT64_STRING(&milliseconds, intern->milliseconds);
		zend_hash_str_update(props, "milliseconds", sizeof("milliseconds") - 1, &milliseconds);
	}

	return props;
} /* }}} */

static HashTable* php_phongo_utcdatetime_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_utcdatetime_get_properties_hash(object, true);
} /* }}} */

static HashTable* php_phongo_utcdatetime_get_properties(phongo_compat_object_handler_type* object) /* {{{ */
{
	return php_phongo_utcdatetime_get_properties_hash(object, false);
} /* }}} */
/* }}} */

void php_phongo_utcdatetime_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "UTCDateTime", php_phongo_utcdatetime_me);
	php_phongo_utcdatetime_ce                = zend_register_internal_class(&ce);
	php_phongo_utcdatetime_ce->create_object = php_phongo_utcdatetime_create_object;
	PHONGO_CE_FINAL(php_phongo_utcdatetime_ce);

	zend_class_implements(php_phongo_utcdatetime_ce, 1, php_phongo_utcdatetime_interface_ce);
	zend_class_implements(php_phongo_utcdatetime_ce, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_utcdatetime_ce, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_utcdatetime_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_utcdatetime, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(utcdatetime);
	php_phongo_handler_utcdatetime.clone_obj      = php_phongo_utcdatetime_clone_object;
	php_phongo_handler_utcdatetime.get_debug_info = php_phongo_utcdatetime_get_debug_info;
	php_phongo_handler_utcdatetime.get_properties = php_phongo_utcdatetime_get_properties;
	php_phongo_handler_utcdatetime.free_obj       = php_phongo_utcdatetime_free_object;
	php_phongo_handler_utcdatetime.offset         = XtOffsetOf(php_phongo_utcdatetime_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
