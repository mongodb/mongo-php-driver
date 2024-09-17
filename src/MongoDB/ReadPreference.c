/*
 * Copyright 2014-present MongoDB, Inc.
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

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <php.h>
#include <zend_smart_str.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"

#include "MongoDB/ReadPreference.h"
#include "ReadPreference_arginfo.h"

zend_class_entry* php_phongo_readpreference_ce;

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_readpreference_init_from_hash(php_phongo_readpreference_t* intern, HashTable* props)
{
	zval *mode, *tagSets, *maxStalenessSeconds, *hedge;

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
		ZVAL_DEREF(tagSets);
		if (Z_TYPE_P(tagSets) == IS_ARRAY) {
			bson_t* tags = bson_new();

			/* Separate tagSets as php_phongo_read_preference_prep_tagsets may
			 * modify these tags. */
			SEPARATE_ZVAL_NOREF(tagSets);

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
			if (Z_LVAL_P(maxStalenessSeconds) != MONGOC_NO_MAX_STALENESS) {
				if (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" field to not be present with \"primary\" mode", ZSTR_VAL(php_phongo_readpreference_ce->name));
					goto failure;
				}
				if (Z_LVAL_P(maxStalenessSeconds) < MONGOC_SMALLEST_MAX_STALENESS_SECONDS) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" integer field to be >= %d", ZSTR_VAL(php_phongo_readpreference_ce->name), MONGOC_SMALLEST_MAX_STALENESS_SECONDS);
					goto failure;
				}
				if (Z_LVAL_P(maxStalenessSeconds) > INT32_MAX) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" integer field to be <= %" PRId32, ZSTR_VAL(php_phongo_readpreference_ce->name), INT32_MAX);
					goto failure;
				}
			}

			mongoc_read_prefs_set_max_staleness_seconds(intern->read_preference, Z_LVAL_P(maxStalenessSeconds));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" field to be integer", ZSTR_VAL(php_phongo_readpreference_ce->name));
			goto failure;
		}
	}

	if ((hedge = zend_hash_str_find(props, "hedge", sizeof("hedge") - 1))) {
		if (Z_TYPE_P(hedge) == IS_ARRAY || Z_TYPE_P(hedge) == IS_OBJECT) {
			bson_t* hedge_doc = bson_new();

			if (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"hedge\" field to not be present with \"primary\" mode", ZSTR_VAL(php_phongo_readpreference_ce->name));
				bson_destroy(hedge_doc);
				goto failure;
			}

			php_phongo_zval_to_bson(hedge, PHONGO_BSON_NONE, hedge_doc, NULL);

			if (EG(exception)) {
				bson_destroy(hedge_doc);
				goto failure;
			}

			mongoc_read_prefs_set_hedge(intern->read_preference, hedge_doc);
			bson_destroy(hedge_doc);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"hedge\" field to be an array or object", ZSTR_VAL(php_phongo_readpreference_ce->name));
			goto failure;
		}
	}

	if (!mongoc_read_prefs_is_valid(intern->read_preference)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Read preference is not valid");
		goto failure;
	}

	return true;

failure:
	mongoc_read_prefs_destroy(intern->read_preference);
	intern->read_preference = NULL;
	return false;
}

static const char* php_phongo_readpreference_get_mode_string(mongoc_read_mode_t mode)
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
}

/* Constructs a new ReadPreference */
static PHP_METHOD(MongoDB_Driver_ReadPreference, __construct)
{
	php_phongo_readpreference_t* intern;
	zval*                        mode;
	zval*                        tagSets = NULL;
	zval*                        options = NULL;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	/* Separate the tagSets zval, since we may end up modifying it in
	 * php_phongo_read_preference_prep_tagsets() below. */
	PHONGO_PARSE_PARAMETERS_START(1, 3)
	Z_PARAM_ZVAL(mode)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_EX(tagSets, 1, 1)
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(mode) == IS_LONG) {
		php_error_docref(NULL, E_DEPRECATED, "Passing an integer mode to \"MongoDB\\Driver\\ReadPreference::__construct\" is deprecated and will be removed in a future release.");

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

	if (options && php_array_exists(options, "hedge")) {
		zval* hedge = php_array_fetchc(options, "hedge");

		if (Z_TYPE_P(hedge) == IS_ARRAY || Z_TYPE_P(hedge) == IS_OBJECT) {
			bson_t* hedge_doc = bson_new();

			if (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "hedge may not be used with primary mode");
				bson_destroy(hedge_doc);
				return;
			}

			php_phongo_zval_to_bson(hedge, PHONGO_BSON_NONE, hedge_doc, NULL);

			if (EG(exception)) {
				bson_destroy(hedge_doc);
				return;
			}

			mongoc_read_prefs_set_hedge(intern->read_preference, hedge_doc);
			bson_destroy(hedge_doc);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"hedge\" field to be an array or object", ZSTR_VAL(php_phongo_readpreference_ce->name));
			return;
		}
	}

	if (!mongoc_read_prefs_is_valid(intern->read_preference)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Read preference is not valid");
		return;
	}
}

static PHP_METHOD(MongoDB_Driver_ReadPreference, __set_state)
{
	php_phongo_readpreference_t* intern;
	HashTable*                   props;
	zval*                        array;

	/* Separate the zval, since we may end up modifying the "tags" element in
	 * php_phongo_read_preference_prep_tagsets(), which is called from
	 * php_phongo_readpreference_init_from_hash. */
	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY_EX(array, 0, 1)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_readpreference_ce);

	intern = Z_READPREFERENCE_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_readpreference_init_from_hash(intern, props);
}

/* Returns the ReadPreference hedge document */
static PHP_METHOD(MongoDB_Driver_ReadPreference, getHedge)
{
	php_phongo_readpreference_t* intern;
	const bson_t*                hedge;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	hedge = mongoc_read_prefs_get_hedge(intern->read_preference);

	if (!bson_empty0(hedge)) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_STATE(state);

		if (!php_phongo_bson_to_zval_ex(hedge, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		RETURN_ZVAL(&state.zchild, 0, 1);
	} else {
		RETURN_NULL();
	}
}

/* Returns the ReadPreference maxStalenessSeconds value */
static PHP_METHOD(MongoDB_Driver_ReadPreference, getMaxStalenessSeconds)
{
	php_phongo_readpreference_t* intern;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference));
}

/* Returns the ReadPreference mode */
static PHP_METHOD(MongoDB_Driver_ReadPreference, getMode)
{
	php_phongo_readpreference_t* intern;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(mongoc_read_prefs_get_mode(intern->read_preference));
}

/* Returns the ReadPreference mode as string */
static PHP_METHOD(MongoDB_Driver_ReadPreference, getModeString)
{
	php_phongo_readpreference_t* intern;
	const char*                  mode_string;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	mode_string = php_phongo_readpreference_get_mode_string(mongoc_read_prefs_get_mode(intern->read_preference));
	if (!mode_string) {
		/* Exception already thrown */
		return;
	}

	RETURN_STRING(mode_string);
}

/* Returns the ReadPreference tag sets */
static PHP_METHOD(MongoDB_Driver_ReadPreference, getTagSets)
{
	php_phongo_readpreference_t* intern;
	const bson_t*                tags;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	tags = mongoc_read_prefs_get_tags(intern->read_preference);

	if (tags->len) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!php_phongo_bson_to_zval_ex(tags, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		RETURN_ZVAL(&state.zchild, 0, 1);
	} else {
		RETURN_NULL();
	}
}

static HashTable* php_phongo_readpreference_get_properties_hash(zend_object* object, bool is_temp)
{
	php_phongo_readpreference_t* intern;
	HashTable*                   props;
	const char*                  modeString = NULL;
	const bson_t*                tags;
	const bson_t*                hedge;
	mongoc_read_mode_t           mode;

	intern = Z_OBJ_READPREFERENCE(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 4);

	if (!intern->read_preference) {
		return props;
	}

	tags       = mongoc_read_prefs_get_tags(intern->read_preference);
	mode       = mongoc_read_prefs_get_mode(intern->read_preference);
	modeString = php_phongo_readpreference_get_mode_string(mode);
	hedge      = mongoc_read_prefs_get_hedge(intern->read_preference);

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
		state.map.root.type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		if (!php_phongo_bson_to_zval_ex(tags, &state)) {
			zval_ptr_dtor(&state.zchild);
			goto done;
		}

		zend_hash_str_update(props, "tags", sizeof("tags") - 1, &state.zchild);
	}

	if (mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference) != MONGOC_NO_MAX_STALENESS) {
		/* Note: valid values for maxStalesnessSeconds will not exceed the range
		 * of 32-bit signed integers, so conditional encoding is not necessary. */
		long maxStalenessSeconds = mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference);
		zval z_max_ss;

		ZVAL_LONG(&z_max_ss, maxStalenessSeconds);
		zend_hash_str_update(props, "maxStalenessSeconds", sizeof("maxStalenessSeconds") - 1, &z_max_ss);
	}

	if (!bson_empty0(hedge)) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_STATE(state);

		if (!php_phongo_bson_to_zval_ex(hedge, &state)) {
			zval_ptr_dtor(&state.zchild);
			goto done;
		}

		zend_hash_str_update(props, "hedge", sizeof("hedge") - 1, &state.zchild);
	}

done:
	return props;
}

static PHP_METHOD(MongoDB_Driver_ReadPreference, bsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	ZVAL_ARR(return_value, php_phongo_readpreference_get_properties_hash(Z_OBJ_P(getThis()), true));
	convert_to_object(return_value);
}

static PHP_METHOD(MongoDB_Driver_ReadPreference, serialize)
{
	php_phongo_readpreference_t* intern;
	zval                         retval;
	php_serialize_data_t         var_hash;
	smart_str                    buf        = { 0 };
	const char*                  modeString = NULL;
	const bson_t*                tags;
	const bson_t*                hedge;
	int64_t                      maxStalenessSeconds;
	mongoc_read_mode_t           mode;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->read_preference) {
		return;
	}

	tags                = mongoc_read_prefs_get_tags(intern->read_preference);
	mode                = mongoc_read_prefs_get_mode(intern->read_preference);
	modeString          = php_phongo_readpreference_get_mode_string(mode);
	maxStalenessSeconds = mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference);
	hedge               = mongoc_read_prefs_get_hedge(intern->read_preference);

	array_init_size(&retval, 4);

	if (modeString) {
		ADD_ASSOC_STRING(&retval, "mode", modeString);
	}

	if (!bson_empty0(tags)) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!php_phongo_bson_to_zval_ex(tags, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "tags", &state.zchild);
	}

	if (maxStalenessSeconds != MONGOC_NO_MAX_STALENESS) {
		ADD_ASSOC_LONG_EX(&retval, "maxStalenessSeconds", maxStalenessSeconds);
	}

	if (!bson_empty0(hedge)) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_STATE(state);

		if (!php_phongo_bson_to_zval_ex(hedge, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "hedge", &state.zchild);
	}

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_Driver_ReadPreference, unserialize)
{
	php_phongo_readpreference_t* intern;
	char*                        serialized;
	size_t                       serialized_len;
	zval                         props;
	php_unserialize_data_t       var_hash;

	intern = Z_READPREFERENCE_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

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
}

static PHP_METHOD(MongoDB_Driver_ReadPreference, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_readpreference_get_properties_hash(Z_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_Driver_ReadPreference, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_readpreference_init_from_hash(Z_READPREFERENCE_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\Driver\ReadPreference object handlers */
static zend_object_handlers php_phongo_handler_readpreference;

static void php_phongo_readpreference_free_object(zend_object* object)
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
}

static zend_object* php_phongo_readpreference_create_object(zend_class_entry* class_type)
{
	php_phongo_readpreference_t* intern = zend_object_alloc(sizeof(php_phongo_readpreference_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_readpreference;

	return &intern->std;
}

static HashTable* php_phongo_readpreference_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_readpreference_get_properties_hash(object, true);
}

static HashTable* php_phongo_readpreference_get_properties(zend_object* object)
{
	return php_phongo_readpreference_get_properties_hash(object, false);
}

void php_phongo_readpreference_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_readpreference_ce                = register_class_MongoDB_Driver_ReadPreference(php_phongo_serializable_ce, zend_ce_serializable);
	php_phongo_readpreference_ce->create_object = php_phongo_readpreference_create_object;

	memcpy(&php_phongo_handler_readpreference, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_readpreference.get_debug_info = php_phongo_readpreference_get_debug_info;
	php_phongo_handler_readpreference.get_properties = php_phongo_readpreference_get_properties;
	php_phongo_handler_readpreference.free_obj       = php_phongo_readpreference_free_object;
	php_phongo_handler_readpreference.offset         = XtOffsetOf(php_phongo_readpreference_t, std);
}

void phongo_readpreference_init(zval* return_value, const mongoc_read_prefs_t* read_prefs)
{
	php_phongo_readpreference_t* intern;

	object_init_ex(return_value, php_phongo_readpreference_ce);

	intern                  = Z_READPREFERENCE_OBJ_P(return_value);
	intern->read_preference = mongoc_read_prefs_copy(read_prefs);
}

const mongoc_read_prefs_t* phongo_read_preference_from_zval(zval* zread_preference)
{
	if (zread_preference) {
		php_phongo_readpreference_t* intern = Z_READPREFERENCE_OBJ_P(zread_preference);

		if (intern) {
			return intern->read_preference;
		}
	}

	return NULL;
}

/* Prepare tagSets for BSON encoding by converting each array in the set to an
 * object. This ensures that empty arrays will serialize as empty documents.
 *
 * php_phongo_read_preference_tags_are_valid() handles actual validation of the
 * tag set structure. */
void php_phongo_read_preference_prep_tagsets(zval* tagSets)
{
	HashTable* ht_data;
	zval*      tagSet;

	if (Z_TYPE_P(tagSets) != IS_ARRAY) {
		return;
	}

	ht_data = HASH_OF(tagSets);

	ZEND_HASH_FOREACH_VAL_IND(ht_data, tagSet)
	{
		ZVAL_DEREF(tagSet);
		if (Z_TYPE_P(tagSet) == IS_ARRAY) {
			SEPARATE_ZVAL_NOREF(tagSet);
			convert_to_object(tagSet);
		}
	}
	ZEND_HASH_FOREACH_END();
}

/* Checks if tags is valid to set on a mongoc_read_prefs_t. It may be null or an
 * array of one or more documents. */
bool php_phongo_read_preference_tags_are_valid(const bson_t* tags)
{
	bson_iter_t iter;

	if (bson_empty0(tags)) {
		return true;
	}

	if (!bson_iter_init(&iter, tags)) {
		return false;
	}

	while (bson_iter_next(&iter)) {
		if (!BSON_ITER_HOLDS_DOCUMENT(&iter)) {
			return false;
		}
	}

	return true;
}
