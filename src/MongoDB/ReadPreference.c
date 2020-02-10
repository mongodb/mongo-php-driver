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

#include "php_array_api.h"
#include "phongo_compat.h"
#include "php_phongo.h"
#include "php_bson.h"

zend_class_entry* php_phongo_readpreference_ce;

#define PHONGO_READ_PRIMARY "primary"
#define PHONGO_READ_PRIMARY_PREFERRED "primaryPreferred"
#define PHONGO_READ_SECONDARY "secondary"
#define PHONGO_READ_SECONDARY_PREFERRED "secondaryPreferred"
#define PHONGO_READ_NEAREST "nearest"

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_readpreference_init_from_hash(php_phongo_readpreference_t* intern, HashTable* props) /* {{{ */
{
	zval *mode, *tagSets, *maxStalenessSeconds;

	if ((mode = zend_hash_str_find(props, "mode", sizeof("mode") - 1)) && Z_TYPE_P(mode) == IS_STRING) {
		if (strcasecmp(Z_STRVAL_P(mode), PHONGO_READ_PRIMARY) == 0) {
			intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_PRIMARY);
		} else if (strcasecmp(Z_STRVAL_P(mode), PHONGO_READ_PRIMARY_PREFERRED) == 0) {
			intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_PRIMARY_PREFERRED);
		} else if (strcasecmp(Z_STRVAL_P(mode), PHONGO_READ_SECONDARY) == 0) {
			intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_SECONDARY);
		} else if (strcasecmp(Z_STRVAL_P(mode), PHONGO_READ_SECONDARY_PREFERRED) == 0) {
			intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_SECONDARY_PREFERRED);
		} else if (strcasecmp(Z_STRVAL_P(mode), PHONGO_READ_NEAREST) == 0) {
			intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_NEAREST);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires specific values for \"mode\" string field", ZSTR_VAL(php_phongo_readpreference_ce->name));
			return false;
		}
	} else {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"mode\" field to be string", ZSTR_VAL(php_phongo_readpreference_ce->name));
		return false;
	}

	if ((tagSets = zend_hash_str_find(props, "tags", sizeof("tags") - 1))) {
		if (Z_TYPE_P(tagSets) == IS_ARRAY) {
			bson_t* tags = bson_new();

			php_phongo_read_preference_prep_tagsets(tagSets);
			php_phongo_zval_to_bson(tagSets, PHONGO_BSON_NONE, (bson_t*) tags, NULL);

			if (!php_phongo_read_preference_tags_are_valid(tags)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"tags\" array field to have zero or more documents", ZSTR_VAL(php_phongo_readpreference_ce->name));
				bson_destroy(tags);
				goto failure;
			}

			if (!bson_empty(tags) && (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"tags\" array field to not be present with \"primary\" mode", ZSTR_VAL(php_phongo_readpreference_ce->name));
				bson_destroy(tags);
				goto failure;
			}

			mongoc_read_prefs_set_tags(intern->read_preference, tags);
			bson_destroy(tags);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"tags\" field to be array", ZSTR_VAL(php_phongo_readpreference_ce->name));
			goto failure;
		}
	}

	if ((maxStalenessSeconds = zend_hash_str_find(props, "maxStalenessSeconds", sizeof("maxStalenessSeconds") - 1))) {
		if (Z_TYPE_P(maxStalenessSeconds) == IS_LONG) {
			if (Z_LVAL_P(maxStalenessSeconds) < MONGOC_SMALLEST_MAX_STALENESS_SECONDS) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" integer field to be >= %d", ZSTR_VAL(php_phongo_readpreference_ce->name), MONGOC_SMALLEST_MAX_STALENESS_SECONDS);
				goto failure;
			}
			if (Z_LVAL_P(maxStalenessSeconds) > INT32_MAX) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" integer field to be <= %" PRId32, ZSTR_VAL(php_phongo_readpreference_ce->name), INT32_MAX);
				goto failure;
			}
			if (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" array field to not be present with \"primary\" mode", ZSTR_VAL(php_phongo_readpreference_ce->name));
				goto failure;
			}

			mongoc_read_prefs_set_max_staleness_seconds(intern->read_preference, Z_LVAL_P(maxStalenessSeconds));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" field to be integer", ZSTR_VAL(php_phongo_readpreference_ce->name));
			goto failure;
		}
	}

	return true;

failure:
	mongoc_read_prefs_destroy(intern->read_preference);
	intern->read_preference = NULL;
	return false;
} /* }}} */

static const char* php_phongo_readpreference_get_mode_string(mongoc_read_mode_t mode) /* {{{ */
{
	switch (mode) {
		case MONGOC_READ_PRIMARY:
			return PHONGO_READ_PRIMARY;
		case MONGOC_READ_PRIMARY_PREFERRED:
			return PHONGO_READ_PRIMARY_PREFERRED;
		case MONGOC_READ_SECONDARY:
			return PHONGO_READ_SECONDARY;
		case MONGOC_READ_SECONDARY_PREFERRED:
			return PHONGO_READ_SECONDARY_PREFERRED;
		case MONGOC_READ_NEAREST:
			return PHONGO_READ_NEAREST;
		default:
			/* Should never happen, but if it does: exception */
			phongo_throw_exception(PHONGO_ERROR_LOGIC, "Mode '%d' should never have been passed to php_phongo_readpreference_get_mode_string, please file a bug report", mode);
			break;
	}

	return NULL;
} /* }}} */

/* {{{ proto void MongoDB\Driver\ReadPreference::__construct(int|string $mode[, array $tagSets = array()[, array $options = array()]])
   Constructs a new ReadPreference */
static PHP_METHOD(ReadPreference, __construct)
{
	php_phongo_readpreference_t* intern;
	zend_error_handling          error_handling;
	zval*                        mode;
	zval*                        tagSets = NULL;
	zval*                        options = NULL;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	intern = Z_READPREFERENCE_OBJ_P(getThis());

	/* Separate the tagSets zval, since we may end up modifying it in
	 * php_phongo_read_preference_prep_tagsets() below. */
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z|a/!a!", &mode, &tagSets, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if (Z_TYPE_P(mode) == IS_LONG) {
		switch (Z_LVAL_P(mode)) {
			case MONGOC_READ_PRIMARY:
			case MONGOC_READ_SECONDARY:
			case MONGOC_READ_PRIMARY_PREFERRED:
			case MONGOC_READ_SECONDARY_PREFERRED:
			case MONGOC_READ_NEAREST:
				intern->read_preference = mongoc_read_prefs_new(Z_LVAL_P(mode));
				break;
			default:
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid mode: %" PHONGO_LONG_FORMAT, Z_LVAL_P(mode));
				return;
		}
	} else if (Z_TYPE_P(mode) == IS_STRING) {
		if (strcasecmp(Z_STRVAL_P(mode), PHONGO_READ_PRIMARY) == 0) {
			intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_PRIMARY);
		} else if (strcasecmp(Z_STRVAL_P(mode), PHONGO_READ_PRIMARY_PREFERRED) == 0) {
			intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_PRIMARY_PREFERRED);
		} else if (strcasecmp(Z_STRVAL_P(mode), PHONGO_READ_SECONDARY) == 0) {
			intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_SECONDARY);
		} else if (strcasecmp(Z_STRVAL_P(mode), PHONGO_READ_SECONDARY_PREFERRED) == 0) {
			intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_SECONDARY_PREFERRED);
		} else if (strcasecmp(Z_STRVAL_P(mode), PHONGO_READ_NEAREST) == 0) {
			intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_NEAREST);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid mode: '%s'", Z_STRVAL_P(mode));
			return;
		}
	} else {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected mode to be integer or string, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(mode));
		return;
	}

	if (tagSets) {
		bson_t* tags = bson_new();

		php_phongo_read_preference_prep_tagsets(tagSets);
		php_phongo_zval_to_bson(tagSets, PHONGO_BSON_NONE, (bson_t*) tags, NULL);

		if (!php_phongo_read_preference_tags_are_valid(tags)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "tagSets must be an array of zero or more documents");
			bson_destroy(tags);
			return;
		}

		if (!bson_empty(tags) && (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "tagSets may not be used with primary mode");
			bson_destroy(tags);
			return;
		}

		mongoc_read_prefs_set_tags(intern->read_preference, tags);
		bson_destroy(tags);
	}

	if (options && php_array_exists(options, "maxStalenessSeconds")) {
		zend_long maxStalenessSeconds = php_array_fetchc_long(options, "maxStalenessSeconds");

		if (maxStalenessSeconds != MONGOC_NO_MAX_STALENESS) {
			if (maxStalenessSeconds < MONGOC_SMALLEST_MAX_STALENESS_SECONDS) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected maxStalenessSeconds to be >= %d, %" PHONGO_LONG_FORMAT " given", MONGOC_SMALLEST_MAX_STALENESS_SECONDS, maxStalenessSeconds);
				return;
			}
			if (maxStalenessSeconds > INT32_MAX) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected maxStalenessSeconds to be <= %" PRId32 ", %" PHONGO_LONG_FORMAT " given", INT32_MAX, maxStalenessSeconds);
				return;
			}
			if (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "maxStalenessSeconds may not be used with primary mode");
				return;
			}
		}

		mongoc_read_prefs_set_max_staleness_seconds(intern->read_preference, maxStalenessSeconds);
	}

	if (!mongoc_read_prefs_is_valid(intern->read_preference)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Read preference is not valid");
		return;
	}
} /* }}} */

/* {{{ proto void MongoDB\BSON\ReadPreference::__set_state(array $properties)
*/
static PHP_METHOD(ReadPreference, __set_state)
{
	php_phongo_readpreference_t* intern;
	HashTable*                   props;
	zval*                        array;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_readpreference_ce);

	intern = Z_READPREFERENCE_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_readpreference_init_from_hash(intern, props);
} /* }}} */

/* {{{ proto integer MongoDB\Driver\ReadPreference::getMaxStalenessSeconds()
   Returns the ReadPreference maxStalenessSeconds value */
static PHP_METHOD(ReadPreference, getMaxStalenessSeconds)
{
	php_phongo_readpreference_t* intern;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference));
} /* }}} */

/* {{{ proto integer MongoDB\Driver\ReadPreference::getMode()
   Returns the ReadPreference mode */
static PHP_METHOD(ReadPreference, getMode)
{
	php_phongo_readpreference_t* intern;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(mongoc_read_prefs_get_mode(intern->read_preference));
} /* }}} */

/* {{{ proto string MongoDB\Driver\ReadPreference::getModeString()
   Returns the ReadPreference mode as string */
static PHP_METHOD(ReadPreference, getModeString)
{
	php_phongo_readpreference_t* intern;
	const char*                  mode_string;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	mode_string = php_phongo_readpreference_get_mode_string(mongoc_read_prefs_get_mode(intern->read_preference));
	if (!mode_string) {
		/* Exception already thrown */
		return;
	}

	RETURN_STRING(mode_string);
} /* }}} */

/* {{{ proto array MongoDB\Driver\ReadPreference::getTagSets()
   Returns the ReadPreference tag sets */
static PHP_METHOD(ReadPreference, getTagSets)
{
	php_phongo_readpreference_t* intern;
	const bson_t*                tags;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	tags = mongoc_read_prefs_get_tags(intern->read_preference);

	if (tags->len) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!php_phongo_bson_to_zval_ex(bson_get_data(tags), tags->len, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		RETURN_ZVAL(&state.zchild, 0, 1);
	} else {
		RETURN_NULL();
	}
} /* }}} */

static HashTable* php_phongo_readpreference_get_properties_hash(zval* object, bool is_debug) /* {{{ */
{
	php_phongo_readpreference_t* intern;
	HashTable*                   props;
	const char*                  modeString = NULL;
	const bson_t*                tags;
	mongoc_read_mode_t           mode;

	intern = Z_READPREFERENCE_OBJ_P(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, 3);

	if (!intern->read_preference) {
		return props;
	}

	tags       = mongoc_read_prefs_get_tags(intern->read_preference);
	mode       = mongoc_read_prefs_get_mode(intern->read_preference);
	modeString = php_phongo_readpreference_get_mode_string(mode);

	if (modeString) {
		zval z_mode;

		ZVAL_STRING(&z_mode, modeString);
		zend_hash_str_update(props, "mode", sizeof("mode") - 1, &z_mode);
	}

	if (!bson_empty0(tags)) {
		php_phongo_bson_state state;

		/* Use PHONGO_TYPEMAP_NATIVE_ARRAY for the root type since tags is an
		 * array; however, inner documents and arrays can use the default. */
		PHONGO_BSON_INIT_STATE(state);
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		if (!php_phongo_bson_to_zval_ex(bson_get_data(tags), tags->len, &state)) {
			zval_ptr_dtor(&state.zchild);
			goto done;
		}

		zend_hash_str_update(props, "tags", sizeof("tags") - 1, &state.zchild);
	}

	if (mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference) != MONGOC_NO_MAX_STALENESS) {
		long maxStalenessSeconds = mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference);
		zval z_max_ss;

		ZVAL_LONG(&z_max_ss, maxStalenessSeconds);
		zend_hash_str_update(props, "maxStalenessSeconds", sizeof("maxStalenessSeconds") - 1, &z_max_ss);
	}

done:
	return props;
} /* }}} */

/* {{{ proto array MongoDB\Driver\ReadPreference::bsonSerialize()
*/
static PHP_METHOD(ReadPreference, bsonSerialize)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ZVAL_ARR(return_value, php_phongo_readpreference_get_properties_hash(getThis(), true));
	convert_to_object(return_value);
} /* }}} */

/* {{{ proto string MongoDB\Driver\ReadPreference::serialize()
*/
static PHP_METHOD(ReadPreference, serialize)
{
	php_phongo_readpreference_t* intern;
	zval                         retval;
	php_serialize_data_t         var_hash;
	smart_str                    buf        = { 0 };
	const char*                  modeString = NULL;
	const bson_t*                tags;
	int64_t                      maxStalenessSeconds;
	mongoc_read_mode_t           mode;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!intern->read_preference) {
		return;
	}

	tags                = mongoc_read_prefs_get_tags(intern->read_preference);
	mode                = mongoc_read_prefs_get_mode(intern->read_preference);
	modeString          = php_phongo_readpreference_get_mode_string(mode);
	maxStalenessSeconds = mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference);

	array_init_size(&retval, 3);

	if (modeString) {
		ADD_ASSOC_STRING(&retval, "mode", modeString);
	}

	if (!bson_empty0(tags)) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!php_phongo_bson_to_zval_ex(bson_get_data(tags), tags->len, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "tags", &state.zchild);
	}

	if (maxStalenessSeconds != MONGOC_NO_MAX_STALENESS) {
		ADD_ASSOC_LONG_EX(&retval, "maxStalenessSeconds", maxStalenessSeconds);
	}

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\Driver\ReadPreference::unserialize(string $serialized)
*/
static PHP_METHOD(ReadPreference, unserialize)
{
	php_phongo_readpreference_t* intern;
	zend_error_handling          error_handling;
	char*                        serialized;
	size_t                       serialized_len;
	zval                         props;
	php_unserialize_data_t       var_hash;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

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
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_readpreference_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_readpreference_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ MongoDB\Driver\ReadPreference function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_ReadPreference___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, mode)
	ZEND_ARG_ARRAY_INFO(0, tagSets, 1)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ReadPreference___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ReadPreference_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ReadPreference_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_readpreference_me[] = {
	/* clang-format off */
	PHP_ME(ReadPreference, __construct, ai_ReadPreference___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, __set_state, ai_ReadPreference___set_state, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(ReadPreference, getMaxStalenessSeconds, ai_ReadPreference_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, getMode, ai_ReadPreference_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, getModeString, ai_ReadPreference_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, getTagSets, ai_ReadPreference_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, bsonSerialize, ai_ReadPreference_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, serialize, ai_ReadPreference_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ReadPreference, unserialize, ai_ReadPreference_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\ReadPreference object handlers */
static zend_object_handlers php_phongo_handler_readpreference;

static void php_phongo_readpreference_free_object(zend_object* object) /* {{{ */
{
	php_phongo_readpreference_t* intern = Z_OBJ_READPREFERENCE(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->read_preference) {
		mongoc_read_prefs_destroy(intern->read_preference);
	}
} /* }}} */

static zend_object* php_phongo_readpreference_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_readpreference_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_readpreference_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_readpreference;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_readpreference_get_debug_info(zval* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	return php_phongo_readpreference_get_properties_hash(object, true);
} /* }}} */

static HashTable* php_phongo_readpreference_get_properties(zval* object) /* {{{ */
{
	return php_phongo_readpreference_get_properties_hash(object, false);
} /* }}} */
/* }}} */

void php_phongo_readpreference_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ReadPreference", php_phongo_readpreference_me);
	php_phongo_readpreference_ce                = zend_register_internal_class(&ce);
	php_phongo_readpreference_ce->create_object = php_phongo_readpreference_create_object;
	PHONGO_CE_FINAL(php_phongo_readpreference_ce);

	zend_class_implements(php_phongo_readpreference_ce, 1, php_phongo_serializable_ce);
	zend_class_implements(php_phongo_readpreference_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_readpreference, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_readpreference.get_debug_info = php_phongo_readpreference_get_debug_info;
	php_phongo_handler_readpreference.get_properties = php_phongo_readpreference_get_properties;
	php_phongo_handler_readpreference.free_obj       = php_phongo_readpreference_free_object;
	php_phongo_handler_readpreference.offset         = XtOffsetOf(php_phongo_readpreference_t, std);

	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_PRIMARY"), MONGOC_READ_PRIMARY);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_PRIMARY_PREFERRED"), MONGOC_READ_PRIMARY_PREFERRED);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_SECONDARY"), MONGOC_READ_SECONDARY);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_SECONDARY_PREFERRED"), MONGOC_READ_SECONDARY_PREFERRED);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("RP_NEAREST"), MONGOC_READ_NEAREST);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("NO_MAX_STALENESS"), MONGOC_NO_MAX_STALENESS);
	zend_declare_class_constant_long(php_phongo_readpreference_ce, ZEND_STRL("SMALLEST_MAX_STALENESS_SECONDS"), MONGOC_SMALLEST_MAX_STALENESS_SECONDS);

	zend_declare_class_constant_string(php_phongo_readpreference_ce, ZEND_STRL("PRIMARY"), PHONGO_READ_PRIMARY);
	zend_declare_class_constant_string(php_phongo_readpreference_ce, ZEND_STRL("PRIMARY_PREFERRED"), PHONGO_READ_PRIMARY_PREFERRED);
	zend_declare_class_constant_string(php_phongo_readpreference_ce, ZEND_STRL("SECONDARY"), PHONGO_READ_SECONDARY);
	zend_declare_class_constant_string(php_phongo_readpreference_ce, ZEND_STRL("SECONDARY_PREFERRED"), PHONGO_READ_SECONDARY_PREFERRED);
	zend_declare_class_constant_string(php_phongo_readpreference_ce, ZEND_STRL("NEAREST"), PHONGO_READ_NEAREST);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
