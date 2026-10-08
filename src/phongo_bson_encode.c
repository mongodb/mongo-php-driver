/*
 * Copyright 2014-present MongoDB, Inc.
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

#include "bson/bson.h"

#include <php.h>
#include <Zend/zend_enum.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_bson.h"
#include "phongo_bson_encode.h"
#include "phongo_compat.h"
#include "phongo_error.h"

#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "PHONGO-BSON"

#if SIZEOF_ZEND_LONG == 8
#define BSON_APPEND_INT(b, key, keylen, val) \
	(val > INT32_MAX || val < INT32_MIN ? bson_append_int64(b, key, keylen, val) : bson_append_int32(b, key, keylen, val))
#elif SIZEOF_ZEND_LONG == 4
#define BSON_APPEND_INT(b, key, keylen, val) \
	bson_append_int32(b, key, keylen, val)
#else
#error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif

/* Forwards declarations */
static void php_phongo_bson_append(bson_t* bson, php_phongo_field_path* field_path, php_phongo_bson_flags_t flags, const char* key, size_t key_len, zval* entry);
static void php_phongo_zval_to_bson_internal(zval* data, php_phongo_field_path* field_path, php_phongo_bson_flags_t flags, bson_t* bson, bson_t** bson_out);

/* Rejects a string or binary length that BSON cannot represent. The encoder
 * narrows these lengths to int32 or uint32 when appending, so an unchecked
 * value would be silently truncated or wrap an allocation size. */
static bool phongo_bson_length_is_valid(size_t length, php_phongo_field_path* field_path, const char* description)
{
	char* path_string;

	if (length <= PHONGO_BSON_MAX_LENGTH) {
		return true;
	}

	path_string = php_phongo_field_path_as_string(field_path);
	phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Expected %s for field path \"%s\" to be <= %" PRId32 " bytes, %zu given", description, path_string, (int32_t) PHONGO_BSON_MAX_LENGTH, length);
	efree(path_string);

	return false;
}

/* Throws if a libbson append call failed. libbson rejects an append when the
 * resulting document would exceed the maximum BSON size, when a key contains
 * an embedded NUL, or when regex options contain invalid characters. Does
 * nothing if an exception is already pending. */
static void phongo_bson_append_check(bool append_ok, php_phongo_field_path* field_path)
{
	char* path_string;

	if (append_ok || EG(exception)) {
		return;
	}

	path_string = php_phongo_field_path_as_string(field_path);
	phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not append BSON data for field path \"%s\".", path_string);
	efree(path_string);
}

/* Determines whether the argument should be serialized as a BSON array or
 * document. IS_ARRAY is returned if the argument's keys are a sequence of
 * integers starting at zero; otherwise, IS_OBJECT is returned. */
static int php_phongo_is_array_or_document(zval* val)
{
	HashTable* ht_data = HASH_OF(val);
	int        count;

	if (Z_TYPE_P(val) != IS_ARRAY) {
		if (Z_TYPE_P(val) == IS_OBJECT && instanceof_function(Z_OBJCE_P(val), php_phongo_packedarray_ce)) {
			return IS_ARRAY;
		}

		return IS_OBJECT;
	}

	count = ht_data ? zend_hash_num_elements(ht_data) : 0;
	if (count > 0) {
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
	} else {
		return Z_TYPE_P(val);
	}

	return IS_ARRAY;
}

/* Checks the return type of a bsonSerialize() method. Returns true on
 * success; otherwise, throws an exception and returns false.
 *
 * TODO: obsolete once the tentative return type in Serializable::bsonSerialize is enforced.
 */
static inline bool phongo_check_bson_serialize_return_type(zval* retval, zend_class_entry* ce)
{
	if (instanceof_function(ce, php_phongo_persistable_ce)) {
		// Instances of Persistable must return an array, stdClass, or MongoDB\BSON\Document
		if (
			Z_TYPE_P(retval) != IS_ARRAY && !(Z_TYPE_P(retval) == IS_OBJECT && (instanceof_function(Z_OBJCE_P(retval), zend_standard_class_def) || instanceof_function(Z_OBJCE_P(retval), php_phongo_document_ce)))) {
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE,
								   "Expected %s::%s() to return an array, stdClass, or %s, %s given",
								   ZSTR_VAL(ce->name),
								   BSON_SERIALIZE_FUNC_NAME,
								   ZSTR_VAL(php_phongo_document_ce->name),
								   zend_zval_type_name(retval));
			return false;
		}

		return true;
	}

	if (instanceof_function(ce, php_phongo_serializable_ce)) {
		// Instances of Serializable must return an array, stdClass, MongoDB\BSON\Document, or MongoDB\BSON\PackedArray
		if (
			Z_TYPE_P(retval) != IS_ARRAY && !(Z_TYPE_P(retval) == IS_OBJECT && (instanceof_function(Z_OBJCE_P(retval), zend_standard_class_def) || instanceof_function(Z_OBJCE_P(retval), php_phongo_document_ce) || instanceof_function(Z_OBJCE_P(retval), php_phongo_packedarray_ce)))) {
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE,
								   "Expected %s::%s() to return an array, stdClass, %s, or %s, %s given",
								   ZSTR_VAL(ce->name),
								   BSON_SERIALIZE_FUNC_NAME,
								   ZSTR_VAL(php_phongo_document_ce->name),
								   ZSTR_VAL(php_phongo_packedarray_ce->name),
								   zend_zval_type_name(retval));
			return false;
		}

		return true;
	}

	phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE,
						   "Expected to receive instance of %s, %s given",
						   ZSTR_VAL(php_phongo_serializable_ce->name),
						   ZSTR_VAL(ce->name));
	return false;
}

/* Appends the array or object argument to the BSON document.
 *
 * For instances of MongoDB\BSON\Document, raw BSON data is appended as document.
 * For instances of MongoDB\BSON\PackedArray, raw BSON data is appended as array.
 * For instances of MongoDB\BSON\Serializable, the return value of bsonSerialize()
 * will be appended as an embedded document.
 * Other MongoDB\BSON\Type instances will be appended as the appropriate BSON
 * type.
 * Other array or object values will be appended as an embedded document.
 */
static void php_phongo_bson_append_object(bson_t* bson, php_phongo_field_path* field_path, php_phongo_bson_flags_t flags, const char* key, size_t key_len, zval* object)
{
	if (Z_TYPE_P(object) == IS_OBJECT && instanceof_function(Z_OBJCE_P(object), php_phongo_cursorid_ce)) {
		phongo_bson_append_check(bson_append_int64(bson, key, key_len, Z_CURSORID_OBJ_P(object)->id), field_path);
		return;
	}

	if (Z_TYPE_P(object) == IS_OBJECT && instanceof_function(Z_OBJCE_P(object), php_phongo_type_ce)) {
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_document_ce)) {
			php_phongo_document_t* intern = Z_DOCUMENT_OBJ_P(object);
			phongo_bson_append_check(bson_append_document(bson, key, key_len, intern->bson), field_path);

			return;
		}

		if (instanceof_function(Z_OBJCE_P(object), php_phongo_packedarray_ce)) {
			php_phongo_packedarray_t* intern = Z_PACKEDARRAY_OBJ_P(object);
			phongo_bson_append_check(bson_append_array(bson, key, key_len, intern->bson), field_path);

			return;
		}

		if (instanceof_function(Z_OBJCE_P(object), php_phongo_serializable_ce)) {
			zval   obj_data;
			bson_t child;

			zend_call_method_with_0_params(Z_OBJ_P(object), NULL, NULL, BSON_SERIALIZE_FUNC_NAME, &obj_data);

			if (Z_ISUNDEF(obj_data)) {
				/* zend_call_method() failed or bsonSerialize() threw an
				 * exception. Either way, there is nothing else to do. */
				return;
			}

			if (!phongo_check_bson_serialize_return_type(&obj_data, Z_OBJCE_P(object))) {
				// Exception already thrown
				zval_ptr_dtor(&obj_data);
				return;
			}

			/* Persistable objects must always be serialized as BSON documents;
			 * otherwise, infer based on bsonSerialize()'s return value. */
			if (instanceof_function(Z_OBJCE_P(object), php_phongo_persistable_ce) || php_phongo_is_array_or_document(&obj_data) != IS_ARRAY) {
				if (instanceof_function(Z_OBJCE_P(object), php_phongo_persistable_ce) && !phongo_bson_length_is_valid(Z_OBJCE_P(object)->name->len, field_path, "class name")) {
					zval_ptr_dtor(&obj_data);
					return;
				}
				/* If the append fails, the child is not initialized and must not
				 * be used. */
				if (!bson_append_document_begin(bson, key, key_len, &child)) {
					phongo_bson_append_check(false, field_path);
					zval_ptr_dtor(&obj_data);
					return;
				}
				if (instanceof_function(Z_OBJCE_P(object), php_phongo_persistable_ce)) {
					phongo_bson_append_check(bson_append_binary(&child, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t*) Z_OBJCE_P(object)->name->val, Z_OBJCE_P(object)->name->len), field_path);
				}
				php_phongo_zval_to_bson_internal(&obj_data, field_path, flags, &child, NULL);
				phongo_bson_append_check(bson_append_document_end(bson, &child), field_path);
			} else {
				if (!bson_append_array_begin(bson, key, key_len, &child)) {
					phongo_bson_append_check(false, field_path);
					zval_ptr_dtor(&obj_data);
					return;
				}
				php_phongo_zval_to_bson_internal(&obj_data, field_path, flags, &child, NULL);
				phongo_bson_append_check(bson_append_array_end(bson, &child), field_path);
			}

			zval_ptr_dtor(&obj_data);
			return;
		}

		if (instanceof_function(Z_OBJCE_P(object), php_phongo_objectid_ce)) {
			bson_oid_t             oid;
			php_phongo_objectid_t* intern = Z_OBJECTID_OBJ_P(object);

			bson_oid_init_from_string(&oid, intern->oid);
			phongo_bson_append_check(bson_append_oid(bson, key, key_len, &oid), field_path);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_utcdatetime_ce)) {
			php_phongo_utcdatetime_t* intern = Z_UTCDATETIME_OBJ_P(object);

			phongo_bson_append_check(bson_append_date_time(bson, key, key_len, intern->milliseconds), field_path);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_binary_ce)) {
			php_phongo_binary_t* intern = Z_BINARY_OBJ_P(object);

			if (!phongo_bson_length_is_valid(intern->data_len, field_path, "binary data")) {
				return;
			}

			phongo_bson_append_check(bson_append_binary(bson, key, key_len, intern->type, (const uint8_t*) intern->data, (uint32_t) intern->data_len), field_path);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_decimal128_ce)) {
			php_phongo_decimal128_t* intern = Z_DECIMAL128_OBJ_P(object);

			phongo_bson_append_check(bson_append_decimal128(bson, key, key_len, &intern->decimal), field_path);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_int64_ce)) {
			php_phongo_int64_t* intern = Z_INT64_OBJ_P(object);

			phongo_bson_append_check(bson_append_int64(bson, key, key_len, intern->integer), field_path);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_regex_ce)) {
			php_phongo_regex_t* intern = Z_REGEX_OBJ_P(object);

			phongo_bson_append_check(bson_append_regex(bson, key, key_len, intern->pattern, intern->flags), field_path);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_javascript_ce)) {
			php_phongo_javascript_t* intern = Z_JAVASCRIPT_OBJ_P(object);

			if (intern->scope) {
				phongo_bson_append_check(bson_append_code_with_scope(bson, key, key_len, intern->code, intern->scope), field_path);
			} else {
				phongo_bson_append_check(bson_append_code(bson, key, key_len, intern->code), field_path);
			}
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_timestamp_ce)) {
			php_phongo_timestamp_t* intern = Z_TIMESTAMP_OBJ_P(object);

			phongo_bson_append_check(bson_append_timestamp(bson, key, key_len, intern->timestamp, intern->increment), field_path);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_maxkey_ce)) {
			phongo_bson_append_check(bson_append_maxkey(bson, key, key_len), field_path);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_minkey_ce)) {
			phongo_bson_append_check(bson_append_minkey(bson, key, key_len), field_path);
			return;
		}

		/* Deprecated types */
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_dbpointer_ce)) {
			bson_oid_t              oid;
			php_phongo_dbpointer_t* intern = Z_DBPOINTER_OBJ_P(object);

			bson_oid_init_from_string(&oid, intern->id);
			phongo_bson_append_check(bson_append_dbpointer(bson, key, key_len, intern->ref, &oid), field_path);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_symbol_ce)) {
			php_phongo_symbol_t* intern = Z_SYMBOL_OBJ_P(object);

			if (!phongo_bson_length_is_valid(intern->symbol_len, field_path, "symbol")) {
				return;
			}

			phongo_bson_append_check(bson_append_symbol(bson, key, key_len, intern->symbol, intern->symbol_len), field_path);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), php_phongo_undefined_ce)) {
			phongo_bson_append_check(bson_append_undefined(bson, key, key_len), field_path);
			return;
		}

		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Unexpected %s instance: %s", ZSTR_VAL(php_phongo_type_ce->name), ZSTR_VAL(Z_OBJCE_P(object)->name));
		return;
	}

	if (Z_TYPE_P(object) == IS_OBJECT && Z_OBJCE_P(object)->ce_flags & ZEND_ACC_ENUM) {
		if (Z_OBJCE_P(object)->enum_backing_type == IS_UNDEF) {
			char* path_string = php_phongo_field_path_as_string(field_path);
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Non-backed enum %s cannot be serialized for field path \"%s\"", ZSTR_VAL(Z_OBJCE_P(object)->name), path_string);
			efree(path_string);
			return;
		}

		php_phongo_bson_append(bson, field_path, flags, key, key_len, zend_enum_fetch_case_value(Z_OBJ_P(object)));
		return;
	}

	{
		bson_t child;

		/* If the append fails, the child is not initialized and must not be
		 * used. */
		if (!bson_append_document_begin(bson, key, key_len, &child)) {
			phongo_bson_append_check(false, field_path);
			return;
		}

		php_phongo_zval_to_bson_internal(object, field_path, flags, &child, NULL);
		phongo_bson_append_check(bson_append_document_end(bson, &child), field_path);
	}
}

/* Appends the zval argument to the BSON document. If the argument is an object,
 * or an array that should be serialized as an embedded document, this function
 * will defer to php_phongo_bson_append_object(). */
static void php_phongo_bson_append(bson_t* bson, php_phongo_field_path* field_path, php_phongo_bson_flags_t flags, const char* key, size_t key_len, zval* entry)
{
	/* A key longer than BSON_MAX_SIZE wraps a size_t/long length to a small
	 * non-negative int in the libbson append, silently truncating the field
	 * name to a prefix (SECBUG-4150). Reject before narrowing. The key is not
	 * included in the message or the field path, as it may be gigabytes. */
	if (key_len > PHONGO_BSON_MAX_LENGTH) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Expected key length to be <= %" PRId32 " bytes, %zu given", (int32_t) PHONGO_BSON_MAX_LENGTH, (size_t) key_len);
		return;
	}

	php_phongo_field_path_write_item_at_current_level(field_path, key);

try_again:
	switch (Z_TYPE_P(entry)) {
		case IS_NULL:
			phongo_bson_append_check(bson_append_null(bson, key, key_len), field_path);
			break;
		case IS_TRUE:
			phongo_bson_append_check(bson_append_bool(bson, key, key_len, true), field_path);
			break;

		case IS_FALSE:
			phongo_bson_append_check(bson_append_bool(bson, key, key_len, false), field_path);
			break;

		case IS_LONG:
			phongo_bson_append_check(BSON_APPEND_INT(bson, key, key_len, Z_LVAL_P(entry)), field_path);
			break;

		case IS_DOUBLE:
			phongo_bson_append_check(bson_append_double(bson, key, key_len, Z_DVAL_P(entry)), field_path);
			break;

		case IS_STRING:
			if (!phongo_bson_length_is_valid(Z_STRLEN_P(entry), field_path, "string")) {
				break;
			}

			if (bson_utf8_validate(Z_STRVAL_P(entry), Z_STRLEN_P(entry), true)) {
				phongo_bson_append_check(bson_append_utf8(bson, key, key_len, Z_STRVAL_P(entry), Z_STRLEN_P(entry)), field_path);
			} else {
				char* path_string = php_phongo_field_path_as_string(field_path);
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected invalid UTF-8 for field path \"%s\": %s", path_string, Z_STRVAL_P(entry));
				efree(path_string);
			}
			break;

		case IS_ARRAY:
			if (php_phongo_is_array_or_document(entry) == IS_ARRAY) {
				bson_t     child;
				HashTable* tmp_ht = HASH_OF(entry);

				if (!php_phongo_zend_hash_apply_protection_begin(tmp_ht)) {
					char* path_string = php_phongo_field_path_as_string(field_path);
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected recursion for field path \"%s\"", path_string);
					efree(path_string);
					break;
				}

				if (!php_phongo_field_path_push(field_path, NULL, PHONGO_FIELD_PATH_ITEM_ARRAY)) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Nesting level too deep");
					php_phongo_zend_hash_apply_protection_end(tmp_ht);
					break;
				}

				/* If the append fails, the child is not initialized and must
				 * not be used. */
				if (!bson_append_array_begin(bson, key, key_len, &child)) {
					phongo_bson_append_check(false, field_path);
					php_phongo_field_path_pop(field_path);
					php_phongo_zend_hash_apply_protection_end(tmp_ht);
					break;
				}

				php_phongo_zval_to_bson_internal(entry, field_path, flags, &child, NULL);
				php_phongo_field_path_pop(field_path);
				phongo_bson_append_check(bson_append_array_end(bson, &child), field_path);

				php_phongo_zend_hash_apply_protection_end(tmp_ht);
				break;
			}
			PHONGO_BREAK_INTENTIONALLY_MISSING

		case IS_OBJECT: {
			HashTable* tmp_ht = HASH_OF(entry);

			if (!php_phongo_zend_hash_apply_protection_begin(tmp_ht)) {
				char* path_string = php_phongo_field_path_as_string(field_path);
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected recursion for field path \"%s\"", path_string);
				efree(path_string);
				break;
			}

			if (Z_TYPE_P(entry) == IS_OBJECT && instanceof_function(Z_OBJCE_P(entry), php_phongo_packedarray_ce)) {
				if (!php_phongo_field_path_push(field_path, NULL, PHONGO_FIELD_PATH_ITEM_ARRAY)) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Nesting level too deep");
					php_phongo_zend_hash_apply_protection_end(tmp_ht);
					break;
				}
			} else {
				if (!php_phongo_field_path_push(field_path, NULL, PHONGO_FIELD_PATH_ITEM_DOCUMENT)) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Nesting level too deep");
					php_phongo_zend_hash_apply_protection_end(tmp_ht);
					break;
				}
			}

			php_phongo_bson_append_object(bson, field_path, flags, key, key_len, entry);
			php_phongo_field_path_pop(field_path);

			php_phongo_zend_hash_apply_protection_end(tmp_ht);
			break;
		}

		case IS_REFERENCE:
			ZVAL_DEREF(entry);
			goto try_again;

		default: {
			char* path_string = php_phongo_field_path_as_string(field_path);
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected unsupported PHP type for field path \"%s\": %d (%s)", path_string, Z_TYPE_P(entry), zend_get_type_by_const(Z_TYPE_P(entry)));
			efree(path_string);
		}
	}
}

/* This is based on bson_copy_to_excluding_noinit() and is necessary because
 * bson_copy_to() cannot be used with a bson_t allocated with bson_new(). */
static void phongo_bson_copy_to_noinit(const bson_t* src, bson_t* dst)
{
	bson_iter_t iter;

	if (bson_iter_init(&iter, src)) {
		while (bson_iter_next(&iter)) {
			if (!bson_append_iter(dst, NULL, 0, &iter)) {
				/* The source is a valid bson_t, so this only happens when the
				 * destination would exceed the maximum BSON size. */
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Error copying \"%s\" field from source document", bson_iter_key(&iter));
				return;
			}
		}
	}
}

static void php_phongo_zval_to_bson_internal(zval* data, php_phongo_field_path* field_path, php_phongo_bson_flags_t flags, bson_t* bson, bson_t** bson_out)
{
	HashTable* ht_data = NULL;
	zval       obj_data;

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
			/* Short-circuit MongoDB\BSON\Document and MongoDB\BSON\PackedArray instances - copy the data */
			if (instanceof_function(Z_OBJCE_P(data), php_phongo_document_ce)) {
				php_phongo_document_t* intern = Z_DOCUMENT_OBJ_P(data);
				bson_iter_t            iter;

				phongo_bson_copy_to_noinit(intern->bson, bson);

				if (EG(exception)) {
					goto cleanup;
				}

				// Check if the document instance already has an _id field
				if (flags & PHONGO_BSON_ADD_ID && bson_iter_init_find(&iter, bson, "_id")) {
					flags &= ~PHONGO_BSON_ADD_ID;
				}

				goto done;
			}

			if (instanceof_function(Z_OBJCE_P(data), php_phongo_packedarray_ce)) {
				/* If we are at the root-level, PackedArray instances should be
				 * prohibited unless PHONGO_BSON_ALLOW_ROOT_ARRAY is set. */
				bool is_root_level = (field_path->size == 0);

				if (is_root_level && !(flags & PHONGO_BSON_ALLOW_ROOT_ARRAY)) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s cannot be serialized as a root document", ZSTR_VAL(Z_OBJCE_P(data)->name));
					return;
				}

				php_phongo_packedarray_t* intern = Z_PACKEDARRAY_OBJ_P(data);

				phongo_bson_copy_to_noinit(intern->bson, bson);

				if (EG(exception)) {
					goto cleanup;
				}

				goto done;
			}

			/* For any MongoDB\BSON\Serializable, invoke the bsonSerialize method
			 * and work with the result. */
			if (instanceof_function(Z_OBJCE_P(data), php_phongo_serializable_ce)) {
				zend_call_method_with_0_params(Z_OBJ_P(data), NULL, NULL, BSON_SERIALIZE_FUNC_NAME, &obj_data);

				if (Z_ISUNDEF(obj_data)) {
					/* zend_call_method() failed or bsonSerialize() threw an
					 * exception. Either way, there is nothing else to do. */
					return;
				}

				if (!phongo_check_bson_serialize_return_type(&obj_data, Z_OBJCE_P(data))) {
					// Exception already thrown
					goto cleanup;
				}

				if (instanceof_function(Z_OBJCE_P(data), php_phongo_persistable_ce)) {
					if (!phongo_bson_length_is_valid(Z_OBJCE_P(data)->name->len, field_path, "class name")) {
						goto cleanup;
					}
					if (!bson_append_binary(bson, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t*) Z_OBJCE_P(data)->name->val, Z_OBJCE_P(data)->name->len)) {
						phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not append \"%s\" field to encoded document. The document may exceed the maximum BSON size of %zu bytes.", PHONGO_ODM_FIELD_NAME, (size_t) BSON_MAX_SIZE);

						goto cleanup;
					}

					/* Ensure that we ignore an existing key with the same name
					 * if one exists in the bsonSerialize() return value. */
					skip_odm_field = true;
				}

				// If bsonSerialize() returns a BSON document or packedArray instance, recurse to copy data over directly
				if (Z_TYPE(obj_data) == IS_OBJECT && (instanceof_function(Z_OBJCE(obj_data), php_phongo_document_ce) || instanceof_function(Z_OBJCE(obj_data), php_phongo_packedarray_ce))) {
					php_phongo_zval_to_bson_internal(&obj_data, field_path, flags, bson, bson_out);

					goto done;
				}

				ht_data = HASH_OF(&obj_data);

				break;
			}

			/* For the error handling that follows, we can safely assume that we
			 * are at the root level, since php_phongo_bson_append_object would
			 * have already been called for a non-root level. */
			if (Z_OBJCE_P(data)->ce_flags & ZEND_ACC_ENUM) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Enum %s cannot be serialized as a root element", ZSTR_VAL(Z_OBJCE_P(data)->name));
				return;
			}

			if (instanceof_function(Z_OBJCE_P(data), php_phongo_type_ce)) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s instance %s cannot be serialized as a root element", ZSTR_VAL(php_phongo_type_ce->name), ZSTR_VAL(Z_OBJCE_P(data)->name));
				return;
			}

			ht_data                 = Z_OBJ_HT_P(data)->get_properties(Z_OBJ_P(data));
			ht_data_from_properties = true;
			break;

		case IS_ARRAY:
			ht_data = HASH_OF(data);
			break;

		default:
			return;
	}

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
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "BSON keys cannot contain null bytes. Unexpected null byte after \"%s\".", ZSTR_VAL(string_key));

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

			php_phongo_bson_append(bson, field_path, flags & ~PHONGO_BSON_ADD_ID, ZSTR_VAL(string_key), strlen(ZSTR_VAL(string_key)), value);

			zend_string_release(string_key);

			/* Stop encoding as soon as a field could not be appended.
			 * Continuing would only throw again for each remaining field. */
			if (EG(exception)) {
				goto cleanup;
			}
		}
		ZEND_HASH_FOREACH_END();
	}

done:
	if (flags & PHONGO_BSON_ADD_ID) {
		bson_oid_t oid;

		bson_oid_init(&oid, NULL);

		if (!bson_append_oid(bson, "_id", strlen("_id"), &oid)) {
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not append \"_id\" field to encoded document. The document may exceed the maximum BSON size of %zu bytes.", (size_t) BSON_MAX_SIZE);

			goto cleanup;
		}
	}

	if (flags & PHONGO_BSON_RETURN_ID && bson_out) {
		bson_iter_t iter;

		*bson_out = bson_new();

		if (bson_iter_init_find(&iter, bson, "_id") && !bson_append_iter(*bson_out, NULL, 0, &iter)) {
			/* This should not be able to happen since we are copying from
			 * within a valid bson_t. */
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Error copying \"_id\" field from encoded document");

			goto cleanup;
		}
	}

cleanup:
	if (!Z_ISUNDEF(obj_data)) {
		zval_ptr_dtor(&obj_data);
	}
}

/* Converts the array or object argument to a BSON document. If the object is an
 * instance of MongoDB\BSON\Serializable, the return value of bsonSerialize()
 * will be used. */
void php_phongo_zval_to_bson(zval* data, php_phongo_bson_flags_t flags, bson_t* bson, bson_t** bson_out)
{
	php_phongo_field_path* field_path = php_phongo_field_path_alloc(false);

	php_phongo_zval_to_bson_internal(data, field_path, flags, bson, bson_out);

	php_phongo_field_path_free(field_path);
}

static bool phongo_zval_to_bson_value_ex(zval* data, php_phongo_bson_flags_t flags, bson_value_t* value)
{
	bool        found = false;
	bson_iter_t iter;
	bson_t      bson = BSON_INITIALIZER;
	zval        data_object;

	array_init_size(&data_object, 1);
	add_assoc_zval(&data_object, "data", data);

	Z_TRY_ADDREF_P(data);

	php_phongo_zval_to_bson(&data_object, flags, &bson, NULL);

	/* If the encoder threw, the value was never appended and must be left
	 * untouched so the caller does not read uninitialized memory. */
	if (!EG(exception)) {
		if (bson_iter_init_find(&iter, &bson, "data")) {
			bson_value_copy(bson_iter_value(&iter), value);
			found = true;
		} else {
			/* Callers rely on an exception being thrown, so make sure one is
			 * pending even if the encoder failed without throwing. */
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not convert value to BSON");
		}
	}

	bson_destroy(&bson);
	zval_ptr_dtor(&data_object);

	return found;
}

/* Converts the argument to a bson_value_t. If the object is an instance of
 * MongoDB\BSON\Serializable, the return value of bsonSerialize() will be
 * used. It is the caller's responsibility to call bson_value_destroy.
 *
 * On success, the zval will be populated and true will be returned. On error,
 * an exception will have been thrown and false will be returned. */
bool phongo_zval_to_bson_value(zval* data, bson_value_t* value)
{
	zend_long lvalue;

	ZVAL_DEREF(data);

	switch (Z_TYPE_P(data)) {
		case IS_UNDEF:
		case IS_NULL:
			value->value_type = BSON_TYPE_NULL;
			return true;

		case IS_FALSE:
			value->value_type   = BSON_TYPE_BOOL;
			value->value.v_bool = false;
			return true;

		case IS_TRUE:
			value->value_type   = BSON_TYPE_BOOL;
			value->value.v_bool = true;
			return true;

		case IS_LONG:
			lvalue = Z_LVAL_P(data);

			if (lvalue > INT32_MAX || lvalue < INT32_MIN) {
				value->value_type    = BSON_TYPE_INT64;
				value->value.v_int64 = lvalue;
			} else {
				value->value_type    = BSON_TYPE_INT32;
				value->value.v_int32 = (int32_t) lvalue;
			}

			return true;

		case IS_DOUBLE:
			value->value_type     = BSON_TYPE_DOUBLE;
			value->value.v_double = Z_DVAL_P(data);
			return true;

		case IS_STRING:
			if (Z_STRLEN_P(data) > PHONGO_BSON_MAX_LENGTH) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Expected string to be <= %" PRId32 " bytes, %zu given", (int32_t) PHONGO_BSON_MAX_LENGTH, Z_STRLEN_P(data));
				return false;
			}

			value->value_type       = BSON_TYPE_UTF8;
			value->value.v_utf8.len = Z_STRLEN_P(data);

			/* Duplicate string as bson_value_t is expected to own values */
			value->value.v_utf8.str = bson_malloc(value->value.v_utf8.len + 1);
			memcpy(value->value.v_utf8.str, Z_STRVAL_P(data), value->value.v_utf8.len);
			value->value.v_utf8.str[value->value.v_utf8.len] = '\0';
			return true;

		case IS_ARRAY:
		case IS_OBJECT:
			/* Use php_phongo_zval_to_bson internally to convert arrays and documents */
			return phongo_zval_to_bson_value_ex(data, PHONGO_BSON_NONE, value);
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Unsupported type %s", zend_zval_type_name(data));
	return false;
}
