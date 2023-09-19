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

#ifndef PHONGO_COMPAT_H
#define PHONGO_COMPAT_H

#include <php.h>
#include <Zend/zend_types.h>
#include <Zend/zend_string.h>
#include <Zend/zend_portability.h>

/* Include stdbool.h as it might not have been implicitly loaded yet */
#include <stdbool.h>
#include "BSON/Int64.h"

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
#elif defined(__clang__) && __clang_major__ >= 12
#define PHONGO_BREAK_INTENTIONALLY_MISSING __attribute__((fallthrough));
#else
#define PHONGO_BREAK_INTENTIONALLY_MISSING
#endif

#if PHP_VERSION_ID >= 80000
#define PHONGO_COMPAT_OBJ_P(val) Z_OBJ_P(val)
#define phongo_compat_object_handler_type zend_object
#define PHONGO_COMPAT_GET_OBJ(val) val
#define PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(type) php_phongo_handler_##type.compare = php_phongo_##type##_compare_objects;
#define phongo_compat_property_accessor_name_type zend_string
#define PHONGO_COMPAT_PROPERTY_ACCESSOR_NAME_TO_STRING(value, key, len) \
	do {                                                                \
		(key) = ZSTR_VAL((value));                                      \
		(len) = ZSTR_LEN((value));                                      \
	} while (0)
#else /* PHP_VERSION_ID < 80000 */
#define PHONGO_COMPAT_OBJ_P(val) val
#define phongo_compat_object_handler_type zval
#define PHONGO_COMPAT_GET_OBJ(val) Z_OBJ_P(val)
#define PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(type) php_phongo_handler_##type.compare_objects = php_phongo_##type##_compare_objects;
#define ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2)
#define phongo_compat_property_accessor_name_type zval
#define PHONGO_COMPAT_PROPERTY_ACCESSOR_NAME_TO_STRING(value, key, len) \
	do {                                                                \
		(key) = Z_STRVAL_P((value));                                    \
		(len) = Z_STRLEN_P((value));                                    \
	} while (0)
#endif /* PHP_VERSION_ID >= 80000 */

#if SIZEOF_ZEND_LONG == 8
#define PHONGO_LONG_FORMAT PRId64
#elif SIZEOF_ZEND_LONG == 4
#define PHONGO_LONG_FORMAT PRId32
#else
#error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif

#define ADD_ASSOC_STR(_zv, _key, _value) add_assoc_string_ex(_zv, ZEND_STRL(_key), (char*) ZSTR_VAL(_value));
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

#define ADD_NEXT_INDEX_INT64_OBJ(_zv, _value) \
	do {                                      \
		zval zchild;                          \
		phongo_int64_new(&zchild, (_value));  \
		add_next_index_zval((_zv), &zchild);  \
	} while (0);
#define ADD_ASSOC_INT64_OBJ(_zv, _key, _value)  \
	do {                                        \
		zval zchild;                            \
		phongo_int64_new(&zchild, (_value));    \
		add_assoc_zval((_zv), (_key), &zchild); \
	} while (0);
#if SIZEOF_ZEND_LONG == 8
#define ADD_INDEX_INT64(_zv, _index, _value) add_index_long((_zv), (_index), (_value))
#define ADD_NEXT_INDEX_INT64(_zv, _value) add_next_index_long((_zv), (_value))
#define ADD_ASSOC_INT64(_zv, _key, _value) add_assoc_long((_zv), (_key), (_value))
#define ZVAL_INT64(_zv, _value) ZVAL_LONG((_zv), (_value))
#elif SIZEOF_ZEND_LONG == 4
/* The following macros do not handle a false return value for phongo_int64_new.
 * As the function currently does not return false this works fine, but will
 * need updating if that changes. */
#define ADD_INDEX_INT64(_zv, _index, _value)            \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		zval zchild;                                    \
		phongo_int64_new(&zchild, (_value));            \
		add_index_zval((_zv), (_index), &zchild);       \
	} else {                                            \
		add_index_long((_zv), (_index), (_value));      \
	}
#define ADD_NEXT_INDEX_INT64(_zv, _value)               \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		zval zchild;                                    \
		phongo_int64_new(&zchild, (_value));            \
		add_next_index_zval((_zv), &zchild);            \
	} else {                                            \
		add_next_index_long((_zv), (_value));           \
	}
#define ADD_ASSOC_INT64(_zv, _key, _value)              \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		zval zchild;                                    \
		phongo_int64_new(&zchild, (_value));            \
		add_assoc_zval((_zv), (_key), &zchild);         \
	} else {                                            \
		add_assoc_long((_zv), (_key), (_value));        \
	}
#define ZVAL_INT64(_zv, _value)                         \
	if ((_value) > INT32_MAX || (_value) < INT32_MIN) { \
		phongo_int64_new((_zv), (_value));              \
	} else {                                            \
		ZVAL_LONG((_zv), (_value));                     \
	}
#else /* SIZEOF_ZEND_LONG != 8 && SIZEOF_ZEND_LONG != 4 */
#error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif /* SIZEOF_ZEND_LONG */

/* Compatibility macros to override error handling logic */
#define PHONGO_PARSE_PARAMETERS_START(min_num_args, max_num_args)               \
	do {                                                                        \
		zend_error_handling error_handling;                                     \
		zend_replace_error_handling(                                            \
			EH_THROW,                                                           \
			phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), \
			&error_handling);                                                   \
	ZEND_PARSE_PARAMETERS_START(min_num_args, max_num_args)

#define PHONGO_PARSE_PARAMETERS_END()                 \
	ZEND_PARSE_PARAMETERS_END_EX(                     \
		zend_restore_error_handling(&error_handling); \
		return );                                     \
	zend_restore_error_handling(&error_handling);     \
	}                                                 \
	while (0)

#ifndef ZEND_PARSE_PARAMETERS_NONE
#define PHONGO_PARSE_PARAMETERS_NONE()                                          \
	do {                                                                        \
		zend_error_handling error_handling;                                     \
		zend_replace_error_handling(                                            \
			EH_THROW,                                                           \
			phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), \
			&error_handling);                                                   \
		if (zend_parse_parameters_none() == FAILURE) {                          \
			zend_restore_error_handling(&error_handling);                       \
			return;                                                             \
		}                                                                       \
		zend_restore_error_handling(&error_handling);                           \
	} while (0)
#else
#define PHONGO_PARSE_PARAMETERS_NONE()                                          \
	do {                                                                        \
		zend_error_handling error_handling;                                     \
		zend_replace_error_handling(                                            \
			EH_THROW,                                                           \
			phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), \
			&error_handling);                                                   \
		if (UNEXPECTED(ZEND_NUM_ARGS() != 0)) {                                 \
			zend_wrong_parameters_none_error();                                 \
			zend_restore_error_handling(&error_handling);                       \
			return;                                                             \
		}                                                                       \
		zend_restore_error_handling(&error_handling);                           \
	} while (0)
#endif

/* Z_PARAM_*_OR_NULL macros were introduced in PHP 8.0.
 * See: https://github.com/php/php-src/commit/e93d20ad7ebc1075ef1248a663935ee5ea69f1cd */
#ifndef Z_PARAM_OBJECT_OF_CLASS_OR_NULL
#define Z_PARAM_OBJECT_OF_CLASS_OR_NULL(dest, _ce) \
	Z_PARAM_OBJECT_OF_CLASS_EX(dest, _ce, 1, 0)
#endif
#ifndef Z_PARAM_STRING_OR_NULL
#define Z_PARAM_STRING_OR_NULL(dest, dest_len) \
	Z_PARAM_STRING_EX(dest, dest_len, 1, 0)
#endif
#ifndef Z_PARAM_ARRAY_OR_NULL
#define Z_PARAM_ARRAY_OR_NULL(dest) \
	Z_PARAM_ARRAY_EX(dest, 1, 0)
#endif
#ifndef Z_PARAM_ZVAL_OR_NULL
#define Z_PARAM_ZVAL_OR_NULL(dest) \
	Z_PARAM_ZVAL_EX(dest, 1, 0)
#endif

/* ZEND_ABSTRACT_ME_WITH_FLAGS was introduced in PHP 8.0. */
#ifndef ZEND_ABSTRACT_ME_WITH_FLAGS
#define ZEND_ABSTRACT_ME_WITH_FLAGS(classname, name, arg_info, flags) ZEND_RAW_FENTRY(#name, NULL, arg_info, flags)
#endif

/* ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE was introduced in PHP 8.0. */
#ifndef ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE
#define ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(pass_by_ref, name, type_hint, allow_null, default_value) \
	ZEND_ARG_TYPE_INFO(pass_by_ref, name, type_hint, allow_null)
#endif

/* ZEND_ARG_INFO_WITH_DEFAULT_VALUE was introduced in PHP 8.0. */
#ifndef ZEND_ARG_INFO_WITH_DEFAULT_VALUE
#define ZEND_ARG_INFO_WITH_DEFAULT_VALUE(pass_by_ref, name, default_value) \
	ZEND_ARG_INFO(pass_by_ref, name)
#endif

/* ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE was introduced in PHP 8.0. */
#ifndef ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE
#define ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(pass_by_ref, name, classname, allow_null, default_value) \
	ZEND_ARG_OBJ_INFO(pass_by_ref, name, classname, allow_null)
#endif

/* The ZEND_BEGIN_ARG_WITH_TENTATIVE_* set of macros was introduced in PHP 8.1 */
#ifndef ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX
#define ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null) \
	ZEND_BEGIN_ARG_INFO_EX(name, 0, return_reference, required_num_args)
#endif
#ifndef ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX
#define ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null) \
	ZEND_BEGIN_ARG_INFO_EX(name, 0, return_reference, required_num_args)
#endif
#ifndef ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_MASK_EX
#define ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_MASK_EX(name, return_reference, required_num_args, type) \
	ZEND_BEGIN_ARG_INFO_EX(name, 0, return_reference, required_num_args)
#endif
#ifndef ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_OBJ_INFO_EX
#define ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_OBJ_INFO_EX(name, return_reference, required_num_args, class_name, allow_null) \
	ZEND_BEGIN_ARG_INFO_EX(name, 0, return_reference, required_num_args)
#endif
#ifndef ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_OBJ_TYPE_MASK_EX
#define ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_OBJ_TYPE_MASK_EX(name, return_reference, required_num_args, class_name, type) \
	ZEND_BEGIN_ARG_INFO_EX(name, 0, return_reference, required_num_args)
#endif

zend_bool php_phongo_zend_hash_apply_protection_begin(HashTable* ht);
zend_bool php_phongo_zend_hash_apply_protection_end(HashTable* ht);

/* zend_get_object_type_case functions were introduced in PHP 8.2 */
#if PHP_VERSION_ID < 80200
const char* zend_get_object_type_case(const zend_class_entry* ce, zend_bool upper_case);
#define zend_get_object_type(ce) zend_get_object_type_case((ce), false)
#define zend_get_object_type_uc(ce) zend_get_object_type_case((ce), true)
#endif /* PHP_VERSION_ID < 80200 */

#if PHP_VERSION_ID < 80100
zend_bool zend_array_is_list(zend_array* array);
#endif /* PHP_VERSION_ID < 80100 */

#if PHP_VERSION_ID < 80000
typedef ZEND_RESULT_CODE zend_result;
#endif

/* get_gc iterator handler was added in PHP 8.0 */
#if PHP_VERSION_ID >= 80000
#define PHONGO_ITERATOR_FUNCS(dtor, valid, get_current_data, get_current_key, move_forward, rewind, invalidate_current, get_gc) \
	{                                                                                                                           \
		(dtor),                                                                                                                 \
			(valid),                                                                                                            \
			(get_current_data),                                                                                                 \
			(get_current_key),                                                                                                  \
			(move_forward),                                                                                                     \
			(rewind),                                                                                                           \
			(invalidate_current),                                                                                               \
			(get_gc),                                                                                                           \
	}
#else /* PHP_VERSION_ID < 80000 */
#define PHONGO_ITERATOR_FUNCS(dtor, valid, get_current_data, get_current_key, move_forward, rewind, invalidate_current, get_gc) \
	{                                                                                                                           \
		(dtor),                                                                                                                 \
			(valid),                                                                                                            \
			(get_current_data),                                                                                                 \
			(get_current_key),                                                                                                  \
			(move_forward),                                                                                                     \
			(rewind),                                                                                                           \
			(invalidate_current),                                                                                               \
	}
#endif /* PHP_VERSION_ID >= 80000 */

/* ZVAL_OBJ_COPY was added in PHP 8.0 */
#ifndef ZVAL_OBJ_COPY
#define ZVAL_OBJ_COPY(z, o)                \
	do {                                   \
		zval*        __z = (z);            \
		zend_object* __o = (o);            \
		GC_ADDREF(__o);                    \
		Z_OBJ_P(__z)       = __o;          \
		Z_TYPE_INFO_P(__z) = IS_OBJECT_EX; \
	} while (0)
#endif

/* zend_string_concat functions were introduced in PHP 8.0 */
#if PHP_VERSION_ID < 80000
zend_string* zend_string_concat3(const char* str1, size_t str1_len, const char* str2, size_t str2_len, const char* str3, size_t str3_len);
#endif /* PHP_VERSION_ID < 80000 */

#endif /* PHONGO_COMPAT_H */
