/*
 * Copyright 2015-2017 MongoDB, Inc.
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

#ifndef PHONGO_COMPAT_H
#define PHONGO_COMPAT_H

#include <php.h>
#include <Zend/zend_string.h>
#include <Zend/zend_portability.h>

#ifdef PHP_WIN32
#include "config.w32.h"
#else
#include <php_config.h>
#endif

#ifndef PHP_FE_END
#define PHP_FE_END       \
	{                    \
		NULL, NULL, NULL \
	}
#endif

#ifndef HASH_KEY_NON_EXISTENT
#define HASH_KEY_NON_EXISTENT HASH_KEY_NON_EXISTANT
#endif

#if defined(__GNUC__)
#define ARG_UNUSED __attribute__((unused))
#else
#define ARG_UNUSED
#endif

#if defined(__GNUC__)
#define PHONGO_GNUC_CHECK_VERSION(major, minor) \
	((__GNUC__ > (major)) ||                    \
	 ((__GNUC__ == (major)) && (__GNUC_MINOR__ >= (minor))))
#else
#define PHONGO_GNUC_CHECK_VERSION(major, minor) 0
#endif

#if PHONGO_GNUC_CHECK_VERSION(7, 0)
#define PHONGO_BREAK_INTENTIONALLY_MISSING __attribute__((fallthrough));
#else
#define PHONGO_BREAK_INTENTIONALLY_MISSING
#endif

#if SIZEOF_ZEND_LONG == 8
#define PHONGO_LONG_FORMAT PRId64
#elif SIZEOF_ZEND_LONG == 4
#define PHONGO_LONG_FORMAT PRId32
#else
#error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif
#define PHONGO_ALLOC_OBJECT_T(_obj_t, _class_type) (_obj_t*) ecalloc(1, sizeof(_obj_t) + zend_object_properties_size(_class_type))
#define ADD_ASSOC_STRING(_zv, _key, _value) add_assoc_string_ex(_zv, ZEND_STRL(_key), (char*) (_value));
#define ADD_ASSOC_STRINGL(_zv, _key, _value, _len) add_assoc_stringl_ex(_zv, ZEND_STRL(_key), (char*) (_value), _len);
#define ADD_ASSOC_STRING_EX(_zv, _key, _key_len, _value, _value_len) add_assoc_stringl_ex(_zv, _key, _key_len, (char*) (_value), _value_len);
#define ADD_ASSOC_LONG_EX(_zv, _key, _value) add_assoc_long_ex(_zv, ZEND_STRL(_key), _value);
#define ADD_ASSOC_ZVAL_EX(_zv, _key, _value) add_assoc_zval_ex(_zv, ZEND_STRL(_key), _value);
#define ADD_ASSOC_ZVAL(_zv, _key, _value) add_assoc_zval(_zv, _key, _value);
#define ADD_ASSOC_NULL_EX(_zv, _key) add_assoc_null_ex(_zv, ZEND_STRL(_key));
#define ADD_ASSOC_BOOL_EX(_zv, _key, _value) add_assoc_bool_ex(_zv, ZEND_STRL(_key), _value);
#define ZVAL_INT64_STRING(_zv, _value)                              \
	do {                                                            \
		char tmp[24];                                               \
		int  tmp_len;                                               \
		tmp_len = snprintf(tmp, sizeof(tmp), "%" PRId64, (_value)); \
		ZVAL_STRINGL((_zv), tmp, tmp_len);                          \
	} while (0)
#define ADD_ASSOC_INT64_AS_STRING(_zv, _key, _value) \
	do {                                             \
		zval z_int;                                  \
		ZVAL_INT64_STRING(&z_int, (_value));         \
		ADD_ASSOC_ZVAL_EX((_zv), (_key), &z_int);    \
	} while (0)
#define ADD_NEXT_INDEX_STRINGL(_zv, _value, _len) add_next_index_stringl(_zv, _value, _len);
#define PHONGO_RETVAL_SMART_STR(val) RETVAL_STRINGL(ZSTR_VAL((val).s), ZSTR_LEN((val).s));
#define ZVAL_STATIC_INIT \
	{                    \
		{                \
			0            \
		}                \
	}

#if SIZEOF_ZEND_LONG == 8
#define ADD_INDEX_INT64(_zv, _index, _value) add_index_long((_zv), (_index), (_value))
#define ADD_NEXT_INDEX_INT64(_zv, _value) add_next_index_long((_zv), (_value))
#define ADD_ASSOC_INT64(_zv, _key, _value) add_assoc_long((_zv), (_key), (_value))
#define ZVAL_INT64(_zv, _value) ZVAL_LONG((_zv), (_value))
#elif SIZEOF_ZEND_LONG == 4
#define ADD_INDEX_INT64(_zv, _index, _value)            \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		zval zchild;                                    \
		php_phongo_bson_new_int64(&zchild, (_value));   \
		add_index_zval((_zv), (_index), &zchild);       \
	} else {                                            \
		add_index_long((_zv), (_index), (_value));      \
	}
#define ADD_NEXT_INDEX_INT64(_zv, _value)               \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		zval zchild;                                    \
		php_phongo_bson_new_int64(&zchild, (_value));   \
		add_next_index_zval((_zv), &zchild);            \
	} else {                                            \
		add_next_index_long((_zv), (_value));           \
	}
#define ADD_ASSOC_INT64(_zv, _key, _value)              \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		zval zchild;                                    \
		php_phongo_bson_new_int64(&zchild, (_value));   \
		add_assoc_zval((_zv), (_key), &zchild);         \
	} else {                                            \
		add_assoc_long((_zv), (_key), (_value));        \
	}
#define ZVAL_INT64(_zv, _value)                         \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		php_phongo_bson_new_int64((_zv), (_value));     \
	} else {                                            \
		ZVAL_LONG((_zv), (_value));                     \
	}
#else /* SIZEOF_ZEND_LONG != 8 && SIZEOF_ZEND_LONG != 4 */
#error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif /* SIZEOF_ZEND_LONG */

void      phongo_add_exception_prop(const char* prop, int prop_len, zval* value);
zend_bool php_phongo_zend_hash_apply_protection_begin(HashTable* ht);
zend_bool php_phongo_zend_hash_apply_protection_end(HashTable* ht);

#endif /* PHONGO_COMPAT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
