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
  | Copyright (c) 2015 MongoDB, Inc.                                          |
  +---------------------------------------------------------------------------+
*/

#ifndef PHONGO_COMPAT_H
#define PHONGO_COMPAT_H

#include <php.h>
#include <Zend/zend_string.h>
#if PHP_VERSION_ID >= 70000
#include <Zend/zend_portability.h>
#endif

#ifdef PHP_WIN32
# include "config.w32.h"
#else
# include <php_config.h>
#endif

#ifndef PHP_FE_END
# define PHP_FE_END { NULL, NULL, NULL }
#endif

#ifndef HASH_KEY_NON_EXISTENT
# define HASH_KEY_NON_EXISTENT HASH_KEY_NON_EXISTANT
#endif

#if PHP_VERSION_ID >= 70000
# define str_efree(s) efree((char*)s)
#else
# include <Zend/zend_string.h>
#endif

#if PHP_VERSION_ID >= 50500
#define ITERATOR_GET_CURRENT_KEY(it, z) \
		(it).funcs->get_current_key(&(it), z TSRMLS_CC);
#else
#define ITERATOR_GET_CURRENT_KEY(it, z) \
{ \
	char *str_key; \
	uint str_key_len; \
	ulong int_key; \
	switch ((it).funcs->get_current_key(&(it), &str_key, &str_key_len, &int_key TSRMLS_CC)) { \
		case HASH_KEY_IS_LONG: \
			ZVAL_LONG(z, int_key); \
			break; \
		case HASH_KEY_IS_STRING: \
			ZVAL_STRINGL(z, str_key, str_key_len-1, 0); \
			break; \
		default: \
			RETURN_NULL(); \
	} \
}
#endif

/* There was a very bad bug in 5.6.0 through 5.6.6 downgrading
 * METHOD_SSLv23 to SSLv2 and SSLv3 exclusively */
#if PHP_VERSION_ID >= 50600 && PHP_VERSION_ID < 50607
# define PHONGO_CRYPTO_METHOD STREAM_CRYPTO_METHOD_ANY_CLIENT
#else
# define PHONGO_CRYPTO_METHOD STREAM_CRYPTO_METHOD_SSLv23_CLIENT
#endif

#if defined(__GNUC__)
#  define ARG_UNUSED  __attribute__ ((unused))
#else
#  define ARG_UNUSED
#endif

#ifndef php_ignore_value
# if defined(__GNUC__) && __GNUC__ >= 4
#  define php_ignore_value(x) (({ __typeof__ (x) __x = (x); (void) __x; }))
# else
#  define php_ignore_value(x) ((void) (x))
# endif
#endif

#if PHP_VERSION_ID >= 70000
# define phongo_char zend_string
# define phongo_char_pdup(str) pestrdup(filename->val, 1)
# define phongo_char_free(str) zend_string_release(str)
# define phongo_long zend_long
#if SIZEOF_ZEND_LONG == 8
#  define PHONGO_LONG_FORMAT PRId64
#elif SIZEOF_ZEND_LONG == 4
#  define PHONGO_LONG_FORMAT PRId32
#else
#  error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif
# define SIZEOF_PHONGO_LONG SIZEOF_ZEND_LONG
# define phongo_create_object_retval zend_object*
# define PHONGO_ALLOC_OBJECT_T(_obj_t, _class_type) (_obj_t *)ecalloc(1, sizeof(_obj_t)+zend_object_properties_size(_class_type))
# define PHONGO_TSRMLS_FETCH_FROM_CTX(user_data)
# define SUPPRESS_UNUSED_WARNING(x)
# define DECLARE_RETURN_VALUE_USED int return_value_used = 1;
# define EXCEPTION_P(_ex, _zp) ZVAL_OBJ(&_zp, _ex)
# define PHONGO_STREAM_ID(stream) stream->res ? stream->res->handle : -1
# define ADD_ASSOC_STRING(_zv, _key, _value) add_assoc_string_ex(_zv, ZEND_STRL(_key), _value);
# define ADD_ASSOC_STRINGL(_zv, _key, _value, _len) add_assoc_stringl_ex(_zv, ZEND_STRL(_key), _value, _len);
# define ADD_ASSOC_LONG_EX(_zv, _key, _value) add_assoc_long_ex(_zv, ZEND_STRL(_key), _value);
# define ADD_ASSOC_STRING_EX(_zv, _key, _key_len, _value, _value_len) add_assoc_stringl_ex(_zv, _key, _key_len, _value, _value_len);
# define ADD_ASSOC_ZVAL_EX(_zv, _key, _value) add_assoc_zval_ex(_zv, ZEND_STRL(_key), _value);
# define ADD_ASSOC_ZVAL(_zv, _key, _value) add_assoc_zval(_zv, _key, _value);
# define ADD_ASSOC_NULL_EX(_zv, _key) add_assoc_null_ex(_zv, ZEND_STRL(_key));
# define ADD_ASSOC_BOOL_EX(_zv, _key, _value) add_assoc_bool_ex(_zv, ZEND_STRL(_key), _value);
# define ADD_INDEX_STRINGL(_zv, _ind, _value, _len) add_index_stringl(_zv, _ind, _value, _len);
# define phongo_free_object_arg zend_object
# define phongo_zpp_char_len size_t
# define ZEND_HASH_APPLY_COUNT(ht) (ht)->u.v.nApplyCount
# define PHONGO_RETVAL_STRINGL(s, slen) RETVAL_STRINGL(s, slen)
# define PHONGO_RETURN_STRINGL(s, slen) RETURN_STRINGL(s, slen)
# define PHONGO_RETVAL_STRING(s) RETVAL_STRING(s)
# define PHONGO_RETURN_STRING(s) RETURN_STRING(s)
# define PHONGO_RETVAL_SMART_STR(val) PHONGO_RETVAL_STRINGL(ZSTR_VAL((val).s), ZSTR_LEN((val).s));
#else
# define phongo_char char
# define phongo_char_pdup(str) pestrdup(filename, 1)
# define phongo_char_free(str) _efree(str ZEND_FILE_LINE_CC ZEND_FILE_LINE_CC)
# define phongo_long long
# define PHONGO_LONG_FORMAT "ld"
# define SIZEOF_PHONGO_LONG SIZEOF_LONG
# define ZSTR_VAL(str) str
# define phongo_create_object_retval zend_object_value
# define PHONGO_ALLOC_OBJECT_T(_obj_t, _class_type) (_obj_t *)ecalloc(1, sizeof(_obj_t))
# define PHONGO_TSRMLS_FETCH_FROM_CTX(user_data) TSRMLS_FETCH_FROM_CTX(user_data)
# define SUPPRESS_UNUSED_WARNING(x) (void)x;
# define DECLARE_RETURN_VALUE_USED
# define EXCEPTION_P(_ex, _zp) _zp = _ex
# define PHONGO_STREAM_ID(stream) stream->rsrc_id
# define ADD_ASSOC_STRING(_zv, _key, _value) add_assoc_string_ex(_zv, ZEND_STRS(_key), _value, 1);
# define ADD_ASSOC_STRINGL(_zv, _key, _value, _len) add_assoc_stringl_ex(_zv, ZEND_STRS(_key), _value, _len, 1);
# define ADD_ASSOC_STRING_EX(_zv, _key, _key_len, _value, _value_len) add_assoc_stringl_ex(_zv, _key, _key_len+1, _value, _value_len, 1);
# define ADD_ASSOC_LONG_EX(_zv, _key, _value) add_assoc_long_ex(_zv, ZEND_STRS(_key), _value);
# define ADD_ASSOC_ZVAL_EX(_zv, _key, _value) add_assoc_zval_ex(_zv, ZEND_STRS(_key), _value);
# define ADD_ASSOC_ZVAL(_zv, _key, _value) add_assoc_zval(_zv, _key, _value);
# define ADD_ASSOC_NULL_EX(_zv, _key) add_assoc_null_ex(_zv, ZEND_STRS(_key));
# define ADD_ASSOC_BOOL_EX(_zv, _key, _value) add_assoc_bool_ex(_zv, ZEND_STRS(_key), _value);
# define ADD_INDEX_STRINGL(_zv, _ind, _value, _len) add_index_stringl(_zv, _ind, _value, _len, 0);
# define Z_PHPDATE_P(object) ((php_date_obj*)zend_object_store_get_object(object TSRMLS_CC))
# define Z_ISUNDEF(x) !x
# define phongo_free_object_arg void
# define phongo_zpp_char_len int
# define ZEND_HASH_APPLY_COUNT(ht) (ht)->nApplyCount
# define PHONGO_RETVAL_STRINGL(s, slen) RETVAL_STRINGL(s, slen, 1)
# define PHONGO_RETURN_STRINGL(s, slen) RETURN_STRINGL(s, slen, 1)
# define PHONGO_RETVAL_STRING(s) RETVAL_STRING(s, 1)
# define PHONGO_RETURN_STRING(s) RETURN_STRING(s, 1)
# define PHONGO_RETVAL_SMART_STR(val) PHONGO_RETVAL_STRINGL((val).c, (val).len);
# define PHP_STREAM_CONTEXT(stream) ((php_stream_context*) (stream)->context)
#endif

#if SIZEOF_PHONGO_LONG == 8
# define ADD_INDEX_INT64(zval, index, value) add_index_long(zval, index, value)
# define ADD_ASSOC_INT64(zval, key, value) add_assoc_long(zval, key, value)
#elif SIZEOF_PHONGO_LONG == 4
# define ADD_INDEX_INT64(zval, index, value) \
    if (value > INT32_MAX || value < INT32_MIN) { \
        char *tmp; \
        int tmp_len; \
        mongoc_log(MONGOC_LOG_LEVEL_WARNING, MONGOC_LOG_DOMAIN, "Integer overflow detected on your platform: %lld", value); \
        tmp_len = spprintf(&tmp, 0, "%lld", value); \
        ADD_INDEX_STRINGL(zval, index, tmp, tmp_len); \
        efree(tmp); \
    } else { \
        add_index_long(zval, index, val); \
    }
# define ADD_ASSOC_INT64(zval, key, value) \
    if (value > INT32_MAX || value < INT32_MIN) { \
        char *tmp; \
        int tmp_len; \
        mongoc_log(MONGOC_LOG_LEVEL_WARNING, MONGOC_LOG_DOMAIN, "Integer overflow detected on your platform: %lld", value); \
        tmp_len = spprintf(&tmp, 0, "%lld", value); \
        ADD_ASSOC_STRING_EX(zval, key, strlen(key), tmp, tmp_len); \
        efree(tmp); \
    } else { \
        add_assoc_long(zval, key, value); \
    }
#else
# error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif

void *x509_from_zval(zval *zval TSRMLS_DC);
void phongo_add_exception_prop(const char *prop, int prop_len, zval *value TSRMLS_DC);


#endif /* PHONGO_COMPAT_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
