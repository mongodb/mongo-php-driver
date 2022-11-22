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

/* Returns the BSON representation of a PHP value */
PHP_FUNCTION(fromPHP)
{
	zval*   data;
	bson_t* bson;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	PHONGO_PARAM_ARRAY_OR_OBJECT(data)
	PHONGO_PARSE_PARAMETERS_END();

	bson = bson_new();
	php_phongo_zval_to_bson(data, PHONGO_BSON_NONE, bson, NULL);

	RETVAL_STRINGL((const char*) bson_get_data(bson), bson->len);
	bson_destroy(bson);
}

/* Returns the PHP representation of a BSON value, optionally converting it into a custom class */
PHP_FUNCTION(toPHP)
{
	char*                 data;
	size_t                data_len;
	zval*                 typemap = NULL;
	php_phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_STRING(data, data_len)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(typemap)
	PHONGO_PARSE_PARAMETERS_END();

	if (!php_phongo_bson_typemap_to_state(typemap, &state.map)) {
		return;
	}

	if (!php_phongo_bson_data_to_zval_ex((const unsigned char*) data, data_len, &state)) {
		zval_ptr_dtor(&state.zchild);
		php_phongo_bson_typemap_dtor(&state.map);
		RETURN_NULL();
	}

	php_phongo_bson_typemap_dtor(&state.map);

	RETURN_ZVAL(&state.zchild, 0, 1);
}

/* Returns the BSON representation of a JSON value */
PHP_FUNCTION(fromJSON)
{
	char*        json;
	size_t       json_len;
	bson_t       bson  = BSON_INITIALIZER;
	bson_error_t error = { 0 };

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(json, json_len)
	PHONGO_PARSE_PARAMETERS_END();

	if (bson_init_from_json(&bson, (const char*) json, json_len, &error)) {
		RETVAL_STRINGL((const char*) bson_get_data(&bson), bson.len);
		bson_destroy(&bson);
	} else {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s", error.domain == BSON_ERROR_JSON ? error.message : "Error parsing JSON");
	}
}

static void phongo_bson_to_json(INTERNAL_FUNCTION_PARAMETERS, php_phongo_json_mode_t mode)
{
	char*          data;
	size_t         data_len;
	const bson_t*  bson;
	bool           eof = false;
	bson_reader_t* reader;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(data, data_len)
	PHONGO_PARSE_PARAMETERS_END();

	reader = bson_reader_new_from_data((const unsigned char*) data, data_len);
	bson   = bson_reader_read(reader, NULL);

	if (!bson) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not read document from BSON reader");
		bson_reader_destroy(reader);
		return;
	}

	if (!php_phongo_bson_to_json(return_value, bson, mode)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not convert BSON document to a JSON string");
		bson_reader_destroy(reader);
		return;
	}

	if (bson_reader_read(reader, &eof) || !eof) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Reading document did not exhaust input buffer");
	}

	bson_reader_destroy(reader);
}

/* Returns the legacy extended JSON representation of a BSON value */
PHP_FUNCTION(toJSON)
{
	phongo_bson_to_json(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHONGO_JSON_MODE_LEGACY);
}

/* Returns the canonical extended JSON representation of a BSON value */
PHP_FUNCTION(toCanonicalExtendedJSON)
{
	phongo_bson_to_json(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHONGO_JSON_MODE_CANONICAL);
}

/* Returns the relaxed extended JSON representation of a BSON value */
PHP_FUNCTION(toRelaxedExtendedJSON)
{
	phongo_bson_to_json(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHONGO_JSON_MODE_RELAXED);
}
