/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2014-2015 Hannes Magnusson                             |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Hannes Magnusson <bjori@php.net>                             |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

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


#define BSON_APPEND_INT32(b,key,val) \
	bson_append_int32 (b, key, (int) strlen (key), val)

#if SIZEOF_LONG == 8
#	define BSON_APPEND_INT(b, key, keylen, val) \
	if (val > INT_MAX || val < INT_MIN) { \
		bson_append_int64(b, key, keylen, val); \
	} else { \
		bson_append_int32(b, key, keylen, val); \
	}
#elif SIZEOF_LONG == 4
#	define BSON_APPEND_INT(b, key, keylen, val) \
	bson_append_int32(b, key, keylen, val);
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

	intern = (php_phongo_objectid_t *)zend_object_store_get_object(object TSRMLS_CC);

	bson_oid_init_from_string(oid, intern->oid);
}
int64_t php_phongo_utcdatetime_get_milliseconds(zval *object TSRMLS_DC)
{
	php_phongo_utcdatetime_t     *intern;

	intern = (php_phongo_utcdatetime_t *)zend_object_store_get_object(object TSRMLS_CC);

	return intern->milliseconds;
}
int32_t php_phongo_timestamp_get_increment(zval *object TSRMLS_DC)
{
	php_phongo_timestamp_t     *intern;

	intern = (php_phongo_timestamp_t *)zend_object_store_get_object(object TSRMLS_CC);

	return intern->increment;
}
int32_t php_phongo_timestamp_get_timestamp(zval *object TSRMLS_DC)
{
	php_phongo_timestamp_t     *intern;

	intern = (php_phongo_timestamp_t *)zend_object_store_get_object(object TSRMLS_CC);

	return intern->timestamp;
}
bool php_phongo_javascript_has_scope(zval *object TSRMLS_DC)
{
	php_phongo_javascript_t *intern;

	intern = (php_phongo_javascript_t *)zend_object_store_get_object(object TSRMLS_CC);

	return !!intern->document;
}
char *php_phongo_javascript_get_javascript(zval *object TSRMLS_DC)
{
	php_phongo_javascript_t *intern;

	intern = (php_phongo_javascript_t *)zend_object_store_get_object(object TSRMLS_CC);

	return intern->javascript;
}
bson_t *php_phongo_javascript_get_scope(zval *object TSRMLS_DC)
{
	php_phongo_javascript_t *intern;

	intern = (php_phongo_javascript_t *)zend_object_store_get_object(object TSRMLS_CC);

	return intern->document;
}
int php_phongo_binary_get_data(zval *object, char **data TSRMLS_DC)
{
	php_phongo_binary_t *intern;

	intern = (php_phongo_binary_t *)zend_object_store_get_object(object TSRMLS_CC);

	*data = intern->data;
	return intern->data_len;
}
int php_phongo_binary_get_type(zval *object TSRMLS_DC)
{
	php_phongo_binary_t *intern;

	intern = (php_phongo_binary_t *)zend_object_store_get_object(object TSRMLS_CC);

	return intern->type;
}
char *php_phongo_regex_get_pattern(zval *object TSRMLS_DC)
{
	php_phongo_regex_t *intern;

	intern = (php_phongo_regex_t *)zend_object_store_get_object(object TSRMLS_CC);

	return intern->pattern;
}
char *php_phongo_regex_get_flags(zval *object TSRMLS_DC)
{
	php_phongo_regex_t *intern;

	intern = (php_phongo_regex_t *)zend_object_store_get_object(object TSRMLS_CC);

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
void php_phongo_bson_visit_corrupt(const bson_iter_t *iter ARG_UNUSED, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;

	mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "Corrupt BSON data detected!");

	zval_ptr_dtor(&retval);
}
/* }}} */
bool php_phongo_bson_visit_double(const bson_iter_t *iter ARG_UNUSED, const char *key, double v_double, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;

	add_assoc_double(retval, key, v_double);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_utf8(const bson_iter_t *iter ARG_UNUSED, const char *key, size_t v_utf8_len, const char *v_utf8, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;

	add_assoc_stringl(retval, key, (char *)v_utf8, v_utf8_len, 1);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_binary(const bson_iter_t *iter ARG_UNUSED, const char *key, bson_subtype_t v_subtype, size_t v_binary_len, const uint8_t *v_binary, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	zval *zchild = NULL;
	TSRMLS_FETCH();

	if (v_subtype == 0x80 && strcmp(key, PHONGO_ODM_FIELD_NAME) == 0) {
		zend_class_entry *found_ce = zend_fetch_class((char *)v_binary, v_binary_len, ZEND_FETCH_CLASS_AUTO|ZEND_FETCH_CLASS_SILENT TSRMLS_CC);

		if (found_ce && PHONGO_IS_CLASS_INSTANTIATABLE(found_ce) && instanceof_function(found_ce, php_phongo_persistable_ce TSRMLS_CC)) {
			((php_phongo_bson_state *)data)->odm = found_ce;
		}
	}

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_binary_from_binary_and_type(zchild, (const char *)v_binary, v_binary_len, v_subtype TSRMLS_CC);

	add_assoc_zval(retval, key, zchild);

	Z_SET_REFCOUNT_P(zchild, 1);

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
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_objectid_new_from_oid(zchild, v_oid TSRMLS_CC);

	add_assoc_zval(retval, key, zchild);

	Z_SET_REFCOUNT_P(zchild, 1);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_bool(const bson_iter_t *iter ARG_UNUSED, const char *key, bool v_bool, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;

	add_assoc_bool(retval, key, v_bool);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_date_time(const bson_iter_t *iter ARG_UNUSED, const char *key, int64_t msec_since_epoch, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_utcdatetime_from_epoch(zchild, msec_since_epoch TSRMLS_CC);

	add_assoc_zval(retval, key, zchild);

	Z_SET_REFCOUNT_P(zchild, 1);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_null(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;

	add_assoc_null(retval, key);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_regex(const bson_iter_t *iter ARG_UNUSED, const char *key, const char *v_regex, const char *v_options, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_regex_from_regex_and_options(zchild, v_regex, v_options TSRMLS_CC);

	add_assoc_zval(retval, key, zchild);

	Z_SET_REFCOUNT_P(zchild, 1);

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
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_javascript_from_javascript(1, zchild, v_code, v_code_len TSRMLS_CC);

	add_assoc_zval(retval, key, zchild);

	Z_SET_REFCOUNT_P(zchild, 1);

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
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_javascript_from_javascript_and_scope(1, zchild, v_code, v_code_len, v_scope TSRMLS_CC);

	add_assoc_zval(retval, key, zchild);

	Z_SET_REFCOUNT_P(zchild, 1);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_int32(const bson_iter_t *iter ARG_UNUSED, const char *key, int32_t v_int32, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;

	add_assoc_long(retval, key, v_int32);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_timestamp(const bson_iter_t *iter ARG_UNUSED, const char *key, uint32_t v_timestamp, uint32_t v_increment, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	php_phongo_new_timestamp_from_increment_and_timestamp(zchild, v_increment, v_timestamp TSRMLS_CC);

	add_assoc_zval(retval, key, zchild);

	Z_SET_REFCOUNT_P(zchild, 1);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_int64(const bson_iter_t *iter ARG_UNUSED, const char *key, int64_t v_int64, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;


	ADD_ASSOC_INT64(retval, key, v_int64);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_maxkey(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	object_init_ex(zchild, php_phongo_maxkey_ce);

	add_assoc_zval(retval, key, zchild);

	Z_SET_REFCOUNT_P(zchild, 1);

	return false;
}
/* }}} */
bool php_phongo_bson_visit_minkey(const bson_iter_t *iter ARG_UNUSED, const char *key, void *data) /* {{{ */
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	zval *zchild = NULL;
	TSRMLS_FETCH();

	MAKE_STD_ZVAL(zchild);
	object_init_ex(zchild, php_phongo_minkey_ce);

	add_assoc_zval(retval, key, zchild);

	Z_SET_REFCOUNT_P(zchild, 1);

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
   { NULL }
};


bool php_phongo_bson_visit_document(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_t *v_document, void *data)
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	bson_iter_t child;
	TSRMLS_FETCH();

	if (bson_iter_init(&child, v_document)) {
		php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;

		state.map = ((php_phongo_bson_state *)data)->map;

		MAKE_STD_ZVAL(state.zchild);
		array_init(state.zchild);

		if (!bson_iter_visit_all(&child, &php_bson_visitors, &state)) {
			/* If php_phongo_bson_visit_binary() finds an ODM class, it should
			 * supersede a default type map and named document class. */
			if (state.odm && state.map.document_type == PHONGO_TYPEMAP_NONE) {
				state.map.document_type = PHONGO_TYPEMAP_CLASS;
			}

			switch(state.map.document_type) {
				case PHONGO_TYPEMAP_NATIVE_ARRAY:
					add_assoc_zval(retval, key, state.zchild);
					Z_SET_REFCOUNT_P(state.zchild, 1);
					break;

				case PHONGO_TYPEMAP_CLASS: {
					zval *obj = NULL;

					MAKE_STD_ZVAL(obj);
					object_init_ex(obj, state.odm ? state.odm : state.map.document);
					zend_call_method_with_1_params(&obj, NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, state.zchild);
					add_assoc_zval(retval, key, obj);
					zval_ptr_dtor(&state.zchild);
					break;
				}

				case PHONGO_TYPEMAP_NATIVE_OBJECT:
				default:
					object_and_properties_init(state.zchild, zend_standard_class_def, Z_ARRVAL_P(state.zchild));
					add_assoc_zval(retval, key, state.zchild);
					Z_SET_REFCOUNT_P(state.zchild, 1);
			}
		}
	}

	return false;
}


bool php_phongo_bson_visit_array(const bson_iter_t *iter ARG_UNUSED, const char *key, const bson_t *v_array, void *data)
{
	zval *retval = ((php_phongo_bson_state *)data)->zchild;
	bson_iter_t child;
	TSRMLS_FETCH();

	if (bson_iter_init(&child, v_array)) {
		php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;

		state.map = ((php_phongo_bson_state *)data)->map;

		MAKE_STD_ZVAL(state.zchild);
		array_init(state.zchild);

		if (!bson_iter_visit_all(&child, &php_bson_visitors, &state)) {

			switch(state.map.array_type) {
				case PHONGO_TYPEMAP_CLASS: {
					zval *obj = NULL;

					MAKE_STD_ZVAL(obj);
					object_init_ex(obj, state.map.array);
					zend_call_method_with_1_params(&obj, NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, state.zchild);
					add_assoc_zval(retval, key, obj);
					zval_ptr_dtor(&state.zchild);
					break;
				}

				case PHONGO_TYPEMAP_NATIVE_OBJECT:
					object_and_properties_init(state.zchild, zend_standard_class_def, Z_ARRVAL_P(state.zchild));
					add_assoc_zval(retval, key, state.zchild);
					Z_SET_REFCOUNT_P(state.zchild, 1);
					break;

				case PHONGO_TYPEMAP_NATIVE_ARRAY:
				default:
					add_assoc_zval(retval, key, state.zchild);
					Z_SET_REFCOUNT_P(state.zchild, 1);
					break;
			}
		}

	}

	return false;
}


int php_phongo_is_array_or_document(zval **val TSRMLS_DC) /* {{{ */
{
	HashTable *ht_data = HASH_OF(*val);
	int        count;

	if (Z_TYPE_PP(val) != IS_ARRAY) {
		return IS_OBJECT;
	}

	count = ht_data ? zend_hash_num_elements(ht_data) : 0;
	if (count > 0) {
		char         *key;
		unsigned int     key_len;
		unsigned long         index = 0;
		unsigned long         idx = 0;
		int           hash_type = 0;
		HashPosition  pos;

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
	} else {
		return Z_TYPE_PP(val);
	}

	return IS_ARRAY;
}
/* }}} */
void object_to_bson(zval *object, php_phongo_bson_flags_t flags, const char *key, long key_len, bson_t *bson TSRMLS_DC)
{
	bson_t child;

	if (Z_TYPE_P(object) == IS_OBJECT && instanceof_function(Z_OBJCE_P(object), php_phongo_type_ce TSRMLS_CC)) {
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_serializable_ce TSRMLS_CC)) {
			zval *obj_data = NULL;
			bson_t child;
			HashTable *tmp_ht;

			zend_call_method_with_0_params(&object, NULL, NULL, BSON_SERIALIZE_FUNC_NAME, &obj_data);

			if (!obj_data) {
				/* zend_call_method() failed */
				return;
			}

			if (Z_TYPE_P(obj_data) != IS_ARRAY && !(Z_TYPE_P(obj_data) == IS_OBJECT && instanceof_function(Z_OBJCE_P(obj_data), zend_standard_class_def TSRMLS_CC))) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Expected %s::%s() to return an array or stdClass, %s given", Z_OBJCE_P(object)->name, BSON_SERIALIZE_FUNC_NAME, (Z_TYPE_P(obj_data) == IS_OBJECT ? Z_OBJCE_P(obj_data)->name : zend_get_type_by_const(Z_TYPE_P(obj_data))));
				zval_ptr_dtor(&obj_data);

				return;
			}

			tmp_ht = HASH_OF(obj_data);

			if (tmp_ht) {
				tmp_ht->nApplyCount++;
			}

			/* Persistable objects must always be serialized as BSON documents;
			 * otherwise, infer based on bsonSerialize()'s return value. */
			if (instanceof_function(Z_OBJCE_P(object), php_phongo_persistable_ce TSRMLS_CC) || php_phongo_is_array_or_document(&obj_data TSRMLS_CC) == IS_OBJECT) {
				bson_append_document_begin(bson, key, key_len, &child);
				if (instanceof_function(Z_OBJCE_P(object), php_phongo_persistable_ce TSRMLS_CC)) {
					if (flags & PHONGO_BSON_ADD_CHILD_ODS) {
						bson_append_binary(&child, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t *)Z_OBJCE_P(object)->name, strlen(Z_OBJCE_P(object)->name));
					}
				}
				zval_to_bson(obj_data, flags, &child, NULL TSRMLS_CC);
				bson_append_document_end(bson, &child);
			} else {
				bson_append_array_begin(bson, key, key_len, &child);
				zval_to_bson(obj_data, flags, &child, NULL TSRMLS_CC);
				bson_append_array_end(bson, &child);
			}

			if (tmp_ht) {
				tmp_ht->nApplyCount--;
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
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_regex_ce TSRMLS_CC)) {
			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Regex");
			bson_append_regex(bson, key, key_len, php_phongo_regex_get_pattern(object TSRMLS_CC), php_phongo_regex_get_flags(object TSRMLS_CC));
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_javascript_ce TSRMLS_CC)) {
			if (php_phongo_javascript_has_scope(object TSRMLS_CC)) {
				mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Javascript with scope");
				bson_append_code(bson, key, key_len, php_phongo_javascript_get_javascript(object TSRMLS_CC));
			} else {
				mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Javascript without scope");
				bson_append_code_with_scope(bson, key, key_len, php_phongo_javascript_get_javascript(object TSRMLS_CC), php_phongo_javascript_get_scope(object TSRMLS_CC));
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

		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Unexpected %s instance: %s", php_phongo_type_ce->name, Z_OBJCE_P(object)->name);
		return;
	}

	mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding document");
	bson_append_document_begin(bson, key, key_len, &child);
	zval_to_bson(object, flags, &child, NULL TSRMLS_CC);
	bson_append_document_end(bson, &child);
}
void phongo_bson_append(bson_t *bson, php_phongo_bson_flags_t flags, const char *key, long key_len, int entry_type, zval *entry TSRMLS_DC)
{
	switch (entry_type)
	{
		case IS_NULL:
			bson_append_null(bson, key, key_len);
			break;

		case IS_BOOL:
			bson_append_bool(bson, key, key_len, Z_BVAL_P(entry));
			break;

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
			if (php_phongo_is_array_or_document(&entry TSRMLS_CC) == IS_ARRAY) {
				bson_t     child;
				HashTable *tmp_ht = HASH_OF(entry);

				if (tmp_ht) {
					tmp_ht->nApplyCount++;
				}

				bson_append_array_begin(bson, key, key_len, &child);
				zval_to_bson(entry, flags, &child, NULL TSRMLS_CC);
				bson_append_array_end(bson, &child);

				if (tmp_ht) {
					tmp_ht->nApplyCount--;
				}
				break;
			}
			/* break intentionally omitted */
		case IS_OBJECT:
			object_to_bson(entry, flags, key, key_len, bson TSRMLS_CC);
			break;

		default:
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Got unsupported type '%s'", zend_get_type_by_const(entry_type));
	}
}

static bool is_public_property(zend_class_entry *ce, const char *prop_name, int prop_name_len TSRMLS_DC) /* {{{ */
{
	zend_property_info *property_info;
	zval member;

	ZVAL_STRINGL(&member, prop_name, prop_name_len, 0);
	property_info = zend_get_property_info(ce, &member, 1 TSRMLS_CC);

	return (property_info && (property_info->flags & ZEND_ACC_PUBLIC));
}
/* }}} */

PHONGO_API void zval_to_bson(zval *data, php_phongo_bson_flags_t flags, bson_t *bson, bson_t **bson_out TSRMLS_DC) /* {{{ */
{
	HashPosition pos;
	HashTable   *ht_data = NULL;
	zval        *obj_data = NULL;

	/* If we will be encoding a class that may contain protected and private
	 * properties, we'll need to filter them out later. */
	bool         ht_data_from_properties = false;

	switch(Z_TYPE_P(data)) {
		case IS_OBJECT:
			if (instanceof_function(Z_OBJCE_P(data), php_phongo_serializable_ce TSRMLS_CC)) {
				zend_call_method_with_0_params(&data, NULL, NULL, BSON_SERIALIZE_FUNC_NAME, &obj_data);

				if (!obj_data) {
					/* zend_call_method() failed */
					break;
				}

				if (Z_TYPE_P(obj_data) != IS_ARRAY && !(Z_TYPE_P(obj_data) == IS_OBJECT && instanceof_function(Z_OBJCE_P(obj_data), zend_standard_class_def TSRMLS_CC))) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Expected %s::%s() to return an array or stdClass, %s given", Z_OBJCE_P(data)->name, BSON_SERIALIZE_FUNC_NAME, (Z_TYPE_P(obj_data) == IS_OBJECT ? Z_OBJCE_P(obj_data)->name : zend_get_type_by_const(Z_TYPE_P(obj_data))));

					break;
				}

				ht_data = HASH_OF(obj_data);

				if (instanceof_function(Z_OBJCE_P(data), php_phongo_persistable_ce TSRMLS_CC)) {
					if (flags & PHONGO_BSON_ADD_ODS) {
						bson_append_binary(bson, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t *)Z_OBJCE_P(data)->name, strlen(Z_OBJCE_P(data)->name));
						zend_hash_del(ht_data, PHONGO_ODM_FIELD_NAME, sizeof(PHONGO_ODM_FIELD_NAME));
					}
				}

				break;
			}

			if (instanceof_function(Z_OBJCE_P(data), php_phongo_type_ce TSRMLS_CC)) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "%s instance %s cannot be serialized as a root element", php_phongo_type_ce->name, Z_OBJCE_P(data)->name);

				break;
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

	if (!ht_data || ht_data->nApplyCount > 1) {
		if (obj_data) {
			zval_ptr_dtor(&obj_data);
		}
		return;
	}

	zend_hash_internal_pointer_reset_ex(ht_data, &pos);
	for (;; zend_hash_move_forward_ex(ht_data, &pos)) {
		unsigned int key_len = 0;
		uint64_t     index = 0;
		char         numbuf[32];
		char        *key = NULL;
		zval       **entry;
		int          hash_type = HASH_KEY_NON_EXISTENT;

		hash_type = zend_hash_get_current_key_ex(ht_data, &key, &key_len, &index, 0, &pos);

		if (hash_type == HASH_KEY_NON_EXISTENT) {
			break;
		}

		if (zend_hash_get_current_data_ex(ht_data, (void **) &entry, &pos) == FAILURE) {
			break;
		}

		if (hash_type == HASH_KEY_IS_STRING) {
			if (ht_data_from_properties) {
				const char *class_name;

				zend_unmangle_property_name(key, key_len-1, &class_name, (const char **)&key);
				key_len = strlen(key);

				/* Ignore non-public properties */
				if (!is_public_property(Z_OBJCE_P(data), key, key_len TSRMLS_CC)) {
					continue;
				}
			} else {
				/* Chop off the \0 from string lengths */
				key_len -= 1;
			}

			if (flags & PHONGO_BSON_ADD_ID) {
				if (!strncmp(key, "_id", sizeof("_id")-1)) {
					flags &= ~PHONGO_BSON_ADD_ID;
				}
			}
		} else {
			key_len = bson_uint32_to_string(index, (const char **)&key, numbuf, sizeof(numbuf));
		}

		phongo_bson_append(bson, flags & ~PHONGO_BSON_ADD_ID, key, key_len, Z_TYPE_PP(entry), *entry TSRMLS_CC);
	}

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
	if (obj_data) {
		zval_ptr_dtor(&obj_data);
	}
}

/* }}} */
int bson_to_zval(const unsigned char *data, int data_len, php_phongo_bson_state *state)
{
	      bson_reader_t *reader;
	      bson_iter_t    iter;
	const bson_t        *b;
	      bool           eof = false;
		  TSRMLS_FETCH();

	/* Ensure that state->zchild has a type, since the calling code may want to
	 * zval_ptr_dtor() it if we throw an exception. */
	ZVAL_NULL(state->zchild);

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
	array_init(state->zchild);
	bson_iter_visit_all(&iter, &php_bson_visitors, state);

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
			zval *obj = NULL;

			MAKE_STD_ZVAL(obj);
			object_init_ex(obj, state->odm ? state->odm : state->map.root);
			zend_call_method_with_1_params(&obj, NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, state->zchild);
			zval_ptr_dtor(&state->zchild);
			state->zchild = obj;
			break;
		}

		case PHONGO_TYPEMAP_NATIVE_OBJECT:
		default:
			object_and_properties_init(state->zchild, zend_standard_class_def, Z_ARRVAL_P(state->zchild));
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

	(void)return_value_ptr; (void)this_ptr; (void)return_value_used; /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &data) == FAILURE) {
		return;
	}

	bson = bson_new();
	zval_to_bson(data, PHONGO_BSON_ADD_ODS|PHONGO_BSON_ADD_CHILD_ODS, bson, NULL TSRMLS_CC);

	RETVAL_STRINGL((const char *) bson_get_data(bson), bson->len, 1);
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
		zend_class_entry *found_ce = zend_fetch_class(classname, classname_len, ZEND_FETCH_CLASS_AUTO|ZEND_FETCH_CLASS_SILENT TSRMLS_CC);

		if (!found_ce) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Class %s does not exist", classname);
		} else if (!PHONGO_IS_CLASS_INSTANTIATABLE(found_ce)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Class %s is not instantiatable", classname);
		} else if (!instanceof_function(found_ce, php_phongo_unserializable_ce TSRMLS_CC)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Class %s does not implement %s\\Unserializable", classname, BSON_NAMESPACE);
		} else {
			*type = PHONGO_TYPEMAP_CLASS;
			*type_ce = found_ce;
		}
	}
}

void php_phongo_bson_typemap_to_state(zval *typemap, php_phongo_bson_typemap *map TSRMLS_DC)
{
	if (typemap) {
		char      *classname;
		int        classname_len;
		zend_bool  classname_free = 0;

		classname = php_array_fetchl_string(typemap, "array", sizeof("array")-1, &classname_len, &classname_free);
		if (classname_len) {
			apply_classname_to_state(classname, classname_len, &map->array_type, &map->array TSRMLS_CC);
		}
		if (classname_free) {
			str_efree(classname);
		}

		classname = php_array_fetchl_string(typemap, "document", sizeof("document")-1, &classname_len, &classname_free);
		if (classname_len) {
			apply_classname_to_state(classname, classname_len, &map->document_type, &map->document TSRMLS_CC);
		}
		if (classname_free) {
			str_efree(classname);
		}

		classname = php_array_fetchl_string(typemap, "root", sizeof("root")-1, &classname_len, &classname_free);
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
	int                    data_len;
	zval                  *typemap = NULL;
	php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

	(void)return_value_ptr; (void)this_ptr; (void)return_value_used; /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a!", &data, &data_len, &typemap) == FAILURE) {
		return;
	}

	php_phongo_bson_typemap_to_state(typemap, &state.map TSRMLS_CC);

	MAKE_STD_ZVAL(state.zchild);
	if (!bson_to_zval((const unsigned char *)data, data_len, &state)) {
		zval_ptr_dtor(&state.zchild);
		RETURN_NULL();
	}
	RETURN_ZVAL(state.zchild, 0, 1);
}
/* }}} */

/* {{{ proto string BSON\toJSON(string $bson)
   Returns the JSON representation of a BSON value */
PHP_FUNCTION(toJSON)
{
	      char          *data;
	      int            data_len;
	const bson_t        *b;
	      bool           eof = false;
	      bson_reader_t *reader;

	(void)return_value_ptr; (void)this_ptr; (void)return_value_used; /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE) {
		return;
	}

	reader = bson_reader_new_from_data((const unsigned char *)data, data_len);
	b = bson_reader_read(reader, NULL);

	if (b) {
		char   *str;
		size_t  str_len;
		str = bson_as_json(b, &str_len);
		RETVAL_STRINGL(str, str_len, 1);
		bson_free(str);
	} else {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Could not read document from BSON reader");
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
	char          *data;
	int            data_len;
	bson_t         b = BSON_INITIALIZER;
	bson_error_t   error;

	(void)return_value_ptr; (void)this_ptr; (void)return_value_used; /* We don't use these */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE) {
		return;
	}

	if (bson_init_from_json(&b, (const char *)data, data_len, &error)) {
		RETVAL_STRINGL((const char *) bson_get_data(&b), b.len, 1);
		bson_destroy(&b);
	} else {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "%s", error.message ? error.message : "Error parsing JSON");
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
