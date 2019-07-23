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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_writeconcern_ce;

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_writeconcern_init_from_hash(php_phongo_writeconcern_t* intern, HashTable* props TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval *w, *wtimeout, *j;

	intern->write_concern = mongoc_write_concern_new();

	if ((w = zend_hash_str_find(props, "w", sizeof("w") - 1))) {
		if (Z_TYPE_P(w) == IS_LONG) {
			if (Z_LVAL_P(w) < -3) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"w\" integer field to be >= -3", ZSTR_VAL(php_phongo_writeconcern_ce->name));
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
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"w\" field to be integer or string", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if ((wtimeout = zend_hash_str_find(props, "wtimeout", sizeof("wtimeout") - 1))) {
		if (Z_TYPE_P(wtimeout) == IS_LONG) {
			if (Z_LVAL_P(wtimeout) < 0) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"wtimeout\" integer field to be >= 0", ZSTR_VAL(php_phongo_writeconcern_ce->name));
				goto failure;
			}
			if (Z_LVAL_P(wtimeout) > INT32_MAX) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"wtimeout\" integer field to be <= %" PRId32, ZSTR_VAL(php_phongo_writeconcern_ce->name), INT32_MAX);
				goto failure;
			}

			mongoc_write_concern_set_wtimeout_int64(intern->write_concern, (int64_t) Z_LVAL_P(wtimeout));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"wtimeout\" field to be integer", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if ((j = zend_hash_str_find(props, "j", sizeof("j") - 1))) {
		if (Z_TYPE_P(j) == IS_TRUE || Z_TYPE_P(j) == IS_FALSE) {
			mongoc_write_concern_set_journal(intern->write_concern, zend_is_true(j));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"j\" field to be boolean", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}
#else
	zval **w, **wtimeout, **j;

	intern->write_concern = mongoc_write_concern_new();

	if (zend_hash_find(props, "w", sizeof("w"), (void**) &w) == SUCCESS) {
		if (Z_TYPE_PP(w) == IS_LONG) {
			if (Z_LVAL_PP(w) < -3) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"w\" integer field to be >= -3", ZSTR_VAL(php_phongo_writeconcern_ce->name));
				goto failure;
			}
			mongoc_write_concern_set_w(intern->write_concern, Z_LVAL_PP(w));
		} else if (Z_TYPE_PP(w) == IS_STRING) {
			if (strcmp(Z_STRVAL_PP(w), PHONGO_WRITE_CONCERN_W_MAJORITY) == 0) {
				mongoc_write_concern_set_w(intern->write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);
			} else {
				mongoc_write_concern_set_wtag(intern->write_concern, Z_STRVAL_PP(w));
			}
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"w\" field to be integer or string", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if (zend_hash_find(props, "wtimeout", sizeof("wtimeout"), (void**) &wtimeout) == SUCCESS) {
		if (Z_TYPE_PP(wtimeout) == IS_LONG) {
			if (Z_LVAL_PP(wtimeout) < 0) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"wtimeout\" integer field to be >= 0", ZSTR_VAL(php_phongo_writeconcern_ce->name));
				goto failure;
			}
			if (Z_LVAL_PP(wtimeout) > INT32_MAX) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"wtimeout\" integer field to be <= %" PRId32, ZSTR_VAL(php_phongo_writeconcern_ce->name), INT32_MAX);
				goto failure;
			}

			mongoc_write_concern_set_wtimeout_int64(intern->write_concern, (int64_t) Z_LVAL_PP(wtimeout));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"wtimeout\" field to be integer", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if (zend_hash_find(props, "j", sizeof("j"), (void**) &j) == SUCCESS) {
		if (Z_TYPE_PP(j) == IS_BOOL) {
			mongoc_write_concern_set_journal(intern->write_concern, Z_BVAL_PP(j));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"j\" field to be boolean", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

#endif

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
	phongo_long                wtimeout = 0;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|lz", &w, &wtimeout, &journal) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	intern->write_concern = mongoc_write_concern_new();

	if (Z_TYPE_P(w) == IS_LONG) {
		if (Z_LVAL_P(w) < -3) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected w to be >= -3, %ld given", Z_LVAL_P(w));
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
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected w to be integer or string, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(w));
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
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected wtimeout to be >= 0, %" PHONGO_LONG_FORMAT " given", wtimeout);
				return;
			}

			if (wtimeout > INT32_MAX) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected wtimeout to be <= %" PRId32 ", %" PHONGO_LONG_FORMAT " given", INT32_MAX, wtimeout);
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

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_writeconcern_ce);

	intern = Z_WRITECONCERN_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_writeconcern_init_from_hash(intern, props TSRMLS_CC);
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
		PHONGO_RETURN_STRING(wtag);
	}

	if (mongoc_write_concern_get_wmajority(intern->write_concern)) {
		PHONGO_RETURN_STRING(PHONGO_WRITE_CONCERN_W_MAJORITY);
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

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	/* Note: PHP currently enforces that wimeoutMS is a 32-bit integer, so
	 * casting will never truncate the value. This may change with PHPC-1411. */
	RETURN_LONG((int32_t) mongoc_write_concern_get_wtimeout_int64(intern->write_concern));
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

static HashTable* php_phongo_write_concern_get_properties_hash(zval* object, bool is_debug TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcern_t* intern;
	HashTable*                 props;
	const char*                wtag;
	int32_t                    w;
	int32_t                    wtimeout;

	intern = Z_WRITECONCERN_OBJ_P(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 4);

	if (!intern->write_concern) {
		return props;
	}

	wtag     = mongoc_write_concern_get_wtag(intern->write_concern);
	w        = mongoc_write_concern_get_w(intern->write_concern);
	/* Note: PHP currently enforces that wimeoutMS is a 32-bit integer, so
	 * casting will never truncate the value. This may change with PHPC-1411. */
	wtimeout = (int32_t) mongoc_write_concern_get_wtimeout_int64(intern->write_concern);

#if PHP_VERSION_ID >= 70000
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

			ZVAL_LONG(&z_wtimeout, wtimeout);
			zend_hash_str_update(props, "wtimeout", sizeof("wtimeout") - 1, &z_wtimeout);
		}
#else
	{
		zval* z_w;

		if (wtag) {
			MAKE_STD_ZVAL(z_w);
			ZVAL_STRING(z_w, wtag, 1);
			zend_hash_update(props, "w", sizeof("w"), &z_w, sizeof(z_w), NULL);
		} else if (mongoc_write_concern_get_wmajority(intern->write_concern)) {
			MAKE_STD_ZVAL(z_w);
			ZVAL_STRING(z_w, PHONGO_WRITE_CONCERN_W_MAJORITY, 1);
			zend_hash_update(props, "w", sizeof("w"), &z_w, sizeof(z_w), NULL);
		} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
			MAKE_STD_ZVAL(z_w);
			ZVAL_LONG(z_w, w);
			zend_hash_update(props, "w", sizeof("w"), &z_w, sizeof(z_w), NULL);
		}

		if (mongoc_write_concern_journal_is_set(intern->write_concern)) {
			zval* z_j;

			MAKE_STD_ZVAL(z_j);
			ZVAL_BOOL(z_j, mongoc_write_concern_get_journal(intern->write_concern));
			zend_hash_update(props, "j", sizeof("j"), &z_j, sizeof(z_j), NULL);
		}

		if (wtimeout != 0) {
			zval* z_wtimeout;

			MAKE_STD_ZVAL(z_wtimeout);
			ZVAL_LONG(z_wtimeout, wtimeout);
			zend_hash_update(props, "wtimeout", sizeof("wtimeout"), &z_wtimeout, sizeof(z_wtimeout), NULL);
		}
#endif
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

	ZVAL_ARR(return_value, php_phongo_write_concern_get_properties_hash(getThis(), true TSRMLS_CC));
	convert_to_object(return_value);
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
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\WriteConcern object handlers */
static zend_object_handlers php_phongo_handler_writeconcern;

static void php_phongo_writeconcern_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcern_t* intern = Z_OBJ_WRITECONCERN(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_writeconcern_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcern_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_writeconcern_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_writeconcern;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeconcern_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_writeconcern;

		return retval;
	}
#endif
} /* }}} */

static HashTable* php_phongo_writeconcern_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_write_concern_get_properties_hash(object, true TSRMLS_CC);
} /* }}} */

static HashTable* php_phongo_writeconcern_get_properties(zval* object TSRMLS_DC) /* {{{ */
{
	return php_phongo_write_concern_get_properties_hash(object, false TSRMLS_CC);
} /* }}} */

void php_phongo_writeconcern_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteConcern", php_phongo_writeconcern_me);
	php_phongo_writeconcern_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_writeconcern_ce->create_object = php_phongo_writeconcern_create_object;
	PHONGO_CE_FINAL(php_phongo_writeconcern_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_writeconcern_ce);

	zend_class_implements(php_phongo_writeconcern_ce TSRMLS_CC, 1, php_phongo_serializable_ce);

	memcpy(&php_phongo_handler_writeconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeconcern.get_debug_info = php_phongo_writeconcern_get_debug_info;
	php_phongo_handler_writeconcern.get_properties = php_phongo_writeconcern_get_properties;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_writeconcern.free_obj = php_phongo_writeconcern_free_object;
	php_phongo_handler_writeconcern.offset   = XtOffsetOf(php_phongo_writeconcern_t, std);
#endif

	zend_declare_class_constant_stringl(php_phongo_writeconcern_ce, ZEND_STRL("MAJORITY"), ZEND_STRL(PHONGO_WRITE_CONCERN_W_MAJORITY) TSRMLS_CC);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
