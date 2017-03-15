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

#include <bson.h>

#include <php.h>
#include <Zend/zend_hash.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "php_bson.h"
#include "phongo_compat.h"
#include "php_array_api.h"

#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "PHONGO-BSON"

#define PHONGO_IS_CLASS_INSTANTIATABLE(ce) \
	(!(ce->ce_flags & (ZEND_ACC_INTERFACE|ZEND_ACC_IMPLICIT_ABSTRACT_CLASS|ZEND_ACC_EXPLICIT_ABSTRACT_CLASS)))

#if PHP_VERSION_ID >= 70000
# define PHONGO_BSON_STATE_ZCHILD(state) (&((php_phongo_bson_state *)(state))->zchild)
#else
# define PHONGO_BSON_STATE_ZCHILD(state) (((php_phongo_bson_state *)(state))->zchild)
#endif

/* Forward declarations */
static bool php_phongo_bson_visit_document(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_t *v_document, void *data);
static bool php_phongo_bson_visit_array(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_t *v_document, void *data);

static void php_phongo_bson_visit_corrupt(const bson_iter_t *iter ARG_UNUSED, void *data ARG_UNUSED) /* {{{ */
{
	mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "Corrupt BSON data detected!");
} /* }}} */

static void php_phongo_bson_visit_unsupported_type(const bson_iter_t *iter ARG_UNUSED, const char *key, uint32_t v_type_code, void *data ARG_UNUSED) /* {{{ */
{
	TSRMLS_FETCH();
	phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected unknown BSON type 0x%02hhx for fieldname \"%s\". Are you using the latest driver?", v_type_code, key);
} /* }}} */

static bool php_phongo_bson_visit_double(const bson_iter_t *iter ARG_UNUSED, const char *key, double v_double, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_double(retval, v_double);
	} else {
		add_assoc_double(retval, key, v_double);
	}

	return false;
} /* }}} */

static bool php_phongo_bson_visit_utf8(const bson_iter_t *iter ARG_UNUSED, const char *key, size_t v_utf8_len, const char *v_utf8, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		ADD_NEXT_INDEX_STRINGL(retval, v_utf8, v_utf8_len);
	} else {
		ADD_ASSOC_STRING_EX(retval, key, strlen(key), v_utf8, v_utf8_len);
	}

	return false;
} /* }}} */

static bool php_phongo_bson_visit_binary(const bson_iter_t *iter ARG_UNUSED, const char *key, bson_subtype_t v_subtype, size_t v_binary_len, const uint8_t *v_binary, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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
#else
		zval *zchild = NULL;

		MAKE_STD_ZVAL(zchild);
		php_phongo_new_binary_from_binary_and_type(zchild, (const char *)v_binary, v_binary_len, v_subtype TSRMLS_CC);

		if (((php_phongo_bson_state *)data)->is_visiting_array) {
			add_next_index_zval(retval, zchild);
		} else {
			ADD_ASSOC_ZVAL(retval, key, zchild);
		}
#endif
	}

	return false;
} /* }}} */

static bool php_phongo_bson_visit_undefined(const bson_iter_t *iter, const char *key, void *data) /* {{{ */
{
	TSRMLS_FETCH();
	phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected unsupported BSON type 0x06 (undefined) for fieldname \"%s\"", key);

	return false;
} /* }}} */

static bool php_phongo_bson_visit_oid(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_oid_t *v_oid, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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
#endif

	return false;
} /* }}} */

static bool php_phongo_bson_visit_bool(const bson_iter_t *iter ARG_UNUSED, const char *key, bool v_bool, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_bool(retval, v_bool);
	} else {
		add_assoc_bool(retval, key, v_bool);
	}

	return false;
} /* }}} */

static bool php_phongo_bson_visit_date_time(const bson_iter_t *iter ARG_UNUSED, const char *key, int64_t msec_since_epoch, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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
#endif

	return false;
} /* }}} */

static bool php_phongo_bson_visit_decimal128(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_decimal128_t *decimal, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
#if PHP_VERSION_ID >= 70000
	zval zchild;

	php_phongo_new_decimal128(&zchild, decimal TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}
#else
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_decimal128(zchild, decimal TSRMLS_CC);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
#endif

	return false;
} /* }}} */

static bool php_phongo_bson_visit_null(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_null(retval);
	} else {
		add_assoc_null(retval, key);
	}

	return false;
} /* }}} */

static bool php_phongo_bson_visit_regex(const bson_iter_t *iter ARG_UNUSED, const char *key, const char *v_regex, const char *v_options, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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
#endif

	return false;
} /* }}} */

static bool php_phongo_bson_visit_symbol(const bson_iter_t *iter, const char *key, size_t symbol_len, const char *symbol, void *data) /* {{{ */
{
	TSRMLS_FETCH();
	phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected unsupported BSON type 0x0E (symbol) for fieldname \"%s\"", key);

	return false;
} /* }}} */

static bool php_phongo_bson_visit_code(const bson_iter_t *iter ARG_UNUSED, const char *key, size_t v_code_len, const char *v_code, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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
#endif

	return false;
} /* }}} */

static bool php_phongo_bson_visit_dbpointer(const bson_iter_t *iter, const char *key, size_t collection_len, const char *collection, const bson_oid_t *oid, void *data) /* {{{ */
{
	TSRMLS_FETCH();
	phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected unsupported BSON type 0x0C (DBPointer) for fieldname \"%s\"", key);

	return false;
} /* }}} */

static bool php_phongo_bson_visit_codewscope(const bson_iter_t *iter ARG_UNUSED, const char *key, size_t v_code_len, const char *v_code, const bson_t *v_scope, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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
#endif

	return false;
} /* }}} */

static bool php_phongo_bson_visit_int32(const bson_iter_t *iter ARG_UNUSED, const char *key, int32_t v_int32, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_long(retval, v_int32);
	} else {
		add_assoc_long(retval, key, v_int32);
	}

	return false;
} /* }}} */

static bool php_phongo_bson_visit_timestamp(const bson_iter_t *iter ARG_UNUSED, const char *key, uint32_t v_timestamp, uint32_t v_increment, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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
#endif

	return false;
} /* }}} */

static bool php_phongo_bson_visit_int64(const bson_iter_t *iter ARG_UNUSED, const char *key, int64_t v_int64, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
#if SIZEOF_PHONGO_LONG == 4
	TSRMLS_FETCH();
#endif

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		ADD_NEXT_INDEX_INT64(retval, v_int64);
	} else {
		ADD_ASSOC_INT64(retval, key, v_int64);
	}

	return false;
} /* }}} */

static bool php_phongo_bson_visit_maxkey(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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
#endif

	return false;
} /* }}} */

static bool php_phongo_bson_visit_minkey(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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

	if (((php_phongo_bson_state *)data)->is_visiting_array) {
		add_next_index_zval(retval, zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, zchild);
	}
#endif

	return false;
} /* }}} */

static const bson_visitor_t php_bson_visitors = {
   NULL /* php_phongo_bson_visit_before*/,
   NULL /*php_phongo_bson_visit_after*/,
   php_phongo_bson_visit_corrupt,
   php_phongo_bson_visit_double,
   php_phongo_bson_visit_utf8,
   php_phongo_bson_visit_document,
   php_phongo_bson_visit_array,
   php_phongo_bson_visit_binary,
   php_phongo_bson_visit_undefined,
   php_phongo_bson_visit_oid,
   php_phongo_bson_visit_bool,
   php_phongo_bson_visit_date_time,
   php_phongo_bson_visit_null,
   php_phongo_bson_visit_regex,
   php_phongo_bson_visit_dbpointer,
   php_phongo_bson_visit_code,
   php_phongo_bson_visit_symbol,
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

static bool php_phongo_bson_visit_document(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_t *v_document, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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
#else
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, state.zchild);
					}
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
					object_and_properties_init(&state.zchild, zend_standard_class_def, Z_ARRVAL(state.zchild));
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, &state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, &state.zchild);
					}
#else
					object_and_properties_init(state.zchild, zend_standard_class_def, Z_ARRVAL_P(state.zchild));
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, state.zchild);
					}
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
} /* }}} */

static bool php_phongo_bson_visit_array(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_t *v_array, void *data) /* {{{ */
{
	zval *retval = PHONGO_BSON_STATE_ZCHILD(data);
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
					object_and_properties_init(&state.zchild, zend_standard_class_def, Z_ARRVAL(state.zchild));
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, &state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, &state.zchild);
					}
#else
					object_and_properties_init(state.zchild, zend_standard_class_def, Z_ARRVAL_P(state.zchild));
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, state.zchild);
					}
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
#else
					if (((php_phongo_bson_state *)data)->is_visiting_array) {
						add_next_index_zval(retval, state.zchild);
					} else {
						ADD_ASSOC_ZVAL(retval, key, state.zchild);
					}
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
} /* }}} */

/* Converts a BSON document to a PHP value using the default typemap. */
#if PHP_VERSION_ID >= 70000
int php_phongo_bson_to_zval(const unsigned char *data, int data_len, zval *zv) /* {{{ */
#else
int php_phongo_bson_to_zval(const unsigned char *data, int data_len, zval **zv)
#endif
{
	int retval = 0;
	php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;

	retval = php_phongo_bson_to_zval_ex(data, data_len, &state);
#if PHP_VERSION_ID >= 70000
	ZVAL_ZVAL(zv, &state.zchild, 1, 1);
#else
	*zv = state.zchild;
#endif

	return retval;
} /* }}} */

/* Converts a BSON document to a PHP value according to the typemap specified in
 * the state argument. The result will be set on the state argument. */
int php_phongo_bson_to_zval_ex(const unsigned char *data, int data_len, php_phongo_bson_state *state) /* {{{ */
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
			object_and_properties_init(&state->zchild, zend_standard_class_def, Z_ARRVAL(state->zchild));
#else
			object_and_properties_init(state->zchild, zend_standard_class_def, Z_ARRVAL_P(state->zchild));
#endif
	}

	if (bson_reader_read(reader, &eof) || !eof) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Reading document did not exhaust input buffer");
		bson_reader_destroy(reader);
		return 0;
	}

	bson_reader_destroy(reader);

	return 1;
} /* }}} */

/* {{{ proto string MongoDB\BSON\fromPHP(array|object $value)
   Returns the BSON representation of a PHP value */
PHP_FUNCTION(MongoDB_BSON_fromPHP)
{
	zval   *data;
	bson_t *bson;

	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(this_ptr) SUPPRESS_UNUSED_WARNING(return_value_used) /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &data) == FAILURE) {
		return;
	}

	bson = bson_new();
	php_phongo_zval_to_bson(data, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);

	PHONGO_RETVAL_STRINGL((const char *) bson_get_data(bson), bson->len);
	bson_destroy(bson);
} /* }}} */

static void php_phongo_apply_classname_to_state(const char *classname, int classname_len, php_phongo_bson_typemap_types *type, zend_class_entry **type_ce TSRMLS_DC) /* {{{ */
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
} /* }}} */

/* Applies the array argument to a typemap struct. An exception may be thrown if
 * an invalid class name is provided. */
void php_phongo_bson_typemap_to_state(zval *typemap, php_phongo_bson_typemap *map TSRMLS_DC) /* {{{ */
{
	if (typemap) {
		char      *classname;
		int        classname_len;
		zend_bool  classname_free = 0;

		classname = php_array_fetchc_string(typemap, "array", &classname_len, &classname_free);
		if (classname_len) {
			php_phongo_apply_classname_to_state(classname, classname_len, &map->array_type, &map->array TSRMLS_CC);
		}
		if (classname_free) {
			str_efree(classname);
		}

		classname = php_array_fetchc_string(typemap, "document", &classname_len, &classname_free);
		if (classname_len) {
			php_phongo_apply_classname_to_state(classname, classname_len, &map->document_type, &map->document TSRMLS_CC);
		}
		if (classname_free) {
			str_efree(classname);
		}

		classname = php_array_fetchc_string(typemap, "root", &classname_len, &classname_free);
		if (classname_len) {
			php_phongo_apply_classname_to_state(classname, classname_len, &map->root_type, &map->root TSRMLS_CC);
		}
		if (classname_free) {
			str_efree(classname);
		}
	}
} /* }}} */

/* {{{ proto array|object MongoDB\BSON\toPHP(string $bson [, array $typemap = array()])
   Returns the PHP representation of a BSON value, optionally converting it into a custom class */
PHP_FUNCTION(MongoDB_BSON_toPHP)
{
	char                  *data;
	phongo_zpp_char_len    data_len;
	zval                  *typemap = NULL;
	php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(this_ptr) SUPPRESS_UNUSED_WARNING(return_value_used) /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a!", &data, &data_len, &typemap) == FAILURE) {
		return;
	}

	php_phongo_bson_typemap_to_state(typemap, &state.map TSRMLS_CC);

	if (!php_phongo_bson_to_zval_ex((const unsigned char *)data, data_len, &state)) {
		zval_ptr_dtor(&state.zchild);
		RETURN_NULL();
	}
#if PHP_VERSION_ID >= 70000
	RETURN_ZVAL(&state.zchild, 0, 1);
#else
	RETURN_ZVAL(state.zchild, 0, 1);
#endif
} /* }}} */

static void phongo_bson_to_json(INTERNAL_FUNCTION_PARAMETERS, int type)
{
	char                *data;
	phongo_zpp_char_len  data_len;
	const bson_t        *b;
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

		if (type == 0) {
			str = bson_as_json(b, &str_len);
		} else {
			str = bson_as_extended_json(b, &str_len);
		}

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
} /* }}} */

/* {{{ proto string MongoDB\BSON\toJSON(string $bson)
   Returns the JSON representation of a BSON value */
PHP_FUNCTION(MongoDB_BSON_toJSON)
{
	return phongo_bson_to_json(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
} /* }}} */

/* {{{ proto string MongoDB\BSON\toExtendedJSON(string $bson)
   Returns the extended JSON representation of a BSON value */
PHP_FUNCTION(MongoDB_BSON_toExtendedJSON)
{
	return phongo_bson_to_json(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
} /* }}} */

/* {{{ proto string MongoDB\BSON\fromJSON(string $json)
   Returns the BSON representation of a JSON value */
PHP_FUNCTION(MongoDB_BSON_fromJSON)
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
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
