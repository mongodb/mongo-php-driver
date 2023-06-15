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
#include <zend_smart_str.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "Symbol_arginfo.h"

zend_class_entry* php_phongo_symbol_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_symbol_init(php_phongo_symbol_t* intern, const char* symbol, size_t symbol_len)
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
static bool php_phongo_symbol_init_from_hash(php_phongo_symbol_t* intern, HashTable* props)
{
	zval* symbol;

	if ((symbol = zend_hash_str_find(props, "symbol", sizeof("symbol") - 1)) && Z_TYPE_P(symbol) == IS_STRING) {
		return php_phongo_symbol_init(intern, Z_STRVAL_P(symbol), Z_STRLEN_P(symbol));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"symbol\" string field", ZSTR_VAL(php_phongo_symbol_ce->name));
	return false;
}

HashTable* php_phongo_symbol_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_symbol_t* intern;
	HashTable*           props;

	intern = Z_OBJ_SYMBOL(PHONGO_COMPAT_GET_OBJ(object));

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
	php_phongo_symbol_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_SYMBOL_OBJ_P(getThis());

	RETURN_STRINGL(intern->symbol, intern->symbol_len);
}

static PHP_METHOD(MongoDB_BSON_Symbol, __set_state)
{
	php_phongo_symbol_t* intern;
	HashTable*           props;
	zval*                array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_symbol_ce);

	intern = Z_SYMBOL_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_symbol_init_from_hash(intern, props);
}

static PHP_METHOD(MongoDB_BSON_Symbol, jsonSerialize)
{
	php_phongo_symbol_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_SYMBOL_OBJ_P(getThis());

	array_init_size(return_value, 1);
	ADD_ASSOC_STRINGL(return_value, "$symbol", intern->symbol, intern->symbol_len);
}

static PHP_METHOD(MongoDB_BSON_Symbol, serialize)
{
	php_phongo_symbol_t* intern;
	zval                 retval;
	php_serialize_data_t var_hash;
	smart_str            buf = { 0 };

	intern = Z_SYMBOL_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(&retval, 1);
	ADD_ASSOC_STRINGL(&retval, "symbol", intern->symbol, intern->symbol_len);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_BSON_Symbol, unserialize)
{
	php_phongo_symbol_t*   intern;
	char*                  serialized;
	size_t                 serialized_len;
	zval                   props;
	php_unserialize_data_t var_hash;

	intern = Z_SYMBOL_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_symbol_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_symbol_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_BSON_Symbol, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	ZVAL_ARR(return_value, php_phongo_symbol_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_Symbol, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_symbol_init_from_hash(Z_SYMBOL_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\Symbol object handlers */
static zend_object_handlers php_phongo_handler_symbol;

static void php_phongo_symbol_free_object(zend_object* object)
{
	php_phongo_symbol_t* intern = Z_OBJ_SYMBOL(object);

	zend_object_std_dtor(&intern->std);

	if (intern->symbol) {
		efree(intern->symbol);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

zend_object* php_phongo_symbol_create_object(zend_class_entry* class_type)
{
	php_phongo_symbol_t* intern = zend_object_alloc(sizeof(php_phongo_symbol_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_symbol;

	return &intern->std;
}

static zend_object* php_phongo_symbol_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_symbol_t* intern;
	php_phongo_symbol_t* new_intern;
	zend_object*         new_object;

	intern     = Z_OBJ_SYMBOL(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_symbol_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_SYMBOL(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_symbol_init(new_intern, intern->symbol, intern->symbol_len);

	return new_object;
}

static int php_phongo_symbol_compare_objects(zval* o1, zval* o2)
{
	php_phongo_symbol_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_SYMBOL_OBJ_P(o1);
	intern2 = Z_SYMBOL_OBJ_P(o2);

	return strcmp(intern1->symbol, intern2->symbol);
}

static HashTable* php_phongo_symbol_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_symbol_get_properties_hash(object, true);
}

static HashTable* php_phongo_symbol_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_symbol_get_properties_hash(object, false);
}

void php_phongo_symbol_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_symbol_ce                = register_class_MongoDB_BSON_Symbol(php_phongo_json_serializable_ce, php_phongo_type_ce, zend_ce_serializable);
	php_phongo_symbol_ce->create_object = php_phongo_symbol_create_object;

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_symbol_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_symbol, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(symbol);
	php_phongo_handler_symbol.clone_obj      = php_phongo_symbol_clone_object;
	php_phongo_handler_symbol.get_debug_info = php_phongo_symbol_get_debug_info;
	php_phongo_handler_symbol.get_properties = php_phongo_symbol_get_properties;
	php_phongo_handler_symbol.free_obj       = php_phongo_symbol_free_object;
	php_phongo_handler_symbol.offset         = XtOffsetOf(php_phongo_symbol_t, std);
}

bool phongo_symbol_new(zval* object, const char* symbol, size_t symbol_len)
{
	php_phongo_symbol_t* intern;

	object_init_ex(object, php_phongo_symbol_ce);

	intern             = Z_SYMBOL_OBJ_P(object);
	intern->symbol     = estrndup(symbol, symbol_len);
	intern->symbol_len = symbol_len;

	return true;
}
