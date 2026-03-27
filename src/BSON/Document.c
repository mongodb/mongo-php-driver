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
#include <Zend/zend_interfaces.h>
#include <Zend/zend_operators.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo.h"
#include "phongo_error.h"
#include "phongo_bson_encode.h"
#include "BSON/Document_arginfo.h"
#include "BSON/Iterator.h"

zend_class_entry* phongo_document_ce;

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_document_init_from_hash(phongo_document_t* intern, HashTable* props)
{
	zval* data;

	if ((data = zend_hash_str_find(props, "data", sizeof("data") - 1)) && Z_TYPE_P(data) == IS_STRING) {
		zend_string* decoded = php_base64_decode_str(Z_STR_P(data));

		intern->bson = bson_new_from_data((const uint8_t*) ZSTR_VAL(decoded), ZSTR_LEN(decoded));
		zend_string_free(decoded);

		if (intern->bson == NULL) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires valid BSON", ZSTR_VAL(phongo_document_ce->name));
			return false;
		}

		return true;
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"data\" string field", ZSTR_VAL(phongo_document_ce->name));
	return false;
}

static HashTable* phongo_document_get_properties_hash(zend_object* object, bool is_temp, int size)
{
	PHONGO_INTERN_FROM_Z_OBJ(document, object);

	HashTable* props;

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, size);

	if (!intern->bson) {
		return props;
	}

	{
		zval data;

		ZVAL_STR(&data, php_base64_encode((const unsigned char*) bson_get_data(intern->bson), intern->bson->len));
		zend_hash_str_update(props, "data", sizeof("data") - 1, &data);
	}

	return props;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_BSON_Document)

static PHP_METHOD(MongoDB_BSON_Document, fromBSON)
{
	zval           zv;
	zend_string*   bson_string;
	const bson_t*  bson;
	bson_reader_t* reader;
	bool           eof = false;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STR(bson_string)
	PHONGO_PARSE_PARAMETERS_END();

	reader = bson_reader_new_from_data((const uint8_t*) ZSTR_VAL(bson_string), ZSTR_LEN(bson_string));
	if (!(bson = bson_reader_read(reader, NULL))) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not read document from BSON reader");

		goto cleanup;
	}

	PHONGO_INTERN_INIT_EX(document, &zv);
	intern->bson = bson_copy(bson);

	if (bson_reader_read(reader, &eof) || !eof) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Reading document did not exhaust input buffer");
		zval_ptr_dtor(&zv);

		goto cleanup;
	}

	RETVAL_ZVAL(&zv, 1, 1);

cleanup:
	if (reader) {
		bson_reader_destroy(reader);
	}
}

static PHP_METHOD(MongoDB_BSON_Document, fromJSON)
{
	zval         zv;
	zend_string* json;
	bson_t*      bson;
	bson_error_t error;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STR(json)
	PHONGO_PARSE_PARAMETERS_END();

	bson = bson_new_from_json((const uint8_t*) ZSTR_VAL(json), ZSTR_LEN(json), &error);
	if (!bson) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s", error.domain == BSON_ERROR_JSON ? error.message : "Error parsing JSON");
		return;
	}

	PHONGO_INTERN_INIT_EX(document, &zv);
	intern->bson = bson;

	RETURN_ZVAL(&zv, 1, 1);
}

static PHP_METHOD(MongoDB_BSON_Document, fromPHP)
{
	zval  zv;
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY_OR_OBJECT(data)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(document, &zv);
	intern->bson = bson_new();

	// Explicitly allow constructing a Document from a PackedArray
	phongo_zval_to_bson(data, PHONGO_BSON_ALLOW_ROOT_ARRAY, intern->bson, NULL);

	RETURN_ZVAL(&zv, 1, 1);
}

static bool phongo_document_get(phongo_document_t* intern, char* key, size_t key_len, zval* return_value, bool null_if_missing)
{
	bson_iter_t iter;

	if (!bson_iter_init(&iter, intern->bson)) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Could not initialize BSON iterator");
		return false;
	}

	if (!bson_iter_find_w_len(&iter, key, key_len)) {
		if (null_if_missing) {
			ZVAL_NULL(return_value);
			return true;
		}

		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Could not find key \"%s\" in BSON document", key);
		return false;
	}

	phongo_bson_value_to_zval(bson_iter_value(&iter), return_value);

	return true;
}

static bool phongo_document_get_by_zval(phongo_document_t* intern, zval* key, zval* return_value, bool null_if_missing)
{
	if (Z_TYPE_P(key) != IS_STRING && Z_TYPE_P(key) != IS_LONG) {
		if (null_if_missing) {
			ZVAL_NULL(return_value);
			return true;
		}

		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Could not find key of type \"%s\" in BSON document", zend_zval_type_name(key));
		return false;
	}

	zend_string* tmp_str;
	zend_string* str = zval_try_get_tmp_string(key, &tmp_str);

	if (!str) {
		// Exception already thrown
		return false;
	}

	if (!phongo_document_get(intern, ZSTR_VAL(str), ZSTR_LEN(str), return_value, null_if_missing)) {
		// Exception already thrown
		zend_tmp_string_release(tmp_str);
		return false;
	}

	zend_tmp_string_release(tmp_str);
	return true;
}

static PHP_METHOD(MongoDB_BSON_Document, get)
{
	PHONGO_INTERN_FROM_THIS(document);

	char*  key;
	size_t key_len;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(key, key_len)
	PHONGO_PARSE_PARAMETERS_END();

	// May throw, in which case we do nothing
	phongo_document_get(intern, key, key_len, return_value, false);
}

static PHP_METHOD(MongoDB_BSON_Document, getIterator)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_iterator_init(return_value, getThis());
}

static bool phongo_document_has(phongo_document_t* intern, char* key, size_t key_len)
{
	bson_iter_t iter;

	if (!bson_iter_init(&iter, intern->bson)) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Could not initialize BSON iterator");
		return false;
	}

	return bson_iter_find_w_len(&iter, key, key_len);
}

static bool phongo_document_has_by_zval(phongo_document_t* intern, zval* key)
{
	if (Z_TYPE_P(key) != IS_STRING && Z_TYPE_P(key) != IS_LONG) {
		return false;
	}

	zend_string* tmp_str;
	zend_string* str = zval_try_get_tmp_string(key, &tmp_str);

	if (!str) {
		// Exception already thrown
		return false;
	}

	if (!phongo_document_has(intern, ZSTR_VAL(str), ZSTR_LEN(str))) {
		// Exception may be thrown if BSON iterator could not be initialized
		zend_tmp_string_release(tmp_str);
		return false;
	}

	zend_tmp_string_release(tmp_str);
	return true;
}

static PHP_METHOD(MongoDB_BSON_Document, has)
{
	PHONGO_INTERN_FROM_THIS(document);

	char*  key;
	size_t key_len;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(key, key_len)
	PHONGO_PARSE_PARAMETERS_END();

	RETURN_BOOL(phongo_document_has(intern, key, key_len));
}

static PHP_METHOD(MongoDB_BSON_Document, toCanonicalExtendedJSON)
{
	PHONGO_INTERN_FROM_THIS(document);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_bson_to_json(return_value, intern->bson, PHONGO_JSON_MODE_CANONICAL);
}

static PHP_METHOD(MongoDB_BSON_Document, toRelaxedExtendedJSON)
{
	PHONGO_INTERN_FROM_THIS(document);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_bson_to_json(return_value, intern->bson, PHONGO_JSON_MODE_RELAXED);
}

static PHP_METHOD(MongoDB_BSON_Document, toPHP)
{
	PHONGO_INTERN_FROM_THIS(document);

	zval*             typemap = NULL;
	phongo_bson_state state;

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY(typemap)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_BSON_INIT_STATE(state);

	if (!phongo_bson_typemap_to_state(typemap, &state.map)) {
		return;
	}

	state.map.int64_as_object = true;

	if (!phongo_bson_to_zval_ex(intern->bson, &state)) {
		zval_ptr_dtor(&state.zchild);
		phongo_bson_typemap_dtor(&state.map);
		RETURN_NULL();
	}

	phongo_bson_typemap_dtor(&state.map);

	RETURN_ZVAL(&state.zchild, 0, 1);
}

static PHP_METHOD(MongoDB_BSON_Document, offsetExists)
{
	PHONGO_INTERN_FROM_THIS(document);

	zval* offset;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ZVAL(offset)
	PHONGO_PARSE_PARAMETERS_END();

	RETURN_BOOL(phongo_document_has_by_zval(intern, offset));
}

static PHP_METHOD(MongoDB_BSON_Document, offsetGet)
{
	PHONGO_INTERN_FROM_THIS(document);

	zval* offset;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ZVAL(offset)
	PHONGO_PARSE_PARAMETERS_END();

	// May throw, in which case we do nothing
	phongo_document_get_by_zval(intern, offset, return_value, false);
}

static PHP_METHOD(MongoDB_BSON_Document, offsetSet)
{
	phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cannot write to %s property", ZSTR_VAL(phongo_document_ce->name));
}

static PHP_METHOD(MongoDB_BSON_Document, offsetUnset)
{
	phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cannot unset %s property", ZSTR_VAL(phongo_document_ce->name));
}

static PHP_METHOD(MongoDB_BSON_Document, __toString)
{
	PHONGO_INTERN_FROM_THIS(document);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_STRINGL((const char*) bson_get_data(intern->bson), intern->bson->len);
}

static PHP_METHOD(MongoDB_BSON_Document, __set_state)
{
	HashTable* props;
	zval*      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(document, return_value);
	props = Z_ARRVAL_P(array);

	phongo_document_init_from_hash(intern, props);
}

static PHP_METHOD(MongoDB_BSON_Document, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(phongo_document_get_properties_hash(Z_OBJ_P(getThis()), true, 1));
}

static PHP_METHOD(MongoDB_BSON_Document, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_document_init_from_hash(Z_DOCUMENT_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\BSON object handlers */
static zend_object_handlers phongo_handler_document;

static void phongo_document_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(document, object);

	zend_object_std_dtor(&intern->std);

	if (intern->bson) {
		bson_destroy(intern->bson);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* phongo_document_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(document, class_type);

	intern->std.handlers = &phongo_handler_document;

	return &intern->std;
}

static zend_object* phongo_document_clone_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(document, object);

	phongo_document_t* new_intern;
	zend_object*       new_object;

	new_object = phongo_document_create_object(object->ce);

	new_intern = Z_OBJ_DOCUMENT(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	new_intern->bson = bson_copy(intern->bson);

	return new_object;
}

static int phongo_document_compare_objects(zval* o1, zval* o2)
{
	phongo_document_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_DOCUMENT_OBJ_P(o1);
	intern2 = Z_DOCUMENT_OBJ_P(o2);

	return bson_compare(intern1->bson, intern2->bson);
}

static HashTable* phongo_document_get_debug_info(zend_object* object, int* is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(document, object);

	HashTable* props;

	*is_temp = 1;

	/* This get_debug_info handler reports an additional property. This does not
	 * conflict with other uses of phongo_document_get_properties_hash since
	 * we always allocated a new HashTable with is_temp=true. */
	props = phongo_document_get_properties_hash(object, true, 2);

	{
		phongo_bson_state state;

		PHONGO_BSON_INIT_STATE(state);
		state.map.array.type    = PHONGO_TYPEMAP_BSON;
		state.map.document.type = PHONGO_TYPEMAP_BSON;
		if (!phongo_bson_to_zval_ex(intern->bson, &state)) {
			zval_ptr_dtor(&state.zchild);
			goto failure;
		}

		zend_hash_str_update(props, "value", sizeof("value") - 1, &state.zchild);
	}

	return props;

failure:
	PHONGO_GET_PROPERTY_HASH_FREE_PROPS(is_temp, props);
	return NULL;
}

static HashTable* phongo_document_get_properties(zend_object* object)
{
	return phongo_document_get_properties_hash(object, false, 1);
}

zval* phongo_document_read_property(zend_object* object, zend_string* member, int type, void** cache_slot, zval* rv)
{
	PHONGO_INTERN_FROM_Z_OBJ(document, object);

	if (!phongo_document_get(intern, ZSTR_VAL(member), ZSTR_LEN(member), rv, type == BP_VAR_IS)) {
		// Exception already thrown
		return &EG(uninitialized_zval);
	}

	return rv;
}

zval* phongo_document_write_property(zend_object* object, zend_string* member, zval* value, void** cache_slot)
{
	phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cannot write to %s property", ZSTR_VAL(phongo_document_ce->name));
	return value;
}

int phongo_document_has_property(zend_object* object, zend_string* member, int has_set_exists, void** cache_slot)
{
	PHONGO_INTERN_FROM_Z_OBJ(document, object);

	return phongo_document_has(intern, ZSTR_VAL(member), ZSTR_LEN(member));
}

void phongo_document_unset_property(zend_object* object, zend_string* member, void** cache_slot)
{
	phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cannot unset %s property", ZSTR_VAL(phongo_document_ce->name));
}

zval* phongo_document_read_dimension(zend_object* object, zval* offset, int type, zval* rv)
{
	PHONGO_INTERN_FROM_Z_OBJ(document, object);

	if (!phongo_document_get_by_zval(intern, offset, rv, type == BP_VAR_IS)) {
		// Exception already thrown
		return &EG(uninitialized_zval);
	}

	return rv;
}

void phongo_document_write_dimension(zend_object* object, zval* offset, zval* value)
{
	phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cannot write to %s property", ZSTR_VAL(phongo_document_ce->name));
}

int phongo_document_has_dimension(zend_object* object, zval* member, int check_empty)
{
	PHONGO_INTERN_FROM_Z_OBJ(document, object);

	return phongo_document_has_by_zval(intern, member);
}

void phongo_document_unset_dimension(zend_object* object, zval* offset)
{
	phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cannot unset %s property", ZSTR_VAL(phongo_document_ce->name));
}

void phongo_document_init_ce(INIT_FUNC_ARGS)
{
	phongo_document_ce                = register_class_MongoDB_BSON_Document(zend_ce_aggregate, zend_ce_arrayaccess, phongo_type_ce, zend_ce_stringable);
	phongo_document_ce->create_object = phongo_document_create_object;

	memcpy(&phongo_handler_document, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_document.compare         = phongo_document_compare_objects;
	phongo_handler_document.clone_obj       = phongo_document_clone_object;
	phongo_handler_document.get_debug_info  = phongo_document_get_debug_info;
	phongo_handler_document.get_properties  = phongo_document_get_properties;
	phongo_handler_document.free_obj        = phongo_document_free_object;
	phongo_handler_document.read_property   = phongo_document_read_property;
	phongo_handler_document.write_property  = phongo_document_write_property;
	phongo_handler_document.has_property    = phongo_document_has_property;
	phongo_handler_document.unset_property  = phongo_document_unset_property;
	phongo_handler_document.read_dimension  = phongo_document_read_dimension;
	phongo_handler_document.write_dimension = phongo_document_write_dimension;
	phongo_handler_document.has_dimension   = phongo_document_has_dimension;
	phongo_handler_document.unset_dimension = phongo_document_unset_dimension;
	phongo_handler_document.offset          = XtOffsetOf(phongo_document_t, std);
}

bool phongo_document_new(zval* object, bson_t* bson, bool copy)
{
	PHONGO_INTERN_INIT_EX(document, object);
	intern->bson = copy ? bson_copy(bson) : bson;

	return true;
}
