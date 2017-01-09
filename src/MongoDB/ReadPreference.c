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
/* PHP array helpers */
#include "php_array_api.h"
/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_readpreference_ce;

zend_object_handlers php_phongo_handler_readpreference;

/* {{{ proto void ReadPreference::__construct(integer $mode[, array $tagSets = array()[, array $options = array()]])
   Constructs a new ReadPreference */
PHP_METHOD(ReadPreference, __construct)
{
	php_phongo_readpreference_t *intern;
	zend_error_handling       error_handling;
	phongo_long               mode;
	zval                     *tagSets = NULL;
	zval                     *options = NULL;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_used)


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_READPREFERENCE_OBJ_P(getThis());

	/* Separate the tagSets zval, since we may end up modifying it in
	 * php_phongo_read_preference_prep_tagsets() below. */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|a/!a!", &mode, &tagSets, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	switch(mode) {
		case MONGOC_READ_PRIMARY:
		case MONGOC_READ_SECONDARY:
		case MONGOC_READ_PRIMARY_PREFERRED:
		case MONGOC_READ_SECONDARY_PREFERRED:
		case MONGOC_READ_NEAREST:
			intern->read_preference = mongoc_read_prefs_new(mode);
			break;
		default:
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Invalid mode: %" PHONGO_LONG_FORMAT, mode);
			return;
	}

	if (tagSets) {
		bson_t *tags = bson_new();

		php_phongo_read_preference_prep_tagsets(tagSets TSRMLS_CC);
		phongo_zval_to_bson(tagSets, PHONGO_BSON_NONE, (bson_t *)tags, NULL TSRMLS_CC);

		if (!php_phongo_read_preference_tags_are_valid(tags)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "tagSets must be an array of zero or more documents");
			bson_destroy(tags);
			return;
		}

		if (!bson_empty(tags) && mode == MONGOC_READ_PRIMARY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "tagSets may not be used with primary mode");
			bson_destroy(tags);
			return;
		}

		mongoc_read_prefs_set_tags(intern->read_preference, tags);
		bson_destroy(tags);
	}

	if (options && php_array_exists(options, "maxStalenessSeconds")) {
		phongo_long maxStalenessSeconds = php_array_fetchc_long(options, "maxStalenessSeconds");

		if (maxStalenessSeconds != MONGOC_NO_MAX_STALENESS) {
			if (maxStalenessSeconds < MONGOC_SMALLEST_MAX_STALENESS_SECONDS) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected maxStalenessSeconds to be >= %d, %" PHONGO_LONG_FORMAT " given", MONGOC_SMALLEST_MAX_STALENESS_SECONDS, maxStalenessSeconds);
				return;
			}
			if (maxStalenessSeconds > INT32_MAX) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected maxStalenessSeconds to be <= %" PRId32 ", %" PHONGO_LONG_FORMAT " given", INT32_MAX, maxStalenessSeconds);
				return;
			}
			if (mode == MONGOC_READ_PRIMARY) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "maxStalenessSeconds may not be used with primary mode");
				return;
			}
		}

		mongoc_read_prefs_set_max_staleness_seconds(intern->read_preference, maxStalenessSeconds);
	}

	if (!mongoc_read_prefs_is_valid(intern->read_preference)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Read preference is not valid");
		return;
	}
}
/* }}} */

/* {{{ proto integer ReadPreference::getMaxStalenessSeconds()
   Returns the ReadPreference maxStalenessSeconds value */
PHP_METHOD(ReadPreference, getMaxStalenessSeconds)
{
	php_phongo_readpreference_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference));
}
/* }}} */

/* {{{ proto integer ReadPreference::getMode()
   Returns the ReadPreference mode */
PHP_METHOD(ReadPreference, getMode)
{
	php_phongo_readpreference_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(mongoc_read_prefs_get_mode(intern->read_preference));
}
/* }}} */

/* {{{ proto array ReadPreference::getTagSets()
   Returns the ReadPreference tag sets */
PHP_METHOD(ReadPreference, getTagSets)
{
	php_phongo_readpreference_t *intern;
	const bson_t                *tags;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	tags = mongoc_read_prefs_get_tags(intern->read_preference);

	if (tags->len) {
		php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;
		/* Use native arrays for debugging output */
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		phongo_bson_to_zval_ex(bson_get_data(tags), tags->len, &state);
#if PHP_VERSION_ID >= 70000
		RETURN_ZVAL(&state.zchild, 0, 1);
#else
		RETURN_ZVAL(state.zchild, 0, 1);
#endif
	} else {
		RETURN_NULL();
	}
}
/* }}} */

/* {{{ proto array ReadPreference::bsonSerialize()
   */
PHP_METHOD(ReadPreference, bsonSerialize)
{
	const mongoc_read_prefs_t *read_preference = phongo_read_preference_from_zval(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_read_preference_to_zval(return_value, read_preference);
	convert_to_object(return_value);
}
/* }}} */

/**
 * Value object for read preferences used in issuing commands and queries.
 */
/* {{{ MongoDB\Driver\ReadPreference */

ZEND_BEGIN_ARG_INFO_EX(ai_ReadPreference___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, mode)
	ZEND_ARG_ARRAY_INFO(0, tagSets, 1)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ReadPreference_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_readpreference_me[] = {
	PHP_ME(ReadPreference, __construct, ai_ReadPreference___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, getMaxStalenessSeconds, ai_ReadPreference_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, getMode, ai_ReadPreference_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, getTagSets, ai_ReadPreference_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, bsonSerialize, ai_ReadPreference_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_readpreference_t object handlers */
static void php_phongo_readpreference_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_readpreference_t *intern = Z_OBJ_READPREFERENCE(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->read_preference) {
		mongoc_read_prefs_destroy(intern->read_preference);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_readpreference_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_readpreference_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_readpreference_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_readpreference;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_readpreference_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_readpreference;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_readpreference_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval                       retval;
#else
	zval                       retval = zval_used_for_init;
#endif
	const mongoc_read_prefs_t *read_prefs = phongo_read_preference_from_zval(object TSRMLS_CC);


	*is_temp = 1;

	php_phongo_read_preference_to_zval(&retval, read_prefs);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */



/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(ReadPreference)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ReadPreference", php_phongo_readpreference_me);
	php_phongo_readpreference_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_readpreference_ce->create_object = php_phongo_readpreference_create_object;
	PHONGO_CE_FINAL(php_phongo_readpreference_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_readpreference_ce);

	zend_class_implements(php_phongo_readpreference_ce TSRMLS_CC, 1, php_phongo_serializable_ce);

	memcpy(&php_phongo_handler_readpreference, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_readpreference.get_debug_info = php_phongo_readpreference_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_readpreference.free_obj = php_phongo_readpreference_free_object;
	php_phongo_handler_readpreference.offset = XtOffsetOf(php_phongo_readpreference_t, std);
#endif

	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_PRIMARY"), MONGOC_READ_PRIMARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_PRIMARY_PREFERRED"), MONGOC_READ_PRIMARY_PREFERRED TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_SECONDARY"), MONGOC_READ_SECONDARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_SECONDARY_PREFERRED"), MONGOC_READ_SECONDARY_PREFERRED TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_NEAREST"), MONGOC_READ_NEAREST TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("NO_MAX_STALENESS"), MONGOC_NO_MAX_STALENESS TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("SMALLEST_MAX_STALENESS_SECONDS"), MONGOC_SMALLEST_MAX_STALENESS_SECONDS TSRMLS_CC);

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
