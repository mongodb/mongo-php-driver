/*
 * Copyright 2015-2017 MongoDB, Inc.
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

zend_class_entry* php_phongo_decimal128_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_decimal128_init(php_phongo_decimal128_t* intern, const char* value) /* {{{ */
{
	if (!bson_decimal128_from_string(value, &intern->decimal)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing Decimal128 string: %s", value);
		return false;
	}

	intern->initialized = true;

	return true;
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_decimal128_init_from_hash(php_phongo_decimal128_t* intern, HashTable* props) /* {{{ */
{
	zval* dec;

	if ((dec = zend_hash_str_find(props, "dec", sizeof("dec") - 1)) && Z_TYPE_P(dec) == IS_STRING) {
		return php_phongo_decimal128_init(intern, Z_STRVAL_P(dec));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"dec\" string field", ZSTR_VAL(php_phongo_decimal128_ce->name));
	return false;
} /* }}} */

/* {{{ proto void MongoDB\BSON\Decimal128::__construct(string $value)
   Construct a new BSON Decimal128 type */
static PHP_METHOD(Decimal128, __construct)
{
	php_phongo_decimal128_t* intern;
	zend_error_handling      error_handling;
	char*                    value;
	size_t                   value_len;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	intern = Z_DECIMAL128_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &value, &value_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	php_phongo_decimal128_init(intern, value);
} /* }}} */

/* {{{ proto void MongoDB\BSON\Decimal128::__set_state(array $properties)
*/
static PHP_METHOD(Decimal128, __set_state)
{
	php_phongo_decimal128_t* intern;
	HashTable*               props;
	zval*                    array;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_decimal128_ce);

	intern = Z_DECIMAL128_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_decimal128_init_from_hash(intern, props);
} /* }}} */

/* {{{ proto string MongoDB\BSON\Decimal128::__toString()
*/
static PHP_METHOD(Decimal128, __toString)
{
	php_phongo_decimal128_t* intern;
	char                     outbuf[BSON_DECIMAL128_STRING];

	intern = Z_DECIMAL128_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	bson_decimal128_to_string(&intern->decimal, outbuf);

	RETURN_STRING(outbuf);
} /* }}} */

/* {{{ proto array MongoDB\BSON\Decimal128::jsonSerialize()
*/
static PHP_METHOD(Decimal128, jsonSerialize)
{
	php_phongo_decimal128_t* intern;
	char                     outbuf[BSON_DECIMAL128_STRING] = "";

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_DECIMAL128_OBJ_P(getThis());

	array_init_size(return_value, 1);
	bson_decimal128_to_string(&intern->decimal, outbuf);
	ADD_ASSOC_STRING(return_value, "$numberDecimal", outbuf);
} /* }}} */

/* {{{ proto string MongoDB\BSON\Decimal128::serialize()
*/
static PHP_METHOD(Decimal128, serialize)
{
	php_phongo_decimal128_t* intern;
	zval                     retval;
	php_serialize_data_t     var_hash;
	smart_str                buf = { 0 };
	char                     outbuf[BSON_DECIMAL128_STRING];

	intern = Z_DECIMAL128_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	bson_decimal128_to_string(&intern->decimal, outbuf);
	array_init_size(&retval, 1);
	ADD_ASSOC_STRING(&retval, "dec", outbuf);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\BSON\Decimal128::unserialize(string $serialized)
*/
static PHP_METHOD(Decimal128, unserialize)
{
	php_phongo_decimal128_t* intern;
	zend_error_handling      error_handling;
	char*                    serialized;
	size_t                   serialized_len;
	zval                     props;
	php_unserialize_data_t   var_hash;

	intern = Z_DECIMAL128_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_decimal128_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_decimal128_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ MongoDB\BSON\Decimal128 function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Decimal128___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Decimal128___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Decimal128_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Decimal128_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_decimal128_me[] = {
	/* clang-format off */
	PHP_ME(Decimal128, __construct, ai_Decimal128___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Decimal128, __set_state, ai_Decimal128___set_state, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Decimal128, __toString, ai_Decimal128_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Decimal128, jsonSerialize, ai_Decimal128_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Decimal128, serialize, ai_Decimal128_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Decimal128, unserialize, ai_Decimal128_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\BSON\Decimal128 object handlers */
static zend_object_handlers php_phongo_handler_decimal128;

static void php_phongo_decimal128_free_object(zend_object* object) /* {{{ */
{
	php_phongo_decimal128_t* intern = Z_OBJ_DECIMAL128(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
} /* }}} */

static zend_object* php_phongo_decimal128_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_decimal128_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_decimal128_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_decimal128;

	return &intern->std;
} /* }}} */

static zend_object* php_phongo_decimal128_clone_object(zval* object) /* {{{ */
{
	php_phongo_decimal128_t* intern;
	php_phongo_decimal128_t* new_intern;
	zend_object*             new_object;

	intern     = Z_DECIMAL128_OBJ_P(object);
	new_object = php_phongo_decimal128_create_object(Z_OBJCE_P(object));

	new_intern = Z_OBJ_DECIMAL128(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	// Use memcpy to copy bson value to avoid converting to string and back
	memcpy(&new_intern->decimal, &intern->decimal, sizeof(bson_decimal128_t));
	new_intern->initialized = true;

	return new_object;
} /* }}} */

static HashTable* php_phongo_decimal128_get_gc(zval* object, zval** table, int* n) /* {{{ */
{
	*table = NULL;
	*n     = 0;

	return Z_DECIMAL128_OBJ_P(object)->properties;
} /* }}} */

static HashTable* php_phongo_decimal128_get_properties_hash(zval* object, bool is_debug) /* {{{ */
{
	php_phongo_decimal128_t* intern;
	HashTable*               props;
	char                     outbuf[BSON_DECIMAL128_STRING] = "";

	intern = Z_DECIMAL128_OBJ_P(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 1);

	if (!intern->initialized) {
		return props;
	}

	bson_decimal128_to_string(&intern->decimal, outbuf);

	{
		zval dec;

		ZVAL_STRING(&dec, outbuf);
		zend_hash_str_update(props, "dec", sizeof("dec") - 1, &dec);
	}

	return props;
} /* }}} */

static HashTable* php_phongo_decimal128_get_debug_info(zval* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_decimal128_get_properties_hash(object, true);
} /* }}} */

static HashTable* php_phongo_decimal128_get_properties(zval* object) /* {{{ */
{
	return php_phongo_decimal128_get_properties_hash(object, false);
} /* }}} */
/* }}} */

void php_phongo_decimal128_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Decimal128", php_phongo_decimal128_me);
	php_phongo_decimal128_ce                = zend_register_internal_class(&ce);
	php_phongo_decimal128_ce->create_object = php_phongo_decimal128_create_object;
	PHONGO_CE_FINAL(php_phongo_decimal128_ce);

	zend_class_implements(php_phongo_decimal128_ce, 1, php_phongo_decimal128_interface_ce);
	zend_class_implements(php_phongo_decimal128_ce, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_decimal128_ce, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_decimal128_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_decimal128, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_decimal128.clone_obj      = php_phongo_decimal128_clone_object;
	php_phongo_handler_decimal128.get_debug_info = php_phongo_decimal128_get_debug_info;
	php_phongo_handler_decimal128.get_gc         = php_phongo_decimal128_get_gc;
	php_phongo_handler_decimal128.get_properties = php_phongo_decimal128_get_properties;
	php_phongo_handler_decimal128.free_obj       = php_phongo_decimal128_free_object;
	php_phongo_handler_decimal128.offset         = XtOffsetOf(php_phongo_decimal128_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
