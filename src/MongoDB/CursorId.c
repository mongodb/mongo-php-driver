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
#include "phongo_util.h"
#include "CursorId_arginfo.h"

zend_class_entry* php_phongo_cursorid_ce;

/* Initialize the object from a numeric string and return whether it was
 * successful. An exception will be thrown on error. */
static bool php_phongo_cursorid_init_from_string(php_phongo_cursorid_t* intern, const char* s_id, size_t s_id_len)
{
	int64_t id;

	if (!php_phongo_parse_int64(&id, s_id, s_id_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit id for %s initialization", s_id, ZSTR_VAL(php_phongo_cursorid_ce->name));
		return false;
	}

	intern->id          = id;
	intern->initialized = true;
	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_cursorid_init_from_hash(php_phongo_cursorid_t* intern, HashTable* props)
{
	zval* value;

	if ((value = zend_hash_str_find(props, "id", sizeof("id") - 1)) && Z_TYPE_P(value) == IS_STRING) {
		return php_phongo_cursorid_init_from_string(intern, Z_STRVAL_P(value), Z_STRLEN_P(value));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"id\" string field", ZSTR_VAL(php_phongo_cursorid_ce->name));
	return false;
}

static HashTable* php_phongo_cursorid_get_properties_hash(zend_object* object, bool is_temp, bool is_serialize)
{
	php_phongo_cursorid_t* intern;
	HashTable*             props;

	intern = Z_OBJ_CURSORID(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 1);

	if (!intern->initialized) {
		return props;
	}

	{
		zval value;

		if (is_serialize) {
			ZVAL_INT64_STRING(&value, intern->id);
		} else {
#if SIZEOF_ZEND_LONG == 4
			if (intern->id > INT32_MAX || intern->id < INT32_MIN) {
				ZVAL_INT64_STRING(&value, intern->id);
			} else {
				ZVAL_LONG(&value, intern->id);
			}
#else
			ZVAL_LONG(&value, intern->id);
#endif
		}
		zend_hash_str_update(props, "id", sizeof("id") - 1, &value);
	}

	return props;
}

PHP_METHOD(MongoDB_Driver_CursorId, __set_state)
{
	php_phongo_cursorid_t* intern;
	HashTable*             props;
	zval*                  array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_cursorid_ce);

	intern = Z_CURSORID_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_cursorid_init_from_hash(intern, props);
}

/* Returns the string representation of the CursorId */
PHP_METHOD(MongoDB_Driver_CursorId, __toString)
{
	php_phongo_cursorid_t* intern;
	char*                  tmp;
	int                    tmp_len;

	intern = Z_CURSORID_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	tmp_len = spprintf(&tmp, 0, "%" PRId64, intern->id);
	RETVAL_STRINGL(tmp, tmp_len);
	efree(tmp);
}

PHP_METHOD(MongoDB_Driver_CursorId, serialize)
{
	php_phongo_cursorid_t* intern;
	zval                   retval;
	php_serialize_data_t   var_hash;
	smart_str              buf = { 0 };

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_CURSORID_OBJ_P(getThis());

	array_init_size(&retval, 1);
	ADD_ASSOC_INT64_AS_STRING(&retval, "id", intern->id);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

PHP_METHOD(MongoDB_Driver_CursorId, unserialize)
{
	php_phongo_cursorid_t* intern;
	char*                  serialized;
	size_t                 serialized_len;
	zval                   props;
	php_unserialize_data_t var_hash;

	intern = Z_CURSORID_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_cursorid_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_cursorid_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

PHP_METHOD(MongoDB_Driver_CursorId, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_cursorid_get_properties_hash(Z_OBJ_P(getThis()), true, true));
}

PHP_METHOD(MongoDB_Driver_CursorId, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_cursorid_init_from_hash(Z_CURSORID_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_CursorId)

/* MongoDB\Driver\CursorId object handlers */
static zend_object_handlers php_phongo_handler_cursorid;

static void php_phongo_cursorid_free_object(zend_object* object)
{
	php_phongo_cursorid_t* intern = Z_OBJ_CURSORID(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* php_phongo_cursorid_create_object(zend_class_entry* class_type)
{
	php_phongo_cursorid_t* intern = zend_object_alloc(sizeof(php_phongo_cursorid_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_cursorid;

	return &intern->std;
}

static HashTable* php_phongo_cursorid_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_cursorid_get_properties_hash(object, true, false);
}

static HashTable* php_phongo_cursorid_get_properties(zend_object* object)
{
	return php_phongo_cursorid_get_properties_hash(object, false, false);
}

void php_phongo_cursorid_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_cursorid_ce                = register_class_MongoDB_Driver_CursorId(zend_ce_serializable, zend_ce_stringable);
	php_phongo_cursorid_ce->create_object = php_phongo_cursorid_create_object;

	memcpy(&php_phongo_handler_cursorid, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_cursorid.get_debug_info = php_phongo_cursorid_get_debug_info;
	php_phongo_handler_cursorid.get_properties = php_phongo_cursorid_get_properties;
	php_phongo_handler_cursorid.free_obj       = php_phongo_cursorid_free_object;
	php_phongo_handler_cursorid.offset         = XtOffsetOf(php_phongo_cursorid_t, std);
}
