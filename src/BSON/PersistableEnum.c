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

#include "php_phongo.h"
#include "phongo_error.h"
#include "PersistableEnum_arginfo.h"

zend_class_entry* php_phongo_persistableenum_ce;

PHP_METHOD(MongoDB_BSON_PersistableEnum, bsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETVAL_ZVAL(getThis(), 1, 0);
	convert_to_array(return_value);

	return;
}

PHP_METHOD(MongoDB_BSON_PersistableEnum, bsonUnserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	return;
}

void php_phongo_persistableenum_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_persistableenum_ce = register_class_MongoDB_BSON_PersistableEnum();
}
