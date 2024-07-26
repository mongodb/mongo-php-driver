/*
 * Copyright 2015-present MongoDB, Inc.
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

#include "mongoc/mongoc.h"

#include <php.h>
#include <zend_smart_str.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "ReadConcern_arginfo.h"

zend_class_entry* php_phongo_readconcern_ce;

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_readconcern_init_from_hash(php_phongo_readconcern_t* intern, HashTable* props)
{
	zval* level;

	intern->read_concern = mongoc_read_concern_new();

	if ((level = zend_hash_str_find(props, "level", sizeof("level") - 1))) {
		if (Z_TYPE_P(level) == IS_STRING) {
			mongoc_read_concern_set_level(intern->read_concern, Z_STRVAL_P(level));
			return true;
		}

		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"level\" string field", ZSTR_VAL(php_phongo_readconcern_ce->name));
		goto failure;
	}

	return true;

failure:
	mongoc_read_concern_destroy(intern->read_concern);
	intern->read_concern = NULL;
	return false;
}

/* Constructs a new ReadConcern */
static PHP_METHOD(MongoDB_Driver_ReadConcern, __construct)
{
	php_phongo_readconcern_t* intern;
	char*                     level     = NULL;
	size_t                    level_len = 0;

	intern = Z_READCONCERN_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_STRING_OR_NULL(level, level_len)
	PHONGO_PARSE_PARAMETERS_END();

	intern->read_concern = mongoc_read_concern_new();

	if (level) {
		mongoc_read_concern_set_level(intern->read_concern, level);
	}
}

static PHP_METHOD(MongoDB_Driver_ReadConcern, __set_state)
{
	php_phongo_readconcern_t* intern;
	HashTable*                props;
	zval*                     array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_readconcern_ce);

	intern = Z_READCONCERN_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_readconcern_init_from_hash(intern, props);
}

/* Returns the ReadConcern "level" option */
static PHP_METHOD(MongoDB_Driver_ReadConcern, getLevel)
{
	php_phongo_readconcern_t* intern;
	const char*               level;

	intern = Z_READCONCERN_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	level = mongoc_read_concern_get_level(intern->read_concern);

	if (level) {
		RETURN_STRING(level);
	}

	RETURN_NULL();
}

/* Returns whether the read concern has not been modified (i.e. constructed
   without a level or from a Manager with no read concern URI options). */
static PHP_METHOD(MongoDB_Driver_ReadConcern, isDefault)
{
	php_phongo_readconcern_t* intern;

	intern = Z_READCONCERN_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(mongoc_read_concern_is_default(intern->read_concern));
}

static HashTable* php_phongo_readconcern_get_properties_hash(zend_object* object, bool is_temp)
{
	php_phongo_readconcern_t* intern;
	HashTable*                props;
	const char*               level;

	intern = Z_OBJ_READCONCERN(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 1);

	if (!intern->read_concern) {
		return props;
	}

	level = mongoc_read_concern_get_level(intern->read_concern);

	if (level) {
		zval z_level;

		ZVAL_STRING(&z_level, level);
		zend_hash_str_update(props, "level", sizeof("level") - 1, &z_level);
	}

	return props;
}

static PHP_METHOD(MongoDB_Driver_ReadConcern, bsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	ZVAL_ARR(return_value, php_phongo_readconcern_get_properties_hash(Z_OBJ_P(getThis()), true));
	convert_to_object(return_value);
}

static PHP_METHOD(MongoDB_Driver_ReadConcern, serialize)
{
	php_phongo_readconcern_t* intern;
	zval                      retval;
	php_serialize_data_t      var_hash;
	smart_str                 buf = { 0 };
	const char*               level;

	intern = Z_READCONCERN_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->read_concern) {
		return;
	}

	level = mongoc_read_concern_get_level(intern->read_concern);

	if (!level) {
		RETURN_STRING("");
	}

	array_init_size(&retval, 1);
	ADD_ASSOC_STRING(&retval, "level", level);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_Driver_ReadConcern, unserialize)
{
	php_phongo_readconcern_t* intern;
	char*                     serialized;
	size_t                    serialized_len;
	zval                      props;
	php_unserialize_data_t    var_hash;

	intern = Z_READCONCERN_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	if (!serialized_len) {
		return;
	}

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_readconcern_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_readconcern_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_Driver_ReadConcern, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_readconcern_get_properties_hash(Z_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_Driver_ReadConcern, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_readconcern_init_from_hash(Z_READCONCERN_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\Driver\ReadConcern object handlers */
static zend_object_handlers php_phongo_handler_readconcern;

static void php_phongo_readconcern_free_object(zend_object* object)
{
	php_phongo_readconcern_t* intern = Z_OBJ_READCONCERN(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->read_concern) {
		mongoc_read_concern_destroy(intern->read_concern);
	}
}

static zend_object* php_phongo_readconcern_create_object(zend_class_entry* class_type)
{
	php_phongo_readconcern_t* intern = zend_object_alloc(sizeof(php_phongo_readconcern_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_readconcern;

	return &intern->std;
}

static HashTable* php_phongo_readconcern_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_readconcern_get_properties_hash(object, true);
}

static HashTable* php_phongo_readconcern_get_properties(zend_object* object)
{
	return php_phongo_readconcern_get_properties_hash(object, false);
}

void php_phongo_readconcern_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_readconcern_ce                = register_class_MongoDB_Driver_ReadConcern(php_phongo_serializable_ce, zend_ce_serializable);
	php_phongo_readconcern_ce->create_object = php_phongo_readconcern_create_object;

	memcpy(&php_phongo_handler_readconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_readconcern.get_debug_info = php_phongo_readconcern_get_debug_info;
	php_phongo_handler_readconcern.get_properties = php_phongo_readconcern_get_properties;
	php_phongo_handler_readconcern.free_obj       = php_phongo_readconcern_free_object;
	php_phongo_handler_readconcern.offset         = XtOffsetOf(php_phongo_readconcern_t, std);
}

void phongo_readconcern_init(zval* return_value, const mongoc_read_concern_t* read_concern)
{
	php_phongo_readconcern_t* intern;

	object_init_ex(return_value, php_phongo_readconcern_ce);

	intern               = Z_READCONCERN_OBJ_P(return_value);
	intern->read_concern = mongoc_read_concern_copy(read_concern);
}

const mongoc_read_concern_t* phongo_read_concern_from_zval(zval* zread_concern)
{
	if (zread_concern) {
		php_phongo_readconcern_t* intern = Z_READCONCERN_OBJ_P(zread_concern);

		if (intern) {
			return intern->read_concern;
		}
	}

	return NULL;
}

void php_phongo_read_concern_to_zval(zval* retval, const mongoc_read_concern_t* read_concern)
{
	const char* level = mongoc_read_concern_get_level(read_concern);

	array_init_size(retval, 1);

	if (level) {
		ADD_ASSOC_STRING(retval, "level", level);
	}
}
