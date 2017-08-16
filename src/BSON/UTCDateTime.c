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

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>
#include <math.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/date/php_date.h>
#include <ext/spl/spl_iterators.h>
#include <ext/standard/php_var.h>
#if PHP_VERSION_ID >= 70000
# include <zend_smart_str.h>
#else
# include <ext/standard/php_smart_str.h>
#endif
/* Our Compatability header */
#include "phongo_compat.h"

#ifdef PHP_WIN32
#include "win32/time.h"
#endif

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_utcdatetime_ce;

zend_object_handlers php_phongo_handler_utcdatetime;

/* Initialize the object and return whether it was successful. */
static bool php_phongo_utcdatetime_init(php_phongo_utcdatetime_t *intern, int64_t milliseconds)
{
	intern->milliseconds = milliseconds;
	intern->initialized = true;

	return true;
}

/* Initialize the object from a numeric string and return whether it was
 * successful. An exception will be thrown on error. */
static bool php_phongo_utcdatetime_init_from_string(php_phongo_utcdatetime_t *intern, const char *s_milliseconds, phongo_zpp_char_len s_milliseconds_len TSRMLS_DC)
{
	int64_t milliseconds;
	char *endptr = NULL;

	errno = 0;

	milliseconds = bson_ascii_strtoll(s_milliseconds, &endptr, 10);

	/* errno will set errno if conversion fails; however, we do not need to
	 * specify the type of error.
	 *
	 * Note: bson_ascii_strtoll() does not properly detect out-of-range values
	 * (see: CDRIVER-1377). strtoll() would be preferable, but it is not
	 * available on all platforms (e.g. HP-UX), and atoll() provides no error
	 * reporting at all. */
	if (errno || (endptr && endptr != ((const char *)s_milliseconds + s_milliseconds_len))) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error parsing \"%s\" as 64-bit integer for %s initialization", s_milliseconds, ZSTR_VAL(php_phongo_utcdatetime_ce->name));
		return false;
	}

	return php_phongo_utcdatetime_init(intern, milliseconds);
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_utcdatetime_init_from_hash(php_phongo_utcdatetime_t *intern, HashTable *props TSRMLS_DC)
{
#if PHP_VERSION_ID >= 70000
	zval *milliseconds;

	if ((milliseconds = zend_hash_str_find(props, "milliseconds", sizeof("milliseconds")-1)) && Z_TYPE_P(milliseconds) == IS_LONG) {
		return php_phongo_utcdatetime_init(intern, Z_LVAL_P(milliseconds));
	}

	if ((milliseconds = zend_hash_str_find(props, "milliseconds", sizeof("milliseconds")-1)) && Z_TYPE_P(milliseconds) == IS_STRING) {
		return php_phongo_utcdatetime_init_from_string(intern, Z_STRVAL_P(milliseconds), Z_STRLEN_P(milliseconds) TSRMLS_CC);
	}
#else
	zval **milliseconds;

	if (zend_hash_find(props, "milliseconds", sizeof("milliseconds"), (void**) &milliseconds) == SUCCESS && Z_TYPE_PP(milliseconds) == IS_LONG) {
		return php_phongo_utcdatetime_init(intern, Z_LVAL_PP(milliseconds));
	}

	if (zend_hash_find(props, "milliseconds", sizeof("milliseconds"), (void**) &milliseconds) == SUCCESS && Z_TYPE_PP(milliseconds) == IS_STRING) {
		return php_phongo_utcdatetime_init_from_string(intern, Z_STRVAL_PP(milliseconds), Z_STRLEN_PP(milliseconds) TSRMLS_CC);
	}
#endif

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"milliseconds\" integer or numeric string field", ZSTR_VAL(php_phongo_utcdatetime_ce->name));
	return false;
}

/* Initialize the object from the current time and return whether it was
 * successful. */
static bool php_phongo_utcdatetime_init_from_current_time(php_phongo_utcdatetime_t *intern)
{
	int64_t        sec, usec;
	struct timeval cur_time;

	gettimeofday(&cur_time, NULL);
	sec = cur_time.tv_sec;
	usec = cur_time.tv_usec;

	intern->milliseconds = (sec * 1000) + (usec / 1000);
	intern->initialized = true;

	return true;
}

/* Initialize the object from a DateTime object and return whether it was
 * successful. */
static bool php_phongo_utcdatetime_init_from_date(php_phongo_utcdatetime_t *intern, php_date_obj *datetime_obj)
{
	int64_t sec, usec;

	/* The following assignments use the same logic as date_format() in php_date.c */
	sec = datetime_obj->time->sse;
#if PHP_VERSION_ID >= 70200
	usec = (int64_t) floor(datetime_obj->time->us);
#else
	usec = (int64_t) floor(datetime_obj->time->f * 1000000 + 0.5);
#endif

	intern->milliseconds = (sec * 1000) + (usec / 1000);
	intern->initialized = true;

	return true;
}

/* {{{ proto void UTCDateTime::__construct([int|float|string|DateTimeInterface $milliseconds = null])
   Construct a new BSON UTCDateTime type from either the current time,
   milliseconds since the epoch, or a DateTimeInterface object. Defaults to the
   current time. */
PHP_METHOD(UTCDateTime, __construct)
{
	php_phongo_utcdatetime_t    *intern;
	zend_error_handling          error_handling;
	zval                        *milliseconds = NULL;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_UTCDATETIME_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z!", &milliseconds) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (milliseconds == NULL) {
		php_phongo_utcdatetime_init_from_current_time(intern);
		return;
	}

	if (Z_TYPE_P(milliseconds) == IS_OBJECT) {
		if (instanceof_function(Z_OBJCE_P(milliseconds), php_date_get_date_ce() TSRMLS_CC) ||
		    (php_phongo_date_immutable_ce && instanceof_function(Z_OBJCE_P(milliseconds), php_phongo_date_immutable_ce TSRMLS_CC))) {
			php_phongo_utcdatetime_init_from_date(intern, Z_PHPDATE_P(milliseconds));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected instance of DateTimeInterface, %s given", ZSTR_VAL(Z_OBJCE_P(milliseconds)->name));
		}
		return;
	}

	if (Z_TYPE_P(milliseconds) == IS_LONG) {
		php_phongo_utcdatetime_init(intern, Z_LVAL_P(milliseconds));
		return;
	}

	if (Z_TYPE_P(milliseconds) == IS_DOUBLE) {
		char tmp[24];
		int tmp_len;

		tmp_len = snprintf(tmp, sizeof(tmp), "%.0f", Z_DVAL_P(milliseconds) > 0 ? floor(Z_DVAL_P(milliseconds)) : ceil(Z_DVAL_P(milliseconds)));

		php_phongo_utcdatetime_init_from_string(intern, tmp, tmp_len TSRMLS_CC);
		return;
	}

	if (Z_TYPE_P(milliseconds) != IS_STRING) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected integer or string, %s given", zend_get_type_by_const(Z_TYPE_P(milliseconds)));
		return;
	}

	php_phongo_utcdatetime_init_from_string(intern, Z_STRVAL_P(milliseconds), Z_STRLEN_P(milliseconds) TSRMLS_CC);
}
/* }}} */

/* {{{ proto void UTCDateTime::__set_state(array $properties)
*/
PHP_METHOD(UTCDateTime, __set_state)
{
	php_phongo_utcdatetime_t *intern;
	HashTable                *props;
	zval                     *array;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_utcdatetime_ce);

	intern = Z_UTCDATETIME_OBJ_P(return_value);
	props = Z_ARRVAL_P(array);

	php_phongo_utcdatetime_init_from_hash(intern, props TSRMLS_CC);
}
/* }}} */

/* {{{ proto string UTCDateTime::__toString()
   Returns the UTCDateTime's milliseconds as a string */
PHP_METHOD(UTCDateTime, __toString)
{
	php_phongo_utcdatetime_t    *intern;
	char *tmp;
	int tmp_len;


	intern = Z_UTCDATETIME_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	tmp_len = spprintf(&tmp, 0, "%" PRId64, intern->milliseconds);
	PHONGO_RETVAL_STRINGL(tmp, tmp_len);
	efree(tmp);
}
/* }}} */

/* {{{ proto DateTime UTCDateTime::toDateTime()
   Returns a DateTime object representing this UTCDateTime */
PHP_METHOD(UTCDateTime, toDateTime)
{
	php_phongo_utcdatetime_t *intern;
	php_date_obj             *datetime_obj;
	char                     *sec;
	size_t                    sec_len;


	intern = Z_UTCDATETIME_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	object_init_ex(return_value, php_date_get_date_ce());
	datetime_obj = Z_PHPDATE_P(return_value);

	sec_len = spprintf(&sec, 0, "@%" PRId64, intern->milliseconds / 1000);
	php_date_initialize(datetime_obj, sec, sec_len, NULL, NULL, 0 TSRMLS_CC);
	efree(sec);

#if PHP_VERSION_ID >= 70200
	datetime_obj->time->us = (intern->milliseconds % 1000) * 1000;
#else
	datetime_obj->time->f = (double) (intern->milliseconds % 1000) / 1000;
#endif
}
/* }}} */

/* {{{ proto array UTCDateTime::jsonSerialize()
*/
PHP_METHOD(UTCDateTime, jsonSerialize)
{
	php_phongo_utcdatetime_t *intern;
	char                      s_milliseconds[24];
	int                       s_milliseconds_len;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	s_milliseconds_len = snprintf(s_milliseconds, sizeof(s_milliseconds), "%" PRId64, intern->milliseconds);

	array_init_size(return_value, 1);

#if PHP_VERSION_ID >= 70000
	{
		zval udt;

		array_init_size(&udt, 1);
		ADD_ASSOC_STRINGL(&udt, "$numberLong", s_milliseconds, s_milliseconds_len);
		ADD_ASSOC_ZVAL_EX(return_value, "$date", &udt);
	}
#else
	{
		zval *udt;

		MAKE_STD_ZVAL(udt);
		array_init_size(udt, 1);
		ADD_ASSOC_STRINGL(udt, "$numberLong", s_milliseconds, s_milliseconds_len);
		ADD_ASSOC_ZVAL_EX(return_value, "$date", udt);
	}
#endif
}
/* }}} */

/* {{{ proto string UTCDateTime::serialize()
*/
PHP_METHOD(UTCDateTime, serialize)
{
	php_phongo_utcdatetime_t *intern;
#if PHP_VERSION_ID >= 70000
	zval                      retval;
#else
	zval                     *retval;
#endif
	php_serialize_data_t      var_hash;
	smart_str                 buf = { 0 };
	char                      s_milliseconds[24];
	int                       s_milliseconds_len;

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	s_milliseconds_len = snprintf(s_milliseconds, sizeof(s_milliseconds), "%" PRId64, intern->milliseconds);

#if PHP_VERSION_ID >= 70000
	array_init_size(&retval, 2);
	ADD_ASSOC_STRINGL(&retval, "milliseconds", s_milliseconds, s_milliseconds_len);
#else
	ALLOC_INIT_ZVAL(retval);
	array_init_size(retval, 2);
	ADD_ASSOC_STRINGL(retval, "milliseconds", s_milliseconds, s_milliseconds_len);
#endif

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash TSRMLS_CC);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}
/* }}} */

/* {{{ proto string UTCDateTime::unserialize(string $serialized)
*/
PHP_METHOD(UTCDateTime, unserialize)
{
	php_phongo_utcdatetime_t *intern;
	zend_error_handling       error_handling;
	char                     *serialized;
	phongo_zpp_char_len       serialized_len;
#if PHP_VERSION_ID >= 70000
	zval                      props;
#else
	zval                     *props;
#endif
	php_unserialize_data_t    var_hash;

	intern = Z_UTCDATETIME_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

#if PHP_VERSION_ID < 70000
	ALLOC_INIT_ZVAL(props);
#endif
	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char *) serialized + serialized_len, &var_hash TSRMLS_CC)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "%s unserialization failed", ZSTR_VAL(php_phongo_utcdatetime_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

#if PHP_VERSION_ID >= 70000
	php_phongo_utcdatetime_init_from_hash(intern, HASH_OF(&props) TSRMLS_CC);
#else
	php_phongo_utcdatetime_init_from_hash(intern, HASH_OF(props) TSRMLS_CC);
#endif
	zval_ptr_dtor(&props);
}
/* }}} */


/* {{{ BSON\UTCDateTime */

ZEND_BEGIN_ARG_INFO_EX(ai_UTCDateTime___construct, 0, 0, 1)
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
	PHP_ME(UTCDateTime, __construct, ai_UTCDateTime___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, __set_state, ai_UTCDateTime___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(UTCDateTime, __toString, ai_UTCDateTime_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, jsonSerialize, ai_UTCDateTime_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, serialize, ai_UTCDateTime_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, unserialize, ai_UTCDateTime_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, toDateTime, ai_UTCDateTime_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_utcdatetime_t object handlers */
static void php_phongo_utcdatetime_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_utcdatetime_t *intern = Z_OBJ_UTCDATETIME(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_utcdatetime_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_utcdatetime_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_utcdatetime_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_utcdatetime;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_utcdatetime_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_utcdatetime;

		return retval;
	}
#endif
} /* }}} */

static int php_phongo_utcdatetime_compare_objects(zval *o1, zval *o2 TSRMLS_DC) /* {{{ */
{
	php_phongo_utcdatetime_t *intern1, *intern2;

	intern1 = Z_UTCDATETIME_OBJ_P(o1);
	intern2 = Z_UTCDATETIME_OBJ_P(o2);

	if (intern1->milliseconds != intern2->milliseconds) {
		return intern1->milliseconds < intern2->milliseconds ? -1 : 1;
	}

	return 0;
} /* }}} */

static HashTable *php_phongo_utcdatetime_get_gc(zval *object, phongo_get_gc_table table, int *n TSRMLS_DC) /* {{{ */
{
	*table = NULL;
	*n = 0;

	return zend_std_get_properties(object TSRMLS_CC);
} /* }}} */

HashTable *php_phongo_utcdatetime_get_properties(zval *object TSRMLS_DC) /* {{{ */
{
	php_phongo_utcdatetime_t *intern;
	HashTable                *props;
	char                      s_milliseconds[24];
	int                       s_milliseconds_len;

	intern = Z_UTCDATETIME_OBJ_P(object);
	props = zend_std_get_properties(object TSRMLS_CC);

	if (!intern->initialized) {
		return props;
	}

	s_milliseconds_len = snprintf(s_milliseconds, sizeof(s_milliseconds), "%" PRId64, intern->milliseconds);

#if PHP_VERSION_ID >= 70000
	{
		zval milliseconds;

		ZVAL_STRINGL(&milliseconds, s_milliseconds, s_milliseconds_len);
		zend_hash_str_update(props, "milliseconds", sizeof("milliseconds")-1, &milliseconds);
	}
#else
	{
		zval *milliseconds;

		MAKE_STD_ZVAL(milliseconds);
		ZVAL_STRINGL(milliseconds, s_milliseconds, s_milliseconds_len, 1);
		zend_hash_update(props, "milliseconds", sizeof("milliseconds"), &milliseconds, sizeof(milliseconds), NULL);
	}
#endif

	return props;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(UTCDateTime)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "UTCDateTime", php_phongo_utcdatetime_me);
	php_phongo_utcdatetime_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_utcdatetime_ce->create_object = php_phongo_utcdatetime_create_object;
	PHONGO_CE_FINAL(php_phongo_utcdatetime_ce);

	zend_class_implements(php_phongo_utcdatetime_ce TSRMLS_CC, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_utcdatetime_ce TSRMLS_CC, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_utcdatetime_ce TSRMLS_CC, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_utcdatetime, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_utcdatetime.compare_objects = php_phongo_utcdatetime_compare_objects;
	php_phongo_handler_utcdatetime.get_gc = php_phongo_utcdatetime_get_gc;
	php_phongo_handler_utcdatetime.get_properties = php_phongo_utcdatetime_get_properties;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_utcdatetime.free_obj = php_phongo_utcdatetime_free_object;
	php_phongo_handler_utcdatetime.offset = XtOffsetOf(php_phongo_utcdatetime_t, std);
#endif

	return SUCCESS;
}
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
