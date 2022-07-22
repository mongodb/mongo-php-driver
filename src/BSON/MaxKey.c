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
#include "MaxKey_arginfo.h"

zend_class_entry* php_phongo_maxkey_ce;

/* {{{ proto MongoDB\BSON\MaxKey MongoDB\BSON\MaxKey::__set_state(array $properties)
*/
PHP_METHOD(MongoDB_BSON_MaxKey, __set_state)
{
	zval* array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_maxkey_ce);
} /* }}} */

/* {{{ proto array MongoDB\BSON\MaxKey::jsonSerialize()
*/
PHP_METHOD(MongoDB_BSON_MaxKey, jsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 1);
	ADD_ASSOC_LONG_EX(return_value, "$maxKey", 1);
} /* }}} */

/* {{{ proto string MongoDB\BSON\MaxKey::serialize()
*/
PHP_METHOD(MongoDB_BSON_MaxKey, serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRING("");
} /* }}} */

/* {{{ proto void MongoDB\BSON\MaxKey::unserialize(string $serialized)
*/
PHP_METHOD(MongoDB_BSON_MaxKey, unserialize)
{
	char*  serialized;
	size_t serialized_len;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();
} /* }}} */

/* {{{ proto array MongoDB\Driver\MaxKey::__serialize()
*/
PHP_METHOD(MongoDB_BSON_MaxKey, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 0);
} /* }}} */

/* {{{ proto void MongoDB\Driver\MaxKey::__unserialize(array $data)
*/
PHP_METHOD(MongoDB_BSON_MaxKey, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();
} /* }}} */

/* {{{ MongoDB\BSON\MaxKey object handlers */
static zend_object_handlers php_phongo_handler_maxkey;

static void php_phongo_maxkey_free_object(zend_object* object) /* {{{ */
{
	php_phongo_maxkey_t* intern = Z_OBJ_MAXKEY(object);

	zend_object_std_dtor(&intern->std);
} /* }}} */

static zend_object* php_phongo_maxkey_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_maxkey_t* intern = zend_object_alloc(sizeof(php_phongo_maxkey_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_maxkey;

	return &intern->std;
} /* }}} */
/* }}} */

void php_phongo_maxkey_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "MaxKey", class_MongoDB_BSON_MaxKey_methods);
	php_phongo_maxkey_ce                = zend_register_internal_class(&ce);
	php_phongo_maxkey_ce->create_object = php_phongo_maxkey_create_object;
	PHONGO_CE_FINAL(php_phongo_maxkey_ce);

	zend_class_implements(php_phongo_maxkey_ce, 1, php_phongo_maxkey_interface_ce);
	zend_class_implements(php_phongo_maxkey_ce, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_maxkey_ce, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_maxkey_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_maxkey, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	/* Re-assign default handler previously removed in php_phongo.c */
	php_phongo_handler_maxkey.clone_obj = zend_objects_clone_obj;
	php_phongo_handler_maxkey.free_obj  = php_phongo_maxkey_free_object;
	php_phongo_handler_maxkey.offset    = XtOffsetOf(php_phongo_maxkey_t, std);
} /* }}} */
