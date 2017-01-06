/*
 * Copyright 2015-2017 MongoDB, Inc.
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

#ifndef PHONGO_STRUCTS_H
#define PHONGO_STRUCTS_H

#include <php.h>

#include "php_bson.h"

#if PHP_VERSION_ID >= 70000
#	define PHONGO_ZEND_OBJECT_PRE
#	define PHONGO_ZEND_OBJECT_POST zend_object std;
#	define PHONGO_STRUCT_ZVAL      zval
#else
#	define PHONGO_ZEND_OBJECT_PRE  zend_object std;
#	define PHONGO_ZEND_OBJECT_POST
#	define PHONGO_STRUCT_ZVAL      zval*
#endif

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	mongoc_bulk_operation_t *bulk;
	size_t                   num_ops;
	bool                     ordered;
	int                      bypass;
	char                    *database;
	char                    *collection;
	bool                     executed;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_bulkwrite_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	bson_t *bson;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_command_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	mongoc_cursor_t       *cursor;
	PHONGO_STRUCT_ZVAL     manager;
	mongoc_client_t       *client;
	int                    server_id;
	php_phongo_bson_state  visitor_data;
	int                    got_iterator;
	long                   current;
	char                  *database;
	char                  *collection;
	PHONGO_STRUCT_ZVAL     query;
	PHONGO_STRUCT_ZVAL     command;
	PHONGO_STRUCT_ZVAL     read_preference;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_cursor_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	uint64_t id;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_cursorid_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	mongoc_client_t    *client;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_manager_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	bson_t                *filter;
	bson_t                *opts;
	mongoc_read_concern_t *read_concern;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_query_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	mongoc_read_concern_t *read_concern;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_readconcern_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	mongoc_read_prefs_t *read_preference;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_readpreference_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	PHONGO_STRUCT_ZVAL  manager;
	mongoc_client_t    *client;
	int                 server_id;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_server_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	mongoc_write_concern_t *write_concern;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_writeconcern_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	int                 code;
	char               *message;
	PHONGO_STRUCT_ZVAL  info;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_writeconcernerror_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	int                 code;
	char               *message;
	PHONGO_STRUCT_ZVAL  info;
	uint32_t            index;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_writeerror_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	mongoc_write_concern_t *write_concern;
	bson_t                 *reply;
	PHONGO_STRUCT_ZVAL      manager;
	mongoc_client_t        *client;
	int                     server_id;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_writeresult_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	char    *data;
	int      data_len;
	uint8_t  type;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_binary_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	bool              initialized;
	bson_decimal128_t decimal;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_decimal128_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	char   *code;
	size_t  code_len;
	bson_t *scope;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_javascript_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	PHONGO_ZEND_OBJECT_POST
} php_phongo_maxkey_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	PHONGO_ZEND_OBJECT_POST
} php_phongo_minkey_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	bool initialized;
	char oid[25];
	PHONGO_ZEND_OBJECT_POST
} php_phongo_objectid_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	char *pattern;
	int   pattern_len;
	char *flags;
	int   flags_len;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_regex_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	bool     initialized;
	uint32_t increment;
	uint32_t timestamp;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_timestamp_t;

typedef struct {
	PHONGO_ZEND_OBJECT_PRE
	bool    initialized;
	int64_t milliseconds;
	PHONGO_ZEND_OBJECT_POST
} php_phongo_utcdatetime_t;

#undef PHONGO_ZEND_OBJECT_PRE
#undef PHONGO_ZEND_OBJECT_POST
#undef PHONGO_STRUCT_ZVAL

#endif /* PHONGO_STRUCTS */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
