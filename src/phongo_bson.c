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
#if PHP_VERSION_ID >= 80100
#include <Zend/zend_enum.h>
#endif
#include <Zend/zend_interfaces.h>

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_bson.h"
#include "phongo_error.h"

#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "PHONGO-BSON"

#define PHONGO_BSON_STATE_ZCHILD(state) (&((php_phongo_bson_state*) (state))->zchild)

#define PHONGO_FIELD_PATH_EXPANSION 8

/* Forward declarations */
static bool php_phongo_bson_visit_document(const bson_iter_t* iter ARG_UNUSED, const char* key, const bson_t* v_document, void* data);
static bool php_phongo_bson_visit_array(const bson_iter_t* iter ARG_UNUSED, const char* key, const bson_t* v_array, void* data);

static inline bool phongo_is_class_instantiatable(const zend_class_entry* ce)
{
	if (ce->ce_flags & (ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_IMPLICIT_ABSTRACT_CLASS | ZEND_ACC_INTERFACE | ZEND_ACC_TRAIT)) {
		return false;
	}

#if PHP_VERSION_ID >= 80100
	if (ce->ce_flags & ZEND_ACC_ENUM) {
		return false;
	}
#endif /* PHP_VERSION_ID < 80100 */

	return true;
}

/* Path builder */
char* php_phongo_field_path_as_string(php_phongo_field_path* field_path)
{
	size_t length = 1; /* NULL character */
	size_t i;
	char*  path;
	char*  ptr;

	if (!field_path) {
		return estrdup("");
	}
	if (!field_path->elements) {
		return estrdup("");
	}

	for (i = 0; i <= field_path->size; i++) {
		if (!field_path->elements[i]) {
			continue;
		}
		length += (1 + strlen(field_path->elements[i]));
	}

	path = emalloc(length);
	ptr  = path;

	for (i = 0; i <= field_path->size; i++) {
		if (!field_path->elements[i]) {
			continue;
		}
		strcpy(ptr, field_path->elements[i]);
		ptr += strlen(field_path->elements[i]);
		ptr[0] = '.';
		ptr++;
	}
	ptr[-1] = '\0';

	return path;
}

php_phongo_field_path* php_phongo_field_path_alloc(bool owns_elements)
{
	php_phongo_field_path* tmp = ecalloc(1, sizeof(php_phongo_field_path));
	tmp->ref_count             = 1;
	tmp->owns_elements         = owns_elements;

	return tmp;
}

void php_phongo_field_path_free(php_phongo_field_path* field_path)
{
	if (field_path->owns_elements) {
		size_t i;

		for (i = 0; i < field_path->size; i++) {
			efree(field_path->elements[i]);
		}
	}
	if (field_path->elements) {
		efree(field_path->elements);
	}
	if (field_path->element_types) {
		efree(field_path->element_types);
	}
	efree(field_path);
}

static void php_phongo_field_path_ensure_allocation(php_phongo_field_path* field_path, size_t level)
{
	if (level >= field_path->allocated_size) {
		size_t i;

		field_path->allocated_size = field_path->size + PHONGO_FIELD_PATH_EXPANSION;
		field_path->elements       = erealloc(field_path->elements, sizeof(char*) * field_path->allocated_size);
		field_path->element_types  = erealloc(field_path->element_types, sizeof(php_phongo_bson_field_path_item_types) * field_path->allocated_size);

		for (i = level; i < field_path->allocated_size; i++) {
			field_path->elements[i]      = NULL;
			field_path->element_types[i] = PHONGO_FIELD_PATH_ITEM_NONE;
		}
	}
}

void php_phongo_field_path_write_item_at_current_level(php_phongo_field_path* field_path, const char* element)
{
	php_phongo_field_path_ensure_allocation(field_path, field_path->size);

	if (field_path->owns_elements) {
		field_path->elements[field_path->size] = estrdup(element);
	} else {
		field_path->elements[field_path->size] = (char*) element;
	}
}

void php_phongo_field_path_write_type_at_current_level(php_phongo_field_path* field_path, php_phongo_bson_field_path_item_types element_type)
{
	php_phongo_field_path_ensure_allocation(field_path, field_path->size);

	field_path->element_types[field_path->size] = element_type;
}

bool php_phongo_field_path_push(php_phongo_field_path* field_path, const char* element, php_phongo_bson_field_path_item_types element_type)
{
	php_phongo_field_path_write_item_at_current_level(field_path, element);
	php_phongo_field_path_write_type_at_current_level(field_path, element_type);

	field_path->size++;

	return true;
}

bool php_phongo_field_path_pop(php_phongo_field_path* field_path)
{
	php_phongo_field_path_ensure_allocation(field_path, field_path->size);

	field_path->elements[field_path->size]      = NULL;
	field_path->element_types[field_path->size] = PHONGO_FIELD_PATH_ITEM_NONE;

	field_path->size--;

	field_path->elements[field_path->size]      = NULL;
	field_path->element_types[field_path->size] = PHONGO_FIELD_PATH_ITEM_NONE;

	return true;
}

inline static bool php_phongo_bson_state_is_initialized(php_phongo_bson_state* state)
{
	return state->field_path != NULL;
}

static void php_phongo_bson_state_ctor(php_phongo_bson_state* state)
{
	state->field_path = php_phongo_field_path_alloc(false);
}

static void php_phongo_bson_state_copy_ctor(php_phongo_bson_state* dst, php_phongo_bson_state* src)
{
	dst->map = src->map;
	if (src->field_path) {
		src->field_path->ref_count++;
	}
	dst->field_path = src->field_path;
}

static void php_phongo_bson_state_dtor(php_phongo_bson_state* state)
{
	if (state->field_path) {
		state->field_path->ref_count--;

		if (state->field_path->ref_count < 1) {
			php_phongo_field_path_free(state->field_path);
		}
		state->field_path = NULL;
	}
}

static void php_phongo_bson_visit_corrupt(const bson_iter_t* iter ARG_UNUSED, void* data ARG_UNUSED)
{
	mongoc_log(MONGOC_LOG_LEVEL_WARNING, MONGOC_LOG_DOMAIN, "Corrupt BSON data detected!");
}

static void php_phongo_bson_visit_unsupported_type(const bson_iter_t* iter ARG_UNUSED, const char* key, uint32_t v_type_code, void* data ARG_UNUSED)
{
	php_phongo_bson_state* state = (php_phongo_bson_state*) data;
	char*                  path_string;

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);
	path_string = php_phongo_field_path_as_string(state->field_path);

	phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected unknown BSON type 0x%02hhx for field path \"%s\". Are you using the latest driver?", (unsigned char) v_type_code, path_string);

	efree(path_string);
}

static bool php_phongo_bson_visit_double(const bson_iter_t* iter ARG_UNUSED, const char* key, double v_double, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;

	if (state->is_visiting_array) {
		add_next_index_double(retval, v_double);
	} else {
		add_assoc_double(retval, key, v_double);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static bool php_phongo_bson_visit_utf8(const bson_iter_t* iter ARG_UNUSED, const char* key, size_t v_utf8_len, const char* v_utf8, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;

	if (state->is_visiting_array) {
		ADD_NEXT_INDEX_STRINGL(retval, v_utf8, v_utf8_len);
	} else {
		ADD_ASSOC_STRING_EX(retval, key, strlen(key), v_utf8, v_utf8_len);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static void php_phongo_bson_new_binary_from_binary_and_type(zval* object, const char* data, size_t data_len, bson_subtype_t type)
{
	php_phongo_binary_t* intern;

	object_init_ex(object, php_phongo_binary_ce);

	intern           = Z_BINARY_OBJ_P(object);
	intern->data     = estrndup(data, data_len);
	intern->data_len = data_len;
	intern->type     = (uint8_t) type;
}

static bool php_phongo_bson_visit_binary(const bson_iter_t* iter ARG_UNUSED, const char* key, bson_subtype_t v_subtype, size_t v_binary_len, const uint8_t* v_binary, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;

	if (v_subtype == 0x80 && strcmp(key, PHONGO_ODM_FIELD_NAME) == 0) {
		zend_string*      zs_classname = zend_string_init((const char*) v_binary, v_binary_len, 0);
		zend_class_entry* found_ce     = zend_fetch_class(zs_classname, ZEND_FETCH_CLASS_AUTO | ZEND_FETCH_CLASS_SILENT);
		zend_string_release(zs_classname);

		if (found_ce && phongo_is_class_instantiatable(found_ce) && instanceof_function(found_ce, php_phongo_persistable_ce)) {
			((php_phongo_bson_state*) data)->odm = found_ce;
		}
	}

	{
		zval zchild;

		php_phongo_bson_new_binary_from_binary_and_type(&zchild, (const char*) v_binary, v_binary_len, v_subtype);

		if (state->is_visiting_array) {
			add_next_index_zval(retval, &zchild);
		} else {
			ADD_ASSOC_ZVAL(retval, key, &zchild);
		}
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static bool php_phongo_bson_visit_undefined(const bson_iter_t* iter, const char* key, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	object_init_ex(&zchild, php_phongo_undefined_ce);

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static void php_phongo_objectid_new_from_oid(zval* object, const bson_oid_t* oid)
{
	php_phongo_objectid_t* intern;

	object_init_ex(object, php_phongo_objectid_ce);

	intern = Z_OBJECTID_OBJ_P(object);
	bson_oid_to_string(oid, intern->oid);
	intern->initialized = true;
}

static bool php_phongo_bson_visit_oid(const bson_iter_t* iter ARG_UNUSED, const char* key, const bson_oid_t* v_oid, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	php_phongo_objectid_new_from_oid(&zchild, v_oid);

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static bool php_phongo_bson_visit_bool(const bson_iter_t* iter ARG_UNUSED, const char* key, bool v_bool, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;

	if (state->is_visiting_array) {
		add_next_index_bool(retval, v_bool);
	} else {
		add_assoc_bool(retval, key, v_bool);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static void php_phongo_bson_new_utcdatetime_from_epoch(zval* object, int64_t msec_since_epoch)
{
	php_phongo_utcdatetime_t* intern;

	object_init_ex(object, php_phongo_utcdatetime_ce);

	intern               = Z_UTCDATETIME_OBJ_P(object);
	intern->milliseconds = msec_since_epoch;
	intern->initialized  = true;
}

static bool php_phongo_bson_visit_date_time(const bson_iter_t* iter ARG_UNUSED, const char* key, int64_t msec_since_epoch, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	php_phongo_bson_new_utcdatetime_from_epoch(&zchild, msec_since_epoch);

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static void php_phongo_bson_new_decimal128(zval* object, const bson_decimal128_t* decimal)
{
	php_phongo_decimal128_t* intern;

	object_init_ex(object, php_phongo_decimal128_ce);

	intern = Z_DECIMAL128_OBJ_P(object);
	memcpy(&intern->decimal, decimal, sizeof(bson_decimal128_t));
	intern->initialized = true;
}

static bool php_phongo_bson_visit_decimal128(const bson_iter_t* iter ARG_UNUSED, const char* key, const bson_decimal128_t* decimal, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	php_phongo_bson_new_decimal128(&zchild, decimal);

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static bool php_phongo_bson_visit_null(const bson_iter_t* iter ARG_UNUSED, const char* key, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;

	if (state->is_visiting_array) {
		add_next_index_null(retval);
	} else {
		add_assoc_null(retval, key);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static void php_phongo_bson_new_regex_from_regex_and_options(zval* object, const char* pattern, const char* flags)
{
	php_phongo_regex_t* intern;

	object_init_ex(object, php_phongo_regex_ce);

	intern              = Z_REGEX_OBJ_P(object);
	intern->pattern_len = strlen(pattern);
	intern->pattern     = estrndup(pattern, intern->pattern_len);
	intern->flags_len   = strlen(flags);
	intern->flags       = estrndup(flags, intern->flags_len);
}

static bool php_phongo_bson_visit_regex(const bson_iter_t* iter ARG_UNUSED, const char* key, const char* v_regex, const char* v_options, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	php_phongo_bson_new_regex_from_regex_and_options(&zchild, v_regex, v_options);

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static void php_phongo_bson_new_symbol(zval* object, const char* symbol, size_t symbol_len)
{
	php_phongo_symbol_t* intern;

	object_init_ex(object, php_phongo_symbol_ce);

	intern             = Z_SYMBOL_OBJ_P(object);
	intern->symbol     = estrndup(symbol, symbol_len);
	intern->symbol_len = symbol_len;
}

static bool php_phongo_bson_visit_symbol(const bson_iter_t* iter, const char* key, size_t v_symbol_len, const char* v_symbol, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	php_phongo_bson_new_symbol(&zchild, v_symbol, v_symbol_len);

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static bool php_phongo_bson_new_javascript_from_javascript_and_scope(zval* object, const char* code, size_t code_len, const bson_t* scope)
{
	php_phongo_javascript_t* intern;

	if (scope) {
		php_phongo_bson_state state;
		bool                  valid_scope;

		PHONGO_BSON_INIT_STATE(state);

		valid_scope = php_phongo_bson_to_zval_ex(scope, &state);
		zval_ptr_dtor(&state.zchild);

		if (!valid_scope) {
			return false;
		}
	}

	object_init_ex(object, php_phongo_javascript_ce);

	intern           = Z_JAVASCRIPT_OBJ_P(object);
	intern->code     = estrndup(code, code_len);
	intern->code_len = code_len;
	intern->scope    = scope ? bson_copy(scope) : NULL;

	return true;
}

static bool php_phongo_bson_new_javascript_from_javascript(zval* object, const char* code, size_t code_len)
{
	return php_phongo_bson_new_javascript_from_javascript_and_scope(object, code, code_len, NULL);
}

static bool php_phongo_bson_visit_code(const bson_iter_t* iter ARG_UNUSED, const char* key, size_t v_code_len, const char* v_code, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	if (!php_phongo_bson_new_javascript_from_javascript(&zchild, v_code, v_code_len)) {
		return true;
	}

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static void php_phongo_bson_new_dbpointer(zval* object, const char* ref, size_t ref_len, const bson_oid_t* oid)
{
	php_phongo_dbpointer_t* intern;

	object_init_ex(object, php_phongo_dbpointer_ce);

	intern          = Z_DBPOINTER_OBJ_P(object);
	intern->ref     = estrndup(ref, ref_len);
	intern->ref_len = ref_len;
	bson_oid_to_string(oid, intern->id);
}

static bool php_phongo_bson_visit_dbpointer(const bson_iter_t* iter, const char* key, size_t namespace_len, const char* namespace, const bson_oid_t* oid, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	php_phongo_bson_new_dbpointer(&zchild, namespace, namespace_len, oid);

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static bool php_phongo_bson_visit_codewscope(const bson_iter_t* iter ARG_UNUSED, const char* key, size_t v_code_len, const char* v_code, const bson_t* v_scope, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	if (!php_phongo_bson_new_javascript_from_javascript_and_scope(&zchild, v_code, v_code_len, v_scope)) {
		return true;
	}

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static bool php_phongo_bson_visit_int32(const bson_iter_t* iter ARG_UNUSED, const char* key, int32_t v_int32, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;

	if (state->is_visiting_array) {
		add_next_index_long(retval, v_int32);
	} else {
		add_assoc_long(retval, key, v_int32);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static bool php_phongo_bson_visit_timestamp(const bson_iter_t* iter ARG_UNUSED, const char* key, uint32_t v_timestamp, uint32_t v_increment, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	php_phongo_bson_new_timestamp_from_increment_and_timestamp(&zchild, v_increment, v_timestamp);

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static bool php_phongo_bson_visit_int64(const bson_iter_t* iter ARG_UNUSED, const char* key, int64_t v_int64, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	if (state->is_visiting_array) {
		ADD_NEXT_INDEX_INT64(retval, v_int64);
	} else {
		ADD_ASSOC_INT64(retval, key, v_int64);
	}

	return false;
}

static bool php_phongo_bson_visit_maxkey(const bson_iter_t* iter ARG_UNUSED, const char* key, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	object_init_ex(&zchild, php_phongo_maxkey_ce);

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

static bool php_phongo_bson_visit_minkey(const bson_iter_t* iter ARG_UNUSED, const char* key, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	php_phongo_bson_state* state  = (php_phongo_bson_state*) data;
	zval                   zchild;

	object_init_ex(&zchild, php_phongo_minkey_ce);

	if (state->is_visiting_array) {
		add_next_index_zval(retval, &zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &zchild);
	}

	php_phongo_field_path_write_item_at_current_level(state->field_path, key);

	return false;
}

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

static inline bool map_element_matches_field_path(php_phongo_field_path_map_element* map_element, php_phongo_field_path* current)
{
	size_t i;

	if (map_element->entry->size != current->size) {
		return false;
	}
	for (i = 0; i < current->size; i++) {
		if (strcmp(map_element->entry->elements[i], "$") == 0) {
			continue;
		}
		if (strcmp(map_element->entry->elements[i], current->elements[i]) != 0) {
			return false;
		}
	}
	return true;
}

static php_phongo_field_path_map_element* map_find_field_path_entry(php_phongo_bson_state* state)
{
	size_t i;

	/* Loop over all field path mappings, and for each, try to see whether it matches the current path */
	for (i = 0; i < state->map.field_paths.size; i++) {
		if (map_element_matches_field_path(state->map.field_paths.map[i], state->field_path)) {
			return state->map.field_paths.map[i];
		}
	}
	return NULL;
}

static void php_phongo_handle_field_path_entry_for_compound_type(php_phongo_bson_state* state, php_phongo_bson_typemap_element* element)
{
	php_phongo_field_path_map_element* entry = map_find_field_path_entry(state);

	if (entry) {
		switch (entry->node.type) {
			case PHONGO_TYPEMAP_NATIVE_ARRAY:
			case PHONGO_TYPEMAP_NATIVE_OBJECT:
			case PHONGO_TYPEMAP_BSON:
				element->type = entry->node.type;
				break;
			case PHONGO_TYPEMAP_CLASS:
				element->type = entry->node.type;
				element->ce   = entry->node.ce;
				break;
			default:
				/* Do nothing - pacify compiler */
				break;
		}
	}
}

static bool php_phongo_bson_visit_document(const bson_iter_t* iter ARG_UNUSED, const char* key, const bson_t* v_document, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	bson_iter_t            child;
	php_phongo_bson_state* parent_state = (php_phongo_bson_state*) data;
	php_phongo_bson_state  state;

	php_phongo_field_path_push(parent_state->field_path, key, PHONGO_FIELD_PATH_ITEM_DOCUMENT);

	PHONGO_BSON_INIT_STATE(state);
	php_phongo_bson_state_copy_ctor(&state, parent_state);

	/* Check for entries in the fieldPath type map key, and use them to
	 * override the default ones for this type */
	php_phongo_handle_field_path_entry_for_compound_type(&state, &state.map.document);

	/* Only traverse BSON document if we're not returning a raw BSON structure */
	if (state.map.document.type != PHONGO_TYPEMAP_BSON) {
		if (!bson_iter_init(&child, v_document)) {
			php_phongo_bson_state_dtor(&state);
			return false;
		}

		array_init(&state.zchild);

		if (bson_iter_visit_all(&child, &php_bson_visitors, &state) || child.err_off) {
			/* Iteration stopped prematurely due to corruption or a failed
			 * visitor. Free state.zchild, which we just initialized, and return
			 * true to stop iteration for our parent context. */
			zval_ptr_dtor(&state.zchild);
			php_phongo_bson_state_dtor(&state);
			return true;
		}
	}

	/* If php_phongo_bson_visit_binary() finds an ODM class, it should
	 * supersede a default type map and named document class. */
	if (state.odm && state.map.document.type == PHONGO_TYPEMAP_NONE) {
		state.map.document.type = PHONGO_TYPEMAP_CLASS;
	}

	switch (state.map.document.type) {
		case PHONGO_TYPEMAP_BSON: {
			php_phongo_bsondocument_t* intern;

			object_init_ex(&state.zchild, php_phongo_bsondocument_ce);

			intern       = Z_BSONDOCUMENT_OBJ_P(&state.zchild);
			intern->bson = bson_copy(v_document);
			break;
		}

		case PHONGO_TYPEMAP_NATIVE_ARRAY:
			/* Do nothing, item will be added later */
			break;

		case PHONGO_TYPEMAP_CLASS: {
			zval              obj;
			zend_class_entry* obj_ce = state.odm ? state.odm : state.map.document.ce;

			object_init_ex(&obj, obj_ce);

			zend_call_method_with_1_params(PHONGO_COMPAT_OBJ_P(&obj), NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, &state.zchild);
			zval_ptr_dtor(&state.zchild);
			ZVAL_COPY_VALUE(&state.zchild, &obj);

			break;
		}

		case PHONGO_TYPEMAP_NATIVE_OBJECT:
		default:
			convert_to_object(&state.zchild);
	}

	if (parent_state->is_visiting_array) {
		add_next_index_zval(retval, &state.zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &state.zchild);
	}

	php_phongo_bson_state_dtor(&state);
	php_phongo_field_path_pop(parent_state->field_path);

	return false;
}

static bool php_phongo_bson_visit_array(const bson_iter_t* iter ARG_UNUSED, const char* key, const bson_t* v_array, void* data)
{
	zval*                  retval = PHONGO_BSON_STATE_ZCHILD(data);
	bson_iter_t            child;
	php_phongo_bson_state* parent_state = (php_phongo_bson_state*) data;
	php_phongo_bson_state  state;

	php_phongo_field_path_push(parent_state->field_path, key, PHONGO_FIELD_PATH_ITEM_ARRAY);

	PHONGO_BSON_INIT_STATE(state);
	php_phongo_bson_state_copy_ctor(&state, parent_state);

	/* Check for entries in the fieldPath type map key, and use them to
	 * override the default ones for this type */
	php_phongo_handle_field_path_entry_for_compound_type(&state, &state.map.array);

	/* Only traverse BSON array if we're not returning a raw BSON structure */
	if (state.map.array.type != PHONGO_TYPEMAP_BSON) {
		if (!bson_iter_init(&child, v_array)) {
			php_phongo_bson_state_dtor(&state);
			return false;
		}

		/* Note that we are visiting an array, so element visitors know to use
		 * add_next_index() (i.e. disregard BSON keys) instead of add_assoc()
		 * when building the PHP array.
		 */
		state.is_visiting_array = true;

		array_init(&state.zchild);

		if (bson_iter_visit_all(&child, &php_bson_visitors, &state) || child.err_off) {
			/* Iteration stopped prematurely due to corruption or a failed
			 * visitor. Free state.zchild, which we just initialized, and return
			 * true to stop iteration for our parent context. */
			zval_ptr_dtor(&state.zchild);
			php_phongo_bson_state_dtor(&state);
			return true;
		}
	}

	switch (state.map.array.type) {
		case PHONGO_TYPEMAP_BSON: {
			php_phongo_arraylist_t* intern;

			object_init_ex(&state.zchild, php_phongo_arraylist_ce);

			intern       = Z_ARRAYLIST_OBJ_P(&state.zchild);
			intern->bson = bson_copy(v_array);
			break;
		}

		case PHONGO_TYPEMAP_CLASS: {
			zval obj;

			object_init_ex(&obj, state.map.array.ce);
			zend_call_method_with_1_params(PHONGO_COMPAT_OBJ_P(&obj), NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, &state.zchild);
			zval_ptr_dtor(&state.zchild);
			ZVAL_COPY_VALUE(&state.zchild, &obj);
			break;
		}

		case PHONGO_TYPEMAP_NATIVE_OBJECT:
			convert_to_object(&state.zchild);
			break;

		case PHONGO_TYPEMAP_NATIVE_ARRAY:
		default:
			/* Do nothing, will be added later */
			break;
	}

	if (parent_state->is_visiting_array) {
		add_next_index_zval(retval, &state.zchild);
	} else {
		ADD_ASSOC_ZVAL(retval, key, &state.zchild);
	}

	php_phongo_bson_state_dtor(&state);
	php_phongo_field_path_pop(parent_state->field_path);

	return false;
}

/* Converts a BSON document to a PHP value using the default typemap. */
bool php_phongo_bson_to_zval(const bson_t* b, zval* zv)
{
	bool                  retval;
	php_phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);

	retval = php_phongo_bson_to_zval_ex(b, &state);
	ZVAL_ZVAL(zv, &state.zchild, 1, 1);

	return retval;
}

/* Converts BSON data to a PHP value using the default typemap. */
bool php_phongo_bson_data_to_zval(const unsigned char* data, int data_len, zval* zv)
{
	bool                  retval;
	php_phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);

	retval = php_phongo_bson_data_to_zval_ex(data, data_len, &state);
	ZVAL_ZVAL(zv, &state.zchild, 1, 1);

	return retval;
}

/* Converts a BSON value to a ZVAL. */
bool php_phongo_bson_value_to_zval(const bson_value_t* value, zval* zv)
{
	bson_t                bson = BSON_INITIALIZER;
	php_phongo_bson_state state;
	zval*                 return_value;
	bool                  retval = false;

	PHONGO_BSON_INIT_STATE(state);
	state.map.root.type = PHONGO_TYPEMAP_NATIVE_ARRAY;

	bson_append_value(&bson, "data", 4, value);
	if (!php_phongo_bson_to_zval_ex(&bson, &state)) {
		/* Exception already thrown */
		goto cleanup;
	}

	retval = true;

	return_value = php_array_fetchc(&state.zchild, "data");

	if (return_value) {
		ZVAL_ZVAL(zv, return_value, 1, 0);
	}

cleanup:
	zval_ptr_dtor(&state.zchild);
	return retval;
}

/* Converts a BSON document to a PHP value according to the typemap specified in
 * the state argument.
 *
 * On success, the result will be set on the state argument and true will be
 * returned. On error, an exception will have been thrown and false will be
 * returned.
 *
 * Note: the result zval in the state argument will always be initialized for
 * PHP 5.x so that the caller may always zval_ptr_dtor() it. The zval is left
 * as-is on PHP 7; however, it should have the type undefined if the state
 * was initialized to zero.
 */
bool php_phongo_bson_to_zval_ex(const bson_t* b, php_phongo_bson_state* state)
{
	bson_iter_t iter;
	bool        retval          = false;
	bool        must_dtor_state = false;

	if (!php_phongo_bson_state_is_initialized(state)) {
		php_phongo_bson_state_ctor(state);
		must_dtor_state = true;
	}

	// Handle raw root type early to avoid creating an iterator and visiting elements
	if (state->map.root.type == PHONGO_TYPEMAP_BSON) {
		zval                       obj;
		php_phongo_bsondocument_t* intern;

		object_init_ex(&obj, php_phongo_bsondocument_ce);

		intern       = Z_BSONDOCUMENT_OBJ_P(&obj);
		intern->bson = bson_copy(b);
		zval_ptr_dtor(&state->zchild);
		ZVAL_COPY_VALUE(&state->zchild, &obj);

		retval = true;
		goto cleanup;
	}

	if (!bson_iter_init(&iter, b)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not initialize BSON iterator");

		goto cleanup;
	}

	/* We initialize an array because it will either be returned as-is (native
	 * array in type map), passed to bsonUnserialize() (ODM class), or used to
	 * initialize a stdClass object (native object in type map). */
	array_init(&state->zchild);

	if (bson_iter_visit_all(&iter, &php_bson_visitors, state) || iter.err_off) {
		/* Iteration stopped prematurely due to corruption or a failed visitor.
		 * While we free the reader, state->zchild should be left as-is, since
		 * the calling code may want to zval_ptr_dtor() it. If an exception has
		 * been thrown already (due to an unsupported BSON type for example,
		 * don't overwrite with a generic exception message. */
		if (!EG(exception)) {
			char* path = php_phongo_field_path_as_string(state->field_path);
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected corrupt BSON data for field path '%s' at offset %d", path, iter.err_off);
			efree(path);
		}

		goto cleanup;
	}

	/* If php_phongo_bson_visit_binary() finds an ODM class, it should supersede
	 * a default type map and named root class. */
	if (state->odm && state->map.root.type == PHONGO_TYPEMAP_NONE) {
		state->map.root.type = PHONGO_TYPEMAP_CLASS;
	}

	switch (state->map.root.type) {
		case PHONGO_TYPEMAP_NATIVE_ARRAY:
			/* Nothing to do here */
			break;

		case PHONGO_TYPEMAP_CLASS: {
			zval              obj;
			zend_class_entry* obj_ce = state->odm ? state->odm : state->map.root.ce;

			object_init_ex(&obj, obj_ce);

			zend_call_method_with_1_params(PHONGO_COMPAT_OBJ_P(&obj), NULL, NULL, BSON_UNSERIALIZE_FUNC_NAME, NULL, &state->zchild);
			zval_ptr_dtor(&state->zchild);
			ZVAL_COPY_VALUE(&state->zchild, &obj);

			break;
		}

		case PHONGO_TYPEMAP_NATIVE_OBJECT:
		default:
			convert_to_object(&state->zchild);
	}

	retval = true;

cleanup:
	if (must_dtor_state) {
		php_phongo_bson_state_dtor(state);
	}

	return retval;
}

/* Converts a BSON document to a PHP value according to the typemap specified in
 * the state argument.
 *
 * On success, the result will be set on the state argument and true will be
 * returned. On error, an exception will have been thrown and false will be
 * returned.
 *
 * Note: the result zval in the state argument will always be initialized for
 * PHP 5.x so that the caller may always zval_ptr_dtor() it. The zval is left
 * as-is on PHP 7; however, it should have the type undefined if the state
 * was initialized to zero.
 */
bool php_phongo_bson_data_to_zval_ex(const unsigned char* data, int data_len, php_phongo_bson_state* state)
{
	bson_reader_t* reader = NULL;
	const bson_t*  b;
	bool           eof    = false;
	bool           retval = false;

	reader = bson_reader_new_from_data(data, data_len);

	if (!(b = bson_reader_read(reader, NULL))) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not read document from BSON reader");

		goto cleanup;
	}

	retval = php_phongo_bson_to_zval_ex(b, state);

	if (bson_reader_read(reader, &eof) || !eof) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Reading document did not exhaust input buffer");
		retval = false;

		goto cleanup;
	}

cleanup:
	if (reader) {
		bson_reader_destroy(reader);
	}

	return retval;
}

/* Fetches a zend_class_entry for the given class name and checks that it is
 * also instantiatable and implements a specified interface. Returns the class
 * on success; otherwise, NULL is returned and an exception is thrown. */
static zend_class_entry* php_phongo_bson_state_fetch_class(const char* classname, int classname_len, zend_class_entry* interface_ce)
{
	zend_string*      zs_classname = zend_string_init(classname, classname_len, 0);
	zend_class_entry* found_ce     = zend_fetch_class(zs_classname, ZEND_FETCH_CLASS_AUTO | ZEND_FETCH_CLASS_SILENT);
	zend_string_release(zs_classname);

	if (!found_ce) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Class %s does not exist", classname);
	} else if (!phongo_is_class_instantiatable(found_ce)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s %s is not instantiatable", zend_get_object_type_uc(found_ce), classname);
	} else if (!instanceof_function(found_ce, interface_ce)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Class %s does not implement %s", classname, ZSTR_VAL(interface_ce->name));
	} else {
		return found_ce;
	}

	return NULL;
}

/* Parses a BSON type (i.e. array, document, or root). On success, the type and
 * type_ce output arguments will be assigned and true will be returned;
 * otherwise, false is returned and an exception is thrown. */
static bool php_phongo_bson_state_parse_type(zval* options, const char* name, php_phongo_bson_typemap_element* element)
{
	char*     classname;
	int       classname_len;
	zend_bool classname_free = 0;
	bool      retval         = true;

	classname = php_array_fetch_string(options, name, &classname_len, &classname_free);

	if (!classname_len) {
		goto cleanup;
	}

	if (!strcasecmp(classname, "array")) {
		element->type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		element->ce   = NULL;
	} else if (!strcasecmp(classname, "bson")) {
		element->type = PHONGO_TYPEMAP_BSON;
		element->ce   = NULL;
	} else if (!strcasecmp(classname, "stdclass") || !strcasecmp(classname, "object")) {
		element->type = PHONGO_TYPEMAP_NATIVE_OBJECT;
		element->ce   = NULL;
	} else {
		if ((element->ce = php_phongo_bson_state_fetch_class(classname, classname_len, php_phongo_unserializable_ce))) {
			element->type = PHONGO_TYPEMAP_CLASS;
		} else {
			/* Exception already thrown */
			retval = false;
		}
	}

cleanup:
	if (classname_free) {
		efree(classname);
	}

	return retval;
}

static void field_path_map_element_set_info(php_phongo_field_path_map_element* element, php_phongo_bson_typemap_element* typemap_element)
{
	element->node.type = typemap_element->type;
	element->node.ce   = typemap_element->ce;
}

static void map_add_field_path_element(php_phongo_bson_typemap* map, php_phongo_field_path_map_element* element)
{
	/* Make sure we have allocated enough */
	if (map->field_paths.allocated_size < map->field_paths.size + 1) {
		map->field_paths.allocated_size += PHONGO_FIELD_PATH_EXPANSION;
		map->field_paths.map = erealloc(map->field_paths.map, sizeof(php_phongo_field_path_map_element) * map->field_paths.allocated_size);
	}

	map->field_paths.map[map->field_paths.size] = element;
	map->field_paths.size++;
}

static php_phongo_field_path_map_element* field_path_map_element_alloc(void)
{
	php_phongo_field_path_map_element* tmp = ecalloc(1, sizeof(php_phongo_field_path_map_element));

	tmp->entry = php_phongo_field_path_alloc(true);

	return tmp;
}

static void field_path_map_element_dtor(php_phongo_field_path_map_element* element)
{
	php_phongo_field_path_free(element->entry);
	efree(element);
}

static bool php_phongo_bson_state_add_field_path(php_phongo_bson_typemap* map, char* field_path_original, php_phongo_bson_typemap_element* typemap_element)
{
	char*                              ptr         = NULL;
	char*                              segment_end = NULL;
	php_phongo_field_path_map_element* field_path_map_element;

	if (field_path_original[0] == '.') {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "A 'fieldPaths' key may not start with a '.'");
		return false;
	}

	if (field_path_original[strlen(field_path_original) - 1] == '.') {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "A 'fieldPaths' key may not end with a '.'");
		return false;
	}

	field_path_map_element = field_path_map_element_alloc();
	ptr                    = field_path_original;

	/* Loop over all the segments. A segment is delimited by a "." */
	while ((segment_end = strchr(ptr, '.')) != NULL) {
		char* tmp = NULL;

		/* Bail out if we have an empty segment */
		if (ptr == segment_end) {
			field_path_map_element_dtor(field_path_map_element);
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "A 'fieldPaths' key may not have an empty segment");
			return false;
		}

		tmp = calloc(1, segment_end - ptr + 1);
		memcpy(tmp, ptr, segment_end - ptr);
		php_phongo_field_path_push(field_path_map_element->entry, tmp, PHONGO_FIELD_PATH_ITEM_NONE);
		free(tmp);

		ptr = segment_end + 1;
	}

	/* Add the last (or single) element */
	php_phongo_field_path_push(field_path_map_element->entry, ptr, PHONGO_FIELD_PATH_ITEM_NONE);

	field_path_map_element_set_info(field_path_map_element, typemap_element);
	map_add_field_path_element(map, field_path_map_element);

	return true;
}

void php_phongo_bson_typemap_dtor(php_phongo_bson_typemap* map)
{
	size_t i;

	if (map->field_paths.map) {
		for (i = 0; i < map->field_paths.size; i++) {
			field_path_map_element_dtor(map->field_paths.map[i]);
		}
		efree(map->field_paths.map);
	}

	map->field_paths.map = NULL;
}

/* Loops over each element in the fieldPaths array (if exists, and is an
 * array), and then checks whether each element is a valid type mapping */
static bool php_phongo_bson_state_parse_fieldpaths(zval* typemap, php_phongo_bson_typemap* map)
{
	zval*      fieldpaths = NULL;
	HashTable* ht_data;

	if (!php_array_existsc(typemap, "fieldPaths")) {
		return true;
	}

	fieldpaths = php_array_fetchc_array(typemap, "fieldPaths");

	if (!fieldpaths) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "The 'fieldPaths' element is not an array");
		return false;
	}

	ht_data = HASH_OF(fieldpaths);

	{
		zend_string* string_key = NULL;
		zend_ulong   num_key    = 0;
		zval*        property;

		ZEND_HASH_FOREACH_KEY_VAL(ht_data, num_key, string_key, property)
		{
			php_phongo_bson_typemap_element element;

			if (!string_key) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "The 'fieldPaths' element is not an associative array");
				return false;
			}

			if (strcmp(ZSTR_VAL(string_key), "") == 0) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "The 'fieldPaths' element may not be an empty string");
				return false;
			}

			if (!php_phongo_bson_state_parse_type(fieldpaths, ZSTR_VAL(string_key), &element)) {
				return false;
			}

			if (!php_phongo_bson_state_add_field_path(map, ZSTR_VAL(string_key), &element)) {
				return false;
			}
		}
		ZEND_HASH_FOREACH_END();
	}

	return true;
}

/* Applies the array argument to a typemap struct. Returns true on success;
 * otherwise, false is returned an an exception is thrown. */
bool php_phongo_bson_typemap_to_state(zval* typemap, php_phongo_bson_typemap* map)
{
	if (!typemap) {
		return true;
	}

	if (!php_phongo_bson_state_parse_type(typemap, "array", &map->array) ||
		!php_phongo_bson_state_parse_type(typemap, "document", &map->document) ||
		!php_phongo_bson_state_parse_type(typemap, "root", &map->root) ||
		!php_phongo_bson_state_parse_fieldpaths(typemap, map)) {

		/* Exception should already have been thrown */
		return false;
	}

	return true;
}

void php_phongo_bson_new_timestamp_from_increment_and_timestamp(zval* object, uint32_t increment, uint32_t timestamp)
{
	php_phongo_timestamp_t* intern;

	object_init_ex(object, php_phongo_timestamp_ce);

	intern              = Z_TIMESTAMP_OBJ_P(object);
	intern->increment   = increment;
	intern->timestamp   = timestamp;
	intern->initialized = true;
}

void php_phongo_bson_new_int64(zval* object, int64_t integer)
{
	php_phongo_int64_t* intern;

	object_init_ex(object, php_phongo_int64_ce);

	intern              = Z_INT64_OBJ_P(object);
	intern->integer     = integer;
	intern->initialized = true;
}

bool php_phongo_bson_to_json(zval* return_value, const bson_t* bson, php_phongo_json_mode_t mode)
{
	char*  json = NULL;
	size_t json_len;

	if (mode == PHONGO_JSON_MODE_LEGACY) {
		json = bson_as_json(bson, &json_len);
	} else if (mode == PHONGO_JSON_MODE_CANONICAL) {
		json = bson_as_canonical_extended_json(bson, &json_len);
	} else if (mode == PHONGO_JSON_MODE_RELAXED) {
		json = bson_as_relaxed_extended_json(bson, &json_len);
	}

	if (!json) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not convert BSON document to a JSON string");
		return false;
	}

	ZVAL_STRINGL(return_value, json, json_len);

	return true;
}

void php_phongo_bson_iter_to_zval(zval* zv, bson_iter_t* iter)
{
	const char*                data;
	const char*                options;
	uint32_t                   data_len;
	uint32_t                   options_len;
	bson_subtype_t             subtype;
	const bson_oid_t*          oid;
	bson_decimal128_t          decimal;
	bson_t                     bson = BSON_INITIALIZER;
	php_phongo_bsondocument_t* bsondocument_intern;
	php_phongo_arraylist_t*    arraylist_intern;

	switch (bson_iter_type(iter)) {
		case BSON_TYPE_UTF8:
			ZVAL_STRING(zv, bson_iter_utf8(iter, NULL));
			return;

		case BSON_TYPE_EOD:
			ZVAL_UNDEF(zv);
			return;

		case BSON_TYPE_DOCUMENT:
			bson_iter_document(iter, &data_len, (const uint8_t**) &data);

			object_init_ex(zv, php_phongo_bsondocument_ce);
			bsondocument_intern       = Z_BSONDOCUMENT_OBJ_P(zv);
			bsondocument_intern->bson = bson_new();
			bson_init_static(bsondocument_intern->bson, (const uint8_t*) data, data_len);
			return;

		case BSON_TYPE_DOUBLE:
			ZVAL_DOUBLE(zv, bson_iter_double(iter));
			return;

		case BSON_TYPE_ARRAY:
			bson_iter_array(iter, &data_len, (const uint8_t**) &data);

			object_init_ex(zv, php_phongo_arraylist_ce);
			arraylist_intern       = Z_ARRAYLIST_OBJ_P(zv);
			arraylist_intern->bson = bson_new();
			bson_init_static(arraylist_intern->bson, (const uint8_t*) data, data_len);
			return;

		case BSON_TYPE_BINARY:
			bson_iter_binary(iter, (bson_subtype_t*) &subtype, &data_len, (const uint8_t**) &data);
			php_phongo_bson_new_binary_from_binary_and_type(zv, data, data_len, subtype);
			return;

		case BSON_TYPE_UNDEFINED:
			object_init_ex(zv, php_phongo_undefined_ce);
			return;

		case BSON_TYPE_OID:
			php_phongo_objectid_new_from_oid(zv, bson_iter_oid(iter));
			return;

		case BSON_TYPE_BOOL:
			ZVAL_BOOL(zv, bson_iter_bool(iter));
			return;

		case BSON_TYPE_DATE_TIME:
			php_phongo_bson_new_utcdatetime_from_epoch(zv, bson_iter_date_time(iter));
			return;

		case BSON_TYPE_NULL:
			ZVAL_NULL(zv);
			return;

		case BSON_TYPE_REGEX:
			data = bson_iter_regex(iter, &options);
			php_phongo_bson_new_regex_from_regex_and_options(zv, data, options);
			return;

		case BSON_TYPE_DBPOINTER:
			bson_iter_dbpointer(iter, &data_len, &data, &oid);
			php_phongo_bson_new_dbpointer(zv, data, data_len, oid);
			return;

		case BSON_TYPE_CODE:
			data = bson_iter_code(iter, &data_len);
			php_phongo_bson_new_javascript_from_javascript(zv, data, data_len);
			return;

		case BSON_TYPE_SYMBOL:
			data = bson_iter_symbol(iter, &data_len);
			php_phongo_bson_new_symbol(zv, data, data_len);
			return;

		case BSON_TYPE_CODEWSCOPE:
			data = bson_iter_codewscope(iter, &data_len, &options_len, (const uint8_t**) &options);
			bson_init_from_json(&bson, options, options_len, NULL);
			php_phongo_bson_new_javascript_from_javascript_and_scope(zv, data, data_len, &bson);
			return;

		case BSON_TYPE_INT32:
		case BSON_TYPE_INT64:
			ZVAL_INT64(zv, bson_iter_as_int64(iter));
			return;

		case BSON_TYPE_TIMESTAMP:
			bson_iter_timestamp(iter, &data_len, &options_len);
			php_phongo_bson_new_timestamp_from_increment_and_timestamp(zv, options_len, data_len);
			return;

		case BSON_TYPE_DECIMAL128:
			bson_iter_decimal128(iter, &decimal);
			php_phongo_bson_new_decimal128(zv, &decimal);
			return;

		case BSON_TYPE_MAXKEY:
			object_init_ex(zv, php_phongo_maxkey_ce);
			return;

		case BSON_TYPE_MINKEY:
			object_init_ex(zv, php_phongo_minkey_ce);
			return;
	}
}
