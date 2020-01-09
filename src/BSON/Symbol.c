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
#include "php_bson.h"

zend_class_entry* php_phongo_symbol_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_symbol_init(php_phongo_symbol_t* intern, const char* symbol, phongo_zpp_char_len symbol_len TSRMLS_DC) /* {{{ */
{
	if (strlen(symbol) != (size_t) symbol_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Symbol cannot contain null bytes");
		return false;
	}

	intern->symbol     = estrndup(symbol, symbol_len);
	intern->symbol_len = symbol_len;

	return true;
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_symbol_init_from_hash(php_phongo_symbol_t* intern, HashTable* props TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval* symbol;

	if ((symbol = zend_hash_str_find(props, "symbol", sizeof("symbol") - 1)) && Z_TYPE_P(symbol) == IS_STRING) {
		return php_phongo_symbol_init(intern, Z_STRVAL_P(symbol), Z_STRLEN_P(symbol) TSRMLS_CC);
	}
#else
	zval** symbol;

	if (zend_hash_find(props, "symbol", sizeof("symbol"), (void**) &symbol) == SUCCESS && Z_TYPE_PP(symbol) == IS_STRING) {
		return php_phongo_symbol_init(intern, Z_STRVAL_PP(symbol), Z_STRLEN_PP(symbol) TSRMLS_CC);
	}
#endif

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"symbol\" string field", ZSTR_VAL(php_phongo_symbol_ce->name));
	return false;
} /* }}} */

/* {{{ proto string MongoDB\BSON\Symbol::__toString()
   Return the Symbol's symbol string. */
static PHP_METHOD(Symbol, __toString)
{
	php_phongo_symbol_t* intern;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_SYMBOL_OBJ_P(getThis());

	PHONGO_RETURN_STRINGL(intern->symbol, intern->symbol_len);
} /* }}} */

/* {{{ proto array MongoDB\BSON\Symbol::jsonSerialize()
*/
static PHP_METHOD(Symbol, jsonSerialize)
{
	php_phongo_symbol_t* intern;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_SYMBOL_OBJ_P(getThis());

	array_init_size(return_value, 1);
	ADD_ASSOC_STRINGL(return_value, "$symbol", intern->symbol, intern->symbol_len);
} /* }}} */

/* {{{ proto string MongoDB\BSON\Symbol::serialize()
*/
static PHP_METHOD(Symbol, serialize)
{
	php_phongo_symbol_t* intern;
	ZVAL_RETVAL_TYPE     retval;
	php_serialize_data_t var_hash;
	smart_str            buf = { 0 };

	intern = Z_SYMBOL_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

#if PHP_VERSION_ID >= 70000
	array_init_size(&retval, 1);
	ADD_ASSOC_STRINGL(&retval, "symbol", intern->symbol, intern->symbol_len);
#else
	ALLOC_INIT_ZVAL(retval);
	array_init_size(retval, 1);
	ADD_ASSOC_STRINGL(retval, "symbol", intern->symbol, intern->symbol_len);
#endif

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash TSRMLS_CC);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\BSON\Symbol::unserialize(string $serialized)
*/
static PHP_METHOD(Symbol, unserialize)
{
	php_phongo_symbol_t* intern;
	zend_error_handling  error_handling;
	char*                serialized;
	phongo_zpp_char_len  serialized_len;
#if PHP_VERSION_ID >= 70000
	zval props;
#else
	zval* props;
#endif
	php_unserialize_data_t var_hash;

	intern = Z_SYMBOL_OBJ_P(getThis());

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
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "%s unserialization failed", ZSTR_VAL(php_phongo_symbol_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

#if PHP_VERSION_ID >= 70000
	php_phongo_symbol_init_from_hash(intern, HASH_OF(&props) TSRMLS_CC);
#else
	php_phongo_symbol_init_from_hash(intern, HASH_OF(props) TSRMLS_CC);
#endif
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ MongoDB\BSON\Symbol function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Symbol_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Symbol_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_symbol_me[] = {
	/* clang-format off */
	/* __set_state intentionally missing */
	PHP_ME(Symbol, __toString, ai_Symbol_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Symbol, jsonSerialize, ai_Symbol_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Symbol, serialize, ai_Symbol_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Symbol, unserialize, ai_Symbol_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_Symbol_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\BSON\Symbol object handlers */
static zend_object_handlers php_phongo_handler_symbol;

static void php_phongo_symbol_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_symbol_t* intern = Z_OBJ_SYMBOL(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->symbol) {
		efree(intern->symbol);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_symbol_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_symbol_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_symbol_t, class_type);
	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_symbol;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_symbol_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_symbol;

		return retval;
	}
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_symbol_clone_object(zval* object TSRMLS_DC) /* {{{ */
{
	php_phongo_symbol_t*        intern;
	php_phongo_symbol_t*        new_intern;
	phongo_create_object_retval new_object;

	intern     = Z_SYMBOL_OBJ_P(object);
	new_object = php_phongo_symbol_create_object(Z_OBJCE_P(object) TSRMLS_CC);

#if PHP_VERSION_ID >= 70000
	new_intern = Z_OBJ_SYMBOL(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std TSRMLS_CC);
#else
	{
		zend_object_handle handle = Z_OBJ_HANDLE_P(object);

		new_intern = (php_phongo_symbol_t*) zend_object_store_get_object_by_handle(new_object.handle TSRMLS_CC);
		zend_objects_clone_members(&new_intern->std, new_object, &intern->std, handle TSRMLS_CC);
	}
#endif

	php_phongo_symbol_init(new_intern, intern->symbol, intern->symbol_len TSRMLS_CC);

	return new_object;
} /* }}} */

static int php_phongo_symbol_compare_objects(zval* o1, zval* o2 TSRMLS_DC) /* {{{ */
{
	php_phongo_symbol_t *intern1, *intern2;

	intern1 = Z_SYMBOL_OBJ_P(o1);
	intern2 = Z_SYMBOL_OBJ_P(o2);

	return strcmp(intern1->symbol, intern2->symbol);
} /* }}} */

static HashTable* php_phongo_symbol_get_gc(zval* object, phongo_get_gc_table table, int* n TSRMLS_DC) /* {{{ */
{
	*table = NULL;
	*n     = 0;

	return Z_SYMBOL_OBJ_P(object)->properties;
} /* }}} */

HashTable* php_phongo_symbol_get_properties_hash(zval* object, bool is_debug TSRMLS_DC) /* {{{ */
{
	php_phongo_symbol_t* intern;
	HashTable*           props;

	intern = Z_SYMBOL_OBJ_P(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 2);

	if (!intern->symbol) {
		return props;
	}

#if PHP_VERSION_ID >= 70000
	{
		zval symbol;

		ZVAL_STRING(&symbol, intern->symbol);
		zend_hash_str_update(props, "symbol", sizeof("symbol") - 1, &symbol);
	}
#else
	{
		zval* symbol;

		MAKE_STD_ZVAL(symbol);
		ZVAL_STRING(symbol, intern->symbol, 1);
		zend_hash_update(props, "symbol", sizeof("symbol"), &symbol, sizeof(symbol), NULL);
	}
#endif

	return props;
} /* }}} */

static HashTable* php_phongo_symbol_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_symbol_get_properties_hash(object, true TSRMLS_CC);
} /* }}} */

static HashTable* php_phongo_symbol_get_properties(zval* object TSRMLS_DC) /* {{{ */
{
	return php_phongo_symbol_get_properties_hash(object, false TSRMLS_CC);
} /* }}} */
/* }}} */

void php_phongo_symbol_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Symbol", php_phongo_symbol_me);
	php_phongo_symbol_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_symbol_ce->create_object = php_phongo_symbol_create_object;
	PHONGO_CE_FINAL(php_phongo_symbol_ce);

	zend_class_implements(php_phongo_symbol_ce TSRMLS_CC, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_symbol_ce TSRMLS_CC, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_symbol_ce TSRMLS_CC, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_symbol, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_symbol.clone_obj       = php_phongo_symbol_clone_object;
	php_phongo_handler_symbol.compare_objects = php_phongo_symbol_compare_objects;
	php_phongo_handler_symbol.get_debug_info  = php_phongo_symbol_get_debug_info;
	php_phongo_handler_symbol.get_gc          = php_phongo_symbol_get_gc;
	php_phongo_handler_symbol.get_properties  = php_phongo_symbol_get_properties;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_symbol.free_obj = php_phongo_symbol_free_object;
	php_phongo_handler_symbol.offset   = XtOffsetOf(php_phongo_symbol_t, std);
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
