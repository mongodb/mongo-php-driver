/*
 * Copyright 2014-2017 MongoDB, Inc.
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
#include <ext/standard/base64.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_operators.h>
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

#define PHONGO_BINARY_UUID_SIZE 16

zend_class_entry* php_phongo_binary_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_binary_init(php_phongo_binary_t* intern, const char* data, phongo_zpp_char_len data_len, phongo_long type TSRMLS_DC) /* {{{ */
{
	if (type < 0 || type > UINT8_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected type to be an unsigned 8-bit integer, %" PHONGO_LONG_FORMAT " given", type);
		return false;
	}

	if ((type == BSON_SUBTYPE_UUID_DEPRECATED || type == BSON_SUBTYPE_UUID) && data_len != PHONGO_BINARY_UUID_SIZE) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected UUID length to be %d bytes, %d given", PHONGO_BINARY_UUID_SIZE, data_len);
		return false;
	}

	intern->data     = estrndup(data, data_len);
	intern->data_len = data_len;
	intern->type     = (uint8_t) type;

	return true;
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_binary_init_from_hash(php_phongo_binary_t* intern, HashTable* props TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *data, *type;

	if ((data = zend_hash_str_find(props, "data", sizeof("data") - 1)) && Z_TYPE_P(data) == IS_STRING &&
		(type = zend_hash_str_find(props, "type", sizeof("type") - 1)) && Z_TYPE_P(type) == IS_LONG) {

		return php_phongo_binary_init(intern, Z_STRVAL_P(data), Z_STRLEN_P(data), Z_LVAL_P(type) TSRMLS_CC);
	}
#else
	zval **data, **type;

	if (zend_hash_find(props, "data", sizeof("data"), (void**) &data) == SUCCESS && Z_TYPE_PP(data) == IS_STRING &&
		zend_hash_find(props, "type", sizeof("type"), (void**) &type) == SUCCESS && Z_TYPE_PP(type) == IS_LONG) {

		return php_phongo_binary_init(intern, Z_STRVAL_PP(data), Z_STRLEN_PP(data), Z_LVAL_PP(type) TSRMLS_CC);
	}
#endif

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"data\" string and \"type\" integer fields", ZSTR_VAL(php_phongo_binary_ce->name));
	return false;
} /* }}} */

/* {{{ proto void MongoDB\BSON\Binary::__construct(string $data, int $type)
   Construct a new BSON binary type */
static PHP_METHOD(Binary, __construct)
{
	php_phongo_binary_t* intern;
	zend_error_handling  error_handling;
	char*                data;
	phongo_zpp_char_len  data_len;
	phongo_long          type;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_BINARY_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &data, &data_len, &type) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	php_phongo_binary_init(intern, data, data_len, type TSRMLS_CC);
} /* }}} */

/* {{{ proto void MongoDB\BSON\Binary::__set_state(array $properties)
*/
static PHP_METHOD(Binary, __set_state)
{
	php_phongo_binary_t* intern;
	HashTable*           props;
	zval*                array;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_binary_ce);

	intern = Z_BINARY_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_binary_init_from_hash(intern, props TSRMLS_CC);
} /* }}} */

/* {{{ proto string MongoDB\BSON\Binary::__toString()
   Return the Binary's data string. */
static PHP_METHOD(Binary, __toString)
{
	php_phongo_binary_t* intern;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_BINARY_OBJ_P(getThis());

	PHONGO_RETURN_STRINGL(intern->data, intern->data_len);
} /* }}} */

/* {{{ proto string MongoDB\BSON\Binary::getData()
*/
static PHP_METHOD(Binary, getData)
{
	php_phongo_binary_t* intern;

	intern = Z_BINARY_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	PHONGO_RETURN_STRINGL(intern->data, intern->data_len);
} /* }}} */

/* {{{ proto integer MongoDB\BSON\Binary::getType()
*/
static PHP_METHOD(Binary, getType)
{
	php_phongo_binary_t* intern;

	intern = Z_BINARY_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(intern->type);
} /* }}} */

/* {{{ proto array MongoDB\BSON\Binary::jsonSerialize()
*/
static PHP_METHOD(Binary, jsonSerialize)
{
	php_phongo_binary_t* intern;
	char                 type[3];
	int                  type_len;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_BINARY_OBJ_P(getThis());

	array_init_size(return_value, 2);

#if PHP_VERSION_ID >= 70000
	{
		zend_string* data = php_base64_encode((unsigned char*) intern->data, intern->data_len);
		ADD_ASSOC_STRINGL(return_value, "$binary", ZSTR_VAL(data), ZSTR_LEN(data));
		zend_string_free(data);
	}
#else
	{
		int            data_len = 0;
		unsigned char* data     = php_base64_encode((unsigned char*) intern->data, intern->data_len, &data_len);
		ADD_ASSOC_STRINGL(return_value, "$binary", (char*) data, data_len);
		efree(data);
	}
#endif

	type_len = snprintf(type, sizeof(type), "%02x", intern->type);
	ADD_ASSOC_STRINGL(return_value, "$type", type, type_len);
} /* }}} */

/* {{{ proto string MongoDB\BSON\Binary::serialize()
*/
static PHP_METHOD(Binary, serialize)
{
	php_phongo_binary_t* intern;
	ZVAL_RETVAL_TYPE     retval;
	php_serialize_data_t var_hash;
	smart_str            buf = { 0 };

	intern = Z_BINARY_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

#if PHP_VERSION_ID >= 70000
	array_init_size(&retval, 2);
	ADD_ASSOC_STRINGL(&retval, "data", intern->data, intern->data_len);
	ADD_ASSOC_LONG_EX(&retval, "type", intern->type);
#else
	ALLOC_INIT_ZVAL(retval);
	array_init_size(retval, 2);
	ADD_ASSOC_STRINGL(retval, "data", intern->data, intern->data_len);
	ADD_ASSOC_LONG_EX(retval, "type", intern->type);
#endif

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash TSRMLS_CC);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\BSON\Binary::unserialize(string $serialized)
*/
static PHP_METHOD(Binary, unserialize)
{
	php_phongo_binary_t* intern;
	zend_error_handling  error_handling;
	char*                serialized;
	phongo_zpp_char_len  serialized_len;
#if PHP_VERSION_ID >= 70000
	zval props;
#else
	zval* props;
#endif
	php_unserialize_data_t var_hash;

	intern = Z_BINARY_OBJ_P(getThis());

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
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "%s unserialization failed", ZSTR_VAL(php_phongo_binary_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

#if PHP_VERSION_ID >= 70000
	php_phongo_binary_init_from_hash(intern, HASH_OF(&props) TSRMLS_CC);
#else
	php_phongo_binary_init_from_hash(intern, HASH_OF(props) TSRMLS_CC);
#endif
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ MongoDB\BSON\Binary function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Binary___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Binary___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Binary_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Binary_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_binary_me[] = {
	/* clang-format off */
	PHP_ME(Binary, __construct, ai_Binary___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Binary, __set_state, ai_Binary___set_state, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Binary, __toString, ai_Binary_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Binary, jsonSerialize, ai_Binary_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Binary, serialize, ai_Binary_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Binary, unserialize, ai_Binary_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Binary, getData, ai_Binary_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Binary, getType, ai_Binary_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\BSON\Binary object handlers */
static zend_object_handlers php_phongo_handler_binary;

static void php_phongo_binary_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_binary_t* intern = Z_OBJ_BINARY(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->data) {
		efree(intern->data);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_binary_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_binary_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_binary_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_binary;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_binary_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_binary;

		return retval;
	}
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_binary_clone_object(zval* object TSRMLS_DC) /* {{{ */
{
	php_phongo_binary_t*        intern;
	php_phongo_binary_t*        new_intern;
	phongo_create_object_retval new_object;

	intern     = Z_BINARY_OBJ_P(object);
	new_object = php_phongo_binary_create_object(Z_OBJCE_P(object) TSRMLS_CC);

#if PHP_VERSION_ID >= 70000
	new_intern = Z_OBJ_BINARY(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std TSRMLS_CC);
#else
	{
		zend_object_handle handle = Z_OBJ_HANDLE_P(object);

		new_intern = (php_phongo_binary_t*) zend_object_store_get_object_by_handle(new_object.handle TSRMLS_CC);
		zend_objects_clone_members(&new_intern->std, new_object, &intern->std, handle TSRMLS_CC);
	}
#endif

	php_phongo_binary_init(new_intern, intern->data, intern->data_len, intern->type TSRMLS_CC);

	return new_object;
} /* }}} */

static int php_phongo_binary_compare_objects(zval* o1, zval* o2 TSRMLS_DC) /* {{{ */
{
	php_phongo_binary_t *intern1, *intern2;

	intern1 = Z_BINARY_OBJ_P(o1);
	intern2 = Z_BINARY_OBJ_P(o2);

	/* MongoDB compares binary types first by the data length, then by the type
	 * byte, and finally by the binary data itself. */
	if (intern1->data_len != intern2->data_len) {
		return intern1->data_len < intern2->data_len ? -1 : 1;
	}

	if (intern1->type != intern2->type) {
		return intern1->type < intern2->type ? -1 : 1;
	}

	return zend_binary_strcmp(intern1->data, intern1->data_len, intern2->data, intern2->data_len);
} /* }}} */

static HashTable* php_phongo_binary_get_gc(zval* object, phongo_get_gc_table table, int* n TSRMLS_DC) /* {{{ */
{
	*table = NULL;
	*n     = 0;

	return Z_BINARY_OBJ_P(object)->properties;
} /* }}} */

static HashTable* php_phongo_binary_get_properties_hash(zval* object, bool is_debug TSRMLS_DC) /* {{{ */
{
	php_phongo_binary_t* intern;
	HashTable*           props;

	intern = Z_BINARY_OBJ_P(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 2);

	if (!intern->data) {
		return props;
	}

#if PHP_VERSION_ID >= 70000
	{
		zval data, type;

		ZVAL_STRINGL(&data, intern->data, intern->data_len);
		zend_hash_str_update(props, "data", sizeof("data") - 1, &data);

		ZVAL_LONG(&type, intern->type);
		zend_hash_str_update(props, "type", sizeof("type") - 1, &type);
	}
#else
	{
		zval *data, *type;

		MAKE_STD_ZVAL(data);
		ZVAL_STRINGL(data, intern->data, intern->data_len, 1);
		zend_hash_update(props, "data", sizeof("data"), &data, sizeof(data), NULL);

		MAKE_STD_ZVAL(type);
		ZVAL_LONG(type, intern->type);
		zend_hash_update(props, "type", sizeof("type"), &type, sizeof(type), NULL);
	}
#endif

	return props;
} /* }}} */

static HashTable* php_phongo_binary_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_binary_get_properties_hash(object, true TSRMLS_CC);
} /* }}} */

static HashTable* php_phongo_binary_get_properties(zval* object TSRMLS_DC) /* {{{ */
{
	return php_phongo_binary_get_properties_hash(object, false TSRMLS_CC);
} /* }}} */
/* }}} */

void php_phongo_binary_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Binary", php_phongo_binary_me);
	php_phongo_binary_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_binary_ce->create_object = php_phongo_binary_create_object;
	PHONGO_CE_FINAL(php_phongo_binary_ce);

	zend_class_implements(php_phongo_binary_ce TSRMLS_CC, 1, php_phongo_binary_interface_ce);
	zend_class_implements(php_phongo_binary_ce TSRMLS_CC, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_binary_ce TSRMLS_CC, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_binary_ce TSRMLS_CC, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_binary, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_binary.clone_obj       = php_phongo_binary_clone_object;
	php_phongo_handler_binary.compare_objects = php_phongo_binary_compare_objects;
	php_phongo_handler_binary.get_debug_info  = php_phongo_binary_get_debug_info;
	php_phongo_handler_binary.get_gc          = php_phongo_binary_get_gc;
	php_phongo_handler_binary.get_properties  = php_phongo_binary_get_properties;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_binary.free_obj = php_phongo_binary_free_object;
	php_phongo_handler_binary.offset   = XtOffsetOf(php_phongo_binary_t, std);
#endif

	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_GENERIC"), BSON_SUBTYPE_BINARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_FUNCTION"), BSON_SUBTYPE_FUNCTION TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_OLD_BINARY"), BSON_SUBTYPE_BINARY_DEPRECATED TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_OLD_UUID"), BSON_SUBTYPE_UUID_DEPRECATED TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_UUID"), BSON_SUBTYPE_UUID TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_MD5"), BSON_SUBTYPE_MD5 TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_ENCRYPTED"), BSON_SUBTYPE_ENCRYPTED TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_binary_ce, ZEND_STRL("TYPE_USER_DEFINED"), BSON_SUBTYPE_USER TSRMLS_CC);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
