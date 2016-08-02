/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2015 MongoDB, Inc.                                         |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2014-2015 MongoDB, Inc.                                     |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/date/php_date.h>
#include <ext/spl/spl_iterators.h>
#include <ext/date/php_date.h>
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

#if defined(PHP_WIN32)
	milliseconds = _atoi64(s_milliseconds);
#else
	milliseconds = bson_ascii_strtoll(s_milliseconds, &endptr, 10);
#endif

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
	usec = (int64_t) floor(datetime_obj->time->f * 1000000 + 0.5);

	intern->milliseconds = (sec * 1000) + (usec / 1000);
	intern->initialized = true;

	return true;
}

/* {{{ proto void UTCDateTime::__construct([integer|DateTimeInterface $milliseconds = null])
   Construct a new BSON UTCDateTime type from either the current time,
   milliseconds since the epoch, or a DateTimeInterface object. */
PHP_METHOD(UTCDateTime, __construct)
{
	php_phongo_utcdatetime_t    *intern;
	zend_error_handling          error_handling;
	zval                        *datetime = NULL;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_UTCDATETIME_OBJ_P(getThis());

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "|o!", &datetime) == SUCCESS) {
		if (datetime == NULL) {
			php_phongo_utcdatetime_init_from_current_time(intern);
		} else if (instanceof_function(Z_OBJCE_P(datetime), php_date_get_date_ce() TSRMLS_CC)) {
			php_phongo_utcdatetime_init_from_date(intern, Z_PHPDATE_P(datetime));
#if PHP_VERSION_ID >= 50500
		} else if (instanceof_function(Z_OBJCE_P(datetime), php_date_get_immutable_ce() TSRMLS_CC)) {
			php_phongo_utcdatetime_init_from_date(intern, Z_PHPDATE_P(datetime));
#endif
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected instance of DateTimeInterface, %s given", ZSTR_VAL(Z_OBJCE_P(datetime)->name));
		}

		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}

#if SIZEOF_PHONGO_LONG == 8
	{
		phongo_long milliseconds;

		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &milliseconds) == FAILURE) {
			zend_restore_error_handling(&error_handling TSRMLS_CC);
			return;
		}

		php_phongo_utcdatetime_init(intern, milliseconds);
	}
#elif SIZEOF_PHONGO_LONG == 4
	{
		char                *s_milliseconds;
		phongo_zpp_char_len  s_milliseconds_len;

		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &s_milliseconds, &s_milliseconds_len) == FAILURE) {
			zend_restore_error_handling(&error_handling TSRMLS_CC);
			return;
		}

		php_phongo_utcdatetime_init_from_string(intern, s_milliseconds, s_milliseconds_len);
	}
#else
# error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif

	zend_restore_error_handling(&error_handling TSRMLS_CC);

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

/* {{{ proto void UTCDateTime::__wakeup()
*/
PHP_METHOD(UTCDateTime, __wakeup)
{
	php_phongo_utcdatetime_t *intern;
	HashTable                *props;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_UTCDATETIME_OBJ_P(getThis());
	props = zend_std_get_properties(getThis() TSRMLS_CC);

	php_phongo_utcdatetime_init_from_hash(intern, props TSRMLS_CC);
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

	datetime_obj->time->f = (double) (intern->milliseconds % 1000) / 1000;
}
/* }}} */


/* {{{ BSON\UTCDateTime */

ZEND_BEGIN_ARG_INFO_EX(ai_UTCDateTime___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, milliseconds)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_UTCDateTime___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_UTCDateTime_void, 0, 0, 0)
ZEND_END_ARG_INFO()


static zend_function_entry php_phongo_utcdatetime_me[] = {
	PHP_ME(UTCDateTime, __construct, ai_UTCDateTime___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, __set_state, ai_UTCDateTime___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(UTCDateTime, __toString, ai_UTCDateTime_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(UTCDateTime, __wakeup, ai_UTCDateTime_void, ZEND_ACC_PUBLIC)
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

HashTable *php_phongo_utcdatetime_get_properties(zval *object TSRMLS_DC) /* {{{ */
{
	php_phongo_utcdatetime_t *intern;
	HashTable              *props;

#if SIZEOF_LONG == 4
	char s_milliseconds[24];
	int s_milliseconds_len;
#endif

	intern = Z_UTCDATETIME_OBJ_P(object);
	props = zend_std_get_properties(object TSRMLS_CC);

	if (!intern->initialized) {
		return props;
	}

#if SIZEOF_LONG == 4
	s_milliseconds_len = snprintf(s_milliseconds, sizeof(s_milliseconds), "%" PRId64, intern->milliseconds);
#endif

#if PHP_VERSION_ID >= 70000
	{
		zval milliseconds;

#if SIZEOF_LONG == 4
		ZVAL_STRINGL(&milliseconds, s_milliseconds, s_milliseconds_len);
#else
		ZVAL_LONG(&milliseconds, intern->milliseconds);
#endif
		zend_hash_str_update(props, "milliseconds", sizeof("milliseconds")-1, &milliseconds);
	}
#else
	{
		zval *milliseconds;

		MAKE_STD_ZVAL(milliseconds);
#if SIZEOF_LONG == 4
		ZVAL_STRINGL(milliseconds, s_milliseconds, s_milliseconds_len, 1);
#else
		ZVAL_LONG(milliseconds, intern->milliseconds);
#endif
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

	INIT_NS_CLASS_ENTRY(ce, BSON_NAMESPACE, "UTCDateTime", php_phongo_utcdatetime_me);
	php_phongo_utcdatetime_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_utcdatetime_ce->create_object = php_phongo_utcdatetime_create_object;
	PHONGO_CE_FINAL(php_phongo_utcdatetime_ce);

	zend_class_implements(php_phongo_utcdatetime_ce TSRMLS_CC, 1, php_phongo_type_ce);
	memcpy(&php_phongo_handler_utcdatetime, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
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
