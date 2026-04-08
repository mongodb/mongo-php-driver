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
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_error.h"
#include "Symbol_arginfo.h"

zend_class_entry* phongo_symbol_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool phongo_symbol_init(phongo_symbol_t* intern, const char* symbol, size_t symbol_len)
{
	if (strlen(symbol) != (size_t) symbol_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Symbol cannot contain null bytes");
		return false;
	}

	intern->symbol     = estrndup(symbol, symbol_len);
	intern->symbol_len = symbol_len;

	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_symbol_init_from_hash(phongo_symbol_t* intern, HashTable* props)
{
	zval* symbol;

	if ((symbol = zend_hash_str_find(props, "symbol", sizeof("symbol") - 1)) && Z_TYPE_P(symbol) == IS_STRING) {
		return phongo_symbol_init(intern, Z_STRVAL_P(symbol), Z_STRLEN_P(symbol));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"symbol\" string field", ZSTR_VAL(phongo_symbol_ce->name));
	return false;
}

HashTable* phongo_symbol_get_properties_hash(zend_object* object, bool is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(symbol, object);

	HashTable* props;

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->symbol) {
		return props;
	}

	{
		zval symbol;

		ZVAL_STRING(&symbol, intern->symbol);
		zend_hash_str_update(props, "symbol", sizeof("symbol") - 1, &symbol);
	}

	return props;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_BSON_Symbol)

/* Return the Symbol's symbol string. */
static PHP_METHOD(MongoDB_BSON_Symbol, __toString)
{
	PHONGO_INTERN_FROM_THIS(symbol);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRINGL(intern->symbol, intern->symbol_len);
}

static PHP_METHOD(MongoDB_BSON_Symbol, __set_state)
{
	HashTable* props;
	zval*      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(symbol, return_value);
	props = Z_ARRVAL_P(array);

	phongo_symbol_init_from_hash(intern, props);
}

static PHP_METHOD(MongoDB_BSON_Symbol, jsonSerialize)
{
	PHONGO_INTERN_FROM_THIS(symbol);

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 1);
	ADD_ASSOC_STRINGL(return_value, "$symbol", intern->symbol, intern->symbol_len);
}

static PHP_METHOD(MongoDB_BSON_Symbol, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	ZVAL_ARR(return_value, phongo_symbol_get_properties_hash(Z_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_Symbol, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_symbol_init_from_hash(Z_SYMBOL_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\Symbol object handlers */
static zend_object_handlers phongo_handler_symbol;

static void phongo_symbol_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(symbol, object);

	zend_object_std_dtor(&intern->std);

	if (intern->symbol) {
		efree(intern->symbol);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

zend_object* phongo_symbol_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(symbol, class_type);

	intern->std.handlers = &phongo_handler_symbol;

	return &intern->std;
}

static zend_object* phongo_symbol_clone_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(symbol, object);

	phongo_symbol_t* new_intern;
	zend_object*     new_object;

	new_object = phongo_symbol_create_object(object->ce);

	new_intern = Z_OBJ_SYMBOL(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	phongo_symbol_init(new_intern, intern->symbol, intern->symbol_len);

	return new_object;
}

static int phongo_symbol_compare_objects(zval* o1, zval* o2)
{
	phongo_symbol_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_SYMBOL_OBJ_P(o1);
	intern2 = Z_SYMBOL_OBJ_P(o2);

	return strcmp(intern1->symbol, intern2->symbol);
}

static HashTable* phongo_symbol_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return phongo_symbol_get_properties_hash(object, true);
}

static HashTable* phongo_symbol_get_properties(zend_object* object)
{
	return phongo_symbol_get_properties_hash(object, false);
}

void phongo_symbol_init_ce(INIT_FUNC_ARGS)
{
	phongo_symbol_ce                = register_class_MongoDB_BSON_Symbol(phongo_json_serializable_ce, phongo_type_ce, zend_ce_stringable);
	phongo_symbol_ce->create_object = phongo_symbol_create_object;

	memcpy(&phongo_handler_symbol, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_symbol.compare        = phongo_symbol_compare_objects;
	phongo_handler_symbol.clone_obj      = phongo_symbol_clone_object;
	phongo_handler_symbol.get_debug_info = phongo_symbol_get_debug_info;
	phongo_handler_symbol.get_properties = phongo_symbol_get_properties;
	phongo_handler_symbol.free_obj       = phongo_symbol_free_object;
	phongo_handler_symbol.offset         = XtOffsetOf(phongo_symbol_t, std);
}

bool phongo_symbol_new(zval* object, const char* symbol, size_t symbol_len)
{
	PHONGO_INTERN_INIT_EX(symbol, object);
	intern->symbol     = estrndup(symbol, symbol_len);
	intern->symbol_len = symbol_len;

	return true;
}
