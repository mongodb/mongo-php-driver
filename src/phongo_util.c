/*
 * Copyright 2022-present MongoDB, Inc.
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

#include "phongo_util.h"

const char* php_phongo_bson_type_to_string(bson_type_t type)
{
	switch (type) {
		case BSON_TYPE_EOD:
			return "EOD";
		case BSON_TYPE_DOUBLE:
			return "double";
		case BSON_TYPE_UTF8:
			return "string";
		case BSON_TYPE_DOCUMENT:
			return "document";
		case BSON_TYPE_ARRAY:
			return "array";
		case BSON_TYPE_BINARY:
			return "Binary";
		case BSON_TYPE_UNDEFINED:
			return "undefined";
		case BSON_TYPE_OID:
			return "ObjectId";
		case BSON_TYPE_BOOL:
			return "boolean";
		case BSON_TYPE_DATE_TIME:
			return "UTCDateTime";
		case BSON_TYPE_NULL:
			return "null";
		case BSON_TYPE_REGEX:
			return "Regex";
		case BSON_TYPE_DBPOINTER:
			return "DBPointer";
		case BSON_TYPE_CODE:
			return "Javascript";
		case BSON_TYPE_SYMBOL:
			return "symbol";
		case BSON_TYPE_CODEWSCOPE:
			return "Javascript with scope";
		case BSON_TYPE_INT32:
			return "32-bit integer";
		case BSON_TYPE_TIMESTAMP:
			return "Timestamp";
		case BSON_TYPE_INT64:
			return "64-bit integer";
		case BSON_TYPE_DECIMAL128:
			return "Decimal128";
		case BSON_TYPE_MAXKEY:
			return "MaxKey";
		case BSON_TYPE_MINKEY:
			return "MinKey";
		default:
			return "unknown";
	}
}

/* If options is not an array, insert it as a field in a newly allocated array.
 * This may be used to convert legacy options (e.g. ReadPreference option for
 * an executeQuery method) into an options array.
 *
 * A pointer to the array zval will always be returned. If allocated is set to
 * true, php_phongo_prep_legacy_option_free() should be used to free the array
 * zval later. */
zval* php_phongo_prep_legacy_option(zval* options, const char* key, bool* allocated)
{
	*allocated = false;

	if (options && Z_TYPE_P(options) != IS_ARRAY) {
		zval* new_options = ecalloc(1, sizeof(zval));

		array_init_size(new_options, 1);
		add_assoc_zval(new_options, key, options);
		Z_ADDREF_P(options);
		*allocated = true;

		return new_options;
	}

	return options;
}

void php_phongo_prep_legacy_option_free(zval* options)
{
	zval_ptr_dtor(options);
	efree(options);
}

bool php_phongo_parse_int64(int64_t* retval, const char* data, size_t data_len)
{
	int64_t value;
	char*   endptr = NULL;

	/* bson_ascii_strtoll() sets errno if conversion fails. If conversion
	 * succeeds, we still want to ensure that the entire string was parsed. */
	value = bson_ascii_strtoll(data, &endptr, 10);

	if (errno || (endptr && endptr != ((const char*) data + data_len))) {
		return false;
	}

	*retval = value;

	return true;
}

/* Splits a namespace name into the database and collection names, allocated with estrdup. */
bool phongo_split_namespace(const char* namespace, char** dbname, char** cname)
{
	char* dot = strchr(namespace, '.');

	if (!dot) {
		return false;
	}

	if (cname) {
		*cname = estrdup(namespace + (dot - namespace) + 1);
	}
	if (dbname) {
		*dbname = estrndup(namespace, dot - namespace);
	}

	return true;
}
