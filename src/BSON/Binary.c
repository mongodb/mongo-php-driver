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

#include <php.h>
#include <ext/standard/base64.h>
#include <Zend/zend_enum.h>
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"
#include "Binary.h"
#include "Binary_arginfo.h"
#include "VectorType.h"

zend_class_entry* phongo_binary_ce;

static phongo_bson_vector_type_t phongo_binary_get_vector_type_from_data(const uint8_t* data, uint32_t data_len);
static phongo_bson_vector_type_t phongo_binary_get_vector_type(const phongo_binary_t* intern);
static void                      phongo_binary_get_vector_as_array(const phongo_binary_t* intern, zval* return_value);

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool phongo_binary_init(phongo_binary_t* intern, const char* data, size_t data_len, zend_long type)
{
	if (type < 0 || type > UINT8_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected type to be an unsigned 8-bit integer, %" PHONGO_LONG_FORMAT " given", type);
		return false;
	}

	if ((type == BSON_SUBTYPE_UUID_DEPRECATED || type == BSON_SUBTYPE_UUID) && data_len != PHONGO_BINARY_UUID_SIZE) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected UUID length to be %d bytes, %d given", PHONGO_BINARY_UUID_SIZE, data_len);
		return false;
	}

	if ((type == BSON_SUBTYPE_VECTOR) && phongo_binary_get_vector_type_from_data((const uint8_t*) data, data_len) == PHONGO_BSON_VECTOR_TYPE_UNKNOWN) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Binary vector data is invalid");
		return false;
	}

	intern->data     = estrndup(data, data_len);
	intern->data_len = data_len;
	intern->type     = (uint8_t) type;

	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_binary_init_from_hash(phongo_binary_t* intern, HashTable* props)
{
	zval *data, *type;

	if ((data = zend_hash_str_find(props, "data", sizeof("data") - 1)) && Z_TYPE_P(data) == IS_STRING &&
		(type = zend_hash_str_find(props, "type", sizeof("type") - 1)) && Z_TYPE_P(type) == IS_LONG) {

		return phongo_binary_init(intern, Z_STRVAL_P(data), Z_STRLEN_P(data), Z_LVAL_P(type));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"data\" string and \"type\" integer fields", ZSTR_VAL(phongo_binary_ce->name));
	return false;
}

static HashTable* phongo_binary_get_properties_hash(zend_object* object, bool is_temp, bool is_debug)
{
	PHONGO_INTERN_FROM_Z_OBJ(binary, object);

	HashTable* props;

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->data) {
		return props;
	}

	{
		zval data, type;

		if (is_debug) {
			ZVAL_NEW_STR(&data, php_base64_encode((unsigned char*) intern->data, intern->data_len));
		} else {
			ZVAL_STRINGL(&data, intern->data, intern->data_len);
		}

		zend_hash_str_update(props, "data", sizeof("data") - 1, &data);

		ZVAL_LONG(&type, intern->type);
		zend_hash_str_update(props, "type", sizeof("type") - 1, &type);
	}

	return props;
}

/* Construct a new BSON binary type */
static PHP_METHOD(MongoDB_BSON_Binary, __construct)
{
	PHONGO_INTERN_FROM_THIS(binary);
	char*     data;
	size_t    data_len;
	zend_long type = BSON_SUBTYPE_BINARY;

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_STRING(data, data_len)
	Z_PARAM_OPTIONAL
	Z_PARAM_LONG(type)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_binary_init(intern, data, data_len, type);
}

static PHP_METHOD(MongoDB_BSON_Binary, __set_state)
{
	HashTable* props;
	zval*      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(binary, return_value);
	props = Z_ARRVAL_P(array);

	phongo_binary_init_from_hash(intern, props);
}

/* Return the Binary's data string. */
static PHP_METHOD(MongoDB_BSON_Binary, __toString)
{
	PHONGO_INTERN_FROM_THIS(binary);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRINGL(intern->data, intern->data_len);
}

static PHP_METHOD(MongoDB_BSON_Binary, getData)
{
	PHONGO_INTERN_FROM_THIS(binary);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRINGL(intern->data, intern->data_len);
}

static PHP_METHOD(MongoDB_BSON_Binary, getType)
{
	PHONGO_INTERN_FROM_THIS(binary);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->type);
}

static PHP_METHOD(MongoDB_BSON_Binary, jsonSerialize)
{
	PHONGO_INTERN_FROM_THIS(binary);
	char type[3];
	int  type_len;

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 2);

	{
		zend_string* data = php_base64_encode((unsigned char*) intern->data, intern->data_len);
		ADD_ASSOC_STRINGL(return_value, "$binary", ZSTR_VAL(data), ZSTR_LEN(data));
		zend_string_free(data);
	}

	type_len = snprintf(type, sizeof(type), "%02x", intern->type);
	ADD_ASSOC_STRINGL(return_value, "$type", type, type_len);
}

static PHP_METHOD(MongoDB_BSON_Binary, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(phongo_binary_get_properties_hash(Z_OBJ_P(getThis()), true, false));
}

static PHP_METHOD(MongoDB_BSON_Binary, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_binary_init_from_hash(Z_BINARY_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\Binary object handlers */
static zend_object_handlers phongo_handler_binary;

static void phongo_binary_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(binary, object);

	zend_object_std_dtor(&intern->std);

	if (intern->data) {
		efree(intern->data);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* phongo_binary_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(binary, class_type);

	intern->std.handlers = &phongo_handler_binary;

	return &intern->std;
}

static zend_object* phongo_binary_clone_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(binary, object);

	phongo_binary_t* new_intern;
	zend_object*     new_object;

	new_object = phongo_binary_create_object(object->ce);

	new_intern = Z_OBJ_BINARY(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	phongo_binary_init(new_intern, intern->data, intern->data_len, intern->type);

	return new_object;
}

static int phongo_binary_compare_objects(zval* o1, zval* o2)
{
	phongo_binary_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_BINARY_OBJ_P(o1);
	intern2 = Z_BINARY_OBJ_P(o2);

	/* MongoDB compares binary types first by the data length, then by the type
	 * byte, and finally by the binary data itself. */
	if (intern1->data_len != intern2->data_len) {
		return intern1->data_len < intern2->data_len ? -1 : 1;
	}

	if (intern1->type != intern2->type) {
		return intern1->type < intern2->type ? -1 : 1;
	}

	return zend_binary_strcmp(intern1->data, intern1->data_len, intern2->data, intern2->data_len);
}

static HashTable* phongo_binary_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp         = 1;
	HashTable* props = phongo_binary_get_properties_hash(object, true, true);

	PHONGO_INTERN_FROM_Z_OBJ(binary, object);

	if (intern->type == BSON_SUBTYPE_VECTOR) {
		zval vector;

		phongo_binary_get_vector_as_array(intern, &vector);

		if (EG(exception)) {
			return props;
		}

		zend_hash_str_update(props, "vector", sizeof("vector") - 1, &vector);

		zval         vector_type;
		zend_object* vector_type_case = phongo_bson_vector_type_to_case(phongo_binary_get_vector_type(intern));

		// The vector should always be valid by this point, but check for an error
		if (!vector_type_case) {
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Binary vector data is invalid");
			return props;
		}

		ZVAL_OBJ_COPY(&vector_type, vector_type_case);
		zend_hash_str_update(props, "vectorType", sizeof("vectorType") - 1, &vector_type);
	}

	return props;
}

static HashTable* phongo_binary_get_properties(zend_object* object)
{
	return phongo_binary_get_properties_hash(object, false, false);
}

void phongo_binary_init_ce(INIT_FUNC_ARGS)
{
	phongo_binary_ce                = register_class_MongoDB_BSON_Binary(phongo_binary_interface_ce, phongo_json_serializable_ce, phongo_type_ce, zend_ce_stringable);
	phongo_binary_ce->create_object = phongo_binary_create_object;

	memcpy(&phongo_handler_binary, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_binary.compare        = phongo_binary_compare_objects;
	phongo_handler_binary.clone_obj      = phongo_binary_clone_object;
	phongo_handler_binary.get_debug_info = phongo_binary_get_debug_info;
	phongo_handler_binary.get_properties = phongo_binary_get_properties;
	phongo_handler_binary.free_obj       = phongo_binary_free_object;
	phongo_handler_binary.offset         = XtOffsetOf(phongo_binary_t, std);
}

bool phongo_binary_new(zval* object, const char* data, size_t data_len, bson_subtype_t type)
{
	object_init_ex(object, phongo_binary_ce);

	return phongo_binary_init(Z_BINARY_OBJ_P(object), data, data_len, type);
}

static inline void phongo_binary_init_vector_from_bson_key(phongo_binary_t* intern, const bson_t* doc, const char* key)
{
	bson_iter_t iter;

	if (!(bson_iter_init_find(&iter, doc, key) && BSON_ITER_HOLDS_VECTOR(&iter))) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "bson_iter_init_find failed to find binary vector in key \"%s\"", key);
		return;
	}

	uint32_t       data_len;
	const uint8_t* data;

	bson_iter_binary(&iter, NULL, &data_len, &data);
	phongo_binary_init(intern, (const char*) data, data_len, BSON_SUBTYPE_VECTOR);
}

static void phongo_binary_init_vector_from_float32_array(phongo_binary_t* intern, HashTable* vector)
{
	if (!zend_array_is_list(vector)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected vector to be a list");
		return;
	}

	const size_t vector_len = zend_array_count(vector);

	bson_t                     doc = BSON_INITIALIZER;
	bson_vector_float32_view_t view;

	if (!BSON_APPEND_VECTOR_FLOAT32_UNINIT(&doc, "vector", vector_len, &view)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "BSON_APPEND_VECTOR_FLOAT32_UNINIT failed for vector of size %zu", vector_len);
		goto cleanup;
	}

	zval*  val;
	size_t i = 0;

	ZEND_HASH_FOREACH_VAL_IND(vector, val)
	{
		if (Z_TYPE_P(val) != IS_DOUBLE) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected vector[%zu] to be a float, %s given", i, zend_zval_type_name(val));
			goto cleanup;
		}

		float v = (float) Z_DVAL_P(val);

		if (!bson_vector_float32_view_write(view, &v, 1, i)) {
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "bson_vector_float32_view_write failed to write vector[%zu]", i);
			goto cleanup;
		}

		i += 1;
	}
	ZEND_HASH_FOREACH_END();

	phongo_binary_init_vector_from_bson_key(intern, &doc, "vector");

cleanup:
	bson_destroy(&doc);
}

static void phongo_binary_init_vector_from_int8_array(phongo_binary_t* intern, HashTable* vector)
{
	if (!zend_array_is_list(vector)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected vector to be a list");
		return;
	}

	const size_t vector_len = zend_array_count(vector);

	bson_t                  doc = BSON_INITIALIZER;
	bson_vector_int8_view_t view;

	if (!BSON_APPEND_VECTOR_INT8_UNINIT(&doc, "vector", vector_len, &view)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "BSON_APPEND_VECTOR_INT8_UNINIT failed for vector of size %zu", vector_len);
		goto cleanup;
	}

	zval*  val;
	size_t i = 0;

	ZEND_HASH_FOREACH_VAL_IND(vector, val)
	{
		if (Z_TYPE_P(val) != IS_LONG) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected vector[%zu] to be an integer, %s given", i, zend_zval_type_name(val));
			goto cleanup;
		}

		if (Z_LVAL_P(val) < INT8_MIN || Z_LVAL_P(val) > INT8_MAX) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected vector[%zu] to be a signed 8-bit integer, %" PHONGO_LONG_FORMAT " given", i, Z_LVAL_P(val));
			goto cleanup;
		}

		int8_t v = (int8_t) Z_LVAL_P(val);

		if (!bson_vector_int8_view_write(view, &v, 1, i)) {
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "bson_vector_int8_view_write failed to write vector[%zu]", i);
			goto cleanup;
		}

		i += 1;
	}
	ZEND_HASH_FOREACH_END();

	phongo_binary_init_vector_from_bson_key(intern, &doc, "vector");

cleanup:
	bson_destroy(&doc);
}

static void phongo_binary_init_vector_from_packed_bit_array(phongo_binary_t* intern, HashTable* vector)
{
	if (!zend_array_is_list(vector)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected vector to be a list");
		return;
	}

	const size_t vector_len = zend_array_count(vector);

	bson_t                        doc = BSON_INITIALIZER;
	bson_vector_packed_bit_view_t view;

	if (!BSON_APPEND_VECTOR_PACKED_BIT_UNINIT(&doc, "vector", vector_len, &view)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "BSON_APPEND_VECTOR_PACKED_BIT_UNINIT failed for vector of size %zu", vector_len);
		goto cleanup;
	}

	zval*  val;
	size_t i = 0;

	ZEND_HASH_FOREACH_VAL_IND(vector, val)
	{
		if (Z_TYPE_P(val) != IS_LONG && Z_TYPE_P(val) != IS_TRUE && Z_TYPE_P(val) != IS_FALSE) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected vector[%zu] to be 0, 1, or a boolean, %s given", i, zend_zval_type_name(val));
			goto cleanup;
		}

		if (Z_TYPE_P(val) == IS_LONG && Z_LVAL_P(val) != 0 && Z_LVAL_P(val) != 1) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected vector[%zu] to be 0 or 1, %" PHONGO_LONG_FORMAT " given", i, Z_LVAL_P(val));
			goto cleanup;
		}

		bool v = zend_is_true(val);

		if (!bson_vector_packed_bit_view_pack_bool(view, &v, 1, i)) {
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "bson_vector_packed_bit_view_pack_bool failed to write vector[%zu]", i);
			goto cleanup;
		}

		i += 1;
	}
	ZEND_HASH_FOREACH_END();

	phongo_binary_init_vector_from_bson_key(intern, &doc, "vector");

cleanup:
	bson_destroy(&doc);
}

static PHP_METHOD(MongoDB_BSON_Binary, fromVector)
{
	HashTable*   vector;
	zend_object* type;

	object_init_ex(return_value, phongo_binary_ce);
	PHONGO_INTERN_FROM_ZVAL(binary, return_value);

	PHONGO_PARSE_PARAMETERS_START(2, 2)
	Z_PARAM_ARRAY_HT(vector)
	Z_PARAM_OBJ_OF_CLASS(type, phongo_vectortype_ce)
	PHONGO_PARSE_PARAMETERS_END();

	switch (phongo_bson_vector_type_from_case(type)) {
		case PHONGO_BSON_VECTOR_TYPE_FLOAT32:
			phongo_binary_init_vector_from_float32_array(intern, vector);
			return;
		case PHONGO_BSON_VECTOR_TYPE_INT8:
			phongo_binary_init_vector_from_int8_array(intern, vector);
			return;
		case PHONGO_BSON_VECTOR_TYPE_PACKED_BIT:
			phongo_binary_init_vector_from_packed_bit_array(intern, vector);
			return;
		default:
			phongo_throw_exception(PHONGO_ERROR_LOGIC, "Unsupported binary vector type: %s", Z_STRVAL_P(zend_enum_fetch_case_name(type)));
			RETURN_THROWS();
	}
}

static phongo_bson_vector_type_t phongo_binary_get_vector_type_from_data(const uint8_t* data, uint32_t data_len)
{
	if (bson_vector_int8_const_view_init(NULL, data, data_len)) {
		return PHONGO_BSON_VECTOR_TYPE_INT8;
	}

	if (bson_vector_float32_const_view_init(NULL, data, data_len)) {
		return PHONGO_BSON_VECTOR_TYPE_FLOAT32;
	}

	if (bson_vector_packed_bit_const_view_init(NULL, data, data_len)) {
		return PHONGO_BSON_VECTOR_TYPE_PACKED_BIT;
	}

	return PHONGO_BSON_VECTOR_TYPE_UNKNOWN;
}

static phongo_bson_vector_type_t phongo_binary_get_vector_type(const phongo_binary_t* intern)
{
	return phongo_binary_get_vector_type_from_data((const uint8_t*) intern->data, intern->data_len);
}

static PHP_METHOD(MongoDB_BSON_Binary, getVectorType)
{
	PHONGO_INTERN_FROM_THIS(binary);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (intern->type != BSON_SUBTYPE_VECTOR) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Expected Binary of type vector (%" PRId8 ") but it is %" PHONGO_LONG_FORMAT, BSON_SUBTYPE_VECTOR, intern->type);
		RETURN_THROWS();
	}

	const char* type_case = phongo_bson_vector_type_to_name(phongo_binary_get_vector_type(intern));

	// The vector should always be valid by this point, but check for an error
	if (!type_case) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Binary vector data is invalid");
		RETURN_THROWS();
	}

	RETVAL_OBJ_COPY(zend_enum_get_case_cstr(phongo_vectortype_ce, type_case));
}

static void phongo_binary_get_vector_as_array(const phongo_binary_t* intern, zval* return_value)
{
	bson_t tmp_doc = BSON_INITIALIZER;

	switch (phongo_binary_get_vector_type(intern)) {
		case PHONGO_BSON_VECTOR_TYPE_INT8: {
			bson_vector_int8_const_view_t view;

			if (!bson_vector_int8_const_view_init(&view, (const uint8_t*) intern->data, intern->data_len) ||
				!BSON_APPEND_ARRAY_FROM_VECTOR_INT8(&tmp_doc, "vector", view)) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Failed to convert binary vector data to an array");
				bson_destroy(&tmp_doc);
				RETURN_THROWS();
			}

			break;
		}
		case PHONGO_BSON_VECTOR_TYPE_FLOAT32: {
			bson_vector_float32_const_view_t view;

			if (!bson_vector_float32_const_view_init(&view, (const uint8_t*) intern->data, intern->data_len) ||
				!BSON_APPEND_ARRAY_FROM_VECTOR_FLOAT32(&tmp_doc, "vector", view)) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Failed to convert binary vector data to an array");
				bson_destroy(&tmp_doc);
				RETURN_THROWS();
			}

			break;
		}
		case PHONGO_BSON_VECTOR_TYPE_PACKED_BIT: {
			bson_vector_packed_bit_const_view_t view;

			if (!bson_vector_packed_bit_const_view_init(&view, (const uint8_t*) intern->data, intern->data_len) ||
				!BSON_APPEND_ARRAY_FROM_VECTOR_PACKED_BIT(&tmp_doc, "vector", view)) {
				phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Failed to convert binary vector data to an array");
				bson_destroy(&tmp_doc);
				RETURN_THROWS();
			}

			break;
		}
		default:
			phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Binary vector data is invalid");
			RETURN_THROWS();
	}

	bson_iter_t iter;

	if (!(bson_iter_init_find(&iter, &tmp_doc, "vector") && BSON_ITER_HOLDS_ARRAY(&iter))) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "bson_iter_init_find failed for appended vector");
		bson_destroy(&tmp_doc);
		RETURN_THROWS();
	}

	uint32_t       data_len;
	const uint8_t* data;

	bson_iter_array(&iter, &data_len, &data);

	bson_t tmp_vector = BSON_INITIALIZER;

	if (!bson_init_static(&tmp_vector, data, data_len)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "bson_init_static failed for appended vector");
		bson_destroy(&tmp_doc);
		RETURN_THROWS();
	}

	phongo_bson_state state;
	PHONGO_BSON_INIT_STATE(state);
	state.is_visiting_array = true;

	if (!phongo_bson_to_zval_ex(&tmp_vector, &state)) {
		// Exception already thrown
		bson_destroy(&tmp_doc);
		zval_ptr_dtor(&state.zchild);
		phongo_bson_typemap_dtor(&state.map);
		RETURN_THROWS();
	}

	bson_destroy(&tmp_doc);
	phongo_bson_typemap_dtor(&state.map);

	RETURN_ZVAL(&state.zchild, 0, 1);
}

static PHP_METHOD(MongoDB_BSON_Binary, toArray)
{
	PHONGO_INTERN_FROM_THIS(binary);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (intern->type != BSON_SUBTYPE_VECTOR) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Expected Binary of type vector (%" PRId8 ") but it is %" PHONGO_LONG_FORMAT, BSON_SUBTYPE_VECTOR, intern->type);
		RETURN_THROWS();
	}

	phongo_binary_get_vector_as_array(intern, return_value);
}
