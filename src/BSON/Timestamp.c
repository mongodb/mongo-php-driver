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

zend_class_entry* php_phongo_timestamp_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_timestamp_init(php_phongo_timestamp_t* intern, int64_t increment, int64_t timestamp) /* {{{ */
{
	if (increment < 0 || increment > UINT32_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected increment to be an unsigned 32-bit integer, %" PRId64 " given", increment);
		return false;
	}

	if (timestamp < 0 || timestamp > UINT32_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected timestamp to be an unsigned 32-bit integer, %" PRId64 " given", timestamp);
		return false;
	}

	intern->increment   = (uint32_t) increment;
	intern->timestamp   = (uint32_t) timestamp;
	intern->initialized = true;

	return true;
} /* }}} */

/* Initialize the object from numeric strings and return whether it was
 * successful. An exception will be thrown on error. */
static bool php_phongo_timestamp_init_from_string(php_phongo_timestamp_t* intern, const char* s_increment, size_t s_increment_len, const char* s_timestamp, size_t s_timestamp_len) /* {{{ */
{
	int64_t increment, timestamp;

	if (!php_phongo_parse_int64(&increment, s_increment, s_increment_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit integer increment for %s initialization", s_increment, ZSTR_VAL(php_phongo_timestamp_ce->name));
		return false;
	}

	if (!php_phongo_parse_int64(&timestamp, s_timestamp, s_timestamp_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit integer timestamp for %s initialization", s_timestamp, ZSTR_VAL(php_phongo_timestamp_ce->name));
		return false;
	}

	return php_phongo_timestamp_init(intern, increment, timestamp);
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_timestamp_init_from_hash(php_phongo_timestamp_t* intern, HashTable* props) /* {{{ */
{
	zval *increment, *timestamp;

	if ((increment = zend_hash_str_find(props, "increment", sizeof("increment") - 1)) && Z_TYPE_P(increment) == IS_LONG &&
		(timestamp = zend_hash_str_find(props, "timestamp", sizeof("timestamp") - 1)) && Z_TYPE_P(timestamp) == IS_LONG) {

		return php_phongo_timestamp_init(intern, Z_LVAL_P(increment), Z_LVAL_P(timestamp));
	}
	if ((increment = zend_hash_str_find(props, "increment", sizeof("increment") - 1)) && Z_TYPE_P(increment) == IS_STRING &&
		(timestamp = zend_hash_str_find(props, "timestamp", sizeof("timestamp") - 1)) && Z_TYPE_P(timestamp) == IS_STRING) {

		return php_phongo_timestamp_init_from_string(intern, Z_STRVAL_P(increment), Z_STRLEN_P(increment), Z_STRVAL_P(timestamp), Z_STRLEN_P(timestamp));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"increment\" and \"timestamp\" integer or numeric string fields", ZSTR_VAL(php_phongo_timestamp_ce->name));
	return false;
} /* }}} */

/* {{{ proto void MongoDB\BSON\Timestamp::__construct(int|string $increment, int|string $timestamp)
   Construct a new BSON timestamp type, which consists of a 4-byte increment and
   4-byte timestamp. */
static PHP_METHOD(Timestamp, __construct)
{
	php_phongo_timestamp_t* intern;
	zend_error_handling     error_handling;
	zval *                  increment = NULL, *timestamp = NULL;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	intern = Z_TIMESTAMP_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &increment, &timestamp) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if (Z_TYPE_P(increment) == IS_LONG && Z_TYPE_P(timestamp) == IS_LONG) {
		php_phongo_timestamp_init(intern, Z_LVAL_P(increment), Z_LVAL_P(timestamp));
		return;
	}

	if (Z_TYPE_P(increment) == IS_LONG) {
		convert_to_string(increment);
	}

	if (Z_TYPE_P(increment) != IS_STRING) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected increment to be an unsigned 32-bit integer or string, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(increment));
		return;
	}

	if (Z_TYPE_P(timestamp) == IS_LONG) {
		convert_to_string(timestamp);
	}

	if (Z_TYPE_P(timestamp) != IS_STRING) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected timestamp to be an unsigned 32-bit integer or string, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(timestamp));
		return;
	}

	php_phongo_timestamp_init_from_string(intern, Z_STRVAL_P(increment), Z_STRLEN_P(increment), Z_STRVAL_P(timestamp), Z_STRLEN_P(timestamp));
} /* }}} */

/* {{{ proto integer MongoDB\BSON\Timestamp::getIncrement()
*/
static PHP_METHOD(Timestamp, getIncrement)
{
	php_phongo_timestamp_t* intern;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETVAL_LONG(intern->increment);
} /* }}} */

/* {{{ proto integer MongoDB\BSON\Timestamp::getTimestamp()
*/
static PHP_METHOD(Timestamp, getTimestamp)
{
	php_phongo_timestamp_t* intern;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETVAL_LONG(intern->timestamp);
} /* }}} */

/* {{{ proto void MongoDB\BSON\Timestamp::__set_state(array $properties)
*/
static PHP_METHOD(Timestamp, __set_state)
{
	php_phongo_timestamp_t* intern;
	HashTable*              props;
	zval*                   array;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_timestamp_ce);

	intern = Z_TIMESTAMP_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_timestamp_init_from_hash(intern, props);
} /* }}} */

/* {{{ proto string MongoDB\BSON\Timestamp::__toString()
   Returns a string in the form: [increment:timestamp] */
static PHP_METHOD(Timestamp, __toString)
{
	php_phongo_timestamp_t* intern;
	char*                   retval;
	int                     retval_len;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	retval_len = spprintf(&retval, 0, "[%" PRIu32 ":%" PRIu32 "]", intern->increment, intern->timestamp);
	RETVAL_STRINGL(retval, retval_len);
	efree(retval);
} /* }}} */

/* {{{ proto array MongoDB\BSON\Timestamp::jsonSerialize()
*/
static PHP_METHOD(Timestamp, jsonSerialize)
{
	php_phongo_timestamp_t* intern;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	array_init_size(return_value, 1);

	{
		zval ts;

		array_init_size(&ts, 2);
		ADD_ASSOC_LONG_EX(&ts, "t", intern->timestamp);
		ADD_ASSOC_LONG_EX(&ts, "i", intern->increment);
		ADD_ASSOC_ZVAL_EX(return_value, "$timestamp", &ts);
	}
} /* }}} */

/* {{{ proto string MongoDB\BSON\Timestamp::serialize()
*/
static PHP_METHOD(Timestamp, serialize)
{
	php_phongo_timestamp_t* intern;
	zval                    retval;
	php_serialize_data_t    var_hash;
	smart_str               buf = { 0 };
	char                    s_increment[12];
	char                    s_timestamp[12];
	int                     s_increment_len;
	int                     s_timestamp_len;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	s_increment_len = snprintf(s_increment, sizeof(s_increment), "%" PRIu32, intern->increment);
	s_timestamp_len = snprintf(s_timestamp, sizeof(s_timestamp), "%" PRIu32, intern->timestamp);

	array_init_size(&retval, 2);
	ADD_ASSOC_STRINGL(&retval, "increment", s_increment, s_increment_len);
	ADD_ASSOC_STRINGL(&retval, "timestamp", s_timestamp, s_timestamp_len);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\BSON\Timestamp::unserialize(string $serialized)
*/
static PHP_METHOD(Timestamp, unserialize)
{
	php_phongo_timestamp_t* intern;
	zend_error_handling     error_handling;
	char*                   serialized;
	size_t                  serialized_len;
	zval                    props;
	php_unserialize_data_t  var_hash;

	intern = Z_TIMESTAMP_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_timestamp_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_timestamp_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ MongoDB\BSON\Timestamp function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Timestamp___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, increment)
	ZEND_ARG_INFO(0, timestamp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Timestamp___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Timestamp_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Timestamp_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_timestamp_me[] = {
	/* clang-format off */
	PHP_ME(Timestamp, __construct, ai_Timestamp___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Timestamp, __set_state, ai_Timestamp___set_state, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Timestamp, __toString, ai_Timestamp_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Timestamp, jsonSerialize, ai_Timestamp_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Timestamp, serialize, ai_Timestamp_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Timestamp, unserialize, ai_Timestamp_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Timestamp, getIncrement, ai_Timestamp_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Timestamp, getTimestamp, ai_Timestamp_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\BSON\Timestamp object handlers */
static zend_object_handlers php_phongo_handler_timestamp;

static void php_phongo_timestamp_free_object(zend_object* object) /* {{{ */
{
	php_phongo_timestamp_t* intern = Z_OBJ_TIMESTAMP(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
} /* }}} */

static zend_object* php_phongo_timestamp_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_timestamp_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_timestamp_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_timestamp;

	return &intern->std;
} /* }}} */

static zend_object* php_phongo_timestamp_clone_object(zval* object) /* {{{ */
{
	php_phongo_timestamp_t* intern;
	php_phongo_timestamp_t* new_intern;
	zend_object*            new_object;

	intern     = Z_TIMESTAMP_OBJ_P(object);
	new_object = php_phongo_timestamp_create_object(Z_OBJCE_P(object));

	new_intern = Z_OBJ_TIMESTAMP(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_timestamp_init(new_intern, intern->increment, intern->timestamp);

	return new_object;
} /* }}} */

static int php_phongo_timestamp_compare_objects(zval* o1, zval* o2) /* {{{ */
{
	php_phongo_timestamp_t *intern1, *intern2;

	intern1 = Z_TIMESTAMP_OBJ_P(o1);
	intern2 = Z_TIMESTAMP_OBJ_P(o2);

	/* MongoDB compares the timestamp before the increment. */
	if (intern1->timestamp != intern2->timestamp) {
		return intern1->timestamp < intern2->timestamp ? -1 : 1;
	}

	if (intern1->increment != intern2->increment) {
		return intern1->increment < intern2->increment ? -1 : 1;
	}

	return 0;
} /* }}} */

static HashTable* php_phongo_timestamp_get_gc(zval* object, zval** table, int* n) /* {{{ */
{
	*table = NULL;
	*n     = 0;

	return Z_TIMESTAMP_OBJ_P(object)->properties;
} /* }}} */

static HashTable* php_phongo_timestamp_get_properties_hash(zval* object, bool is_debug) /* {{{ */
{
	php_phongo_timestamp_t* intern;
	HashTable*              props;
	char                    s_increment[24];
	char                    s_timestamp[24];
	int                     s_increment_len;
	int                     s_timestamp_len;

	intern = Z_TIMESTAMP_OBJ_P(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 2);

	if (!intern->initialized) {
		return props;
	}

	s_increment_len = snprintf(s_increment, sizeof(s_increment), "%" PRIu32, intern->increment);
	s_timestamp_len = snprintf(s_timestamp, sizeof(s_timestamp), "%" PRIu32, intern->timestamp);

	{
		zval increment, timestamp;

		ZVAL_STRINGL(&increment, s_increment, s_increment_len);
		zend_hash_str_update(props, "increment", sizeof("increment") - 1, &increment);

		ZVAL_STRINGL(&timestamp, s_timestamp, s_timestamp_len);
		zend_hash_str_update(props, "timestamp", sizeof("timestamp") - 1, &timestamp);
	}

	return props;
} /* }}} */

static HashTable* php_phongo_timestamp_get_debug_info(zval* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_timestamp_get_properties_hash(object, true);
} /* }}} */

static HashTable* php_phongo_timestamp_get_properties(zval* object) /* {{{ */
{
	return php_phongo_timestamp_get_properties_hash(object, false);
} /* }}} */
/* }}} */

void php_phongo_timestamp_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Timestamp", php_phongo_timestamp_me);
	php_phongo_timestamp_ce                = zend_register_internal_class(&ce);
	php_phongo_timestamp_ce->create_object = php_phongo_timestamp_create_object;
	PHONGO_CE_FINAL(php_phongo_timestamp_ce);

	zend_class_implements(php_phongo_timestamp_ce, 1, php_phongo_timestamp_interface_ce);
	zend_class_implements(php_phongo_timestamp_ce, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_timestamp_ce, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_timestamp_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_timestamp, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_timestamp.clone_obj       = php_phongo_timestamp_clone_object;
	php_phongo_handler_timestamp.compare_objects = php_phongo_timestamp_compare_objects;
	php_phongo_handler_timestamp.get_debug_info  = php_phongo_timestamp_get_debug_info;
	php_phongo_handler_timestamp.get_gc          = php_phongo_timestamp_get_gc;
	php_phongo_handler_timestamp.get_properties  = php_phongo_timestamp_get_properties;
	php_phongo_handler_timestamp.free_obj        = php_phongo_timestamp_free_object;
	php_phongo_handler_timestamp.offset          = XtOffsetOf(php_phongo_timestamp_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
