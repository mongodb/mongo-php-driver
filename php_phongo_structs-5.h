/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2015 MongoDB, Inc.                                              |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2015 MongoDB, Inc.                                          |
  +---------------------------------------------------------------------------+
*/


#ifndef PHONGO_STRUCTS_H
#define PHONGO_STRUCTS_H

/* PHP Core stuff */
#include <php.h>
#include <mongoc-bulk-operation-private.h>

/* Our stuffz */
#include "php_bson.h"


typedef struct {
	zend_object              std;
	bson_t                  *bson;
} php_phongo_command_t;

typedef struct {
	zend_object              std;
	mongoc_cursor_t         *cursor;
	mongoc_client_t         *client;
	int                      server_id;
	php_phongo_bson_state    visitor_data;
	int                      got_iterator;
} php_phongo_cursor_t;

typedef struct {
	zend_object              std;
	uint64_t                 id;
} php_phongo_cursorid_t;

typedef struct {
	zend_object              std;
	mongoc_client_t         *client;
} php_phongo_manager_t;

typedef struct {
	zend_object              std;
	bson_t                  *query;
	bson_t                  *selector;
	mongoc_query_flags_t     flags;
	uint32_t                 skip;
	uint32_t                 limit;
	uint32_t                 batch_size;
	mongoc_read_concern_t   *read_concern;
} php_phongo_query_t;

typedef struct {
	zend_object              std;
	mongoc_read_concern_t   *read_concern;
} php_phongo_readconcern_t;

typedef struct {
	zend_object              std;
	mongoc_read_prefs_t     *read_preference;
} php_phongo_readpreference_t;

typedef struct {
	zend_object              std;
	mongoc_client_t         *client;
	int                      server_id;
} php_phongo_server_t;

typedef struct {
	zend_object              std;
	mongoc_bulk_operation_t *bulk;
} php_phongo_bulkwrite_t;

typedef struct {
	zend_object              std;
	mongoc_write_concern_t  *write_concern;
} php_phongo_writeconcern_t;

typedef struct {
	zend_object              std;
	int                      code;
	char                    *message;
	zval                    *info;
} php_phongo_writeconcernerror_t;

typedef struct {
	zend_object              std;
	int                      code;
	char                    *message;
	zval                    *info;
	uint32_t                 index;
} php_phongo_writeerror_t;

typedef struct {
	zend_object              std;
	mongoc_write_concern_t  *write_concern;
	mongoc_write_result_t    write_result;
	mongoc_client_t         *client;
	int                      server_id;
} php_phongo_writeresult_t;

typedef struct {
	zend_object              std;
	char                    *data;
	int                      data_len;
	int                      type;
} php_phongo_binary_t;
typedef struct {
	zend_object              std;
} php_phongo_int32_t;
typedef struct {
	zend_object              std;
} php_phongo_int64_t;
typedef struct {
	zend_object              std;
	char                    *javascript;
	size_t                   javascript_len;
	bson_t                  *document;
} php_phongo_javascript_t;
typedef struct {
	zend_object              std;
} php_phongo_log_t;
typedef struct {
	zend_object              std;
} php_phongo_maxkey_t;
typedef struct {
	zend_object              std;
} php_phongo_minkey_t;
typedef struct {
	zend_object              std;
	char                     oid[25];
} php_phongo_objectid_t;
typedef struct {
	zend_object              std;
	char                    *pattern;
	int                      pattern_len;
	char                    *flags;
	int                      flags_len;
} php_phongo_regex_t;
typedef struct {
	zend_object              std;
	uint32_t                 increment;
	uint32_t                 timestamp;
} php_phongo_timestamp_t;
typedef struct {
	zend_object              std;
	int64_t                  milliseconds;
} php_phongo_utcdatetime_t;

#endif /* PHONGO_STRUCTS */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
