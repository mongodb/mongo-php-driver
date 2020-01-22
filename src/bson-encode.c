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

#include <bson/bson.h>

#include <php.h>
#include <Zend/zend_hash.h>
#include <Zend/zend_interfaces.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_phongo.h"
#include "php_bson.h"
#include "phongo_compat.h"

#if SIZEOF_PHONGO_LONG == 8
#define BSON_APPEND_INT(b, key, keylen, val)    \
	if (val > INT32_MAX || val < INT32_MIN) {   \
		bson_append_int64(b, key, keylen, val); \
	} else {                                    \
		bson_append_int32(b, key, keylen, val); \
	}
#elif SIZEOF_PHONGO_LONG == 4
#define BSON_APPEND_INT(b, key, keylen, val) \
	bson_append_int32(b, key, keylen, val)
#else
#error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif

#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "PHONGO-BSON"

/* Forwards declarations */
static void php_phongo_zval_to_bson_internal(zval* data, php_phongo_field_path* field_path, php_phongo_bson_flags_t flags, bson_t* bson, bson_t** bson_out TSRMLS_DC);

/* Determines whether the argument should be serialized as a BSON array or
 * document. IS_ARRAY is returned if the argument's keys are a sequence of
 * integers starting at zero; otherwise, IS_OBJECT is returned. */
static int php_phongo_is_array_or_document(zval* val TSRMLS_DC) /* {{{ */
{
	HashTable* ht_data = HASH_OF(val);
	int        count;

	if (Z_TYPE_P(val) != IS_ARRAY) {
		return IS_OBJECT;
	}

	count = ht_data ? zend_hash_num_elements(ht_data) : 0;
	if (count > 0) {
#if PHP_VERSION_ID >= 70000
		zend_string* key;
		zend_ulong   index, idx;

		idx = 0;
		ZEND_HASH_FOREACH_KEY(ht_data, index, key)
		{
			if (key) {
				return IS_OBJECT;
			} else {
				if (index != idx) {
					return IS_OBJECT;
				}
			}
			idx++;
		}
		ZEND_HASH_FOREACH_END();
#else
		char*         key;
		unsigned int  key_len;
		unsigned long index     = 0;
		unsigned long idx       = 0;
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
#endif
	} else {
		return Z_TYPE_P(val);
	}

	return IS_ARRAY;
} /* }}} */

/* Appends the array or object argument to the BSON document. If the object is
 * an instance of MongoDB\BSON\Serializable, the return value of bsonSerialize()
 * will be appended as an embedded document. Other MongoDB\BSON\Type instances
 * will be appended as the appropriate BSON type. Other array or object values
 * will be appended as an embedded document. */
static void php_phongo_bson_append_object(bson_t* bson, php_phongo_field_path* field_path, php_phongo_bson_flags_t flags, const char* key, long key_len, zval* object TSRMLS_DC) /* {{{ */
{
	if (Z_TYPE_P(object) == IS_OBJECT && instanceof_function(Z_OBJCE_P(object), php_phongo_cursorid_ce TSRMLS_CC)) {
		bson_append_int64(bson, key, key_len, Z_CURSORID_OBJ_P(object)->id);
		return;
	}

	if (Z_TYPE_P(object) == IS_OBJECT && instanceof_function(Z_OBJCE_P(object), php_phongo_type_ce TSRMLS_CC)) {
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_serializable_ce TSRMLS_CC)) {
#if PHP_VERSION_ID >= 70000
			zval obj_data;
#else
			zval* obj_data = NULL;
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
									   ZSTR_VAL(Z_OBJCE_P(object)->name),
									   BSON_SERIALIZE_FUNC_NAME,
									   PHONGO_ZVAL_CLASS_OR_TYPE_NAME(obj_data));
				zval_ptr_dtor(&obj_data);
#else
			if (Z_TYPE_P(obj_data) != IS_ARRAY && !(Z_TYPE_P(obj_data) == IS_OBJECT && instanceof_function(Z_OBJCE_P(obj_data), zend_standard_class_def TSRMLS_CC))) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC,
									   "Expected %s::%s() to return an array or stdClass, %s given",
									   ZSTR_VAL(Z_OBJCE_P(object)->name),
									   BSON_SERIALIZE_FUNC_NAME,
									   PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(obj_data));
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
					bson_append_binary(&child, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t*) Z_OBJCE_P(object)->name->val, Z_OBJCE_P(object)->name->len);
#else
					bson_append_binary(&child, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t*) Z_OBJCE_P(object)->name, strlen(Z_OBJCE_P(object)->name));
#endif
				}
#if PHP_VERSION_ID >= 70000
				php_phongo_zval_to_bson_internal(&obj_data, field_path, flags, &child, NULL TSRMLS_CC);
#else
				php_phongo_zval_to_bson_internal(obj_data, field_path, flags, &child, NULL TSRMLS_CC);
#endif
				bson_append_document_end(bson, &child);
			} else {
				bson_append_array_begin(bson, key, key_len, &child);
#if PHP_VERSION_ID >= 70000
				php_phongo_zval_to_bson_internal(&obj_data, field_path, flags, &child, NULL TSRMLS_CC);
#else
				php_phongo_zval_to_bson_internal(obj_data, field_path, flags, &child, NULL TSRMLS_CC);
#endif
				bson_append_array_end(bson, &child);
			}

			zval_ptr_dtor(&obj_data);
			return;
		}

		if (instanceof_function(Z_OBJCE_P(object), php_phongo_objectid_ce TSRMLS_CC)) {
			bson_oid_t             oid;
			php_phongo_objectid_t* intern = Z_OBJECTID_OBJ_P(object);

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding ObjectId");
			bson_oid_init_from_string(&oid, intern->oid);
			bson_append_oid(bson, key, key_len, &oid);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_utcdatetime_ce TSRMLS_CC)) {
			php_phongo_utcdatetime_t* intern = Z_UTCDATETIME_OBJ_P(object);

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding UTCDateTime");
			bson_append_date_time(bson, key, key_len, intern->milliseconds);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_binary_ce TSRMLS_CC)) {
			php_phongo_binary_t* intern = Z_BINARY_OBJ_P(object);

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Binary");
			bson_append_binary(bson, key, key_len, intern->type, (const uint8_t*) intern->data, (uint32_t) intern->data_len);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_decimal128_ce TSRMLS_CC)) {
			php_phongo_decimal128_t* intern = Z_DECIMAL128_OBJ_P(object);

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Decimal128");
			bson_append_decimal128(bson, key, key_len, &intern->decimal);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_int64_ce TSRMLS_CC)) {
			php_phongo_int64_t* intern = Z_INT64_OBJ_P(object);

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Int64");
			bson_append_int64(bson, key, key_len, intern->integer);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_regex_ce TSRMLS_CC)) {
			php_phongo_regex_t* intern = Z_REGEX_OBJ_P(object);

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Regex");
			bson_append_regex(bson, key, key_len, intern->pattern, intern->flags);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_javascript_ce TSRMLS_CC)) {
			php_phongo_javascript_t* intern = Z_JAVASCRIPT_OBJ_P(object);

			if (intern->scope) {
				mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Javascript with scope");
				bson_append_code_with_scope(bson, key, key_len, intern->code, intern->scope);
			} else {
				mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Javascript without scope");
				bson_append_code(bson, key, key_len, intern->code);
			}
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_timestamp_ce TSRMLS_CC)) {
			php_phongo_timestamp_t* intern = Z_TIMESTAMP_OBJ_P(object);

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Timestamp");
			bson_append_timestamp(bson, key, key_len, intern->timestamp, intern->increment);
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

		/* Deprecated types */
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_dbpointer_ce TSRMLS_CC)) {
			bson_oid_t              oid;
			php_phongo_dbpointer_t* intern = Z_DBPOINTER_OBJ_P(object);

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding DBPointer");
			bson_oid_init_from_string(&oid, intern->id);
			bson_append_dbpointer(bson, key, key_len, intern->ref, &oid);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_symbol_ce TSRMLS_CC)) {
			php_phongo_symbol_t* intern = Z_SYMBOL_OBJ_P(object);

			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Symbol");
			bson_append_symbol(bson, key, key_len, intern->symbol, intern->symbol_len);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_undefined_ce TSRMLS_CC)) {
			mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding Undefined");
			bson_append_undefined(bson, key, key_len);
			return;
		}

		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Unexpected %s instance: %s", ZSTR_VAL(php_phongo_type_ce->name), ZSTR_VAL(Z_OBJCE_P(object)->name));
		return;
	} else {
		bson_t child;

		mongoc_log(MONGOC_LOG_LEVEL_TRACE, MONGOC_LOG_DOMAIN, "encoding document");
		bson_append_document_begin(bson, key, key_len, &child);
		php_phongo_zval_to_bson_internal(object, field_path, flags, &child, NULL TSRMLS_CC);
		bson_append_document_end(bson, &child);
	}
} /* }}} */

/* Appends the zval argument to the BSON document. If the argument is an object,
 * or an array that should be serialized as an embedded document, this function
 * will defer to php_phongo_bson_append_object(). */
static void php_phongo_bson_append(bson_t* bson, php_phongo_field_path* field_path, php_phongo_bson_flags_t flags, const char* key, long key_len, zval* entry TSRMLS_DC) /* {{{ */
{
	php_phongo_field_path_write_item_at_current_level(field_path, key);

#if PHP_VERSION_ID >= 70000
try_again:
#endif
	switch (Z_TYPE_P(entry)) {
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
				char* path_string = php_phongo_field_path_as_string(field_path);
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected invalid UTF-8 for field path \"%s\": %s", path_string, Z_STRVAL_P(entry));
				efree(path_string);
			}
			break;

		case IS_ARRAY:
			if (php_phongo_is_array_or_document(entry TSRMLS_CC) == IS_ARRAY) {
				bson_t     child;
				HashTable* tmp_ht = HASH_OF(entry);

				if (!php_phongo_zend_hash_apply_protection_begin(tmp_ht)) {
					char* path_string = php_phongo_field_path_as_string(field_path);
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected recursion for field path \"%s\"", path_string);
					efree(path_string);
					break;
				}

				bson_append_array_begin(bson, key, key_len, &child);
				php_phongo_field_path_write_type_at_current_level(field_path, PHONGO_FIELD_PATH_ITEM_ARRAY);
				field_path->size++;
				php_phongo_zval_to_bson_internal(entry, field_path, flags, &child, NULL TSRMLS_CC);
				field_path->size--;
				bson_append_array_end(bson, &child);

				php_phongo_zend_hash_apply_protection_end(tmp_ht);
				break;
			}
			PHONGO_BREAK_INTENTIONALLY_MISSING

		case IS_OBJECT: {
			HashTable* tmp_ht = HASH_OF(entry);

			if (!php_phongo_zend_hash_apply_protection_begin(tmp_ht)) {
				char* path_string = php_phongo_field_path_as_string(field_path);
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected recursion for field path \"%s\"", path_string);
				efree(path_string);
				break;
			}

			php_phongo_field_path_write_type_at_current_level(field_path, PHONGO_FIELD_PATH_ITEM_DOCUMENT);
			field_path->size++;
			php_phongo_bson_append_object(bson, field_path, flags, key, key_len, entry TSRMLS_CC);
			field_path->size--;

			php_phongo_zend_hash_apply_protection_end(tmp_ht);
			break;
		}

#if PHP_VERSION_ID >= 70000
		case IS_REFERENCE:
			ZVAL_DEREF(entry);
			goto try_again;
#endif

		default: {
			char* path_string = php_phongo_field_path_as_string(field_path);
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Detected unsupported PHP type for field path \"%s\": %d (%s)", path_string, Z_TYPE_P(entry), zend_get_type_by_const(Z_TYPE_P(entry)));
			efree(path_string);
		}
	}
} /* }}} */

static void php_phongo_zval_to_bson_internal(zval* data, php_phongo_field_path* field_path, php_phongo_bson_flags_t flags, bson_t* bson, bson_t** bson_out TSRMLS_DC) /* {{{ */
{
	HashTable* ht_data = NULL;
#if PHP_VERSION_ID >= 70000
	zval obj_data;
#else
	HashPosition pos;
	zval* obj_data = NULL;
#endif

	/* If we will be encoding a class that may contain protected and private
	 * properties, we'll need to filter them out later. */
	bool ht_data_from_properties = false;

	/* If the object is an instance of MongoDB\BSON\Persistable, we will need to
	 * inject the PHP class name as a BSON key and ignore any existing key in
	 * the return value of bsonSerialize(). */
	bool skip_odm_field = false;

	ZVAL_UNDEF(&obj_data);

	switch (Z_TYPE_P(data)) {
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
					phongo_throw_exception(
						PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC,
						"Expected %s::%s() to return an array or stdClass, %s given",
						ZSTR_VAL(Z_OBJCE_P(data)->name),
						BSON_SERIALIZE_FUNC_NAME,
#if PHP_VERSION_ID >= 70000
						PHONGO_ZVAL_CLASS_OR_TYPE_NAME(obj_data)
#else
						PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(obj_data)
#endif
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
					bson_append_binary(bson, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t*) Z_OBJCE_P(data)->name->val, Z_OBJCE_P(data)->name->len);
#else
					bson_append_binary(bson, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t*) Z_OBJCE_P(data)->name, strlen(Z_OBJCE_P(data)->name));
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

			ht_data                 = Z_OBJ_HT_P(data)->get_properties(data TSRMLS_CC);
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
		zend_string* string_key = NULL;
		zend_ulong   num_key    = 0;
		zval*        value;

		ZEND_HASH_FOREACH_KEY_VAL_IND(ht_data, num_key, string_key, value)
		{
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

			php_phongo_bson_append(bson, field_path, flags & ~PHONGO_BSON_ADD_ID, ZSTR_VAL(string_key), strlen(ZSTR_VAL(string_key)), value TSRMLS_CC);

			zend_string_release(string_key);
		}
		ZEND_HASH_FOREACH_END();
	}
#else
	zend_hash_internal_pointer_reset_ex(ht_data, &pos);
	for (;; zend_hash_move_forward_ex(ht_data, &pos)) {
		char* string_key = NULL;
		uint string_key_len = 0;
		ulong num_key = 0;
		zval** value;
		int hash_type;

		hash_type = zend_hash_get_current_key_ex(ht_data, &string_key, &string_key_len, &num_key, 0, &pos);

		if (hash_type == HASH_KEY_NON_EXISTENT) {
			break;
		}

		if (zend_hash_get_current_data_ex(ht_data, (void**) &value, &pos) == FAILURE) {
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

		php_phongo_bson_append(bson, field_path, flags & ~PHONGO_BSON_ADD_ID, string_key, strlen(string_key), *value TSRMLS_CC);

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
	}

	if (flags & PHONGO_BSON_RETURN_ID && bson_out) {
		bson_iter_t iter;

		*bson_out = bson_new();

		if (bson_iter_init_find(&iter, bson, "_id") && !bson_append_iter(*bson_out, NULL, 0, &iter)) {
			/* This should not be able to happen since we are copying from
			 * within a valid bson_t. */
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Error copying \"_id\" field from encoded document");

			goto cleanup;
		}
	}

cleanup:
	if (!Z_ISUNDEF(obj_data)) {
		zval_ptr_dtor(&obj_data);
	}
} /* }}} */

/* Converts the array or object argument to a BSON document. If the object is an
 * instance of MongoDB\BSON\Serializable, the return value of bsonSerialize()
 * will be used. */
void php_phongo_zval_to_bson(zval* data, php_phongo_bson_flags_t flags, bson_t* bson, bson_t** bson_out TSRMLS_DC) /* {{{ */
{
	php_phongo_field_path* field_path = php_phongo_field_path_alloc(false);

	php_phongo_zval_to_bson_internal(data, field_path, flags, bson, bson_out TSRMLS_CC);

	php_phongo_field_path_free(field_path);
} /* }}} */

/* Converts the argument to a bson_value_t. If the object is an instance of
 * MongoDB\BSON\Serializable, the return value of bsonSerialize() will be
 * used. */
void php_phongo_zval_to_bson_value(zval* data, php_phongo_bson_flags_t flags, bson_value_t* value TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	bson_t      bson = BSON_INITIALIZER;

#if PHP_VERSION_ID >= 70000
	zval* data_object = ecalloc(sizeof(zval), 1);
#else
	zval* data_object = NULL;
	ALLOC_INIT_ZVAL(data_object);
#endif

	array_init_size(data_object, 1);
	add_assoc_zval(data_object, "data", data);

#if PHP_VERSION_ID >= 70000
	Z_TRY_ADDREF_P(data);
#else
	Z_ADDREF_P(data);
#endif

	php_phongo_zval_to_bson(data_object, flags, &bson, NULL TSRMLS_CC);

	if (bson_iter_init_find(&iter, &bson, "data")) {
		bson_value_copy(bson_iter_value(&iter), value);
	}

#if PHP_VERSION_ID >= 70000
	zval_ptr_dtor(data_object);
	efree(data_object);
#else
	zval_ptr_dtor(&data_object);
#endif
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
