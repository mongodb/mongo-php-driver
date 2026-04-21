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

#include "phongo.h"
#include "phongo_bson.h"
#include "phongo_bson_encode.h"
#include "phongo_compat.h"
#include "phongo_error.h"

#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "PHONGO-BSON"

#if SIZEOF_ZEND_LONG == 8
#define BSON_APPEND_INT(b, key, keylen, val)    \
	if (val > INT32_MAX || val < INT32_MIN) {   \
		bson_append_int64(b, key, keylen, val); \
	} else {                                    \
		bson_append_int32(b, key, keylen, val); \
	}
#elif SIZEOF_ZEND_LONG == 4
#define BSON_APPEND_INT(b, key, keylen, val) \
	bson_append_int32(b, key, keylen, val)
#else
#error Unsupported architecture (integers are neither 32-bit nor 64-bit)
#endif

/* Forwards declarations */
static void phongo_bson_append(bson_t* bson, phongo_field_path* field_path, phongo_bson_flags_t flags, const char* key, long key_len, zval* entry);
static void phongo_zval_to_bson_internal(zval* data, phongo_field_path* field_path, phongo_bson_flags_t flags, bson_t* bson, bson_t** bson_out);

/* Determines whether the argument should be serialized as a BSON array or
 * document. IS_ARRAY is returned if the argument's keys are a sequence of
 * integers starting at zero; otherwise, IS_OBJECT is returned. */
static int phongo_is_array_or_document(zval* val)
{
	HashTable* ht_data = HASH_OF(val);

	if (Z_TYPE_P(val) != IS_ARRAY) {
		if (Z_TYPE_P(val) == IS_OBJECT && instanceof_function(Z_OBJCE_P(val), phongo_packedarray_ce)) {
			return IS_ARRAY;
		}

		return IS_OBJECT;
	}

	if (!zend_array_is_list(ht_data)) {
		return IS_OBJECT;
	}

	return IS_ARRAY;
}

static bool phongo_bson_encode_serializable(zval* object, zval* out_data)
{
	ZVAL_UNDEF(out_data);
	zend_call_method_with_0_params(Z_OBJ_P(object), NULL, NULL, BSON_SERIALIZE_FUNC_NAME, out_data);

	if (Z_ISUNDEF_P(out_data)) {
		/* zend_call_method() failed or bsonSerialize() threw an
		 * exception. Either way, there is nothing else to do. */
		return false;
	}

	return true;
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
static void phongo_bson_append_object(bson_t* bson, phongo_field_path* field_path, phongo_bson_flags_t flags, const char* key, long key_len, zval* object)
{
	if (Z_TYPE_P(object) == IS_OBJECT && instanceof_function(Z_OBJCE_P(object), phongo_type_ce)) {
		if (instanceof_function(Z_OBJCE_P(object), phongo_document_ce)) {
			PHONGO_INTERN_FROM_ZVAL(document, object);
			bson_append_document(bson, key, key_len, intern->bson);

			return;
		}

		if (instanceof_function(Z_OBJCE_P(object), phongo_packedarray_ce)) {
			PHONGO_INTERN_FROM_ZVAL(packedarray, object);
			bson_append_array(bson, key, key_len, intern->bson);

			return;
		}

		if (instanceof_function(Z_OBJCE_P(object), phongo_serializable_ce)) {
			zval   obj_data;
			bson_t child;

			if (!phongo_bson_encode_serializable(object, &obj_data)) {
				// Exception already thrown
				return;
			}

			/* Persistable objects must always be serialized as BSON documents;
			 * otherwise, infer based on bsonSerialize()'s return value. */
			if (instanceof_function(Z_OBJCE_P(object), phongo_persistable_ce) || phongo_is_array_or_document(&obj_data) != IS_ARRAY) {
				bson_append_document_begin(bson, key, key_len, &child);
				if (instanceof_function(Z_OBJCE_P(object), phongo_persistable_ce)) {
					bson_append_binary(&child, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t*) Z_OBJCE_P(object)->name->val, Z_OBJCE_P(object)->name->len);
				}
				phongo_zval_to_bson_internal(&obj_data, field_path, flags, &child, NULL);
				bson_append_document_end(bson, &child);
			} else {
				bson_append_array_unsafe_begin(bson, key, key_len, &child);
				phongo_zval_to_bson_internal(&obj_data, field_path, flags, &child, NULL);
				bson_append_array_end(bson, &child);
			}

			zval_ptr_dtor(&obj_data);
			return;
		}

		if (instanceof_function(Z_OBJCE_P(object), phongo_objectid_ce)) {
			PHONGO_INTERN_FROM_ZVAL(objectid, object);
			bson_oid_t oid;

			bson_oid_init_from_string(&oid, intern->oid);
			bson_append_oid(bson, key, key_len, &oid);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), phongo_utcdatetime_ce)) {
			PHONGO_INTERN_FROM_ZVAL(utcdatetime, object);

			bson_append_date_time(bson, key, key_len, intern->milliseconds);
			return;
		}
		// TODO: confirm that this handles binary vector
		if (instanceof_function(Z_OBJCE_P(object), phongo_binary_ce)) {
			PHONGO_INTERN_FROM_ZVAL(binary, object);

			bson_append_binary(bson, key, key_len, intern->type, (const uint8_t*) intern->data, (uint32_t) intern->data_len);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), phongo_decimal128_ce)) {
			PHONGO_INTERN_FROM_ZVAL(decimal128, object);

			bson_append_decimal128(bson, key, key_len, &intern->decimal);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), phongo_int64_ce)) {
			PHONGO_INTERN_FROM_ZVAL(int64, object);

			bson_append_int64(bson, key, key_len, intern->integer);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), phongo_regex_ce)) {
			PHONGO_INTERN_FROM_ZVAL(regex, object);

			bson_append_regex(bson, key, key_len, intern->pattern, intern->flags);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), phongo_javascript_ce)) {
			PHONGO_INTERN_FROM_ZVAL(javascript, object);

			if (intern->scope) {
				bson_append_code_with_scope(bson, key, key_len, intern->code, intern->scope);
			} else {
				bson_append_code(bson, key, key_len, intern->code);
			}
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), phongo_timestamp_ce)) {
			PHONGO_INTERN_FROM_ZVAL(timestamp, object);

			bson_append_timestamp(bson, key, key_len, intern->timestamp, intern->increment);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), phongo_maxkey_ce)) {
			bson_append_maxkey(bson, key, key_len);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), phongo_minkey_ce)) {
			bson_append_minkey(bson, key, key_len);
			return;
		}

		/* Deprecated types */
		if (instanceof_function(Z_OBJCE_P(object), phongo_dbpointer_ce)) {
			PHONGO_INTERN_FROM_ZVAL(dbpointer, object);
			bson_oid_t oid;

			bson_oid_init_from_string(&oid, intern->id);
			bson_append_dbpointer(bson, key, key_len, intern->ref, &oid);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), phongo_symbol_ce)) {
			PHONGO_INTERN_FROM_ZVAL(symbol, object);

			bson_append_symbol(bson, key, key_len, intern->symbol, intern->symbol_len);
			return;
		}
		if (instanceof_function(Z_OBJCE_P(object), phongo_undefined_ce)) {
			bson_append_undefined(bson, key, key_len);
			return;
		}

		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Unexpected %s instance: %s", ZSTR_VAL(phongo_type_ce->name), ZSTR_VAL(Z_OBJCE_P(object)->name));
		return;
	}

	if (Z_TYPE_P(object) == IS_OBJECT && Z_OBJCE_P(object)->ce_flags & ZEND_ACC_ENUM) {
		if (Z_OBJCE_P(object)->enum_backing_type == IS_UNDEF) {
			char* path_string = phongo_field_path_as_string(field_path);
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Non-backed enum %s cannot be serialized for field path \"%s\"", ZSTR_VAL(Z_OBJCE_P(object)->name), path_string);
			efree(path_string);
			return;
		}

		phongo_bson_append(bson, field_path, flags, key, key_len, zend_enum_fetch_case_value(Z_OBJ_P(object)));
		return;
	}

	{
		bson_t child;

		bson_append_document_begin(bson, key, key_len, &child);
		phongo_zval_to_bson_internal(object, field_path, flags, &child, NULL);
		bson_append_document_end(bson, &child);
	}
}

/* Appends the zval argument to the BSON document. If the argument is an object,
 * or an array that should be serialized as an embedded document, this function
 * will defer to phongo_bson_append_object(). */
static void phongo_bson_append(bson_t* bson, phongo_field_path* field_path, phongo_bson_flags_t flags, const char* key, long key_len, zval* entry)
{
	phongo_field_path_write_item_at_current_level(field_path, key);

try_again:
	switch (Z_TYPE_P(entry)) {
		case IS_NULL:
			bson_append_null(bson, key, key_len);
			break;
		case IS_TRUE:
			bson_append_bool(bson, key, key_len, true);
			break;

		case IS_FALSE:
			bson_append_bool(bson, key, key_len, false);
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
				char* path_string = phongo_field_path_as_string(field_path);
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected invalid UTF-8 for field path \"%s\": %s", path_string, Z_STRVAL_P(entry));
				efree(path_string);
			}
			break;

		case IS_ARRAY:
			if (phongo_is_array_or_document(entry) == IS_ARRAY) {
				bson_t     child;
				HashTable* tmp_ht = HASH_OF(entry);

				if (!phongo_zend_hash_apply_protection_begin(tmp_ht)) {
					char* path_string = phongo_field_path_as_string(field_path);
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected recursion for field path \"%s\"", path_string);
					efree(path_string);
					break;
				}

				if (!phongo_field_path_push(field_path, NULL, PHONGO_FIELD_PATH_ITEM_ARRAY)) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Nesting level too deep");
					phongo_zend_hash_apply_protection_end(tmp_ht);
					break;
				}

				bson_append_array_unsafe_begin(bson, key, key_len, &child);
				phongo_zval_to_bson_internal(entry, field_path, flags, &child, NULL);
				phongo_field_path_pop(field_path);
				bson_append_array_end(bson, &child);

				phongo_zend_hash_apply_protection_end(tmp_ht);
				break;
			}
			PHONGO_BREAK_INTENTIONALLY_MISSING

		case IS_OBJECT: {
			HashTable* tmp_ht = HASH_OF(entry);

			if (!phongo_zend_hash_apply_protection_begin(tmp_ht)) {
				char* path_string = phongo_field_path_as_string(field_path);
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected recursion for field path \"%s\"", path_string);
				efree(path_string);
				break;
			}

			if (Z_TYPE_P(entry) == IS_OBJECT && instanceof_function(Z_OBJCE_P(entry), phongo_packedarray_ce)) {
				if (!phongo_field_path_push(field_path, NULL, PHONGO_FIELD_PATH_ITEM_ARRAY)) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Nesting level too deep");
					phongo_zend_hash_apply_protection_end(tmp_ht);
					break;
				}
			} else {
				if (!phongo_field_path_push(field_path, NULL, PHONGO_FIELD_PATH_ITEM_DOCUMENT)) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Nesting level too deep");
					phongo_zend_hash_apply_protection_end(tmp_ht);
					break;
				}
			}

			phongo_bson_append_object(bson, field_path, flags, key, key_len, entry);
			phongo_field_path_pop(field_path);

			phongo_zend_hash_apply_protection_end(tmp_ht);
			break;
		}

		case IS_REFERENCE:
			ZVAL_DEREF(entry);
			goto try_again;

		default: {
			char* path_string = phongo_field_path_as_string(field_path);
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
				/* This should not be able to happen since we are copying from
				 * within a valid bson_t. */
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Error copying \"%s\" field from source document", bson_iter_key(&iter));
				return;
			}
		}
	}
}

static void phongo_zval_to_bson_internal(zval* data, phongo_field_path* field_path, phongo_bson_flags_t flags, bson_t* bson, bson_t** bson_out)
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
			if (instanceof_function(Z_OBJCE_P(data), phongo_document_ce)) {
				PHONGO_INTERN_FROM_ZVAL(document, data);
				bson_iter_t iter;

				phongo_bson_copy_to_noinit(intern->bson, bson);

				// Check if the document instance already has an _id field
				if (flags & PHONGO_BSON_ADD_ID && bson_iter_init_find(&iter, bson, "_id")) {
					flags &= ~PHONGO_BSON_ADD_ID;
				}

				goto done;
			}

			if (instanceof_function(Z_OBJCE_P(data), phongo_packedarray_ce)) {
				/* If we are at the root-level, PackedArray instances should be
				 * prohibited unless PHONGO_BSON_ALLOW_ROOT_ARRAY is set. */
				bool is_root_level = (field_path->size == 0);

				if (is_root_level && !(flags & PHONGO_BSON_ALLOW_ROOT_ARRAY)) {
					phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s cannot be serialized as a root document", ZSTR_VAL(Z_OBJCE_P(data)->name));
					return;
				}

				PHONGO_INTERN_FROM_ZVAL(packedarray, data);

				phongo_bson_copy_to_noinit(intern->bson, bson);

				goto done;
			}

			/* For any MongoDB\BSON\Serializable, invoke the bsonSerialize method
			 * and work with the result. */
			if (instanceof_function(Z_OBJCE_P(data), phongo_serializable_ce)) {
				if (!phongo_bson_encode_serializable(data, &obj_data)) {
					// Exception already thrown
					return;
				}

				if (instanceof_function(Z_OBJCE_P(data), phongo_persistable_ce)) {
					bson_append_binary(bson, PHONGO_ODM_FIELD_NAME, -1, 0x80, (const uint8_t*) Z_OBJCE_P(data)->name->val, Z_OBJCE_P(data)->name->len);
					/* Ensure that we ignore an existing key with the same name
					 * if one exists in the bsonSerialize() return value. */
					skip_odm_field = true;
				}

				// If bsonSerialize() returns a BSON document or packedArray instance, recurse to copy data over directly
				if (Z_TYPE(obj_data) == IS_OBJECT && (instanceof_function(Z_OBJCE(obj_data), phongo_document_ce) || instanceof_function(Z_OBJCE(obj_data), phongo_packedarray_ce))) {
					phongo_zval_to_bson_internal(&obj_data, field_path, flags, bson, bson_out);

					goto done;
				}

				ht_data = HASH_OF(&obj_data);

				break;
			}

			/* For the error handling that follows, we can safely assume that we
			 * are at the root level, since phongo_bson_append_object would
			 * have already been called for a non-root level. */
			if (Z_OBJCE_P(data)->ce_flags & ZEND_ACC_ENUM) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Enum %s cannot be serialized as a root element", ZSTR_VAL(Z_OBJCE_P(data)->name));
				return;
			}

			if (instanceof_function(Z_OBJCE_P(data), phongo_type_ce)) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s instance %s cannot be serialized as a root element", ZSTR_VAL(phongo_type_ce->name), ZSTR_VAL(Z_OBJCE_P(data)->name));
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

			phongo_bson_append(bson, field_path, flags & ~PHONGO_BSON_ADD_ID, ZSTR_VAL(string_key), strlen(ZSTR_VAL(string_key)), value);

			zend_string_release(string_key);
		}
		ZEND_HASH_FOREACH_END();
	}

done:
	if (flags & PHONGO_BSON_ADD_ID) {
		bson_oid_t oid;

		bson_oid_init(&oid, NULL);
		bson_append_oid(bson, ZEND_STRL("_id"), &oid);
	}

	if (flags & PHONGO_BSON_RETURN_ID && bson_out) {
		bson_iter_t iter;

		/* This should not be able to happen since we are copying from
		 * within a valid bson_t. */
		if (!bson_iter_init_find(&iter, bson, "_id")) {
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Error copying \"_id\" field from encoded document");

			goto cleanup;
		}

		*bson_out = bson_new();

		if (!bson_append_iter(*bson_out, NULL, 0, &iter)) {
			/* This should not be able to happen since we are copying from
			 * within a valid bson_t. */
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Error copying \"_id\" field from encoded document");
			bson_clear(bson_out);

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
void phongo_zval_to_bson(zval* data, phongo_bson_flags_t flags, bson_t* bson, bson_t** bson_out)
{
	phongo_field_path* field_path = phongo_field_path_alloc(false);

	phongo_zval_to_bson_internal(data, field_path, flags, bson, bson_out);

	phongo_field_path_free(field_path);
}

static void phongo_zval_to_bson_value_ex(zval* data, phongo_bson_flags_t flags, bson_value_t* value)
{
	bson_iter_t iter;
	bson_t      bson = BSON_INITIALIZER;
	zval        data_object;

	array_init_size(&data_object, 1);
	add_assoc_zval(&data_object, "data", data);

	Z_TRY_ADDREF_P(data);

	phongo_zval_to_bson(&data_object, flags, &bson, NULL);

	if (bson_iter_init_find(&iter, &bson, "data")) {
		bson_value_copy(bson_iter_value(&iter), value);
	}

	bson_destroy(&bson);
	zval_ptr_dtor(&data_object);
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
			value->value_type       = BSON_TYPE_UTF8;
			value->value.v_utf8.len = Z_STRLEN_P(data);

			/* Duplicate string as bson_value_t is expected to own values */
			value->value.v_utf8.str = bson_malloc(value->value.v_utf8.len + 1);
			memcpy(value->value.v_utf8.str, Z_STRVAL_P(data), value->value.v_utf8.len);
			value->value.v_utf8.str[value->value.v_utf8.len] = '\0';
			return true;

		case IS_ARRAY:
		case IS_OBJECT:
			/* Use phongo_zval_to_bson internally to convert arrays and documents */
			phongo_zval_to_bson_value_ex(data, PHONGO_BSON_NONE, value);
			return true;
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Unsupported type %s", zend_zval_type_name(data));
	return false;
}
