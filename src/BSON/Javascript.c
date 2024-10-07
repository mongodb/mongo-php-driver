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
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"
#include "Javascript_arginfo.h"

zend_class_entry* php_phongo_javascript_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_javascript_init(php_phongo_javascript_t* intern, const char* code, size_t code_len, zval* scope)
{
	if (scope && Z_TYPE_P(scope) != IS_OBJECT && Z_TYPE_P(scope) != IS_ARRAY && Z_TYPE_P(scope) != IS_NULL) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected scope to be array or object, %s given", zend_get_type_by_const(Z_TYPE_P(scope)));
		return false;
	}

	if (strlen(code) != (size_t) code_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Code cannot contain null bytes");
		return false;
	}

	intern->code     = estrndup(code, code_len);
	intern->code_len = code_len;

	if (scope && (Z_TYPE_P(scope) == IS_OBJECT || Z_TYPE_P(scope) == IS_ARRAY)) {
		intern->scope = bson_new();
		php_phongo_zval_to_bson(scope, PHONGO_BSON_NONE, intern->scope, NULL);
	} else {
		intern->scope = NULL;
	}

	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_javascript_init_from_hash(php_phongo_javascript_t* intern, HashTable* props)
{
	zval *code, *scope;

	if ((code = zend_hash_str_find(props, "code", sizeof("code") - 1)) && Z_TYPE_P(code) == IS_STRING) {
		scope = zend_hash_str_find(props, "scope", sizeof("scope") - 1);

		return php_phongo_javascript_init(intern, Z_STRVAL_P(code), Z_STRLEN_P(code), scope);
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"code\" string field", ZSTR_VAL(php_phongo_javascript_ce->name));
	return false;
}

HashTable* php_phongo_javascript_get_properties_hash(zend_object* object, bool is_temp)
{
	php_phongo_javascript_t* intern;
	HashTable*               props;

	intern = Z_OBJ_JAVASCRIPT(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->code) {
		return props;
	}

	{
		zval code;

		ZVAL_STRING(&code, intern->code);
		zend_hash_str_update(props, "code", sizeof("code") - 1, &code);

		if (intern->scope) {
			php_phongo_bson_state state;

			PHONGO_BSON_INIT_STATE(state);
			if (!php_phongo_bson_to_zval_ex(intern->scope, &state)) {
				zval_ptr_dtor(&state.zchild);
				goto failure;
			}

			zend_hash_str_update(props, "scope", sizeof("scope") - 1, &state.zchild);
		} else {
			zval scope;

			ZVAL_NULL(&scope);
			zend_hash_str_update(props, "scope", sizeof("scope") - 1, &scope);
		}
	}

	return props;

failure:
	PHONGO_GET_PROPERTY_HASH_FREE_PROPS(is_temp, props);
	return NULL;
}

/* Construct a new BSON Javascript type. The scope is a document mapping
   identifiers and values, representing the scope in which the code string will
   be evaluated. Note that this type cannot be represented as Extended JSON. */
static PHP_METHOD(MongoDB_BSON_Javascript, __construct)
{
	php_phongo_javascript_t* intern;
	char*                    code;
	size_t                   code_len;
	zval*                    scope = NULL;

	intern = Z_JAVASCRIPT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_STRING(code, code_len)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_OBJECT_EX(scope, 1, 0)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_javascript_init(intern, code, code_len, scope);
}

static PHP_METHOD(MongoDB_BSON_Javascript, __set_state)
{
	php_phongo_javascript_t* intern;
	HashTable*               props;
	zval*                    array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_javascript_ce);

	intern = Z_JAVASCRIPT_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_javascript_init_from_hash(intern, props);
}

/* Return the Javascript's code string. */
static PHP_METHOD(MongoDB_BSON_Javascript, __toString)
{
	php_phongo_javascript_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_JAVASCRIPT_OBJ_P(getThis());

	RETURN_STRINGL(intern->code, intern->code_len);
}

static PHP_METHOD(MongoDB_BSON_Javascript, getCode)
{
	php_phongo_javascript_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_JAVASCRIPT_OBJ_P(getThis());

	RETURN_STRINGL(intern->code, intern->code_len);
}

static PHP_METHOD(MongoDB_BSON_Javascript, getScope)
{
	php_phongo_javascript_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_JAVASCRIPT_OBJ_P(getThis());

	if (!intern->scope) {
		RETURN_NULL();
	}

	if (intern->scope->len) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_STATE(state);

		if (!php_phongo_bson_to_zval_ex(intern->scope, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		RETURN_ZVAL(&state.zchild, 0, 1);
	} else {
		RETURN_NULL();
	}
}

static PHP_METHOD(MongoDB_BSON_Javascript, jsonSerialize)
{
	php_phongo_javascript_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_JAVASCRIPT_OBJ_P(getThis());

	array_init_size(return_value, 2);
	ADD_ASSOC_STRINGL(return_value, "$code", intern->code, intern->code_len);

	if (intern->scope && intern->scope->len) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_STATE(state);
		if (!php_phongo_bson_to_zval_ex(intern->scope, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		ADD_ASSOC_ZVAL_EX(return_value, "$scope", &state.zchild);
	}
}

static PHP_METHOD(MongoDB_BSON_Javascript, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_javascript_get_properties_hash(Z_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_Javascript, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_javascript_init_from_hash(Z_JAVASCRIPT_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\Javascript object handlers */
static zend_object_handlers php_phongo_handler_javascript;

static void php_phongo_javascript_free_object(zend_object* object)
{
	php_phongo_javascript_t* intern = Z_OBJ_JAVASCRIPT(object);

	zend_object_std_dtor(&intern->std);

	if (intern->code) {
		efree(intern->code);
	}
	if (intern->scope) {
		bson_destroy(intern->scope);
		intern->scope = NULL;
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

zend_object* php_phongo_javascript_create_object(zend_class_entry* class_type)
{
	php_phongo_javascript_t* intern = zend_object_alloc(sizeof(php_phongo_javascript_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_javascript;

	return &intern->std;
}

static zend_object* php_phongo_javascript_clone_object(zend_object* object)
{
	php_phongo_javascript_t* intern;
	php_phongo_javascript_t* new_intern;
	zend_object*             new_object;

	intern     = Z_OBJ_JAVASCRIPT(object);
	new_object = php_phongo_javascript_create_object(object->ce);

	new_intern = Z_OBJ_JAVASCRIPT(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_javascript_init(new_intern, intern->code, intern->code_len, NULL);
	new_intern->scope = bson_copy(intern->scope);

	return new_object;
}

static int php_phongo_javascript_compare_objects(zval* o1, zval* o2)
{
	php_phongo_javascript_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_JAVASCRIPT_OBJ_P(o1);
	intern2 = Z_JAVASCRIPT_OBJ_P(o2);

	/* Do not consider the scope document for comparisons */
	return strcmp(intern1->code, intern2->code);
}

static HashTable* php_phongo_javascript_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_javascript_get_properties_hash(object, true);
}

static HashTable* php_phongo_javascript_get_properties(zend_object* object)
{
	return php_phongo_javascript_get_properties_hash(object, false);
}

void php_phongo_javascript_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_javascript_ce                = register_class_MongoDB_BSON_Javascript(php_phongo_javascript_interface_ce, php_phongo_json_serializable_ce, php_phongo_type_ce, zend_ce_stringable);
	php_phongo_javascript_ce->create_object = php_phongo_javascript_create_object;

	memcpy(&php_phongo_handler_javascript, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_javascript.compare        = php_phongo_javascript_compare_objects;
	php_phongo_handler_javascript.clone_obj      = php_phongo_javascript_clone_object;
	php_phongo_handler_javascript.get_debug_info = php_phongo_javascript_get_debug_info;
	php_phongo_handler_javascript.get_properties = php_phongo_javascript_get_properties;
	php_phongo_handler_javascript.free_obj       = php_phongo_javascript_free_object;
	php_phongo_handler_javascript.offset         = XtOffsetOf(php_phongo_javascript_t, std);
}

bool phongo_javascript_new(zval* object, const char* code, size_t code_len, const bson_t* scope)
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
