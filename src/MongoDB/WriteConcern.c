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

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_writeconcern_ce;

zend_object_handlers php_phongo_handler_writeconcern;

/* {{{ proto void WriteConcern::__construct(integer|string $w[, integer $wtimeout[, boolean $journal]])
   Constructs a new WriteConcern */
PHP_METHOD(WriteConcern, __construct)
{
	php_phongo_writeconcern_t *intern;
	zend_error_handling       error_handling;
	zval                     *w, *journal;
	phongo_long               wtimeout = 0;
	SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


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
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected w to be integer or string, %s given", zend_get_type_by_const(Z_TYPE_P(w)));
		return;
	}

	switch(ZEND_NUM_ARGS()) {
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

			mongoc_write_concern_set_wtimeout(intern->write_concern, wtimeout);
	}
}
/* }}} */

/* {{{ proto string|integer|null WriteConcern::getW()
   Returns the WriteConcern "w" option */
PHP_METHOD(WriteConcern, getW)
{
	php_phongo_writeconcern_t *intern;
	const char *wtag;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

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
}
/* }}} */

/* {{{ proto integer WriteConcern::getWtimeout()
   Returns the WriteConcern "wtimeout" option */
PHP_METHOD(WriteConcern, getWtimeout)
{
	php_phongo_writeconcern_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(mongoc_write_concern_get_wtimeout(intern->write_concern));
}
/* }}} */

/* {{{ proto null|boolean WriteConcern::getJournal()
   Returns the WriteConcern "journal" option */
PHP_METHOD(WriteConcern, getJournal)
{
	php_phongo_writeconcern_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (mongoc_write_concern_journal_is_set(intern->write_concern)) {
		RETURN_BOOL(mongoc_write_concern_get_journal(intern->write_concern));
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ proto array WriteConcern::bsonSerialize()
   */
PHP_METHOD(WriteConcern, bsonSerialize)
{
	const mongoc_write_concern_t *write_concern = phongo_write_concern_from_zval(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_write_concern_to_zval(return_value, write_concern);
	convert_to_object(return_value);
}
/* }}} */

/**
 * Value object for write concern used in issuing write operations.
 */
/* {{{ MongoDB\Driver\WriteConcern */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcern___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, w)
	ZEND_ARG_INFO(0, wtimeout)
	ZEND_ARG_INFO(0, journal)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcern_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_writeconcern_me[] = {
	PHP_ME(WriteConcern, __construct, ai_WriteConcern___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, getW, ai_WriteConcern_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, getWtimeout, ai_WriteConcern_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, getJournal, ai_WriteConcern_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteConcern, bsonSerialize, ai_WriteConcern_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_writeconcern_t object handlers */
static void php_phongo_writeconcern_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcern_t *intern = Z_OBJ_WRITECONCERN(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_writeconcern_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcern_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_writeconcern_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_writeconcern;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeconcern_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_writeconcern;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_writeconcern_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval                          retval;
#else
	zval                          retval = zval_used_for_init;
#endif
	const mongoc_write_concern_t *write_concern = phongo_write_concern_from_zval(object TSRMLS_CC);


	*is_temp = 1;
	php_phongo_write_concern_to_zval(&retval, write_concern);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteConcern)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteConcern", php_phongo_writeconcern_me);
	php_phongo_writeconcern_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_writeconcern_ce->create_object = php_phongo_writeconcern_create_object;
	PHONGO_CE_FINAL(php_phongo_writeconcern_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_writeconcern_ce);

	zend_class_implements(php_phongo_writeconcern_ce TSRMLS_CC, 1, php_phongo_serializable_ce);

	memcpy(&php_phongo_handler_writeconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeconcern.get_debug_info = php_phongo_writeconcern_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_writeconcern.free_obj = php_phongo_writeconcern_free_object;
	php_phongo_handler_writeconcern.offset = XtOffsetOf(php_phongo_writeconcern_t, std);
#endif

	zend_declare_class_constant_stringl(php_phongo_writeconcern_ce, ZEND_STRL("MAJORITY"), ZEND_STRL(PHONGO_WRITE_CONCERN_W_MAJORITY) TSRMLS_CC);

	return SUCCESS;
}
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
