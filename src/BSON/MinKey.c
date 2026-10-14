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
#include "MinKey_arginfo.h"

zend_class_entry* phongo_minkey_ce;

static PHP_METHOD(MongoDB_BSON_MinKey, __set_state)
{
	zval* array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, phongo_minkey_ce);
}

static PHP_METHOD(MongoDB_BSON_MinKey, jsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 1);
	ADD_ASSOC_LONG_EX(return_value, "$minKey", 1);
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

void phongo_minkey_init_ce(INIT_FUNC_ARGS)
{
	phongo_minkey_ce = register_class_MongoDB_BSON_MinKey(phongo_minkey_interface_ce, phongo_json_serializable_ce, phongo_type_ce);
}
