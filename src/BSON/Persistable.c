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
#include "Persistable_arginfo.h"

zend_class_entry* php_phongo_persistable_ce;

static int php_phongo_implement_persistable(zend_class_entry* interface, zend_class_entry* class_type)
{
	if (class_type->ce_flags & ZEND_ACC_ENUM) {
		zend_error_noreturn(E_ERROR, "Enum class %s cannot implement interface %s", ZSTR_VAL(class_type->name), ZSTR_VAL(interface->name));
		return FAILURE;
	}

	return SUCCESS;
}

void php_phongo_persistable_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_persistable_ce                             = register_class_MongoDB_BSON_Persistable(php_phongo_serializable_ce, php_phongo_unserializable_ce);
	php_phongo_persistable_ce->interface_gets_implemented = php_phongo_implement_persistable;
}
