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

/* PHP Core stuff */
#include <php.h>
#include <ext/spl/spl_array.h>
#include <Zend/zend_hash.h>
#include <Zend/zend_interfaces.h>

/* PHP array helpers */
#include "php_array_api.h"

/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"

#if SIZEOF_PHONGO_LONG == 8
# define BSON_APPEND_INT(b, key, keylen, val) \
	if (val > INT32_MAX || val < INT32_MIN) { \
		bson_append_int64(b, key, keylen, val); \
	} else { \
		bson_append_int32(b, key, keylen, val); \
	}
#elif SIZEOF_PHONGO_LONG == 4
# define BSON_APPEND_INT(b, key, keylen, val) \
	bson_append_int32(b, key, keylen, val)
#else
# error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif

#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "PHONGO-BSON"

#define PHONGO_ODM_FIELD_NAME "__pclass"

#define PHONGO_IS_CLASS_INSTANTIATABLE(ce) \
	(!(ce->ce_flags & (ZEND_ACC_INTERFACE|ZEND_ACC_IMPLICIT_ABSTRACT_CLASS|ZEND_ACC_EXPLICIT_ABSTRACT_CLASS)))

PHP_MINIT_FUNCTION(bson)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	(void)module_number; /* Really doesn't matter which module number we are */

	/* Register BSONable */
	return SUCCESS;
}

/* Forward declarations */
bool php_phongo_bson_visit_document(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_t *v_document, void *data);
bool php_phongo_bson_visit_array(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_t *v_document, void *data);

/* {{{ Santa's Little Helper: Object getters */
void php_phongo_objectid_get_id(zval *object, bson_oid_t *oid TSRMLS_DC)
{
	php_phongo_objectid_t     *intern;

	intern = Z_OBJECTID_OBJ_P(object);

	bson_oid_init_from_string(oid, intern->oid);
}
int64_t php_phongo_utcdatetime_get_milliseconds(zval *object TSRMLS_DC)
{
	php_phongo_utcdatetime_t     *intern;

	intern = Z_UTCDATETIME_OBJ_P(object);

	return intern->milliseconds;
}
uint32_t php_phongo_timestamp_get_increment(zval *object TSRMLS_DC)
{
	php_phongo_timestamp_t     *intern;

	intern = Z_TIMESTAMP_OBJ_P(object);

	return intern->increment;
}
uint32_t php_phongo_timestamp_get_timestamp(zval *object TSRMLS_DC)
{
	php_phongo_timestamp_t     *intern;

	intern = Z_TIMESTAMP_OBJ_P(object);

	return intern->timestamp;
}
bool php_phongo_javascript_has_scope(zval *object TSRMLS_DC)
{
	php_phongo_javascript_t *intern;

	intern = Z_JAVASCRIPT_OBJ_P(object);

	return !!intern->scope;
}
char *php_phongo_javascript_get_code(zval *object TSRMLS_DC)
{
	php_phongo_javascript_t *intern;

	intern = Z_JAVASCRIPT_OBJ_P(object);

	return intern->code;
}
bson_t *php_phongo_javascript_get_scope(zval *object TSRMLS_DC)
{
	php_phongo_javascript_t *intern;

	intern = Z_JAVASCRIPT_OBJ_P(object);

	return intern->scope;
}
int php_phongo_binary_get_data(zval *object, char **data TSRMLS_DC)
{
	php_phongo_binary_t *intern;

	intern = Z_BINARY_OBJ_P(object);

	*data = intern->data;
	return intern->data_len;
}
uint8_t php_phongo_binary_get_type(zval *object TSRMLS_DC)
{
	php_phongo_binary_t *intern;

	intern = Z_BINARY_OBJ_P(object);

	return intern->type;
}

static bson_decimal128_t *php_phongo_decimal128_get_decimal128(zval *object TSRMLS_DC)
{
	php_phongo_decimal128_t *intern;

	intern = Z_DECIMAL128_OBJ_P(object);

	return &intern->decimal;
}

char *php_phongo_regex_get_pattern(zval *object TSRMLS_DC)
{
	php_phongo_regex_t *intern;

	intern = Z_REGEX_OBJ_P(object);

	return intern->pattern;
}
char *php_phongo_regex_get_flags(zval *object TSRMLS_DC)
{
	php_phongo_regex_t *intern;

	intern = Z_REGEX_OBJ_P(object);

	return intern->flags;
}
/* }}} */
#if 0
bool php_phongo_bson_visit_before(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
	return false;
}
/* }}} */
bool php_phongo_bson_visit_after(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
	return false;
}
/* }}} */
#endif
void php_phongo_bson_visit_corrupt(const bson_iter_t *iter ARG_UNUSED, void *data ARG_UNUSED) /* {{{ */
{
	mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "Corrupt BSON data detected!");
}

void php_phongo_bson_visit_unsupported_type(const bson_iter_t *iter ARG_UNUSED, const char *key, uint32_t v_type_code, void *data ARG_UNUSED) /* {{{ */
{
	TSRMLS_FETCH();
	phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected unknown BSON type 0x%02hhx for fieldname \"%s\". Are you using the latest driver?", v_type_code, key);
}

bool php_phongo_bson_visit_double(const bson_iter_t *iter ARG_UNUSED, const char *key, double v_double, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_double(retval, v_double);
	} else {
		add_assoc_double(retval, key, v_double);
	}

	return false;
}
/* }}} */
bool php_phongo_bson_visit_utf8(const bson_iter_t *iter ARG_UNUSED, const char *key, size_t v_utf8_len, const char *v_utf8, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		ADD_NEXT_INDEX_STRINGL(retval, v_utf8, v_utf8_len);
	} else {
		ADD_ASSOC_STRING_EX(retval, key, strlen(key), v_utf8, v_utf8_len);
	}

	return false;
}
/* }}} */
bool php_phongo_bson_visit_binary(const bson_iter_t *iter ARG_UNUSED, const char *key, bson_subtype_t v_subtype, size_t v_binary_len, const uint8_t *v_binary, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
	TSRMLS_FETCH();

	if (v_subtype == 0x80 && strcmp(key, PHONGO_ODM_FIELD_NAME) == 0) {
#if PHP_VERSION_ID >= 70000
		zend_string *zs_classname = zend_string_init((const char *)v_binary, v_binary_len, 0);
		zend_class_entry *found_ce = zend_fetch_class(zs_classname, ZEND_FETCH_CLASS_AUTO|ZEND_FETCH_CLASS_SILENT TSRMLS_CC);
		zend_string_release(zs_classname);
#else
		zend_class_entry *found_ce = zend_fetch_class((const char *)v_binary, v_binary_len, ZEND_FETCH_CLASS_AUTO|ZEND_FETCH_CLASS_SILENT TSRMLS_CC);
#endif

		if (found_ce && PHONGO_IS_CLASS_INSTANTIATABLE(found_ce) && instanceof_function(found_ce, php_phongo_persistable_ce TSRMLS_CC)) {
			((php_phongo_bson_state *)data)->odm = found_ce;
		}
	}

	{
#if PHP_VERSION_ID >= 70000
		zval zchild;

		php_phongo_new_binary_from_binary_and_type(&zchild, (const char *)v_binary, v_binary_len, v_subtype TSRMLS_CC);

		if (((php_phongo_bson_state *)data)->is_visiting_array) {
			add_next_index_zval(retval, &zchild);
		} else {
			ADD_ASSOC_ZVAL(retval, key, &zchild);
		}
		Z_SET_REFCOUNT(zchild, 1);
#else
		zval *zchild = NULL;

		MAKE_STD_ZVAL(zchild);
		php_phongo_new_binary_from_binary_and_type(zchild, (const char *)v_binary, v_binary_len, v_subtype TSRMLS_CC);

		if (((php_phongo_bson_state *)data)->is_visiting_array) {
			add_next_index_zval(retval, zchild);
		} else {
			ADD_ASSOC_ZVAL(retval, key, zchild);
		}
		Z_SET_REFCOUNT_P(zchild, 1);
#endif
	}

	return false;
}
/* }}} */
#if 0
bool php_phongo_bson_visit_undefined(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
#endif

bool php_phongo_bson_visit_oid(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_oid_t *v_oid, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
#if PHP_VERSION_ID >= 70000
	zval zchild;

	php_phongo_objectid_new_from_oid(&zchild, v_oid TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}
#else
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_objectid_new_from_oid(zchild, v_oid TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
	Z_SET_REFCOUNT_P(zchild, 1);
#endif

	return false;
}
/* }}} */
bool php_phongo_bson_visit_bool(const bson_iter_t *iter ARG_UNUSED, const char *key, bool v_bool, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_bool(retval, v_bool);
	} else {
		add_assoc_bool(retval, key, v_bool);
	}

	return false;
}
/* }}} */
bool php_phongo_bson_visit_date_time(const bson_iter_t *iter ARG_UNUSED, const char *key, int64_t msec_since_epoch, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
#if PHP_VERSION_ID >= 70000
	zval zchild;

	php_phongo_new_utcdatetime_from_epoch(&zchild, msec_since_epoch TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}
#else
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_utcdatetime_from_epoch(zchild, msec_since_epoch TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
	Z_SET_REFCOUNT_P(zchild, 1);
#endif

	return false;
}
/* }}} */

bool php_phongo_bson_visit_decimal128(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_decimal128_t *decimal, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
	zval zchild;

	php_phongo_new_decimal128(&zchild, decimal TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_decimal128(zchild, decimal TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
	Z_SET_REFCOUNT_P(zchild, 1);
#endif

	return false;
}
/* }}} */

bool php_phongo_bson_visit_null(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_null(retval);
	} else {
		add_assoc_null(retval, key);
	}

	return false;
}
/* }}} */
bool php_phongo_bson_visit_regex(const bson_iter_t *iter ARG_UNUSED, const char *key, const char *v_regex, const char *v_options, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
#if PHP_VERSION_ID >= 70000
	zval zchild;

	php_phongo_new_regex_from_regex_and_options(&zchild, v_regex, v_options TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}
#else
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_regex_from_regex_and_options(zchild, v_regex, v_options TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
	Z_SET_REFCOUNT_P(zchild, 1);
#endif

	return false;
}
/* }}} */
#if 0
bool php_phongo_bson_visit_dbpointer(const bson_iter_t *iter ARG_UNUSED, const char *key, size_t v_collection_len, const char *v_collection, const bson_oid_t *v_oid, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
#endif
bool php_phongo_bson_visit_code(const bson_iter_t *iter ARG_UNUSED, const char *key, size_t v_code_len, const char *v_code, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
#if PHP_VERSION_ID >= 70000
	zval zchild;

	php_phongo_new_javascript_from_javascript(1, &zchild, v_code, v_code_len TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}
#else
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_javascript_from_javascript(1, zchild, v_code, v_code_len TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
	Z_SET_REFCOUNT_P(zchild, 1);
#endif

	return false;
}
/* }}} */
#if 0
bool php_phongo_bson_visit_symbol(const bson_iter_t *iter ARG_UNUSED, const char *key, size_t v_symbol_len, const char *v_symbol, void *data) /* {{{ */
{
	printf("Not Implemented\n");

	return true;
}
/* }}} */
#endif
bool php_phongo_bson_visit_codewscope(const bson_iter_t *iter ARG_UNUSED, const char *key, size_t v_code_len, const char *v_code, const bson_t *v_scope, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
#if PHP_VERSION_ID >= 70000
	zval zchild;

	php_phongo_new_javascript_from_javascript_and_scope(1, &zchild, v_code, v_code_len, v_scope TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}
#else
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_javascript_from_javascript_and_scope(1, zchild, v_code, v_code_len, v_scope TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
	Z_SET_REFCOUNT_P(zchild, 1);
#endif

	return false;
}
/* }}} */
bool php_phongo_bson_visit_int32(const bson_iter_t *iter ARG_UNUSED, const char *key, int32_t v_int32, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_long(retval, v_int32);
	} else {
		add_assoc_long(retval, key, v_int32);
	}

	return false;
}
/* }}} */
bool php_phongo_bson_visit_timestamp(const bson_iter_t *iter ARG_UNUSED, const char *key, uint32_t v_timestamp, uint32_t v_increment, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
#if PHP_VERSION_ID >= 70000
	zval zchild;

	php_phongo_new_timestamp_from_increment_and_timestamp(&zchild, v_increment, v_timestamp TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}
#else
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_timestamp_from_increment_and_timestamp(zchild, v_increment, v_timestamp TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
	Z_SET_REFCOUNT_P(zchild, 1);
#endif

	return false;
}
/* }}} */
bool php_phongo_bson_visit_int64(const bson_iter_t *iter ARG_UNUSED, const char *key, int64_t v_int64, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
#if SIZEOF_PHONGO_LONG == 4
	TSRMLS_FETCH();
#endif

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		ADD_NEXT_INDEX_INT64(retval, v_int64);
	} else {
		ADD_ASSOC_INT64(retval, key, v_int64);
	}

	return false;
}
/* }}} */
bool php_phongo_bson_visit_maxkey(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
#if PHP_VERSION_ID >= 70000
	zval zchild;

	object_init_ex(&zchild, php_phongo_maxkey_ce);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}
#else
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	object_init_ex(zchild, php_phongo_maxkey_ce);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
	Z_SET_REFCOUNT_P(zchild, 1);
#endif

	return false;
}
/* }}} */
bool php_phongo_bson_visit_minkey(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
#if PHP_VERSION_ID >= 70000
	zval zchild;

	object_init_ex(&zchild, php_phongo_minkey_ce);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}
#else
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	object_init_ex(zchild, php_phongo_minkey_ce);

	Z_SET_REFCOUNT_P(zchild, 1);
	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
#endif

	return false;
}
/* }}} */


static const bson_visitor_t php_bson_visitors = {
   NULL /* php_phongo_bson_visit_before*/,
   NULL /*php_phongo_bson_visit_after*/,
   php_phongo_bson_visit_corrupt,
   php_phongo_bson_visit_double,
   php_phongo_bson_visit_utf8,
   php_phongo_bson_visit_document,
   php_phongo_bson_visit_array,
   php_phongo_bson_visit_binary,
   NULL /*php_phongo_bson_visit_undefined*/,
   php_phongo_bson_visit_oid,
   php_phongo_bson_visit_bool,
   php_phongo_bson_visit_date_time,
   php_phongo_bson_visit_null,
   php_phongo_bson_visit_regex,
   NULL /*php_phongo_bson_visit_dbpointer*/,
   php_phongo_bson_visit_code,
   NULL /*php_phongo_bson_visit_symbol*/,
   php_phongo_bson_visit_codewscope,
   php_phongo_bson_visit_int32,
   php_phongo_bson_visit_timestamp,
   php_phongo_bson_visit_int64,
   php_phongo_bson_visit_maxkey,
   php_phongo_bson_visit_minkey,
   php_phongo_bson_visit_unsupported_type,
   php_phongo_bson_visit_decimal128,
   { NULL }
};


bool php_phongo_bson_visit_document(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_t *v_document, void *data)
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
	bson_iter_t child;
	TSRMLS_FETCH();

	if (bson_iter_init(&child, v_document)) {
		php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;

		state.map = ((php_phongo_bson_state *)data)->map;

#if PHP_VERSION_ID >= 70000
		array_init(&state.zchild);
#else
		MAKE_STD_ZVAL(state.zchild);
		array_init(state.zchild);
#endif

		if (!bson_iter_visit_all(&child, &php_bson_visitors, &state) && !child.err_off) {
			/* If php_phongo_bson_visit_binary() finds an ODM class, it should
			 * supersede a default type map and named document class. */
			if (state.odm && state.map.document_type == PHONGO_TYPEMAP_NONE) {
				state.map.document_type = PHONGO_TYPEMAP_CLASS;
			}

			switch(state.map.document_type) {
				case PHONGO_TYPEMAP_NATIVE_ARRAY:
#if PHP_VERSION_ID >= 70000
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, &state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, &state.zchild);
					}
					Z_SET_REFCOUNT(state.zchild, 1);
#else
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, state.zchild);
					}
					Z_SET_REFCOUNT_P(state.zchild, 1);
#endif
					break;

				case PHONGO_TYPEMAP_CLASS: {
#if PHP_VERSION_ID >= 70000
					zval obj;

					object_init_ex(&obj, state.odm ? state.odm : state.map.document);
					zend_call_method_with_1_params(&obj, NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, &state.zchild);
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, &obj);
					} else {
						ADD_ASSOC_ZVAL(retval, key, &obj);
					}
					zval_ptr_dtor(&state.zchild);
#else
					zval *obj = NULL;

					MAKE_STD_ZVAL(obj);
					object_init_ex(obj, state.odm ? state.odm : state.map.document);
					zend_call_method_with_1_params(&obj, NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, state.zchild);
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, obj);
					} else {
						ADD_ASSOC_ZVAL(retval, key, obj);
					}
					zval_ptr_dtor(&state.zchild);
#endif
					break;
				}

				case PHONGO_TYPEMAP_NATIVE_OBJECT:
				default:
#if PHP_VERSION_ID >= 70000
					convert_to_object(&state.zchild);
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, &state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, &state.zchild);
					}
					Z_SET_REFCOUNT(state.zchild, 1);
#else
					convert_to_object(state.zchild);
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, state.zchild);
					}
					Z_SET_REFCOUNT_P(state.zchild, 1);
#endif
			}
		} else {
			/* Iteration stopped prematurely due to corruption or a failed
			 * visitor. Free state.zchild, which we just initialized, and return
			 * true to stop iteration for our parent context. */
			zval_ptr_dtor(&state.zchild);
			return true;
		}
	}

	return false;
}


bool php_phongo_bson_visit_array(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_t *v_array, void *data)
{
#if PHP_VERSION_ID >= 70000
	zval *retval = &((php_phongo_bson_state *)data)->zchild;
#else
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
#endif
	bson_iter_t child;
	TSRMLS_FETCH();

	if (bson_iter_init(&child, v_array)) {
		php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;

		state.map = ((php_phongo_bson_state *)data)->map;

		/* Note that we are visiting an array, so element visitors know to use
		 * add_next_index() (i.e. disregard BSON keys) instead of add_assoc()
		 * when building the PHP array.
		 */
		state.is_visiting_array = true;

#if PHP_VERSION_ID >= 70000
		array_init(&state.zchild);
#else
		MAKE_STD_ZVAL(state.zchild);
		array_init(state.zchild);
#endif

		if (!bson_iter_visit_all(&child, &php_bson_visitors, &state) && !child.err_off) {

			switch(state.map.array_type) {
				case PHONGO_TYPEMAP_CLASS: {
#if PHP_VERSION_ID >= 70000
					zval obj;

					object_init_ex(&obj, state.map.array);
					zend_call_method_with_1_params(&obj, NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, &state.zchild);
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, &obj);
					} else {
						ADD_ASSOC_ZVAL(retval, key, &obj);
					}
					zval_ptr_dtor(&state.zchild);
#else
					zval *obj = NULL;

					MAKE_STD_ZVAL(obj);
					object_init_ex(obj, state.map.array);
					zend_call_method_with_1_params(&obj, NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, state.zchild);
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, obj);
					} else {
						ADD_ASSOC_ZVAL(retval, key, obj);
					}
					zval_ptr_dtor(&state.zchild);
#endif
					break;
				}

				case PHONGO_TYPEMAP_NATIVE_OBJECT:
#if PHP_VERSION_ID >= 70000
					convert_to_object(&state.zchild);
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, &state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, &state.zchild);
					}
					Z_SET_REFCOUNT(state.zchild, 1);
#else
					convert_to_object(state.zchild);
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, state.zchild);
					}
					Z_SET_REFCOUNT_P(state.zchild, 1);
#endif
					break;

				case PHONGO_TYPEMAP_NATIVE_ARRAY:
				default:
#if PHP_VERSION_ID >= 70000
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, &state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, &state.zchild);
					}
					Z_SET_REFCOUNT(state.zchild, 1);
#else
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, state.zchild);
					}
					Z_SET_REFCOUNT_P(state.zchild, 1);
#endif
					break;
			}
		} else {
			/* Iteration stopped prematurely due to corruption or a failed
			 * visitor. Free state.zchild, which we just initialized, and return
			 * true to stop iteration for our parent context. */
			zval_ptr_dtor(&state.zchild);
			return true;
		}

	}

	return false;
}


int php_phongo_is_array_or_document(zval *val TSRMLS_DC) /* {{{ */
{
	HashTable *ht_data = HASH_OF(val);
	int        count;

	if (Z_TYPE_P(val) != IS_ARRAY) {
		return IS_OBJECT;
	}

	count = ht_data ? zend_hash_num_elements(ht_data) : 0;
	if (count > 0) {
#if PHP_VERSION_ID >= 70000
		zend_string *key;
		zend_ulong index, idx;

		idx = 0;
		ZEND_HASH_FOREACH_KEY(ht_data, index, key) {
			if (key) {
				return IS_OBJECT;
			} else {
				if (index != idx) {
					return IS_OBJECT;
				}
			}
			idx++;
		} ZEND_HASH_FOREACH_END();
#else
		char          *key;
		unsigned int   key_len;
		unsigned long  index = 0;
		unsigned long  idx = 0;
		int            hash_type = 0;
		HashPosition   pos;

		zend_hash_internal_pointer_reset_ex(ht_data, &pos);
		for (;; zend_hash_move_forward_ex(ht_data, &pos)) {
			hash_type = zend_hash_get_current_key_ex(ht_data, &key, &key_len, &index, 0, &pos);
			if (hash_type == HASH_KEY_NON_EXISTENT) {
				break;
			}

			if (hash_type == HASH_KEY_IS_STRING) {
				return IS_OBJECT;
			} else {
				if (index != idx) {
					return IS_OBJECT;
				}
			}
			idx++;
		}
#endif
	} else {
		return Z_TYPE_P(val);
	}

	return IS_ARRAY;
}
/* }}} */
void object_to_bson(zval *object, php_phongo_bson_flags_t flags, const char *key, long key_len, bson_t *bson TSRMLS_DC)
{
	bson_t child;

	if (Z_TYPE_P(object) == IS_OBJECT && instanceof_function(Z_OBJCE_P(object), php_phongo_type_ce TSRMLS_CC)) {
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_serializable_ce TSRMLS_CC)) {
#if PHP_VERSION_ID >= 70000
			zval obj_data;
#else
			zval *obj_data = NULL;
#endif
			bson_t child;

#if PHP_VERSION_ID >= 70000
			zend_call_method_with_0_params(object, NULL, NULL, BSON_SERIALIZE_FUNC_NAME, &obj_data);
#else
			zend_call_method_with_0_params(&object, NULL, NULL, BSON_SERIALIZE_FUNC_NAME, &obj_data);
#endif

			if (Z_ISUNDEF(obj_data)) {
				/* zend_call_method() failed or bsonSerialize() threw an
				 * exception. Either way, there is nothing else to do. */
				return;
			}

#if PHP_VERSION_ID >= 70000
			if (Z_TYPE(obj_data) != IS_ARRAY && !(Z_TYPE(obj_data) == IS_OBJECT && instanceof_function(Z_OBJCE(obj_data), zend_standard_class_def TSRMLS_CC))) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC,
					"Expected %s::%s() to return an array or stdClass, %s given",
					Z_OBJCE_P(object)->name->val,
					BSON_SERIALIZE_FUNC_NAME,
					(Z_TYPE(obj_data) == IS_OBJECT
						 ? Z_OBJCE(obj_data)->name->val
						 : zend_get_type_by_const(Z_TYPE(obj_data))
					)
				);
				zval_ptr_dtor(&obj_data);
#else
			if (Z_TYPE_P(obj_data) != IS_ARRAY && !(Z_TYPE_P(obj_data) == IS_OBJECT && instanceof_function(Z_OBJCE_P(obj_data), zend_standard_class_def TSRMLS_CC))) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC,
					"Expected %s::%s() to return an array or stdClass, %s given",
					Z_OBJCE_P(object)->name,
					BSON_SERIALIZE_FUNC_NAME,
					(Z_TYPE_P(obj_data) == IS_OBJECT
						 ? Z_OBJCE_P(obj_data)->name
						 : zend_get_type_by_const(Z_TYPE_P(obj_data))
					)
				);
				zval_ptr_dtor(&obj_data);
#endif

				return;
			}

			/* Persistable objects must always be serialized as BSON documents;
			 * otherwise, infer based on bsonSerialize()'s return value. */
#if PHP_VERSION_ID >= 70000
			if (instanceof_function(Z_OBJCE_P(object), php_phongo_persistable_ce TSRMLS_CC) || php_phongo_is_array_or_document(&obj_data TSRMLS_CC) == IS_OBJECT) {
#else
			if (instanceof_function(Z_OBJCE_P(object), php_phongo_persistable_ce TSRMLS_CC) || php_phongo_is_array_or_document(obj_data TSRMLS_CC) == IS_OBJECT) {
#endif
				bson_append_document_begin(bson, key, key_len, &child);
				if (instanceof_function(Z_OBJCE_P(object), php_phongo_persistable_ce TSRMLS_CC)) {
#if PHP_VERSION_ID >= 70000
					bson_append_binary(&child, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t *)Z_OBJCE_P(object)->name->val, Z_OBJCE_P(object)->name->len);
#else
					bson_append_binary(&child, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t *)Z_OBJCE_P(object)->name, strlen(Z_OBJCE_P(object)->name));
#endif
				}
#if PHP_VERSION_ID >= 70000
				phongo_zval_to_bson(&obj_data, flags, &child, NULL TSRMLS_CC);
#else
				phongo_zval_to_bson(obj_data, flags, &child, NULL TSRMLS_CC);
#endif
				bson_append_document_end(bson, &child);
			} else {
				bson_append_array_begin(bson, key, key_len, &child);
#if PHP_VERSION_ID >= 70000
				phongo_zval_to_bson(&obj_data, flags, &child, NULL TSRMLS_CC);
#else
				phongo_zval_to_bson(obj_data, flags, &child, NULL TSRMLS_CC);
#endif
				bson_append_array_end(bson, &child);
			}

			zval_ptr_dtor(&obj_data);
			return;
		}

		if (instanceof_function(Z_OBJCE_P(object), php_phongo_objectid_ce TSRMLS_CC)) {
			bson_oid_t oid;

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding ObjectId");
			php_phongo_objectid_get_id(object, &oid TSRMLS_CC);
			bson_append_oid(bson, key, key_len, &oid);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_utcdatetime_ce TSRMLS_CC)) {
			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding UTCDateTime");
			bson_append_date_time(bson, key, key_len, php_phongo_utcdatetime_get_milliseconds(object TSRMLS_CC));
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_binary_ce TSRMLS_CC)) {
			const unsigned char *data;
			size_t data_len;

			data_len = php_phongo_binary_get_data(object, (char **)&data TSRMLS_CC);

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Binary");
			bson_append_binary(bson, key, key_len, php_phongo_binary_get_type(object TSRMLS_CC), data, data_len);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_decimal128_ce TSRMLS_CC)) {
			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Decimal128");
			bson_append_decimal128(bson, key, key_len, php_phongo_decimal128_get_decimal128(object TSRMLS_CC));
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_regex_ce TSRMLS_CC)) {
			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Regex");
			bson_append_regex(bson, key, key_len, php_phongo_regex_get_pattern(object TSRMLS_CC), php_phongo_regex_get_flags(object TSRMLS_CC));
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_javascript_ce TSRMLS_CC)) {
			if (php_phongo_javascript_has_scope(object TSRMLS_CC)) {
				mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Javascript with scope");
				bson_append_code_with_scope(bson, key, key_len, php_phongo_javascript_get_code(object TSRMLS_CC), php_phongo_javascript_get_scope(object TSRMLS_CC));
			} else {
				mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Javascript without scope");
				bson_append_code(bson, key, key_len, php_phongo_javascript_get_code(object TSRMLS_CC));
			}
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_timestamp_ce TSRMLS_CC)) {
			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Timestamp");
			/*
			 * WHOOPS! libbson swaps the increment/timestamp compared to BSON
			 *
			 * "Timestamp - Special internal type used by MongoDB replication and sharding.
			 *  First 4 bytes are an increment, second 4 are a timestamp."
			 */
			bson_append_timestamp(bson, key, key_len, php_phongo_timestamp_get_timestamp(object TSRMLS_CC), php_phongo_timestamp_get_increment(object TSRMLS_CC));
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_maxkey_ce TSRMLS_CC)) {
			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding MaxKey");
			bson_append_maxkey(bson, key, key_len);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_minkey_ce TSRMLS_CC)) {
			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding MinKey");
			bson_append_minkey(bson, key, key_len);
			return;
		}

		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Unexpected %s instance: %s", ZSTR_VAL(php_phongo_type_ce->name), ZSTR_VAL(Z_OBJCE_P(object)->name));
		return;
	} else {
		mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding document");
		bson_append_document_begin(bson, key, key_len, &child);
		phongo_zval_to_bson(object, flags, &child, NULL TSRMLS_CC);
		bson_append_document_end(bson, &child);
	}
}

static void phongo_bson_append(bson_t *bson, php_phongo_bson_flags_t flags, const char *key, long key_len, zval *entry TSRMLS_DC)
{
#if PHP_VERSION_ID >= 70000
try_again:
#endif
	switch (Z_TYPE_P(entry))
	{
		case IS_NULL:
			bson_append_null(bson, key, key_len);
			break;
#if PHP_VERSION_ID >= 70000
		case IS_TRUE:
			bson_append_bool(bson, key, key_len, true);
			break;

		case IS_FALSE:
			bson_append_bool(bson, key, key_len, false);
			break;
#else
		case IS_BOOL:
			bson_append_bool(bson, key, key_len, Z_BVAL_P(entry));
			break;
#endif

		case IS_LONG:
			BSON_APPEND_INT(bson, key, key_len, Z_LVAL_P(entry));
			break;

		case IS_DOUBLE:
			bson_append_double(bson, key, key_len, Z_DVAL_P(entry));
			break;

		case IS_STRING:
			if (bson_utf8_validate(Z_STRVAL_P(entry), Z_STRLEN_P(entry), true)) {
				bson_append_utf8(bson, key, key_len, Z_STRVAL_P(entry), Z_STRLEN_P(entry));
			} else {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Got invalid UTF-8 value serializing '%s'", Z_STRVAL_P(entry));
			}
			break;

		case IS_ARRAY:
			if (php_phongo_is_array_or_document(entry TSRMLS_CC) == IS_ARRAY) {
				bson_t     child;
				HashTable *tmp_ht = HASH_OF(entry);

				if (tmp_ht && ZEND_HASH_GET_APPLY_COUNT(tmp_ht) > 0) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected recursion for fieldname \"%s\"", key);
					break;
				}

				if (tmp_ht && ZEND_HASH_APPLY_PROTECTION(tmp_ht)) {
					ZEND_HASH_INC_APPLY_COUNT(tmp_ht);
				}

				bson_append_array_begin(bson, key, key_len, &child);
				phongo_zval_to_bson(entry, flags, &child, NULL TSRMLS_CC);
				bson_append_array_end(bson, &child);

				if (tmp_ht && ZEND_HASH_APPLY_PROTECTION(tmp_ht)) {
					ZEND_HASH_DEC_APPLY_COUNT(tmp_ht);
				}
				break;
			}
			/* break intentionally omitted */
		case IS_OBJECT: {
			HashTable *tmp_ht = HASH_OF(entry);

			if (tmp_ht && ZEND_HASH_GET_APPLY_COUNT(tmp_ht) > 0) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected recursion for fieldname \"%s\"", key);
				break;
			}

			if (tmp_ht && ZEND_HASH_APPLY_PROTECTION(tmp_ht)) {
				ZEND_HASH_INC_APPLY_COUNT(tmp_ht);
			}

			object_to_bson(entry, flags, key, key_len, bson TSRMLS_CC);

			if (tmp_ht && ZEND_HASH_APPLY_PROTECTION(tmp_ht)) {
				ZEND_HASH_DEC_APPLY_COUNT(tmp_ht);
			}
			break;
		}

#if PHP_VERSION_ID >= 70000
		case IS_INDIRECT:
			phongo_bson_append(bson, flags, key, key_len, Z_INDIRECT_P(entry) TSRMLS_DC);
			break;

		case IS_REFERENCE:
			ZVAL_DEREF(entry);
			goto try_again;
#endif

		default:
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Got unsupported type %d '%s'", Z_TYPE_P(entry), zend_get_type_by_const(Z_TYPE_P(entry)));
	}
}

void phongo_zval_to_bson(zval *data, php_phongo_bson_flags_t flags, bson_t *bson, bson_t **bson_out TSRMLS_DC) /* {{{ */
{
	HashTable   *ht_data = NULL;
#if PHP_VERSION_ID >= 70000
	zval        obj_data;
#else
	HashPosition pos;
	zval        *obj_data = NULL;
#endif

	/* If we will be encoding a class that may contain protected and private
	 * properties, we'll need to filter them out later. */
	bool         ht_data_from_properties = false;

	/* If the object is an instance of MongoDB\BSON\Persistable, we will need to
	 * inject the PHP class name as a BSON key and ignore any existing key in
	 * the return value of bsonSerialize(). */
	bool         skip_odm_field = false;

	ZVAL_UNDEF(&obj_data);

	switch(Z_TYPE_P(data)) {
		case IS_OBJECT:
			if (instanceof_function(Z_OBJCE_P(data), php_phongo_serializable_ce TSRMLS_CC)) {
#if PHP_VERSION_ID >= 70000
				zend_call_method_with_0_params(data, NULL, NULL, BSON_SERIALIZE_FUNC_NAME, &obj_data);
#else
				zend_call_method_with_0_params(&data, NULL, NULL, BSON_SERIALIZE_FUNC_NAME, &obj_data);
#endif

				if (Z_ISUNDEF(obj_data)) {
					/* zend_call_method() failed or bsonSerialize() threw an
					 * exception. Either way, there is nothing else to do. */
					return;
				}

#if PHP_VERSION_ID >= 70000
				if (Z_TYPE(obj_data) != IS_ARRAY && !(Z_TYPE(obj_data) == IS_OBJECT && instanceof_function(Z_OBJCE(obj_data), zend_standard_class_def TSRMLS_CC))) {
#else
				if (Z_TYPE_P(obj_data) != IS_ARRAY && !(Z_TYPE_P(obj_data) == IS_OBJECT && instanceof_function(Z_OBJCE_P(obj_data), zend_standard_class_def TSRMLS_CC))) {
#endif
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC,
						"Expected %s::%s() to return an array or stdClass, %s given",
#if PHP_VERSION_ID >= 70000
						Z_OBJCE_P(data)->name->val,
#else
						Z_OBJCE_P(data)->name,
#endif
						BSON_SERIALIZE_FUNC_NAME,
#if PHP_VERSION_ID >= 70000
						(Z_TYPE(obj_data) == IS_OBJECT
							 ? Z_OBJCE(obj_data)->name->val
							 : zend_get_type_by_const(Z_TYPE(obj_data))
#else
						(Z_TYPE_P(obj_data) == IS_OBJECT
							 ? Z_OBJCE_P(obj_data)->name
							 : zend_get_type_by_const(Z_TYPE_P(obj_data))
#endif
						)
					);

					goto cleanup;
				}

#if PHP_VERSION_ID >= 70000
				ht_data = HASH_OF(&obj_data);
#else
				ht_data = HASH_OF(obj_data);
#endif

				if (instanceof_function(Z_OBJCE_P(data), php_phongo_persistable_ce TSRMLS_CC)) {
#if PHP_VERSION_ID >= 70000
					bson_append_binary(bson, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t *)Z_OBJCE_P(data)->name->val, Z_OBJCE_P(data)->name->len);
#else
					bson_append_binary(bson, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t *)Z_OBJCE_P(data)->name, strlen(Z_OBJCE_P(data)->name));
#endif
					/* Ensure that we ignore an existing key with the same name
					 * if one exists in the bsonSerialize() return value. */
					skip_odm_field = true;
				}

				break;
			}

			if (instanceof_function(Z_OBJCE_P(data), php_phongo_type_ce TSRMLS_CC)) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "%s instance %s cannot be serialized as a root element", ZSTR_VAL(php_phongo_type_ce->name), ZSTR_VAL(Z_OBJCE_P(data)->name));

				return;
			}

			ht_data = Z_OBJ_HT_P(data)->get_properties(data TSRMLS_CC);
			ht_data_from_properties = true;
			break;

		case IS_ARRAY:
			ht_data = HASH_OF(data);
			break;

		default:
			return;
	}

#if PHP_VERSION_ID >= 70000
	{
		zend_string *string_key = NULL;
		zend_ulong   num_key = 0;
		zval        *value;

		ZEND_HASH_FOREACH_KEY_VAL(ht_data, num_key, string_key, value) {
			if (string_key) {
				if (ht_data_from_properties) {
					/* Skip protected and private properties */
					if (ZSTR_VAL(string_key)[0] == '\0' && ZSTR_LEN(string_key) > 0) {
						continue;
					}
				}

				if (strlen(ZSTR_VAL(string_key)) != ZSTR_LEN(string_key)) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "BSON keys cannot contain null bytes. Unexpected null byte after \"%s\".", ZSTR_VAL(string_key));

					goto cleanup;
				}

				if (skip_odm_field && !strcmp(ZSTR_VAL(string_key), PHONGO_ODM_FIELD_NAME)) {
					continue;
				}

				if (flags & PHONGO_BSON_ADD_ID) {
					if (!strcmp(ZSTR_VAL(string_key), "_id")) {
						flags &= ~PHONGO_BSON_ADD_ID;
					}
				}
			}

			/* Ensure we're working with a string key */
			if (!string_key) {
				string_key = zend_long_to_str(num_key);
			} else {
				zend_string_addref(string_key);
			}

			phongo_bson_append(bson, flags & ~PHONGO_BSON_ADD_ID, ZSTR_VAL(string_key), strlen(ZSTR_VAL(string_key)), value TSRMLS_CC);

			zend_string_release(string_key);
		} ZEND_HASH_FOREACH_END();
	}
#else
	zend_hash_internal_pointer_reset_ex(ht_data, &pos);
	for (;; zend_hash_move_forward_ex(ht_data, &pos)) {
		char   *string_key = NULL;
		uint    string_key_len = 0;
		ulong   num_key = 0;
		zval  **value;
		int     hash_type;

		hash_type = zend_hash_get_current_key_ex(ht_data, &string_key, &string_key_len, &num_key, 0, &pos);

		if (hash_type == HASH_KEY_NON_EXISTENT) {
			break;
		}

		if (zend_hash_get_current_data_ex(ht_data, (void **) &value, &pos) == FAILURE) {
			break;
		}

		if (hash_type == HASH_KEY_IS_STRING) {
			if (ht_data_from_properties) {
				/* Skip protected and private properties */
				if (string_key[0] == '\0' && string_key_len > 1) {
					continue;
				}
			}

			if (strlen(string_key) != string_key_len - 1) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "BSON keys cannot contain null bytes. Unexpected null byte after \"%s\".", ZSTR_VAL(string_key));

				goto cleanup;
			}

			if (skip_odm_field && !strcmp(string_key, PHONGO_ODM_FIELD_NAME)) {
				continue;
			}

			if (flags & PHONGO_BSON_ADD_ID) {
				if (!strcmp(string_key, "_id")) {
					flags &= ~PHONGO_BSON_ADD_ID;
				}
			}
		}

		/* Ensure we're working with a string key */
		if (hash_type == HASH_KEY_IS_LONG) {
			spprintf(&string_key, 0, "%ld", num_key);
		}

		phongo_bson_append(bson, flags & ~PHONGO_BSON_ADD_ID, string_key, strlen(string_key), *value TSRMLS_CC);

		if (hash_type == HASH_KEY_IS_LONG) {
			efree(string_key);
		}
	}
#endif

	if (flags & PHONGO_BSON_ADD_ID) {
		bson_oid_t oid;

		bson_oid_init(&oid, NULL);
		bson_append_oid(bson, "_id", strlen("_id"), &oid);
		mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "Added new _id");
		if (flags & PHONGO_BSON_RETURN_ID) {
			if (bson_out) {
				*bson_out = bson_new();
				bson_append_oid(*bson_out, "_id", strlen("_id"), &oid);
			}
		}
	}

cleanup:
	if (!Z_ISUNDEF(obj_data)) {
		zval_ptr_dtor(&obj_data);
	}
}

/* }}} */
#if PHP_VERSION_ID >= 70000
int phongo_bson_to_zval(const unsigned char *data, int data_len, zval *zv)
#else
int phongo_bson_to_zval(const unsigned char *data, int data_len, zval **zv)
#endif
{
	int retval = 0;
	php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;

	retval = phongo_bson_to_zval_ex(data, data_len, &state);
#if PHP_VERSION_ID >= 70000
	ZVAL_ZVAL(zv, &state.zchild, 1, 1);
#else
	*zv = state.zchild;
#endif

	return retval;
}

int phongo_bson_to_zval_ex(const unsigned char *data, int data_len, php_phongo_bson_state *state)
{
	      bson_reader_t *reader;
	      bson_iter_t    iter;
	const bson_t        *b;
	      bool           eof = false;
		  TSRMLS_FETCH();

#if PHP_VERSION_ID < 70000
	MAKE_STD_ZVAL(state->zchild);

	/* Ensure that state->zchild has a type, since the calling code may want to
	 * zval_ptr_dtor() it if we throw an exception. */
	ZVAL_NULL(state->zchild);

#endif

	reader = bson_reader_new_from_data(data, data_len);

	if (!(b = bson_reader_read(reader, NULL))) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Could not read document from BSON reader");
		bson_reader_destroy(reader);
		return 0;
	}

	if (!bson_iter_init(&iter, b)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Could not initialize BSON iterator");
		bson_reader_destroy(reader);
		return 0;
	}

	/* We initialize an array because it will either be returned as-is (native
	 * array in type map), passed to bsonUnserialize() (ODM class), or used to
	 * initialize a stdClass object (native object in type map). */
#if PHP_VERSION_ID >= 70000
	array_init(&state->zchild);
#else
	array_init(state->zchild);
#endif

	if (bson_iter_visit_all(&iter, &php_bson_visitors, state) || iter.err_off) {
		/* Iteration stopped prematurely due to corruption or a failed visitor.
		 * While we free the reader, state->zchild should be left as-is, since
		 * the calling code may want to zval_ptr_dtor() it. If an exception has
		 * been thrown already (due to an unsupported BSON type for example,
		 * don't overwrite with a generic exception message. */
		if (!EG(exception)) {
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected corrupt BSON data");
		}
		bson_reader_destroy(reader);
		return 0;
	}

	/* If php_phongo_bson_visit_binary() finds an ODM class, it should supersede
	 * a default type map and named root class. */
	if (state->odm && state->map.root_type == PHONGO_TYPEMAP_NONE) {
		state->map.root_type = PHONGO_TYPEMAP_CLASS;
	}

	switch (state->map.root_type) {
		case PHONGO_TYPEMAP_NATIVE_ARRAY:
			/* Nothing to do here */
			break;

		case PHONGO_TYPEMAP_CLASS: {
#if PHP_VERSION_ID >= 70000
			zval obj;

			object_init_ex(&obj, state->odm ? state->odm : state->map.root);
			zend_call_method_with_1_params(&obj, NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, &state->zchild);
			zval_ptr_dtor(&state->zchild);
			ZVAL_COPY_VALUE(&state->zchild, &obj);
#else
			zval *obj = NULL;

			MAKE_STD_ZVAL(obj);
			object_init_ex(obj, state->odm ? state->odm : state->map.root);
			zend_call_method_with_1_params(&obj, NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, state->zchild);
			zval_ptr_dtor(&state->zchild);
			state->zchild = obj;
#endif

			break;
		}

		case PHONGO_TYPEMAP_NATIVE_OBJECT:
		default:
#if PHP_VERSION_ID >= 70000
			convert_to_object(&state->zchild);
#else
			convert_to_object(state->zchild);
#endif
	}

	if (bson_reader_read(reader, &eof) || !eof) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Reading document did not exhaust input buffer");
		bson_reader_destroy(reader);
		return 0;
	}

	bson_reader_destroy(reader);

	return 1;
}

/* {{{ proto string BSON\fromPHP(array|object $value)
   Returns the BSON representation of a PHP value */
PHP_FUNCTION(fromPHP)
{
	zval   *data;
	bson_t *bson;

	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(this_ptr) SUPPRESS_UNUSED_WARNING(return_value_used) /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &data) == FAILURE) {
		return;
	}

	bson = bson_new();
	phongo_zval_to_bson(data, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);

	PHONGO_RETVAL_STRINGL((const char *) bson_get_data(bson), bson->len);
	bson_destroy(bson);
}
/* }}} */

static void apply_classname_to_state(const char *classname, int classname_len, php_phongo_bson_typemap_types *type, zend_class_entry **type_ce TSRMLS_DC)
{
	if (!strcasecmp(classname, "array")) {
		*type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		*type_ce = NULL;
	} else if (!strcasecmp(classname, "stdclass") || !strcasecmp(classname, "object")) {
		*type = PHONGO_TYPEMAP_NATIVE_OBJECT;
		*type_ce = NULL;
	} else {
#if PHP_VERSION_ID >= 70000
		zend_string* zs_classname = zend_string_init(classname, classname_len, 0);
		zend_class_entry *found_ce = zend_fetch_class(zs_classname, ZEND_FETCH_CLASS_AUTO|ZEND_FETCH_CLASS_SILENT TSRMLS_CC);
		zend_string_release(zs_classname);
#else
		zend_class_entry *found_ce = zend_fetch_class(classname, classname_len, ZEND_FETCH_CLASS_AUTO|ZEND_FETCH_CLASS_SILENT TSRMLS_CC);
#endif

		if (!found_ce) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Class %s does not exist", classname);
		} else if (!PHONGO_IS_CLASS_INSTANTIATABLE(found_ce)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Class %s is not instantiatable", classname);
		} else if (!instanceof_function(found_ce, php_phongo_unserializable_ce TSRMLS_CC)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Class %s does not implement %s", classname, ZSTR_VAL(php_phongo_unserializable_ce->name));
		} else {
			*type = PHONGO_TYPEMAP_CLASS;
			*type_ce = found_ce;
		}
	}
}

void phongo_bson_typemap_to_state(zval *typemap, php_phongo_bson_typemap *map TSRMLS_DC)
{
	if (typemap) {
		char      *classname;
		int        classname_len;
		zend_bool  classname_free = 0;

		classname = php_array_fetchc_string(typemap, "array", &classname_len, &classname_free);
		if (classname_len) {
			apply_classname_to_state(classname, classname_len, &map->array_type, &map->array TSRMLS_CC);
		}
		if (classname_free) {
			str_efree(classname);
		}

		classname = php_array_fetchc_string(typemap, "document", &classname_len, &classname_free);
		if (classname_len) {
			apply_classname_to_state(classname, classname_len, &map->document_type, &map->document TSRMLS_CC);
		}
		if (classname_free) {
			str_efree(classname);
		}

		classname = php_array_fetchc_string(typemap, "root", &classname_len, &classname_free);
		if (classname_len) {
			apply_classname_to_state(classname, classname_len, &map->root_type, &map->root TSRMLS_CC);
		}
		if (classname_free) {
			str_efree(classname);
		}
	}
}

/* {{{ proto array|object BSON\toPHP(string $bson [, array $typemap = array()])
   Returns the PHP representation of a BSON value, optionally converting it into a custom class */
PHP_FUNCTION(toPHP)
{
	char                  *data;
	phongo_zpp_char_len    data_len;
	zval                  *typemap = NULL;
	php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(this_ptr) SUPPRESS_UNUSED_WARNING(return_value_used) /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a!", &data, &data_len, &typemap) == FAILURE) {
		return;
	}

	phongo_bson_typemap_to_state(typemap, &state.map TSRMLS_CC);

	if (!phongo_bson_to_zval_ex((const unsigned char *)data, data_len, &state)) {
		zval_ptr_dtor(&state.zchild);
		RETURN_NULL();
	}
#if PHP_VERSION_ID >= 70000
	RETURN_ZVAL(&state.zchild, 0, 1);
#else
	RETURN_ZVAL(state.zchild, 0, 1);
#endif
}
/* }}} */

/* {{{ proto string BSON\toJSON(string $bson)
   Returns the JSON representation of a BSON value */
PHP_FUNCTION(toJSON)
{
	      char                *data;
	      phongo_zpp_char_len  data_len;
	const bson_t              *b;
	      bool                 eof = false;
	      bson_reader_t       *reader;

	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(this_ptr) SUPPRESS_UNUSED_WARNING(return_value_used) /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE) {
		return;
	}

	reader = bson_reader_new_from_data((const unsigned char *)data, data_len);
	b = bson_reader_read(reader, NULL);

	if (b) {
		char   *str;
		size_t  str_len;
		str = bson_as_json(b, &str_len);

		if (!str) {
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Could not convert BSON document to a JSON string");
			bson_reader_destroy(reader);
			return;
		}

		PHONGO_RETVAL_STRINGL(str, str_len);
		bson_free(str);
	} else {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Could not read document from BSON reader");
		bson_reader_destroy(reader);
		return;
	}

	if (bson_reader_read(reader, &eof) || !eof) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Reading document did not exhaust input buffer");
	}

	bson_reader_destroy(reader);
}
/* }}} */

/* {{{ proto string BSON\fromJSON(string $json)
   Returns the BSON representation of a JSON value */
PHP_FUNCTION(fromJSON)
{
	char                *data;
	phongo_zpp_char_len  data_len;
	bson_t               b = BSON_INITIALIZER;
	bson_error_t         error;

	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(this_ptr) SUPPRESS_UNUSED_WARNING(return_value_used) /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE) {
		return;
	}

	if (bson_init_from_json(&b, (const char *)data, data_len, &error)) {
		PHONGO_RETVAL_STRINGL((const char *) bson_get_data(&b), b.len);
		bson_destroy(&b);
	} else {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "%s", error.domain == BSON_ERROR_JSON ? error.message : "Error parsing JSON");
	}
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
