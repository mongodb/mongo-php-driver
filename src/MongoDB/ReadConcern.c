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
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_error.h"
#include "ReadConcern_arginfo.h"

zend_class_entry* phongo_readconcern_ce;

static void phongo_readconcern_update_properties(phongo_readconcern_t* intern)
{
	const char* level = mongoc_read_concern_get_level(intern->read_concern);

	if (level) {
		zend_update_property_string(phongo_readconcern_ce, &intern->std, ZEND_STRL("level"), level);
	} else {
		zend_update_property_null(phongo_readconcern_ce, &intern->std, ZEND_STRL("level"));
	}
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_readconcern_init_from_hash(phongo_readconcern_t* intern, HashTable* props)
{
	zval* level;

	intern->read_concern = mongoc_read_concern_new();

	if ((level = zend_hash_str_find(props, "level", sizeof("level") - 1))) {
		if (Z_TYPE_P(level) == IS_STRING) {
			mongoc_read_concern_set_level(intern->read_concern, Z_STRVAL_P(level));
		} else if (Z_TYPE_P(level) != IS_NULL) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"level\" string field", ZSTR_VAL(phongo_readconcern_ce->name));
			goto failure;
		}
	}

	phongo_readconcern_update_properties(intern);

	return true;

failure:
	mongoc_read_concern_destroy(intern->read_concern);
	intern->read_concern = NULL;
	return false;
}

/* Constructs a new ReadConcern */
static PHP_METHOD(MongoDB_Driver_ReadConcern, __construct)
{
	PHONGO_INTERN_FROM_THIS(readconcern);

	char*  level     = NULL;
	size_t level_len = 0;

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_STRING_OR_NULL(level, level_len)
	PHONGO_PARSE_PARAMETERS_END();

	intern->read_concern = mongoc_read_concern_new();

	if (level) {
		mongoc_read_concern_set_level(intern->read_concern, level);
	}

	phongo_readconcern_update_properties(intern);
}

static PHP_METHOD(MongoDB_Driver_ReadConcern, __set_state)
{
	HashTable* props;
	zval*      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(readconcern, return_value);
	props = Z_ARRVAL_P(array);

	phongo_readconcern_init_from_hash(intern, props);
}

/* Returns the ReadConcern "level" option */
static PHP_METHOD(MongoDB_Driver_ReadConcern, getLevel)
{
	PHONGO_INTERN_FROM_THIS(readconcern);

	const char* level;

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
	PHONGO_INTERN_FROM_THIS(readconcern);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(mongoc_read_concern_is_default(intern->read_concern));
}

static PHP_METHOD(MongoDB_Driver_ReadConcern, bsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 1);

	{
		zend_string* string_key;
		zval*        val;

		ZEND_HASH_FOREACH_STR_KEY_VAL_IND(HASH_OF(getThis()), string_key, val)
		{
			if (Z_TYPE_P(val) == IS_NULL) {
				continue;
			}

			Z_TRY_ADDREF_P(val);
			add_assoc_zval(return_value, ZSTR_VAL(string_key), val);
		}
		ZEND_HASH_FOREACH_END();
	}

	convert_to_object(return_value);
}

static PHP_METHOD(MongoDB_Driver_ReadConcern, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_readconcern_init_from_hash(Z_READCONCERN_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\Driver\ReadConcern object handlers */
static zend_object_handlers phongo_handler_readconcern;

static void phongo_readconcern_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(readconcern, object);

	zend_object_std_dtor(&intern->std);

	if (intern->read_concern) {
		mongoc_read_concern_destroy(intern->read_concern);
	}
}

static zend_object* phongo_readconcern_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(readconcern, class_type);

	intern->std.handlers = &phongo_handler_readconcern;

	return &intern->std;
}

void phongo_readconcern_init_ce(INIT_FUNC_ARGS)
{
	phongo_readconcern_ce                = register_class_MongoDB_Driver_ReadConcern(phongo_serializable_ce);
	phongo_readconcern_ce->create_object = phongo_readconcern_create_object;

	memcpy(&phongo_handler_readconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_readconcern.free_obj = phongo_readconcern_free_object;
	phongo_handler_readconcern.offset   = XtOffsetOf(phongo_readconcern_t, std);
}

void phongo_readconcern_init(zval* return_value, const mongoc_read_concern_t* read_concern)
{
	PHONGO_INTERN_INIT_EX(readconcern, return_value);
	intern->read_concern = mongoc_read_concern_copy(read_concern);

	phongo_readconcern_update_properties(intern);
}

const mongoc_read_concern_t* phongo_read_concern_from_zval(zval* zread_concern)
{
	if (zread_concern) {
		PHONGO_INTERN_FROM_ZVAL(readconcern, zread_concern);

		if (intern) {
			return intern->read_concern;
		}
	}

	return NULL;
}
