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

#include "php_phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"

#include "MongoDB/Query.h"
#include "MongoDB/ReadConcern.h"
#include "Query_arginfo.h"

zend_class_entry* php_phongo_query_ce;

/* Appends a string field into the BSON options. Returns true on success;
 * otherwise, false is returned and an exception is thrown. */
static bool php_phongo_query_opts_append_string(bson_t* opts, const char* opts_key, zval* zarr, const char* zarr_key)
{
	zval* value = php_array_fetch(zarr, zarr_key);

	if (Z_TYPE_P(value) != IS_STRING) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"%s\" %s to be string, %s given", zarr_key, zarr_key[0] == '$' ? "modifier" : "option", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(value));
		return false;
	}

	if (!bson_append_utf8(opts, opts_key, strlen(opts_key), Z_STRVAL_P(value), Z_STRLEN_P(value))) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"%s\" option", opts_key);
		return false;
	}

	return true;
}

/* Appends a document field for the given opts document and key. Returns true on
 * success; otherwise, false is returned and an exception is thrown. */
static bool php_phongo_query_opts_append_document(bson_t* opts, const char* opts_key, zval* zarr, const char* zarr_key)
{
	zval*  value = php_array_fetch(zarr, zarr_key);
	bson_t b     = BSON_INITIALIZER;

	if (Z_TYPE_P(value) != IS_OBJECT && Z_TYPE_P(value) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"%s\" %s to be array or object, %s given", zarr_key, zarr_key[0] == '$' ? "modifier" : "option", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(value));
		return false;
	}

	php_phongo_zval_to_bson(value, PHONGO_BSON_NONE, &b, NULL);

	if (EG(exception)) {
		bson_destroy(&b);
		return false;
	}

	if (!bson_validate(&b, BSON_VALIDATE_EMPTY_KEYS, NULL)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Cannot use empty keys in \"%s\" %s", zarr_key, zarr_key[0] == '$' ? "modifier" : "option");
		bson_destroy(&b);
		return false;
	}

	if (!BSON_APPEND_DOCUMENT(opts, opts_key, &b)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"%s\" option", opts_key);
		bson_destroy(&b);
		return false;
	}

	bson_destroy(&b);
	return true;
}

/* Appends an arbitrary BSON value for the given opts document and key. Returns
 * true on success; otherwise, false is returned and an exception is thrown. */
static bool php_phongo_query_opts_append_value(bson_t* opts, const char* opts_key, zval* zarr, const char* zarr_key)
{
	bson_value_t value = { 0 };

	phongo_zval_to_bson_value(php_array_fetch(zarr, zarr_key), &value);

	if (EG(exception)) {
		/* Exception should already have been thrown */
		bson_value_destroy(&value);
		return false;
	}

	if (!BSON_APPEND_VALUE(opts, opts_key, &value)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"%s\" option", opts_key);
		bson_value_destroy(&value);
		return false;
	}

	bson_value_destroy(&value);
	return true;
}

#define PHONGO_QUERY_OPT_BOOL_EX(opt, zarr, key, deprecated)                                                                      \
	if ((zarr) && php_array_existsc((zarr), (key))) {                                                                             \
		if ((deprecated)) {                                                                                                       \
			php_error_docref(NULL, E_DEPRECATED, "The \"%s\" option is deprecated and will be removed in a future release", key); \
		}                                                                                                                         \
		if (!BSON_APPEND_BOOL(intern->opts, (opt), php_array_fetchc_bool((zarr), (key)))) {                                       \
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"%s\" option", (opt));                        \
			return false;                                                                                                         \
		}                                                                                                                         \
	}

#define PHONGO_QUERY_OPT_BOOL(opt, zarr, key) PHONGO_QUERY_OPT_BOOL_EX((opt), (zarr), (key), 0)
#define PHONGO_QUERY_OPT_BOOL_DEPRECATED(opt, zarr, key) PHONGO_QUERY_OPT_BOOL_EX((opt), (zarr), (key), 1)

#define PHONGO_QUERY_OPT_BSON_VALUE(opt, zarr, key)                                    \
	if ((zarr) && php_array_existsc((zarr), (key))) {                                  \
		if (!php_phongo_query_opts_append_value(intern->opts, (opt), (zarr), (key))) { \
			return false;                                                              \
		}                                                                              \
	}

#define PHONGO_QUERY_OPT_DOCUMENT(opt, zarr, key)                                         \
	if ((zarr) && php_array_existsc((zarr), (key))) {                                     \
		if (!php_phongo_query_opts_append_document(intern->opts, (opt), (zarr), (key))) { \
			return false;                                                                 \
		}                                                                                 \
	}

/* Note: handling of integer options will depend on SIZEOF_ZEND_LONG and we
 * are not converting strings to 64-bit integers for 32-bit platforms. */

#define PHONGO_QUERY_OPT_INT64_EX(opt, zarr, key, deprecated)                                                                     \
	if ((zarr) && php_array_existsc((zarr), (key))) {                                                                             \
		if ((deprecated)) {                                                                                                       \
			php_error_docref(NULL, E_DEPRECATED, "The \"%s\" option is deprecated and will be removed in a future release", key); \
		}                                                                                                                         \
		if (!BSON_APPEND_INT64(intern->opts, (opt), php_array_fetchc_long((zarr), (key)))) {                                      \
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"%s\" option", (opt));                        \
			return false;                                                                                                         \
		}                                                                                                                         \
	}

#define PHONGO_QUERY_OPT_INT64(opt, zarr, key) PHONGO_QUERY_OPT_INT64_EX((opt), (zarr), (key), 0)
#define PHONGO_QUERY_OPT_INT64_DEPRECATED(opt, zarr, key) PHONGO_QUERY_OPT_INT64_EX((opt), (zarr), (key), 1)

#define PHONGO_QUERY_OPT_STRING(opt, zarr, key)                                         \
	if ((zarr) && php_array_existsc((zarr), (key))) {                                   \
		if (!php_phongo_query_opts_append_string(intern->opts, (opt), (zarr), (key))) { \
			return false;                                                               \
		}                                                                               \
	}

/* Initialize the "hint" option. Returns true on success; otherwise, false is
 * returned and an exception is thrown.
 *
 * The "hint" option (or "$hint" modifier) must be a string or document. Check
 * for both types and merge into BSON options accordingly. */
static bool php_phongo_query_init_hint(php_phongo_query_t* intern, zval* options, zval* modifiers)
{
	/* The "hint" option (or "$hint" modifier) must be a string or document.
	 * Check for both types and merge into BSON options accordingly. */
	if (php_array_existsc(options, "hint")) {
		zend_uchar type = Z_TYPE_P(php_array_fetchc(options, "hint"));

		if (type == IS_STRING) {
			PHONGO_QUERY_OPT_STRING("hint", options, "hint");
		} else if (type == IS_OBJECT || type == IS_ARRAY) {
			PHONGO_QUERY_OPT_DOCUMENT("hint", options, "hint");
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"hint\" option to be string, array, or object, %s given", zend_get_type_by_const(type));
			return false;
		}
	} else if (modifiers && php_array_existsc(modifiers, "$hint")) {
		zend_uchar type = Z_TYPE_P(php_array_fetchc(modifiers, "$hint"));

		if (type == IS_STRING) {
			PHONGO_QUERY_OPT_STRING("hint", modifiers, "$hint");
		} else if (type == IS_OBJECT || type == IS_ARRAY) {
			PHONGO_QUERY_OPT_DOCUMENT("hint", modifiers, "$hint");
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"$hint\" modifier to be string, array, or object, %s given", zend_get_type_by_const(type));
			return false;
		}
	}

	return true;
}

/* Initialize the "limit" and "singleBatch" options. Returns true on success;
 * otherwise, false is returned and an exception is thrown.
 *
 * mongoc_collection_find_with_opts() requires a non-negative limit. For
 * backwards compatibility, a negative limit should be set as a positive value
 * and default singleBatch to true. */
static bool php_phongo_query_init_limit_and_singlebatch(php_phongo_query_t* intern, zval* options)
{
	if (php_array_fetchc_long(options, "limit") < 0) {
		zend_long limit = php_array_fetchc_long(options, "limit");

		if (!BSON_APPEND_INT64(intern->opts, "limit", -limit)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"limit\" option");
			return false;
		}

		if (php_array_existsc(options, "singleBatch") && !php_array_fetchc_bool(options, "singleBatch")) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Negative \"limit\" option conflicts with false \"singleBatch\" option");
			return false;
		} else {
			if (!BSON_APPEND_BOOL(intern->opts, "singleBatch", true)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"singleBatch\" option");
				return false;
			}
		}
	} else {
		PHONGO_QUERY_OPT_INT64("limit", options, "limit");
		PHONGO_QUERY_OPT_BOOL("singleBatch", options, "singleBatch");
	}

	return true;
}

/* Initialize the "readConcern" option. Returns true on success; otherwise,
 * false is returned and an exception is thrown.
 *
 * The "readConcern" option should be a MongoDB\Driver\ReadConcern instance,
 * which must be converted to a mongoc_read_concern_t. */
static bool php_phongo_query_init_readconcern(php_phongo_query_t* intern, zval* options)
{
	zval* read_concern;

	if (!php_array_existsc(options, "readConcern")) {
		return true;
	}

	read_concern = php_array_fetchc(options, "readConcern");

	if (Z_TYPE_P(read_concern) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(read_concern), php_phongo_readconcern_ce)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"readConcern\" option to be %s, %s given", ZSTR_VAL(php_phongo_readconcern_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(read_concern));
		return false;
	}

	intern->read_concern = mongoc_read_concern_copy(phongo_read_concern_from_zval(read_concern));

	return true;
}

/* Initialize the "maxAwaitTimeMS" option. Returns true on success; otherwise,
 * false is returned and an exception is thrown.
 *
 * The "maxAwaitTimeMS" option is assigned to the cursor after query execution
 * via mongoc_cursor_set_max_await_time_ms(). */
static bool php_phongo_query_init_max_await_time_ms(php_phongo_query_t* intern, zval* options)
{
	int64_t max_await_time_ms;

	if (!php_array_existsc(options, "maxAwaitTimeMS")) {
		return true;
	}

	max_await_time_ms = php_array_fetchc_long(options, "maxAwaitTimeMS");

	if (max_await_time_ms < 0) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"maxAwaitTimeMS\" option to be >= 0, %" PRId64 " given", max_await_time_ms);
		return false;
	}

	if (max_await_time_ms > UINT32_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"maxAwaitTimeMS\" option to be <= %" PRIu32 ", %" PRId64 " given", UINT32_MAX, max_await_time_ms);
		return false;
	}

	intern->max_await_time_ms = (uint32_t) max_await_time_ms;

	return true;
}

/* Initializes the query from filter and options arguments and returns whether
 * an error occurred. If query is undefined, it will be initialized.
 *
 * This function will fall back to a modifier in the absence of a top-level
 * option (where applicable). */
bool phongo_query_init(zval* return_value, zval* filter, zval* options)
{
	php_phongo_query_t* intern;
	zval*               modifiers = NULL;

	if (Z_ISUNDEF_P(return_value)) {
		object_init_ex(return_value, php_phongo_query_ce);
	}

	if (Z_TYPE_P(return_value) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(return_value), php_phongo_query_ce)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Expected initialization object to be %s, %s given", ZSTR_VAL(php_phongo_query_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(return_value));
		return false;
	}

	intern = Z_QUERY_OBJ_P(return_value);

	intern->filter            = bson_new();
	intern->opts              = bson_new();
	intern->max_await_time_ms = 0;

	if (filter) {
		php_phongo_zval_to_bson(filter, PHONGO_BSON_NONE, intern->filter, NULL);
	}

	/* Note: if any exceptions are thrown, we can simply return as PHP will
	 * invoke php_phongo_query_free_object to destruct the object. */
	if (EG(exception)) {
		return false;
	}

	if (!bson_validate(intern->filter, BSON_VALIDATE_EMPTY_KEYS, NULL)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Cannot use empty keys in filter document");
		return false;
	}

	if (!options) {
		return true;
	}

	if (php_array_existsc(options, "modifiers")) {
		modifiers = php_array_fetchc(options, "modifiers");

		if (Z_TYPE_P(modifiers) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"modifiers\" option to be array, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(modifiers));
			return false;
		}
	}

	PHONGO_QUERY_OPT_BOOL("allowDiskUse", options, "allowDiskUse")
	PHONGO_QUERY_OPT_BOOL("allowPartialResults", options, "allowPartialResults")
	else PHONGO_QUERY_OPT_BOOL("allowPartialResults", options, "partial");
	PHONGO_QUERY_OPT_BOOL("awaitData", options, "awaitData");
	PHONGO_QUERY_OPT_INT64("batchSize", options, "batchSize");
	PHONGO_QUERY_OPT_DOCUMENT("collation", options, "collation");
	PHONGO_QUERY_OPT_BSON_VALUE("comment", options, "comment")
	else PHONGO_QUERY_OPT_BSON_VALUE("comment", modifiers, "$comment");
	PHONGO_QUERY_OPT_BOOL("exhaust", options, "exhaust");
	PHONGO_QUERY_OPT_DOCUMENT("let", options, "let");
	PHONGO_QUERY_OPT_DOCUMENT("max", options, "max")
	else PHONGO_QUERY_OPT_DOCUMENT("max", modifiers, "$max");
	PHONGO_QUERY_OPT_INT64_DEPRECATED("maxScan", options, "maxScan")
	else PHONGO_QUERY_OPT_INT64_DEPRECATED("maxScan", modifiers, "$maxScan");
	PHONGO_QUERY_OPT_INT64("maxTimeMS", options, "maxTimeMS")
	else PHONGO_QUERY_OPT_INT64("maxTimeMS", modifiers, "$maxTimeMS");
	PHONGO_QUERY_OPT_DOCUMENT("min", options, "min")
	else PHONGO_QUERY_OPT_DOCUMENT("min", modifiers, "$min");
	PHONGO_QUERY_OPT_BOOL("noCursorTimeout", options, "noCursorTimeout");
	PHONGO_QUERY_OPT_BOOL_DEPRECATED("oplogReplay", options, "oplogReplay");
	PHONGO_QUERY_OPT_DOCUMENT("projection", options, "projection");
	PHONGO_QUERY_OPT_BOOL("returnKey", options, "returnKey")
	else PHONGO_QUERY_OPT_BOOL("returnKey", modifiers, "$returnKey");
	PHONGO_QUERY_OPT_BOOL("showRecordId", options, "showRecordId")
	else PHONGO_QUERY_OPT_BOOL("showRecordId", modifiers, "$showDiskLoc");
	PHONGO_QUERY_OPT_INT64("skip", options, "skip");
	PHONGO_QUERY_OPT_DOCUMENT("sort", options, "sort")
	else PHONGO_QUERY_OPT_DOCUMENT("sort", modifiers, "$orderby");
	PHONGO_QUERY_OPT_BOOL_DEPRECATED("snapshot", options, "snapshot")
	else PHONGO_QUERY_OPT_BOOL_DEPRECATED("snapshot", modifiers, "$snapshot");
	PHONGO_QUERY_OPT_BOOL("tailable", options, "tailable");

	/* The "$explain" modifier should be converted to an "explain" option, which
	 * libmongoc will later convert back to a modifier for the OP_QUERY code
	 * path. This modifier will be ignored for the find command code path. */
	PHONGO_QUERY_OPT_BOOL("explain", modifiers, "$explain");

	if (!php_phongo_query_init_hint(intern, options, modifiers)) {
		return false;
	}

	if (!php_phongo_query_init_limit_and_singlebatch(intern, options)) {
		return false;
	}

	if (!php_phongo_query_init_readconcern(intern, options)) {
		return false;
	}

	if (!php_phongo_query_init_max_await_time_ms(intern, options)) {
		return false;
	}

	return true;
}

#undef PHONGO_QUERY_OPT_BOOL_EX
#undef PHONGO_QUERY_OPT_BOOL
#undef PHONGO_QUERY_OPT_BOOL_DEPRECATED
#undef PHONGO_QUERY_OPT_BSON_VALUE
#undef PHONGO_QUERY_OPT_DOCUMENT
#undef PHONGO_QUERY_OPT_INT64_EX
#undef PHONGO_QUERY_OPT_INT64
#undef PHONGO_QUERY_OPT_INT64_DEPRECATED
#undef PHONGO_QUERY_OPT_STRING

/* Constructs a new Query */
static PHP_METHOD(MongoDB_Driver_Query, __construct)
{
	zval* filter;
	zval* options = NULL;

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_ARRAY_OR_OBJECT(filter)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_query_init(getThis(), filter, options);
}

/* MongoDB\Driver\Query object handlers */
static zend_object_handlers php_phongo_handler_query;

static void php_phongo_query_free_object(zend_object* object)
{
	php_phongo_query_t* intern = Z_OBJ_QUERY(object);

	zend_object_std_dtor(&intern->std);

	if (intern->filter) {
		bson_clear(&intern->filter);
	}

	if (intern->opts) {
		bson_clear(&intern->opts);
	}

	if (intern->read_concern) {
		mongoc_read_concern_destroy(intern->read_concern);
	}
}

static zend_object* php_phongo_query_create_object(zend_class_entry* class_type)
{
	php_phongo_query_t* intern = zend_object_alloc(sizeof(php_phongo_query_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_query;

	return &intern->std;
}

static HashTable* php_phongo_query_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_query_t* intern;
	zval                retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_OBJ_QUERY(object);

	array_init_size(&retval, 3);

	/* Avoid using PHONGO_TYPEMAP_NATIVE_ARRAY for decoding filter and opts
	 * documents so that users can differentiate BSON arrays and documents. */
	if (intern->filter) {
		zval zv;

		if (!php_phongo_bson_to_zval(intern->filter, &zv)) {
			zval_ptr_dtor(&zv);
			goto done;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "filter", &zv);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "filter");
	}

	if (intern->opts) {
		zval zv;

		if (!php_phongo_bson_to_zval(intern->opts, &zv)) {
			zval_ptr_dtor(&zv);
			goto done;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "options", &zv);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "options");
	}

	if (intern->read_concern) {
		zval read_concern;

		php_phongo_read_concern_to_zval(&read_concern, intern->read_concern);
		ADD_ASSOC_ZVAL_EX(&retval, "readConcern", &read_concern);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "readConcern");
	}

done:
	return Z_ARRVAL(retval);
}

void php_phongo_query_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_query_ce                = register_class_MongoDB_Driver_Query();
	php_phongo_query_ce->create_object = php_phongo_query_create_object;

	memcpy(&php_phongo_handler_query, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_query.get_debug_info = php_phongo_query_get_debug_info;
	php_phongo_handler_query.free_obj       = php_phongo_query_free_object;
	php_phongo_handler_query.offset         = XtOffsetOf(php_phongo_query_t, std);
}
