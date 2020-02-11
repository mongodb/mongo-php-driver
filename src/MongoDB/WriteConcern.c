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

zend_class_entry* php_phongo_writeconcern_ce;

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_writeconcern_init_from_hash(php_phongo_writeconcern_t* intern, HashTable* props) /* {{{ */
{
	zval *w, *wtimeout, *j;

	intern->write_concern = mongoc_write_concern_new();

	if ((w = zend_hash_str_find(props, "w", sizeof("w") - 1))) {
		if (Z_TYPE_P(w) == IS_LONG) {
			if (Z_LVAL_P(w) < -3) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"w\" integer field to be >= -3", ZSTR_VAL(php_phongo_writeconcern_ce->name));
				goto failure;
			}
			mongoc_write_concern_set_w(intern->write_concern, Z_LVAL_P(w));
		} else if (Z_TYPE_P(w) == IS_STRING) {
			if (strcmp(Z_STRVAL_P(w), PHONGO_WRITE_CONCERN_W_MAJORITY) == 0) {
				mongoc_write_concern_set_w(intern->write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);
			} else {
				mongoc_write_concern_set_wtag(intern->write_concern, Z_STRVAL_P(w));
			}
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"w\" field to be integer or string", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if ((wtimeout = zend_hash_str_find(props, "wtimeout", sizeof("wtimeout") - 1))) {
		if (Z_TYPE_P(wtimeout) == IS_LONG) {
			if (Z_LVAL_P(wtimeout) < 0) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"wtimeout\" integer field to be >= 0", ZSTR_VAL(php_phongo_writeconcern_ce->name));
				goto failure;
			}

			mongoc_write_concern_set_wtimeout_int64(intern->write_concern, (int64_t) Z_LVAL_P(wtimeout));
		} else if (Z_TYPE_P(wtimeout) == IS_STRING) {
			int64_t timeout;

			if (!php_phongo_parse_int64(&timeout, Z_STRVAL_P(wtimeout), Z_STRLEN_P(wtimeout))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit value for %s initialization", Z_STRVAL_P(wtimeout), ZSTR_VAL(php_phongo_writeconcern_ce->name));
				return false;
			}

			mongoc_write_concern_set_wtimeout_int64(intern->write_concern, timeout);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"wtimeout\" field to be integer or string", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if ((j = zend_hash_str_find(props, "j", sizeof("j") - 1))) {
		if (Z_TYPE_P(j) == IS_TRUE || Z_TYPE_P(j) == IS_FALSE) {
			mongoc_write_concern_set_journal(intern->write_concern, zend_is_true(j));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"j\" field to be boolean", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	return true;

failure:
	mongoc_write_concern_destroy(intern->write_concern);
	intern->write_concern = NULL;
	return false;
} /* }}} */

/* {{{ proto void MongoDB\Driver\WriteConcern::__construct(integer|string $w[, integer $wtimeout[, boolean $journal]])
   Constructs a new WriteConcern */
static PHP_METHOD(WriteConcern, __construct)
{
	php_phongo_writeconcern_t* intern;
	zend_error_handling        error_handling;
	zval *                     w, *journal;
	zend_long                  wtimeout = 0;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z|lz", &w, &wtimeout, &journal) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	intern->write_concern = mongoc_write_concern_new();

	if (Z_TYPE_P(w) == IS_LONG) {
		if (Z_LVAL_P(w) < -3) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected w to be >= -3, %ld given", Z_LVAL_P(w));
			return;
		}
		mongoc_write_concern_set_w(intern->write_concern, Z_LVAL_P(w));
	} else if (Z_TYPE_P(w) == IS_STRING) {
		if (strcmp(Z_STRVAL_P(w), PHONGO_WRITE_CONCERN_W_MAJORITY) == 0) {
			mongoc_write_concern_set_w(intern->write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);
		} else {
			mongoc_write_concern_set_wtag(intern->write_concern, Z_STRVAL_P(w));
		}
	} else {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected w to be integer or string, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(w));
		return;
	}

	switch (ZEND_NUM_ARGS()) {
		case 3:
			if (Z_TYPE_P(journal) != IS_NULL) {
#ifdef ZEND_ENGINE_3
				mongoc_write_concern_set_journal(intern->write_concern, zend_is_true(journal));
#else
				mongoc_write_concern_set_journal(intern->write_concern, Z_BVAL_P(journal));
#endif
			}
			/* fallthrough */
		case 2:
			if (wtimeout < 0) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected wtimeout to be >= 0, %" PHONGO_LONG_FORMAT " given", wtimeout);
				return;
			}

			mongoc_write_concern_set_wtimeout_int64(intern->write_concern, (int64_t) wtimeout);
	}
} /* }}} */

/* {{{ proto void MongoDB\BSON\WriteConcern::__set_state(array $properties)
*/
static PHP_METHOD(WriteConcern, __set_state)
{
	php_phongo_writeconcern_t* intern;
	HashTable*                 props;
	zval*                      array;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_writeconcern_ce);

	intern = Z_WRITECONCERN_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_writeconcern_init_from_hash(intern, props);
} /* }}} */

/* {{{ proto string|integer|null MongoDB\Driver\WriteConcern::getW()
   Returns the WriteConcern "w" option */
static PHP_METHOD(WriteConcern, getW)
{
	php_phongo_writeconcern_t* intern;
	const char*                wtag;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	wtag = mongoc_write_concern_get_wtag(intern->write_concern);

	if (wtag) {
		RETURN_STRING(wtag);
	}

	if (mongoc_write_concern_get_wmajority(intern->write_concern)) {
		RETURN_STRING(PHONGO_WRITE_CONCERN_W_MAJORITY);
	}

	if (mongoc_write_concern_get_w(intern->write_concern) != MONGOC_WRITE_CONCERN_W_DEFAULT) {
		RETURN_LONG(mongoc_write_concern_get_w(intern->write_concern));
	}

	RETURN_NULL();
} /* }}} */

/* {{{ proto integer MongoDB\Driver\WriteConcern::getWtimeout()
   Returns the WriteConcern "wtimeout" option */
static PHP_METHOD(WriteConcern, getWtimeout)
{
	php_phongo_writeconcern_t* intern;
	int64_t                    wtimeout;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	wtimeout = mongoc_write_concern_get_wtimeout_int64(intern->write_concern);

#if SIZEOF_LONG == 4
	if (wtimeout > INT32_MAX || wtimeout < INT32_MIN) {
		zend_error(E_WARNING, "Truncating 64-bit value for wTimeoutMS");
	}
#endif

	RETURN_LONG(wtimeout);
} /* }}} */

/* {{{ proto null|boolean MongoDB\Driver\WriteConcern::getJournal()
   Returns the WriteConcern "journal" option */
static PHP_METHOD(WriteConcern, getJournal)
{
	php_phongo_writeconcern_t* intern;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (mongoc_write_concern_journal_is_set(intern->write_concern)) {
		RETURN_BOOL(mongoc_write_concern_get_journal(intern->write_concern));
	}

	RETURN_NULL();
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\WriteConcern::isDefault()
   Returns whether the write concern has not been modified (i.e. from a Manager
   with no write concern URI options). */
static PHP_METHOD(WriteConcern, isDefault)
{
	php_phongo_writeconcern_t* intern;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_BOOL(mongoc_write_concern_is_default(intern->write_concern));
} /* }}} */

static HashTable* php_phongo_write_concern_get_properties_hash(zval* object, bool is_debug, bool is_bson) /* {{{ */
{
	php_phongo_writeconcern_t* intern;
	HashTable*                 props;
	const char*                wtag;
	int32_t                    w;
	int64_t                    wtimeout;

	intern = Z_WRITECONCERN_OBJ_P(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 4);

	if (!intern->write_concern) {
		return props;
	}

	wtag     = mongoc_write_concern_get_wtag(intern->write_concern);
	w        = mongoc_write_concern_get_w(intern->write_concern);
	wtimeout = mongoc_write_concern_get_wtimeout_int64(intern->write_concern);

	{
		zval z_w;

		if (wtag) {
			ZVAL_STRING(&z_w, wtag);
			zend_hash_str_update(props, "w", sizeof("w") - 1, &z_w);
		} else if (mongoc_write_concern_get_wmajority(intern->write_concern)) {
			ZVAL_STRING(&z_w, PHONGO_WRITE_CONCERN_W_MAJORITY);
			zend_hash_str_update(props, "w", sizeof("w") - 1, &z_w);
		} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
			ZVAL_LONG(&z_w, w);
			zend_hash_str_update(props, "w", sizeof("w") - 1, &z_w);
		}

		if (mongoc_write_concern_journal_is_set(intern->write_concern)) {
			zval z_j;

			ZVAL_BOOL(&z_j, mongoc_write_concern_get_journal(intern->write_concern));
			zend_hash_str_update(props, "j", sizeof("j") - 1, &z_j);
		}

		if (wtimeout != 0) {
			zval z_wtimeout;

			if (is_bson) {
				ZVAL_INT64(&z_wtimeout, wtimeout);
			} else {
#if SIZEOF_LONG == 4
				if (wtimeout > INT32_MAX || wtimeout < INT32_MIN) {
					ZVAL_INT64_STRING(&z_wtimeout, wtimeout);
				} else {
					ZVAL_LONG(&z_wtimeout, wtimeout);
				}
#else
				ZVAL_LONG(&z_wtimeout, wtimeout);
#endif
			}

			zend_hash_str_update(props, "wtimeout", sizeof("wtimeout") - 1, &z_wtimeout);
		}
	}

	return props;
} /* }}} */

/* {{{ proto array MongoDB\Driver\WriteConcern::bsonSerialize()
*/
static PHP_METHOD(WriteConcern, bsonSerialize)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ZVAL_ARR(return_value, php_phongo_write_concern_get_properties_hash(getThis(), true, true));
	convert_to_object(return_value);
} /* }}} */

/* {{{ proto string MongoDB\Driver\WriteConcern::serialize()
*/
static PHP_METHOD(WriteConcern, serialize)
{
	php_phongo_writeconcern_t* intern;
	zval                       retval;
	php_serialize_data_t       var_hash;
	smart_str                  buf = { 0 };
	const char*                wtag;
	int32_t                    w;
	int64_t                    wtimeout;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!intern->write_concern) {
		return;
	}

	wtag     = mongoc_write_concern_get_wtag(intern->write_concern);
	w        = mongoc_write_concern_get_w(intern->write_concern);
	wtimeout = mongoc_write_concern_get_wtimeout_int64(intern->write_concern);

	array_init_size(&retval, 3);

	if (wtag) {
		ADD_ASSOC_STRING(&retval, "w", wtag);
	} else if (mongoc_write_concern_get_wmajority(intern->write_concern)) {
		ADD_ASSOC_STRING(&retval, "w", PHONGO_WRITE_CONCERN_W_MAJORITY);
	} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
		ADD_ASSOC_LONG_EX(&retval, "w", w);
	}

	if (mongoc_write_concern_journal_is_set(intern->write_concern)) {
		ADD_ASSOC_BOOL_EX(&retval, "j", mongoc_write_concern_get_journal(intern->write_concern));
	}

	if (wtimeout != 0) {
		if (wtimeout > INT32_MAX || wtimeout < INT32_MIN) {
			ADD_ASSOC_INT64_AS_STRING(&retval, "wtimeout", wtimeout);
		} else {
			ADD_ASSOC_LONG_EX(&retval, "wtimeout", wtimeout);
		}
	}

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\Driver\WriteConcern::unserialize(string $serialized)
*/
static PHP_METHOD(WriteConcern, unserialize)
{
	php_phongo_writeconcern_t* intern;
	zend_error_handling        error_handling;
	char*                      serialized;
	size_t                     serialized_len;
	zval                       props;
	php_unserialize_data_t     var_hash;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

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
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_writeconcern_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_writeconcern_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ MongoDB\Driver\WriteConcern function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcern___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, w)
	ZEND_ARG_INFO(0, wtimeout)
	ZEND_ARG_INFO(0, journal)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcern___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcern_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcern_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_writeconcern_me[] = {
	/* clang-format off */
	PHP_ME(WriteConcern, __construct, ai_WriteConcern___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, __set_state, ai_WriteConcern___set_state, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(WriteConcern, getW, ai_WriteConcern_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, getWtimeout, ai_WriteConcern_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, getJournal, ai_WriteConcern_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, isDefault, ai_WriteConcern_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, bsonSerialize, ai_WriteConcern_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, serialize, ai_WriteConcern_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, unserialize, ai_WriteConcern_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\WriteConcern object handlers */
static zend_object_handlers php_phongo_handler_writeconcern;

static void php_phongo_writeconcern_free_object(zend_object* object) /* {{{ */
{
	php_phongo_writeconcern_t* intern = Z_OBJ_WRITECONCERN(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}
} /* }}} */

static zend_object* php_phongo_writeconcern_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_writeconcern_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_writeconcern_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_writeconcern;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_writeconcern_get_debug_info(zval* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_write_concern_get_properties_hash(object, true, false);
} /* }}} */

static HashTable* php_phongo_writeconcern_get_properties(zval* object) /* {{{ */
{
	return php_phongo_write_concern_get_properties_hash(object, false, false);
} /* }}} */

void php_phongo_writeconcern_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteConcern", php_phongo_writeconcern_me);
	php_phongo_writeconcern_ce                = zend_register_internal_class(&ce);
	php_phongo_writeconcern_ce->create_object = php_phongo_writeconcern_create_object;
	PHONGO_CE_FINAL(php_phongo_writeconcern_ce);

	zend_class_implements(php_phongo_writeconcern_ce, 1, php_phongo_serializable_ce);
	zend_class_implements(php_phongo_writeconcern_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_writeconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeconcern.get_debug_info = php_phongo_writeconcern_get_debug_info;
	php_phongo_handler_writeconcern.get_properties = php_phongo_writeconcern_get_properties;
	php_phongo_handler_writeconcern.free_obj       = php_phongo_writeconcern_free_object;
	php_phongo_handler_writeconcern.offset         = XtOffsetOf(php_phongo_writeconcern_t, std);

	zend_declare_class_constant_stringl(php_phongo_writeconcern_ce, ZEND_STRL("MAJORITY"), ZEND_STRL(PHONGO_WRITE_CONCERN_W_MAJORITY));
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
