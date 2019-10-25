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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"
#include "php_bson.h"

typedef enum {
	PHONGO_JSON_MODE_LEGACY,
	PHONGO_JSON_MODE_CANONICAL,
	PHONGO_JSON_MODE_RELAXED,
} php_phongo_json_mode_t;

/* {{{ proto string MongoDB\BSON\fromPHP(array|object $value)
   Returns the BSON representation of a PHP value */
PHP_FUNCTION(MongoDB_BSON_fromPHP)
{
	zval*   data;
	bson_t* bson;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &data) == FAILURE) {
		return;
	}

	bson = bson_new();
	php_phongo_zval_to_bson(data, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);

	PHONGO_RETVAL_STRINGL((const char*) bson_get_data(bson), bson->len);
	bson_destroy(bson);
} /* }}} */

/* {{{ proto array|object MongoDB\BSON\toPHP(string $bson [, array $typemap = array()])
   Returns the PHP representation of a BSON value, optionally converting it into a custom class */
PHP_FUNCTION(MongoDB_BSON_toPHP)
{
	char*                 data;
	phongo_zpp_char_len   data_len;
	zval*                 typemap = NULL;
	php_phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a!", &data, &data_len, &typemap) == FAILURE) {
		return;
	}

	if (!php_phongo_bson_typemap_to_state(typemap, &state.map TSRMLS_CC)) {
		return;
	}

	if (!php_phongo_bson_to_zval_ex((const unsigned char*) data, data_len, &state)) {
		zval_ptr_dtor(&state.zchild);
		php_phongo_bson_typemap_dtor(&state.map);
		RETURN_NULL();
	}

	php_phongo_bson_typemap_dtor(&state.map);

#if PHP_VERSION_ID >= 70000
	RETURN_ZVAL(&state.zchild, 0, 1);
#else
	RETURN_ZVAL(state.zchild, 0, 1);
#endif
} /* }}} */

/* {{{ proto string MongoDB\BSON\fromJSON(string $json)
   Returns the BSON representation of a JSON value */
PHP_FUNCTION(MongoDB_BSON_fromJSON)
{
	char*               json;
	phongo_zpp_char_len json_len;
	bson_t              bson  = BSON_INITIALIZER;
	bson_error_t        error = { 0 };

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &json, &json_len) == FAILURE) {
		return;
	}

	if (bson_init_from_json(&bson, (const char*) json, json_len, &error)) {
		PHONGO_RETVAL_STRINGL((const char*) bson_get_data(&bson), bson.len);
		bson_destroy(&bson);
	} else {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "%s", error.domain == BSON_ERROR_JSON ? error.message : "Error parsing JSON");
	}
} /* }}} */

static void phongo_bson_to_json(INTERNAL_FUNCTION_PARAMETERS, php_phongo_json_mode_t mode)
{
	char*               data;
	phongo_zpp_char_len data_len;
	const bson_t*       bson;
	bool                eof = false;
	bson_reader_t*      reader;
	char*               json = NULL;
	size_t              json_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE) {
		return;
	}

	reader = bson_reader_new_from_data((const unsigned char*) data, data_len);
	bson   = bson_reader_read(reader, NULL);

	if (!bson) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Could not read document from BSON reader");
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
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Could not convert BSON document to a JSON string");
		bson_reader_destroy(reader);
		return;
	}

	PHONGO_RETVAL_STRINGL(json, json_len);
	bson_free(json);

	if (bson_reader_read(reader, &eof) || !eof) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "Reading document did not exhaust input buffer");
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

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
