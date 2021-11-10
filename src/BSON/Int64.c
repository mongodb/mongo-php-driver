/*
 * Copyright 2018-present MongoDB, Inc.
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
#include <ext/standard/php_var.h>
#include <zend_smart_str.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"
#include "php_bson.h"

zend_class_entry* php_phongo_int64_ce;

/* Initialize the object and return whether it was successful. */
static bool php_phongo_int64_init(php_phongo_int64_t* intern, int64_t integer) /* {{{ */
{
	intern->integer     = integer;
	intern->initialized = true;

	return true;
} /* }}} */

/* Initialize the object from a numeric string and return whether it was
 * successful. An exception will be thrown on error. */
static bool php_phongo_int64_init_from_string(php_phongo_int64_t* intern, const char* s_integer, size_t s_integer_len) /* {{{ */
{
	int64_t integer;

	if (!php_phongo_parse_int64(&integer, s_integer, s_integer_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit integer for %s initialization", s_integer, ZSTR_VAL(php_phongo_int64_ce->name));
		return false;
	}

	return php_phongo_int64_init(intern, integer);
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_int64_init_from_hash(php_phongo_int64_t* intern, HashTable* props) /* {{{ */
{
	zval* value;

	if ((value = zend_hash_str_find(props, "integer", sizeof("integer") - 1)) && Z_TYPE_P(value) == IS_STRING) {
		return php_phongo_int64_init_from_string(intern, Z_STRVAL_P(value), Z_STRLEN_P(value));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"integer\" string field", ZSTR_VAL(php_phongo_int64_ce->name));
	return false;
} /* }}} */

HashTable* php_phongo_int64_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp) /* {{{ */
{
	php_phongo_int64_t* intern;
	HashTable*          props;

	intern = Z_OBJ_INT64(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->initialized) {
		return props;
	}

	{
		zval value;

		ZVAL_INT64_STRING(&value, intern->integer);
		zend_hash_str_update(props, "integer", sizeof("integer") - 1, &value);
	}

	return props;
} /* }}} */

/* {{{ proto string MongoDB\BSON\Int64::__toString()
   Return the Int64's value as a string. */
static PHP_METHOD(Int64, __toString)
{
	zend_error_handling error_handling;
	php_phongo_int64_t* intern;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	intern = Z_INT64_OBJ_P(getThis());

	ZVAL_INT64_STRING(return_value, intern->integer);
} /* }}} */

/* {{{ proto array MongoDB\BSON\Int64::jsonSerialize()
*/
static PHP_METHOD(Int64, jsonSerialize)
{
	zend_error_handling error_handling;
	php_phongo_int64_t* intern;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	intern = Z_INT64_OBJ_P(getThis());

	array_init_size(return_value, 1);

	ADD_ASSOC_INT64_AS_STRING(return_value, "$numberLong", intern->integer);
} /* }}} */

/* {{{ proto string MongoDB\BSON\Int64::serialize()
*/
static PHP_METHOD(Int64, serialize)
{
	zend_error_handling  error_handling;
	php_phongo_int64_t*  intern;
	zval                 retval;
	php_serialize_data_t var_hash;
	smart_str            buf = { 0 };

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	intern = Z_INT64_OBJ_P(getThis());

	array_init_size(&retval, 1);
	ADD_ASSOC_INT64_AS_STRING(&retval, "integer", intern->integer);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\BSON\Int64::unserialize(string $serialized)
*/
static PHP_METHOD(Int64, unserialize)
{
	zend_error_handling    error_handling;
	php_phongo_int64_t*    intern;
	char*                  serialized;
	size_t                 serialized_len;
	zval                   props;
	php_unserialize_data_t var_hash;

	intern = Z_INT64_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_int64_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_int64_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ proto array MongoDB\Driver\Int64::__serialize()
*/
static PHP_METHOD(Int64, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_int64_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true));
} /* }}} */

/* {{{ proto void MongoDB\Driver\Int64::__unserialize(array $data)
*/
static PHP_METHOD(Int64, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_int64_init_from_hash(Z_INT64_OBJ_P(getThis()), Z_ARRVAL_P(data));
} /* }}} */

/* {{{ MongoDB\BSON\Int64 function entries */
/* clang-format off */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(ai_Int64___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Int64___unserialize, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(ai_Int64_jsonSerialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Int64_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Int64_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_int64_me[] = {
	/* __set_state intentionally missing */
	PHP_ME(Int64, __serialize, ai_Int64_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Int64, __toString, ai_Int64___toString, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Int64, __unserialize, ai_Int64___unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Int64, jsonSerialize, ai_Int64_jsonSerialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Int64, serialize, ai_Int64_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Int64, unserialize, ai_Int64_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_Int64_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_FE_END
};
/* clang-format on */
/* }}} */

/* {{{ MongoDB\BSON\Int64 object handlers */
static zend_object_handlers php_phongo_handler_int64;

static void php_phongo_int64_free_object(zend_object* object) /* {{{ */
{
	php_phongo_int64_t* intern = Z_OBJ_INT64(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
} /* }}} */

zend_object* php_phongo_int64_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_int64_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_int64_t, class_type);
	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_int64;

	return &intern->std;
} /* }}} */

static zend_object* php_phongo_int64_clone_object(phongo_compat_object_handler_type* object) /* {{{ */
{
	php_phongo_int64_t* intern;
	php_phongo_int64_t* new_intern;
	zend_object*        new_object;

	intern     = Z_OBJ_INT64(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_int64_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_INT64(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_int64_init(new_intern, intern->integer);

	return new_object;
} /* }}} */

static int php_phongo_int64_compare_objects(zval* o1, zval* o2) /* {{{ */
{
	php_phongo_int64_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_INT64_OBJ_P(o1);
	intern2 = Z_INT64_OBJ_P(o2);

	if (intern1->integer != intern2->integer) {
		return intern1->integer < intern2->integer ? -1 : 1;
	}

	return 0;
} /* }}} */

static HashTable* php_phongo_int64_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_int64_get_properties_hash(object, true);
} /* }}} */

static HashTable* php_phongo_int64_get_properties(phongo_compat_object_handler_type* object) /* {{{ */
{
	return php_phongo_int64_get_properties_hash(object, false);
} /* }}} */
/* }}} */

void php_phongo_int64_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Int64", php_phongo_int64_me);
	php_phongo_int64_ce                = zend_register_internal_class(&ce);
	php_phongo_int64_ce->create_object = php_phongo_int64_create_object;
	PHONGO_CE_FINAL(php_phongo_int64_ce);

	zend_class_implements(php_phongo_int64_ce, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_int64_ce, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_int64_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_int64, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(int64);
	php_phongo_handler_int64.clone_obj      = php_phongo_int64_clone_object;
	php_phongo_handler_int64.get_debug_info = php_phongo_int64_get_debug_info;
	php_phongo_handler_int64.get_properties = php_phongo_int64_get_properties;
	php_phongo_handler_int64.free_obj       = php_phongo_int64_free_object;
	php_phongo_handler_int64.offset         = XtOffsetOf(php_phongo_int64_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
