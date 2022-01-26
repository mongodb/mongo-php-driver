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

#include <php.h>

#include "php_phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"

typedef enum {
	PHONGO_JSON_MODE_LEGACY,
	PHONGO_JSON_MODE_CANONICAL,
	PHONGO_JSON_MODE_RELAXED,
} php_phongo_json_mode_t;

/* {{{ proto string MongoDB\BSON\fromPHP(array|object $value)
   Returns the BSON representation of a PHP value */
PHP_FUNCTION(MongoDB_BSON_fromPHP)
{
	zend_error_handling error_handling;
	zval*               data;
	bson_t*             bson;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "A", &data) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	bson = bson_new();
	php_phongo_zval_to_bson(data, PHONGO_BSON_NONE, bson, NULL);

	RETVAL_STRINGL((const char*) bson_get_data(bson), bson->len);
	bson_destroy(bson);
} /* }}} */

/* {{{ proto array|object MongoDB\BSON\toPHP(string $bson [, array $typemap = array()])
   Returns the PHP representation of a BSON value, optionally converting it into a custom class */
PHP_FUNCTION(MongoDB_BSON_toPHP)
{
	zend_error_handling   error_handling;
	char*                 data;
	size_t                data_len;
	zval*                 typemap = NULL;
	php_phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|a!", &data, &data_len, &typemap) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if (!php_phongo_bson_typemap_to_state(typemap, &state.map)) {
		return;
	}

	if (!php_phongo_bson_to_zval_ex((const unsigned char*) data, data_len, &state)) {
		zval_ptr_dtor(&state.zchild);
		php_phongo_bson_typemap_dtor(&state.map);
		RETURN_NULL();
	}

	php_phongo_bson_typemap_dtor(&state.map);

	RETURN_ZVAL(&state.zchild, 0, 1);
} /* }}} */

/* {{{ proto string MongoDB\BSON\fromJSON(string $json)
   Returns the BSON representation of a JSON value */
PHP_FUNCTION(MongoDB_BSON_fromJSON)
{
	zend_error_handling error_handling;
	char*               json;
	size_t              json_len;
	bson_t              bson  = BSON_INITIALIZER;
	bson_error_t        error = { 0 };

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &json, &json_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if (bson_init_from_json(&bson, (const char*) json, json_len, &error)) {
		RETVAL_STRINGL((const char*) bson_get_data(&bson), bson.len);
		bson_destroy(&bson);
	} else {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s", error.domain == BSON_ERROR_JSON ? error.message : "Error parsing JSON");
	}
} /* }}} */

static void phongo_bson_to_json(INTERNAL_FUNCTION_PARAMETERS, php_phongo_json_mode_t mode)
{
	zend_error_handling error_handling;
	char*               data;
	size_t              data_len;
	const bson_t*       bson;
	bool                eof = false;
	bson_reader_t*      reader;
	char*               json = NULL;
	size_t              json_len;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &data, &data_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	reader = bson_reader_new_from_data((const unsigned char*) data, data_len);
	bson   = bson_reader_read(reader, NULL);

	if (!bson) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not read document from BSON reader");
		bson_reader_destroy(reader);
		return;
	}

	if (mode == PHONGO_JSON_MODE_LEGACY) {
		json = bson_as_json(bson, &json_len);
	} else if (mode == PHONGO_JSON_MODE_CANONICAL) {
		json = bson_as_canonical_extended_json(bson, &json_len);
	} else if (mode == PHONGO_JSON_MODE_RELAXED) {
		json = bson_as_relaxed_extended_json(bson, &json_len);
	}

	if (!json) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not convert BSON document to a JSON string");
		bson_reader_destroy(reader);
		return;
	}

	RETVAL_STRINGL(json, json_len);
	bson_free(json);

	if (bson_reader_read(reader, &eof) || !eof) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Reading document did not exhaust input buffer");
	}

	bson_reader_destroy(reader);
} /* }}} */

/* {{{ proto string MongoDB\BSON\toJSON(string $bson)
   Returns the legacy extended JSON representation of a BSON value */
PHP_FUNCTION(MongoDB_BSON_toJSON)
{
	phongo_bson_to_json(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHONGO_JSON_MODE_LEGACY);
} /* }}} */

/* {{{ proto string MongoDB\BSON\toCanonicalExtendedJSON(string $bson)
   Returns the canonical extended JSON representation of a BSON value */
PHP_FUNCTION(MongoDB_BSON_toCanonicalExtendedJSON)
{
	phongo_bson_to_json(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHONGO_JSON_MODE_CANONICAL);
} /* }}} */

/* {{{ proto string MongoDB\BSON\toRelaxedExtendedJSON(string $bson)
   Returns the relaxed extended JSON representation of a BSON value */
PHP_FUNCTION(MongoDB_BSON_toRelaxedExtendedJSON)
{
	phongo_bson_to_json(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHONGO_JSON_MODE_RELAXED);
} /* }}} */
