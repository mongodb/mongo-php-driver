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
#include "MinKey_arginfo.h"

zend_class_entry* php_phongo_minkey_ce;

static PHP_METHOD(MongoDB_BSON_MinKey, __set_state)
{
	zval* array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_minkey_ce);
}

static PHP_METHOD(MongoDB_BSON_MinKey, jsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 1);
	ADD_ASSOC_LONG_EX(return_value, "$minKey", 1);
}

static PHP_METHOD(MongoDB_BSON_MinKey, serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRING("");
}

static PHP_METHOD(MongoDB_BSON_MinKey, unserialize)
{
	char*  serialized;
	size_t serialized_len;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();
}

static PHP_METHOD(MongoDB_BSON_MinKey, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 0);
}

static PHP_METHOD(MongoDB_BSON_MinKey, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();
}

/* MongoDB\BSON\MinKey object handlers */
static zend_object_handlers php_phongo_handler_minkey;

static void php_phongo_minkey_free_object(zend_object* object)
{
	php_phongo_minkey_t* intern = Z_OBJ_MINKEY(object);

	zend_object_std_dtor(&intern->std);
}

static zend_object* php_phongo_minkey_create_object(zend_class_entry* class_type)
{
	php_phongo_minkey_t* intern = zend_object_alloc(sizeof(php_phongo_minkey_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_minkey;

	return &intern->std;
}

void php_phongo_minkey_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_minkey_ce                = register_class_MongoDB_BSON_MinKey(php_phongo_minkey_interface_ce, php_phongo_json_serializable_ce, php_phongo_type_ce, zend_ce_serializable);
	php_phongo_minkey_ce->create_object = php_phongo_minkey_create_object;

	memcpy(&php_phongo_handler_minkey, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	/* Re-assign default handler previously removed in php_phongo.c */
	php_phongo_handler_minkey.clone_obj = zend_objects_clone_obj;
	php_phongo_handler_minkey.free_obj  = php_phongo_minkey_free_object;
	php_phongo_handler_minkey.offset    = XtOffsetOf(php_phongo_minkey_t, std);
}
