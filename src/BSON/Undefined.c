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
#include "Undefined_arginfo.h"

zend_class_entry* phongo_undefined_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_BSON_Undefined)

/* Return the empty string. */
static PHP_METHOD(MongoDB_BSON_Undefined, __toString)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRINGL("", 0);
}

static PHP_METHOD(MongoDB_BSON_Undefined, __set_state)
{
	zval* array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, phongo_undefined_ce);
}

static PHP_METHOD(MongoDB_BSON_Undefined, jsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 1);
	ADD_ASSOC_BOOL_EX(return_value, "$undefined", 1);
}

static PHP_METHOD(MongoDB_BSON_Undefined, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 0);
}

static PHP_METHOD(MongoDB_BSON_Undefined, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();
}

/* MongoDB\BSON\Undefined object handlers */
static zend_object_handlers phongo_handler_undefined;

static void phongo_undefined_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(undefined, object);

	zend_object_std_dtor(&intern->std);
}

static zend_object* phongo_undefined_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(undefined, class_type);

	intern->std.handlers = &phongo_handler_undefined;

	return &intern->std;
}

void phongo_undefined_init_ce(INIT_FUNC_ARGS)
{
	phongo_undefined_ce                = register_class_MongoDB_BSON_Undefined(phongo_json_serializable_ce, phongo_type_ce, zend_ce_stringable);
	phongo_undefined_ce->create_object = phongo_undefined_create_object;

	memcpy(&phongo_handler_undefined, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	/* Re-assign default handler previously removed in phongo.c */
	phongo_handler_undefined.clone_obj = zend_objects_clone_obj;
	phongo_handler_undefined.free_obj  = phongo_undefined_free_object;
	phongo_handler_undefined.offset    = XtOffsetOf(phongo_undefined_t, std);
}
