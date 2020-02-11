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
#include <zend_smart_str.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"
#include "php_bson.h"

zend_class_entry* php_phongo_symbol_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_symbol_init(php_phongo_symbol_t* intern, const char* symbol, size_t symbol_len) /* {{{ */
{
	if (strlen(symbol) != (size_t) symbol_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Symbol cannot contain null bytes");
		return false;
	}

	intern->symbol     = estrndup(symbol, symbol_len);
	intern->symbol_len = symbol_len;

	return true;
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_symbol_init_from_hash(php_phongo_symbol_t* intern, HashTable* props) /* {{{ */
{
	zval* symbol;

	if ((symbol = zend_hash_str_find(props, "symbol", sizeof("symbol") - 1)) && Z_TYPE_P(symbol) == IS_STRING) {
		return php_phongo_symbol_init(intern, Z_STRVAL_P(symbol), Z_STRLEN_P(symbol));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"symbol\" string field", ZSTR_VAL(php_phongo_symbol_ce->name));
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

	RETURN_STRINGL(intern->symbol, intern->symbol_len);
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
	zval                 retval;
	php_serialize_data_t var_hash;
	smart_str            buf = { 0 };

	intern = Z_SYMBOL_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	array_init_size(&retval, 1);
	ADD_ASSOC_STRINGL(&retval, "symbol", intern->symbol, intern->symbol_len);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
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
	php_phongo_symbol_t*   intern;
	zend_error_handling    error_handling;
	char*                  serialized;
	size_t                 serialized_len;
	zval                   props;
	php_unserialize_data_t var_hash;

	intern = Z_SYMBOL_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_symbol_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_symbol_init_from_hash(intern, HASH_OF(&props));
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

static void php_phongo_symbol_free_object(zend_object* object) /* {{{ */
{
	php_phongo_symbol_t* intern = Z_OBJ_SYMBOL(object);

	zend_object_std_dtor(&intern->std);

	if (intern->symbol) {
		efree(intern->symbol);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
} /* }}} */

zend_object* php_phongo_symbol_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_symbol_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_symbol_t, class_type);
	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_symbol;

	return &intern->std;
} /* }}} */

static zend_object* php_phongo_symbol_clone_object(zval* object) /* {{{ */
{
	php_phongo_symbol_t* intern;
	php_phongo_symbol_t* new_intern;
	zend_object*         new_object;

	intern     = Z_SYMBOL_OBJ_P(object);
	new_object = php_phongo_symbol_create_object(Z_OBJCE_P(object));

	new_intern = Z_OBJ_SYMBOL(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_symbol_init(new_intern, intern->symbol, intern->symbol_len);

	return new_object;
} /* }}} */

static int php_phongo_symbol_compare_objects(zval* o1, zval* o2) /* {{{ */
{
	php_phongo_symbol_t *intern1, *intern2;

	intern1 = Z_SYMBOL_OBJ_P(o1);
	intern2 = Z_SYMBOL_OBJ_P(o2);

	return strcmp(intern1->symbol, intern2->symbol);
} /* }}} */

static HashTable* php_phongo_symbol_get_gc(zval* object, zval** table, int* n) /* {{{ */
{
	*table = NULL;
	*n     = 0;

	return Z_SYMBOL_OBJ_P(object)->properties;
} /* }}} */

HashTable* php_phongo_symbol_get_properties_hash(zval* object, bool is_debug) /* {{{ */
{
	php_phongo_symbol_t* intern;
	HashTable*           props;

	intern = Z_SYMBOL_OBJ_P(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 2);

	if (!intern->symbol) {
		return props;
	}

	{
		zval symbol;

		ZVAL_STRING(&symbol, intern->symbol);
		zend_hash_str_update(props, "symbol", sizeof("symbol") - 1, &symbol);
	}

	return props;
} /* }}} */

static HashTable* php_phongo_symbol_get_debug_info(zval* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_symbol_get_properties_hash(object, true);
} /* }}} */

static HashTable* php_phongo_symbol_get_properties(zval* object) /* {{{ */
{
	return php_phongo_symbol_get_properties_hash(object, false);
} /* }}} */
/* }}} */

void php_phongo_symbol_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Symbol", php_phongo_symbol_me);
	php_phongo_symbol_ce                = zend_register_internal_class(&ce);
	php_phongo_symbol_ce->create_object = php_phongo_symbol_create_object;
	PHONGO_CE_FINAL(php_phongo_symbol_ce);

	zend_class_implements(php_phongo_symbol_ce, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_symbol_ce, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_symbol_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_symbol, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_symbol.clone_obj       = php_phongo_symbol_clone_object;
	php_phongo_handler_symbol.compare_objects = php_phongo_symbol_compare_objects;
	php_phongo_handler_symbol.get_debug_info  = php_phongo_symbol_get_debug_info;
	php_phongo_handler_symbol.get_gc          = php_phongo_symbol_get_gc;
	php_phongo_handler_symbol.get_properties  = php_phongo_symbol_get_properties;
	php_phongo_handler_symbol.free_obj        = php_phongo_symbol_free_object;
	php_phongo_handler_symbol.offset          = XtOffsetOf(php_phongo_symbol_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
