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
#if PHP_VERSION_ID >= 70000
#include <zend_smart_str.h>
#else
#include <ext/standard/php_smart_str.h>
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_decimal128_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_decimal128_init(php_phongo_decimal128_t* intern, const char* value TSRMLS_DC) /* {{{ */
{
	if (!bson_decimal128_from_string(value, &intern->decimal)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Error parsing Decimal128 string: %s", value);
		return false;
	}

	intern->initialized = true;

	return true;
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_decimal128_init_from_hash(php_phongo_decimal128_t* intern, HashTable* props TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval* dec;

	if ((dec = zend_hash_str_find(props, "dec", sizeof("dec") - 1)) && Z_TYPE_P(dec) == IS_STRING) {
		return php_phongo_decimal128_init(intern, Z_STRVAL_P(dec) TSRMLS_CC);
	}
#else
	zval** dec;

	if (zend_hash_find(props, "dec", sizeof("dec"), (void**) &dec) == SUCCESS && Z_TYPE_PP(dec) == IS_STRING) {
		return php_phongo_decimal128_init(intern, Z_STRVAL_PP(dec) TSRMLS_CC);
	}
#endif

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"dec\" string field", ZSTR_VAL(php_phongo_decimal128_ce->name));
	return false;
} /* }}} */

/* {{{ proto void MongoDB\BSON\Decimal128::__construct(string $value)
   Construct a new BSON Decimal128 type */
static PHP_METHOD(Decimal128, __construct)
{
	php_phongo_decimal128_t* intern;
	zend_error_handling      error_handling;
	char*                    value;
	phongo_zpp_char_len      value_len;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_DECIMAL128_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	php_phongo_decimal128_init(intern, value TSRMLS_CC);
} /* }}} */

/* {{{ proto void MongoDB\BSON\Decimal128::__set_state(array $properties)
*/
static PHP_METHOD(Decimal128, __set_state)
{
	php_phongo_decimal128_t* intern;
	HashTable*               props;
	zval*                    array;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_decimal128_ce);

	intern = Z_DECIMAL128_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_decimal128_init_from_hash(intern, props TSRMLS_CC);
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

	PHONGO_RETURN_STRING(outbuf);
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
	ZVAL_RETVAL_TYPE         retval;
	php_serialize_data_t     var_hash;
	smart_str                buf = { 0 };
	char                     outbuf[BSON_DECIMAL128_STRING];

	intern = Z_DECIMAL128_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	bson_decimal128_to_string(&intern->decimal, outbuf);
#if PHP_VERSION_ID >= 70000
	array_init_size(&retval, 1);
	ADD_ASSOC_STRING(&retval, "dec", outbuf);
#else
	ALLOC_INIT_ZVAL(retval);
	array_init_size(retval, 1);
	ADD_ASSOC_STRING(retval, "dec", outbuf);
#endif

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash TSRMLS_CC);
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
	phongo_zpp_char_len      serialized_len;
#if PHP_VERSION_ID >= 70000
	zval props;
#else
	zval* props;
#endif
	php_unserialize_data_t var_hash;

	intern = Z_DECIMAL128_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

#if PHP_VERSION_ID < 70000
	ALLOC_INIT_ZVAL(props);
#endif
	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash TSRMLS_CC)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "%s unserialization failed", ZSTR_VAL(php_phongo_decimal128_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

#if PHP_VERSION_ID >= 70000
	php_phongo_decimal128_init_from_hash(intern, HASH_OF(&props) TSRMLS_CC);
#else
	php_phongo_decimal128_init_from_hash(intern, HASH_OF(props) TSRMLS_CC);
#endif
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

static void php_phongo_decimal128_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_decimal128_t* intern = Z_OBJ_DECIMAL128(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_decimal128_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_decimal128_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_decimal128_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_decimal128;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_decimal128_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_decimal128;

		return retval;
	}
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_decimal128_clone_object(zval* object TSRMLS_DC) /* {{{ */
{
	php_phongo_decimal128_t*    intern;
	php_phongo_decimal128_t*    new_intern;
	phongo_create_object_retval new_object;

	intern     = Z_DECIMAL128_OBJ_P(object);
	new_object = php_phongo_decimal128_create_object(Z_OBJCE_P(object) TSRMLS_CC);

#if PHP_VERSION_ID >= 70000
	new_intern = Z_OBJ_DECIMAL128(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std TSRMLS_CC);
#else
	{
		zend_object_handle handle = Z_OBJ_HANDLE_P(object);

		new_intern = (php_phongo_decimal128_t*) zend_object_store_get_object_by_handle(new_object.handle TSRMLS_CC);
		zend_objects_clone_members(&new_intern->std, new_object, &intern->std, handle TSRMLS_CC);
	}
#endif

	// Use memcpy to copy bson value to avoid converting to string and back
	memcpy(&new_intern->decimal, &intern->decimal, sizeof(bson_decimal128_t));
	new_intern->initialized = true;

	return new_object;
} /* }}} */

static HashTable* php_phongo_decimal128_get_gc(zval* object, phongo_get_gc_table table, int* n TSRMLS_DC) /* {{{ */
{
	*table = NULL;
	*n     = 0;

	return Z_DECIMAL128_OBJ_P(object)->properties;
} /* }}} */

static HashTable* php_phongo_decimal128_get_properties_hash(zval* object, bool is_debug TSRMLS_DC) /* {{{ */
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

#if PHP_VERSION_ID >= 70000
	{
		zval dec;

		ZVAL_STRING(&dec, outbuf);
		zend_hash_str_update(props, "dec", sizeof("dec") - 1, &dec);
	}
#else
	{
		zval* dec;

		MAKE_STD_ZVAL(dec);
		ZVAL_STRING(dec, outbuf, 1);
		zend_hash_update(props, "dec", sizeof("dec"), &dec, sizeof(dec), NULL);
	}
#endif

	return props;
} /* }}} */

static HashTable* php_phongo_decimal128_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_decimal128_get_properties_hash(object, true TSRMLS_CC);
} /* }}} */

static HashTable* php_phongo_decimal128_get_properties(zval* object TSRMLS_DC) /* {{{ */
{
	return php_phongo_decimal128_get_properties_hash(object, false TSRMLS_CC);
} /* }}} */
/* }}} */

void php_phongo_decimal128_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Decimal128", php_phongo_decimal128_me);
	php_phongo_decimal128_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_decimal128_ce->create_object = php_phongo_decimal128_create_object;
	PHONGO_CE_FINAL(php_phongo_decimal128_ce);

	zend_class_implements(php_phongo_decimal128_ce TSRMLS_CC, 1, php_phongo_decimal128_interface_ce);
	zend_class_implements(php_phongo_decimal128_ce TSRMLS_CC, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_decimal128_ce TSRMLS_CC, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_decimal128_ce TSRMLS_CC, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_decimal128, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_decimal128.clone_obj      = php_phongo_decimal128_clone_object;
	php_phongo_handler_decimal128.get_debug_info = php_phongo_decimal128_get_debug_info;
	php_phongo_handler_decimal128.get_gc         = php_phongo_decimal128_get_gc;
	php_phongo_handler_decimal128.get_properties = php_phongo_decimal128_get_properties;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_decimal128.free_obj = php_phongo_decimal128_free_object;
	php_phongo_handler_decimal128.offset   = XtOffsetOf(php_phongo_decimal128_t, std);
#endif
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
