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
#include <ext/standard/php_var.h>
#include <zend_smart_str.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_bson_encode.h"
#include "BSON/Document_arginfo.h"
#include "BSON/Iterator.h"

zend_class_entry* php_phongo_document_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_document_init(php_phongo_document_t* intern, bson_t* bson)
{
	intern->bson = bson;

	return true;
}

static HashTable* php_phongo_document_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_document_t* intern;
	HashTable*             props;

	intern = Z_OBJ_DOCUMENT(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->bson) {
		return props;
	}

	{
		zval data, length;

		ZVAL_STRINGL(&data, (const char*) bson_get_data(intern->bson), intern->bson->len);
		zend_hash_str_update(props, "data", sizeof("data") - 1, &data);

		ZVAL_LONG(&length, intern->bson->len);
		zend_hash_str_update(props, "length", sizeof("length") - 1, &length);
	}

	return props;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_BSON_Document)

static PHP_METHOD(MongoDB_BSON_Document, fromBSONString)
{
	zval                   zv;
	php_phongo_document_t* intern = NULL;
	zend_string*           bson_string;
	const bson_t*          bson;
	bson_reader_t*         reader;
	bool                   eof = false;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STR(bson_string)
	PHONGO_PARSE_PARAMETERS_END();

	reader = bson_reader_new_from_data((const uint8_t*) ZSTR_VAL(bson_string), ZSTR_LEN(bson_string));
	if (!(bson = bson_reader_read(reader, NULL))) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not read document from BSON reader");

		goto cleanup;
	}

	object_init_ex(&zv, php_phongo_document_ce);
	intern       = Z_DOCUMENT_OBJ_P(&zv);
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
	zval                   zv;
	php_phongo_document_t* intern;
	zend_string*           json;
	bson_t*                bson;
	bson_error_t           error;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STR(json)
	PHONGO_PARSE_PARAMETERS_END();

	bson = bson_new_from_json((const uint8_t*) ZSTR_VAL(json), ZSTR_LEN(json), &error);
	if (!bson) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s", error.domain == BSON_ERROR_JSON ? error.message : "Error parsing JSON");
		return;
	}

	object_init_ex(&zv, php_phongo_document_ce);
	intern       = Z_DOCUMENT_OBJ_P(&zv);
	intern->bson = bson;

	RETURN_ZVAL(&zv, 1, 1);
}

static PHP_METHOD(MongoDB_BSON_Document, fromPHP)
{
	zval                   zv;
	php_phongo_document_t* intern;
	zval*                  data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	PHONGO_PARAM_ARRAY_OR_OBJECT(data)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(&zv, php_phongo_document_ce);
	intern = Z_DOCUMENT_OBJ_P(&zv);

	intern->bson = bson_new();
	php_phongo_zval_to_bson(data, PHONGO_BSON_NONE, intern->bson, NULL);

	RETURN_ZVAL(&zv, 1, 1);
}

static PHP_METHOD(MongoDB_BSON_Document, get)
{
	php_phongo_document_t* intern;
	char*                  key;
	size_t                 key_len;
	bson_iter_t            iter;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(key, key_len)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_DOCUMENT_OBJ_P(getThis());
	if (!bson_iter_init(&iter, intern->bson)) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Could not initialize BSON iterator.");
	}

	if (!bson_iter_find_w_len(&iter, key, key_len)) {
		RETURN_NULL();
	}

	php_phongo_bson_iter_to_zval(return_value, &iter);
}

static PHP_METHOD(MongoDB_BSON_Document, getIterator)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_iterator_init(return_value, getThis());
}

static PHP_METHOD(MongoDB_BSON_Document, has)
{
	php_phongo_document_t* intern;
	char*                  key;
	size_t                 key_len;
	bson_iter_t            iter;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(key, key_len)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_DOCUMENT_OBJ_P(getThis());
	if (!bson_iter_init(&iter, intern->bson)) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Could not initialize BSON iterator.");
	}

	RETURN_BOOL(bson_iter_find_w_len(&iter, key, key_len));
}

static PHP_METHOD(MongoDB_BSON_Document, toCanonicalExtendedJSON)
{
	php_phongo_document_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_DOCUMENT_OBJ_P(getThis());

	php_phongo_bson_to_json(return_value, intern->bson, PHONGO_JSON_MODE_CANONICAL);
}

static PHP_METHOD(MongoDB_BSON_Document, toRelaxedExtendedJSON)
{
	php_phongo_document_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_DOCUMENT_OBJ_P(getThis());

	php_phongo_bson_to_json(return_value, intern->bson, PHONGO_JSON_MODE_RELAXED);
}

static PHP_METHOD(MongoDB_BSON_Document, toPHP)
{
	php_phongo_document_t* intern;
	zval*                  typemap = NULL;
	php_phongo_bson_state  state;

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY(typemap)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_BSON_INIT_STATE(state);

	if (!php_phongo_bson_typemap_to_state(typemap, &state.map)) {
		return;
	}

	intern = Z_DOCUMENT_OBJ_P(getThis());

	if (!php_phongo_bson_to_zval_ex(intern->bson, &state)) {
		zval_ptr_dtor(&state.zchild);
		php_phongo_bson_typemap_dtor(&state.map);
		RETURN_NULL();
	}

	php_phongo_bson_typemap_dtor(&state.map);

	RETURN_ZVAL(&state.zchild, 0, 1);
}

static PHP_METHOD(MongoDB_BSON_Document, __toString)
{
	php_phongo_document_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_DOCUMENT_OBJ_P(getThis());

	RETVAL_STRINGL((const char*) bson_get_data(intern->bson), intern->bson->len);
}

/* MongoDB\BSON\BSON object handlers */
static zend_object_handlers php_phongo_handler_document;

static void php_phongo_document_free_object(zend_object* object)
{
	php_phongo_document_t* intern = Z_OBJ_DOCUMENT(object);

	zend_object_std_dtor(&intern->std);

	if (intern->bson) {
		bson_destroy(intern->bson);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* php_phongo_document_create_object(zend_class_entry* class_type)
{
	php_phongo_document_t* intern = zend_object_alloc(sizeof(php_phongo_document_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_document;

	return &intern->std;
}

static zend_object* php_phongo_document_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_document_t* intern;
	php_phongo_document_t* new_intern;
	zend_object*           new_object;

	intern     = Z_OBJ_DOCUMENT(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_document_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_DOCUMENT(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_document_init(new_intern, bson_copy(intern->bson));

	return new_object;
}

static int php_phongo_document_compare_objects(zval* o1, zval* o2)
{
	php_phongo_document_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_DOCUMENT_OBJ_P(o1);
	intern2 = Z_DOCUMENT_OBJ_P(o2);

	return bson_compare(intern1->bson, intern2->bson);
}

static HashTable* php_phongo_document_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_document_get_properties_hash(object, true);
}

static HashTable* php_phongo_document_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_document_get_properties_hash(object, false);
}

void php_phongo_document_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_document_ce                = register_class_MongoDB_BSON_Document(zend_ce_aggregate);
	php_phongo_document_ce->create_object = php_phongo_document_create_object;

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_document_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_document, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(document);
	php_phongo_handler_document.clone_obj      = php_phongo_document_clone_object;
	php_phongo_handler_document.get_debug_info = php_phongo_document_get_debug_info;
	php_phongo_handler_document.get_properties = php_phongo_document_get_properties;
	php_phongo_handler_document.free_obj       = php_phongo_document_free_object;
	php_phongo_handler_document.offset         = XtOffsetOf(php_phongo_document_t, std);
}
