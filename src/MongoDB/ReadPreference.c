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
#include <Zend/zend_interfaces.h>

#include "php_array_api.h"

#include "phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"

#include "MongoDB/ReadPreference.h"
#include "ReadPreference_arginfo.h"

zend_class_entry* phongo_readpreference_ce;

static const char* phongo_readpreference_get_mode_string(const mongoc_read_prefs_t* read_prefs)
{
	switch (mongoc_read_prefs_get_mode(read_prefs)) {
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
			return "";
	}
}

static void phongo_readpreference_update_properties(phongo_readpreference_t* intern)
{
	const bson_t* bson;

	zend_update_property_string(phongo_readpreference_ce, &intern->std, ZEND_STRL("mode"), phongo_readpreference_get_mode_string(intern->read_preference));
	zend_update_property_long(phongo_readpreference_ce, &intern->std, ZEND_STRL("maxStalenessSeconds"), mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference));

	bson = mongoc_read_prefs_get_tags(intern->read_preference);

	if (!bson_empty0(bson)) {
		phongo_bson_state state;

		PHONGO_BSON_INIT_STATE(state);
		state.map.root.type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		if (!phongo_bson_to_zval_ex(bson, &state)) {
			// Exception already thrown
			zval_ptr_dtor(&state.zchild);
			return;
		}

		zend_update_property(phongo_readpreference_ce, &intern->std, ZEND_STRL("tags"), &state.zchild);
		zval_ptr_dtor(&state.zchild);
	} else {
		zend_update_property_null(phongo_readpreference_ce, &intern->std, ZEND_STRL("tags"));
	}

	bson = mongoc_read_prefs_get_hedge(intern->read_preference);

	if (!bson_empty0(bson)) {
		phongo_bson_state state;

		PHONGO_BSON_INIT_STATE(state);

		if (!phongo_bson_to_zval_ex(bson, &state)) {
			// Exception already thrown
			zval_ptr_dtor(&state.zchild);
			return;
		}

		zend_update_property(phongo_readpreference_ce, &intern->std, ZEND_STRL("hedge"), &state.zchild);
		zval_ptr_dtor(&state.zchild);
	} else {
		zend_update_property_null(phongo_readpreference_ce, &intern->std, ZEND_STRL("hedge"));
	}
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_readpreference_init_from_hash(phongo_readpreference_t* intern, HashTable* props)
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
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires specific values for \"mode\" string field", ZSTR_VAL(phongo_readpreference_ce->name));
			return false;
		}
	} else {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"mode\" field to be string", ZSTR_VAL(phongo_readpreference_ce->name));
		return false;
	}

	if ((tagSets = zend_hash_str_find(props, "tags", sizeof("tags") - 1))) {
		ZVAL_DEREF(tagSets);
		if (Z_TYPE_P(tagSets) == IS_ARRAY) {
			bson_t* tags = bson_new();

			/* Separate tagSets as phongo_read_preference_prep_tagsets may
			 * modify these tags. */
			SEPARATE_ZVAL_NOREF(tagSets);

			phongo_read_preference_prep_tagsets(tagSets);
			phongo_zval_to_bson(tagSets, PHONGO_BSON_NONE, (bson_t*) tags, NULL);

			if (!phongo_read_preference_tags_are_valid(tags)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"tags\" array field to have zero or more documents", ZSTR_VAL(phongo_readpreference_ce->name));
				bson_destroy(tags);
				goto failure;
			}

			if (!bson_empty(tags) && (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"tags\" array field to not be present with \"primary\" mode", ZSTR_VAL(phongo_readpreference_ce->name));
				bson_destroy(tags);
				goto failure;
			}

			mongoc_read_prefs_set_tags(intern->read_preference, tags);
			bson_destroy(tags);
		} else if (Z_TYPE_P(tagSets) != IS_NULL) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"tags\" field to be array", ZSTR_VAL(phongo_readpreference_ce->name));
			goto failure;
		}
	}

	if ((maxStalenessSeconds = zend_hash_str_find(props, "maxStalenessSeconds", sizeof("maxStalenessSeconds") - 1))) {
		if (Z_TYPE_P(maxStalenessSeconds) == IS_LONG) {
			if (Z_LVAL_P(maxStalenessSeconds) != MONGOC_NO_MAX_STALENESS) {
				if (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" field to not be present with \"primary\" mode", ZSTR_VAL(phongo_readpreference_ce->name));
					goto failure;
				}
				if (Z_LVAL_P(maxStalenessSeconds) < MONGOC_SMALLEST_MAX_STALENESS_SECONDS) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" integer field to be >= %d", ZSTR_VAL(phongo_readpreference_ce->name), MONGOC_SMALLEST_MAX_STALENESS_SECONDS);
					goto failure;
				}
				if (Z_LVAL_P(maxStalenessSeconds) > INT32_MAX) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" integer field to be <= %" PRId32, ZSTR_VAL(phongo_readpreference_ce->name), INT32_MAX);
					goto failure;
				}
			}

			mongoc_read_prefs_set_max_staleness_seconds(intern->read_preference, Z_LVAL_P(maxStalenessSeconds));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"maxStalenessSeconds\" field to be integer", ZSTR_VAL(phongo_readpreference_ce->name));
			goto failure;
		}
	}

	if ((hedge = zend_hash_str_find(props, "hedge", sizeof("hedge") - 1))) {
		if (Z_TYPE_P(hedge) == IS_ARRAY || Z_TYPE_P(hedge) == IS_OBJECT) {
			bson_t* hedge_doc = bson_new();

			php_error_docref(NULL, E_DEPRECATED, "The \"hedge\" option is deprecated as of MongoDB 8.0 and will be removed in a future release");

			if (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"hedge\" field to not be present with \"primary\" mode", ZSTR_VAL(phongo_readpreference_ce->name));
				bson_destroy(hedge_doc);
				goto failure;
			}

			phongo_zval_to_bson(hedge, PHONGO_BSON_NONE, hedge_doc, NULL);

			if (EG(exception)) {
				bson_destroy(hedge_doc);
				goto failure;
			}

			mongoc_read_prefs_set_hedge(intern->read_preference, hedge_doc);
			bson_destroy(hedge_doc);
		} else if (Z_TYPE_P(hedge) != IS_NULL) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"hedge\" field to be an array or object", ZSTR_VAL(phongo_readpreference_ce->name));
			goto failure;
		}
	}

	if (!mongoc_read_prefs_is_valid(intern->read_preference)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Read preference is not valid");
		goto failure;
	}

	phongo_readpreference_update_properties(intern);

	return true;

failure:
	mongoc_read_prefs_destroy(intern->read_preference);
	intern->read_preference = NULL;
	return false;
}

/* Constructs a new ReadPreference */
static PHP_METHOD(MongoDB_Driver_ReadPreference, __construct)
{
	PHONGO_INTERN_FROM_THIS(readpreference);

	zend_string* mode;
	zval*        tagSets = NULL;
	zval*        options = NULL;

	/* Separate the tagSets zval, since we may end up modifying it in
	 * phongo_read_preference_prep_tagsets() below. */
	PHONGO_PARSE_PARAMETERS_START(1, 3)
	Z_PARAM_STR(mode)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_EX(tagSets, 1, 1)
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	if (zend_string_equals_literal_ci(mode, PHONGO_READ_PRIMARY)) {
		intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_PRIMARY);
	} else if (zend_string_equals_literal_ci(mode, PHONGO_READ_PRIMARY_PREFERRED)) {
		intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_PRIMARY_PREFERRED);
	} else if (zend_string_equals_literal_ci(mode, PHONGO_READ_SECONDARY)) {
		intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_SECONDARY);
	} else if (zend_string_equals_literal_ci(mode, PHONGO_READ_SECONDARY_PREFERRED)) {
		intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_SECONDARY_PREFERRED);
	} else if (zend_string_equals_literal_ci(mode, PHONGO_READ_NEAREST)) {
		intern->read_preference = mongoc_read_prefs_new(MONGOC_READ_NEAREST);
	} else {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid mode: '%s'", ZSTR_VAL(mode));
		return;
	}

	if (tagSets) {
		bson_t* tags = bson_new();

		phongo_read_preference_prep_tagsets(tagSets);
		phongo_zval_to_bson(tagSets, PHONGO_BSON_NONE, (bson_t*) tags, NULL);

		if (!phongo_read_preference_tags_are_valid(tags)) {
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
		zval* hedge = php_array_fetchc_deref(options, "hedge");

		if (Z_TYPE_P(hedge) == IS_ARRAY || Z_TYPE_P(hedge) == IS_OBJECT) {
			bson_t* hedge_doc = bson_new();

			php_error_docref(NULL, E_DEPRECATED, "The \"hedge\" option is deprecated as of MongoDB 8.0 and will be removed in a future release");

			if (mongoc_read_prefs_get_mode(intern->read_preference) == MONGOC_READ_PRIMARY) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "hedge may not be used with primary mode");
				bson_destroy(hedge_doc);
				return;
			}

			phongo_zval_to_bson(hedge, PHONGO_BSON_NONE, hedge_doc, NULL);

			if (EG(exception)) {
				bson_destroy(hedge_doc);
				return;
			}

			mongoc_read_prefs_set_hedge(intern->read_preference, hedge_doc);
			bson_destroy(hedge_doc);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"hedge\" field to be an array or object", ZSTR_VAL(phongo_readpreference_ce->name));
			return;
		}
	}

	if (!mongoc_read_prefs_is_valid(intern->read_preference)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Read preference is not valid");
		return;
	}

	phongo_readpreference_update_properties(intern);
}

static PHP_METHOD(MongoDB_Driver_ReadPreference, __set_state)
{
	HashTable* props;
	zval*      array;

	/* Separate the zval, since we may end up modifying the "tags" element in
	 * phongo_read_preference_prep_tagsets(), which is called from
	 * phongo_readpreference_init_from_hash. */
	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY_EX(array, 0, 1)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(readpreference, return_value);
	props = Z_ARRVAL_P(array);

	phongo_readpreference_init_from_hash(intern, props);
}

/* Returns the ReadPreference hedge document */
static PHP_METHOD(MongoDB_Driver_ReadPreference, getHedge)
{
	PHONGO_INTERN_FROM_THIS(readpreference);

	const bson_t* hedge;

	PHONGO_PARSE_PARAMETERS_NONE();

	hedge = mongoc_read_prefs_get_hedge(intern->read_preference);

	if (!bson_empty0(hedge)) {
		phongo_bson_state state;

		PHONGO_BSON_INIT_STATE(state);

		if (!phongo_bson_to_zval_ex(hedge, &state)) {
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
	PHONGO_INTERN_FROM_THIS(readpreference);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(mongoc_read_prefs_get_max_staleness_seconds(intern->read_preference));
}

/* Returns the ReadPreference mode as string */
static PHP_METHOD(MongoDB_Driver_ReadPreference, getModeString)
{
	PHONGO_INTERN_FROM_THIS(readpreference);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRING(phongo_readpreference_get_mode_string(intern->read_preference));
}

/* Returns the ReadPreference tag sets */
static PHP_METHOD(MongoDB_Driver_ReadPreference, getTagSets)
{
	PHONGO_INTERN_FROM_THIS(readpreference);

	const bson_t* tags;

	PHONGO_PARSE_PARAMETERS_NONE();

	tags = mongoc_read_prefs_get_tags(intern->read_preference);

	if (tags->len) {
		phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!phongo_bson_to_zval_ex(tags, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		RETURN_ZVAL(&state.zchild, 0, 1);
	} else {
		RETURN_NULL();
	}
}

static PHP_METHOD(MongoDB_Driver_ReadPreference, bsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 4);

	{
		zend_string* string_key;
		zval*        val;

		ZEND_HASH_FOREACH_STR_KEY_VAL_IND(HASH_OF(getThis()), string_key, val)
		{
			if (
				(Z_TYPE_P(val) == IS_NULL) || (!strcmp(ZSTR_VAL(string_key), "maxStalenessSeconds") && Z_TYPE_P(val) == IS_LONG && Z_LVAL_P(val) == MONGOC_NO_MAX_STALENESS)) {
				continue;
			}

			// Increase the refcount of our zval, as add_assoc_zval takes ownership, leading to the property value being
			// freed once the return value goes out of scope
			Z_TRY_ADDREF_P(val);
			add_assoc_zval(return_value, ZSTR_VAL(string_key), val);
		}
		ZEND_HASH_FOREACH_END();
	}

	convert_to_object(return_value);
}

static PHP_METHOD(MongoDB_Driver_ReadPreference, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_readpreference_init_from_hash(Z_READPREFERENCE_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\Driver\ReadPreference object handlers */
static zend_object_handlers phongo_handler_readpreference;

static void phongo_readpreference_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(readpreference, object);

	zend_object_std_dtor(&intern->std);

	if (intern->read_preference) {
		mongoc_read_prefs_destroy(intern->read_preference);
	}
}

static zend_object* phongo_readpreference_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(readpreference, class_type);

	intern->std.handlers = &phongo_handler_readpreference;

	return &intern->std;
}

static zval* phongo_readpreference_read_property(zend_object* zobj, zend_string* name, int type, void** cache_slot, zval* rv)
{
	if (!strcmp(ZSTR_VAL(name), "hedge")) {
		php_error_docref(NULL, E_DEPRECATED, "Property MongoDB\\Driver\\ReadPreference::hedge is deprecated");
	}

	return zend_std_read_property(zobj, name, type, cache_slot, rv);
}

void phongo_readpreference_init_ce(INIT_FUNC_ARGS)
{
	phongo_readpreference_ce                = register_class_MongoDB_Driver_ReadPreference(phongo_serializable_ce);
	phongo_readpreference_ce->create_object = phongo_readpreference_create_object;

	memcpy(&phongo_handler_readpreference, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_readpreference.read_property = phongo_readpreference_read_property;
	phongo_handler_readpreference.free_obj      = phongo_readpreference_free_object;
	phongo_handler_readpreference.offset        = XtOffsetOf(phongo_readpreference_t, std);
}

void phongo_readpreference_init(zval* return_value, const mongoc_read_prefs_t* read_prefs)
{
	PHONGO_INTERN_INIT_EX(readpreference, return_value);
	intern->read_preference = mongoc_read_prefs_copy(read_prefs);

	phongo_readpreference_update_properties(intern);
}

const mongoc_read_prefs_t* phongo_read_preference_from_zval(zval* zread_preference)
{
	if (zread_preference) {
		PHONGO_INTERN_FROM_ZVAL(readpreference, zread_preference);

		if (intern) {
			return intern->read_preference;
		}
	}

	return NULL;
}

/* Prepare tagSets for BSON encoding by converting each array in the set to an
 * object. This ensures that empty arrays will serialize as empty documents.
 *
 * phongo_read_preference_tags_are_valid() handles actual validation of the
 * tag set structure. */
void phongo_read_preference_prep_tagsets(zval* tagSets)
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
bool phongo_read_preference_tags_are_valid(const bson_t* tags)
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
