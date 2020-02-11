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

zend_class_entry* php_phongo_readconcern_ce;

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_readconcern_init_from_hash(php_phongo_readconcern_t* intern, HashTable* props) /* {{{ */
{
	zval* level;

	intern->read_concern = mongoc_read_concern_new();

	if ((level = zend_hash_str_find(props, "level", sizeof("level") - 1))) {
		if (Z_TYPE_P(level) == IS_STRING) {
			mongoc_read_concern_set_level(intern->read_concern, Z_STRVAL_P(level));
			return true;
		}

		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"level\" string field", ZSTR_VAL(php_phongo_readconcern_ce->name));
		goto failure;
	}

	return true;

failure:
	mongoc_read_concern_destroy(intern->read_concern);
	intern->read_concern = NULL;
	return false;
} /* }}} */

/* {{{ proto void MongoDB\Driver\ReadConcern::__construct([string $level])
   Constructs a new ReadConcern */
static PHP_METHOD(ReadConcern, __construct)
{
	php_phongo_readconcern_t* intern;
	zend_error_handling       error_handling;
	char*                     level     = NULL;
	size_t                    level_len = 0;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	intern = Z_READCONCERN_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "|s!", &level, &level_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	intern->read_concern = mongoc_read_concern_new();

	if (level) {
		mongoc_read_concern_set_level(intern->read_concern, level);
	}
} /* }}} */

/* {{{ proto void MongoDB\BSON\ReadConcern::__set_state(array $properties)
*/
static PHP_METHOD(ReadConcern, __set_state)
{
	php_phongo_readconcern_t* intern;
	HashTable*                props;
	zval*                     array;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_readconcern_ce);

	intern = Z_READCONCERN_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_readconcern_init_from_hash(intern, props);
} /* }}} */

/* {{{ proto string|null MongoDB\Driver\ReadConcern::getLevel()
   Returns the ReadConcern "level" option */
static PHP_METHOD(ReadConcern, getLevel)
{
	php_phongo_readconcern_t* intern;
	const char*               level;

	intern = Z_READCONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	level = mongoc_read_concern_get_level(intern->read_concern);

	if (level) {
		RETURN_STRING(level);
	}

	RETURN_NULL();
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\ReadConcern::isDefault()
   Returns whether the read concern has not been modified (i.e. constructed
   without a level or from a Manager with no read concern URI options). */
static PHP_METHOD(ReadConcern, isDefault)
{
	php_phongo_readconcern_t* intern;

	intern = Z_READCONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_BOOL(mongoc_read_concern_is_default(intern->read_concern));
} /* }}} */

static HashTable* php_phongo_read_concern_get_properties_hash(zval* object, bool is_debug) /* {{{ */
{
	php_phongo_readconcern_t* intern;
	HashTable*                props;
	const char*               level;

	intern = Z_READCONCERN_OBJ_P(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 1);

	if (!intern->read_concern) {
		return props;
	}

	level = mongoc_read_concern_get_level(intern->read_concern);

	if (level) {
		zval z_level;

		ZVAL_STRING(&z_level, level);
		zend_hash_str_update(props, "level", sizeof("level") - 1, &z_level);
	}

	return props;
} /* }}} */

/* {{{ proto array MongoDB\Driver\ReadConcern::bsonSerialize()
*/
static PHP_METHOD(ReadConcern, bsonSerialize)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ZVAL_ARR(return_value, php_phongo_read_concern_get_properties_hash(getThis(), true));
	convert_to_object(return_value);
} /* }}} */

/* {{{ proto string MongoDB\Driver\ReadConcern::serialize()
*/
static PHP_METHOD(ReadConcern, serialize)
{
	php_phongo_readconcern_t* intern;
	zval                      retval;
	php_serialize_data_t      var_hash;
	smart_str                 buf = { 0 };
	const char*               level;

	intern = Z_READCONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!intern->read_concern) {
		return;
	}

	level = mongoc_read_concern_get_level(intern->read_concern);

	if (!level) {
		RETURN_STRING("");
	}

	array_init_size(&retval, 1);
	ADD_ASSOC_STRING(&retval, "level", level);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\Driver\ReadConcern::unserialize(string $serialized)
*/
static PHP_METHOD(ReadConcern, unserialize)
{
	php_phongo_readconcern_t* intern;
	zend_error_handling       error_handling;
	char*                     serialized;
	size_t                    serialized_len;
	zval                      props;
	php_unserialize_data_t    var_hash;

	intern = Z_READCONCERN_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if (!serialized_len) {
		return;
	}

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_readconcern_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_readconcern_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ MongoDB\Driver\ReadConcern function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_ReadConcern___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, level)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ReadConcern___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ReadConcern_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ReadConcern_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_readconcern_me[] = {
	/* clang-format off */
	PHP_ME(ReadConcern, __construct, ai_ReadConcern___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadConcern, __set_state, ai_ReadConcern___set_state, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(ReadConcern, getLevel, ai_ReadConcern_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadConcern, isDefault, ai_ReadConcern_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadConcern, bsonSerialize, ai_ReadConcern_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadConcern, serialize, ai_ReadConcern_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadConcern, unserialize, ai_ReadConcern_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\ReadConcern object handlers */
static zend_object_handlers php_phongo_handler_readconcern;

static void php_phongo_readconcern_free_object(zend_object* object) /* {{{ */
{
	php_phongo_readconcern_t* intern = Z_OBJ_READCONCERN(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->read_concern) {
		mongoc_read_concern_destroy(intern->read_concern);
	}
}

static zend_object* php_phongo_readconcern_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_readconcern_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_readconcern_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_readconcern;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_readconcern_get_debug_info(zval* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_read_concern_get_properties_hash(object, true);
} /* }}} */

static HashTable* php_phongo_readconcern_get_properties(zval* object) /* {{{ */
{
	return php_phongo_read_concern_get_properties_hash(object, false);
} /* }}} */

void php_phongo_readconcern_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ReadConcern", php_phongo_readconcern_me);
	php_phongo_readconcern_ce                = zend_register_internal_class(&ce);
	php_phongo_readconcern_ce->create_object = php_phongo_readconcern_create_object;
	PHONGO_CE_FINAL(php_phongo_readconcern_ce);

	zend_class_implements(php_phongo_readconcern_ce, 1, php_phongo_serializable_ce);
	zend_class_implements(php_phongo_readconcern_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_readconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_readconcern.get_debug_info = php_phongo_readconcern_get_debug_info;
	php_phongo_handler_readconcern.get_properties = php_phongo_readconcern_get_properties;
	php_phongo_handler_readconcern.free_obj       = php_phongo_readconcern_free_object;
	php_phongo_handler_readconcern.offset         = XtOffsetOf(php_phongo_readconcern_t, std);

	zend_declare_class_constant_stringl(php_phongo_readconcern_ce, ZEND_STRL("LOCAL"), ZEND_STRL(MONGOC_READ_CONCERN_LEVEL_LOCAL));
	zend_declare_class_constant_stringl(php_phongo_readconcern_ce, ZEND_STRL("MAJORITY"), ZEND_STRL(MONGOC_READ_CONCERN_LEVEL_MAJORITY));
	zend_declare_class_constant_stringl(php_phongo_readconcern_ce, ZEND_STRL("LINEARIZABLE"), ZEND_STRL(MONGOC_READ_CONCERN_LEVEL_LINEARIZABLE));
	zend_declare_class_constant_stringl(php_phongo_readconcern_ce, ZEND_STRL("AVAILABLE"), ZEND_STRL(MONGOC_READ_CONCERN_LEVEL_AVAILABLE));
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
