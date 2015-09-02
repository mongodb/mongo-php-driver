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
	bson_t                  *bson;
	zend_object              std;
} php_phongo_command_t;

typedef struct {
	mongoc_cursor_t         *cursor;
	mongoc_client_t         *client;
	int                      server_id;
	php_phongo_bson_state    visitor_data;
	int                      got_iterator;
	zend_object              std;
} php_phongo_cursor_t;

typedef struct {
	uint64_t                 id;
	zend_object              std;
} php_phongo_cursorid_t;

typedef struct {
	mongoc_client_t         *client;
	zend_object              std;
} php_phongo_manager_t;

typedef struct {
	bson_t                  *query;
	bson_t                  *selector;
	mongoc_query_flags_t     flags;
	uint32_t                 skip;
	uint32_t                 limit;
	uint32_t                 batch_size;
	zend_object              std;
} php_phongo_query_t;

typedef struct {
	mongoc_read_concern_t   *read_concern;
	zend_object              std;
} php_phongo_readconcern_t;

typedef struct {
	mongoc_read_prefs_t     *read_preference;
	zend_object              std;
} php_phongo_readpreference_t;

typedef struct {
	mongoc_client_t         *client;
	int                      server_id;
	zend_object              std;
} php_phongo_server_t;

typedef struct {
	mongoc_bulk_operation_t *bulk;
	zend_object              std;
} php_phongo_bulkwrite_t;

typedef struct {
	mongoc_write_concern_t  *write_concern;
	zend_object              std;
} php_phongo_writeconcern_t;

typedef struct {
	int                      code;
	char                    *message;
	zval                     info;
	zend_object              std;
} php_phongo_writeconcernerror_t;

typedef struct {
	int                      code;
	char                    *message;
	zval                     info;
	uint32_t                 index;
	zend_object              std;
} php_phongo_writeerror_t;

typedef struct {
	mongoc_write_concern_t  *write_concern;
	mongoc_write_result_t    write_result;
	mongoc_client_t         *client;
	int                      server_id;
	zend_object              std;
} php_phongo_writeresult_t;

typedef struct {
	char                    *data;
	int                      data_len;
	int                      type;
	zend_object              std;
} php_phongo_binary_t;
typedef struct {
	zend_object              std;
} php_phongo_int32_t;
typedef struct {
	zend_object              std;
} php_phongo_int64_t;
typedef struct {
	char                    *javascript;
	size_t                   javascript_len;
	bson_t                  *document;
	zend_object              std;
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
	char                     oid[25];
	zend_object              std;
} php_phongo_objectid_t;
typedef struct {
	char                    *pattern;
	int                      pattern_len;
	char                    *flags;
	int                      flags_len;
	zend_object              std;
} php_phongo_regex_t;
typedef struct {
	int32_t                  increment;
	int32_t                  timestamp;
	zend_object              std;
} php_phongo_timestamp_t;
typedef struct {
	int64_t                  milliseconds;
	zend_object              std;
} php_phongo_utcdatetime_t;

static inline php_phongo_command_t* php_command_fetch_object(zend_object *obj) {
    return (php_phongo_command_t *)((char *)obj - XtOffsetOf(php_phongo_command_t, std));
}
static inline php_phongo_cursor_t* php_cursor_fetch_object(zend_object *obj) {
    return (php_phongo_cursor_t *)((char *)obj - XtOffsetOf(php_phongo_cursor_t, std));
}
static inline php_phongo_cursorid_t* php_cursorid_fetch_object(zend_object *obj) {
    return (php_phongo_cursorid_t *)((char *)obj - XtOffsetOf(php_phongo_cursorid_t, std));
}
static inline php_phongo_manager_t* php_manager_fetch_object(zend_object *obj) {
    return (php_phongo_manager_t *)((char *)obj - XtOffsetOf(php_phongo_manager_t, std));
}
static inline php_phongo_query_t* php_query_fetch_object(zend_object *obj) {
    return (php_phongo_query_t *)((char *)obj - XtOffsetOf(php_phongo_query_t, std));
}
static inline php_phongo_readpreference_t* php_readpreference_fetch_object(zend_object *obj) {
    return (php_phongo_readpreference_t *)((char *)obj - XtOffsetOf(php_phongo_readpreference_t, std));
}
static inline php_phongo_server_t* php_server_fetch_object(zend_object *obj) {
    return (php_phongo_server_t *)((char *)obj - XtOffsetOf(php_phongo_server_t, std));
}
static inline php_phongo_bulkwrite_t* php_bulkwrite_fetch_object(zend_object *obj) {
    return (php_phongo_bulkwrite_t *)((char *)obj - XtOffsetOf(php_phongo_bulkwrite_t, std));
}
static inline php_phongo_writeconcern_t* php_writeconcern_fetch_object(zend_object *obj) {
    return (php_phongo_writeconcern_t *)((char *)obj - XtOffsetOf(php_phongo_writeconcern_t, std));
}
static inline php_phongo_writeconcernerror_t* php_writeconcernerror_fetch_object(zend_object *obj) {
    return (php_phongo_writeconcernerror_t *)((char *)obj - XtOffsetOf(php_phongo_writeconcernerror_t, std));
}
static inline php_phongo_writeerror_t* php_writeerror_fetch_object(zend_object *obj) {
    return (php_phongo_writeerror_t *)((char *)obj - XtOffsetOf(php_phongo_writeerror_t, std));
}
static inline php_phongo_writeresult_t* php_writeresult_fetch_object(zend_object *obj) {
    return (php_phongo_writeresult_t *)((char *)obj - XtOffsetOf(php_phongo_writeresult_t, std));
}
static inline php_phongo_binary_t* php_binary_fetch_object(zend_object *obj) {
    return (php_phongo_binary_t *)((char *)obj - XtOffsetOf(php_phongo_binary_t, std));
}
static inline php_phongo_int32_t* php_int32_fetch_object(zend_object *obj) {
    return (php_phongo_int32_t *)((char *)obj - XtOffsetOf(php_phongo_int32_t, std));
}
static inline php_phongo_int64_t* php_int64_fetch_object(zend_object *obj) {
    return (php_phongo_int64_t *)((char *)obj - XtOffsetOf(php_phongo_int64_t, std));
}
static inline php_phongo_javascript_t* php_javascript_fetch_object(zend_object *obj) {
    return (php_phongo_javascript_t *)((char *)obj - XtOffsetOf(php_phongo_javascript_t, std));
}
static inline php_phongo_log_t* php_log_fetch_object(zend_object *obj) {
    return (php_phongo_log_t *)((char *)obj - XtOffsetOf(php_phongo_log_t, std));
}
static inline php_phongo_maxkey_t* php_maxkey_fetch_object(zend_object *obj) {
    return (php_phongo_maxkey_t *)((char *)obj - XtOffsetOf(php_phongo_maxkey_t, std));
}
static inline php_phongo_minkey_t* php_minkey_fetch_object(zend_object *obj) {
    return (php_phongo_minkey_t *)((char *)obj - XtOffsetOf(php_phongo_minkey_t, std));
}
static inline php_phongo_objectid_t* php_objectid_fetch_object(zend_object *obj) {
    return (php_phongo_objectid_t *)((char *)obj - XtOffsetOf(php_phongo_objectid_t, std));
}
static inline php_phongo_regex_t* php_regex_fetch_object(zend_object *obj) {
    return (php_phongo_regex_t *)((char *)obj - XtOffsetOf(php_phongo_regex_t, std));
}
static inline php_phongo_timestamp_t* php_timestamp_fetch_object(zend_object *obj) {
    return (php_phongo_timestamp_t *)((char *)obj - XtOffsetOf(php_phongo_timestamp_t, std));
}
static inline php_phongo_utcdatetime_t* php_utcdatetime_fetch_object(zend_object *obj) {
    return (php_phongo_utcdatetime_t *)((char *)obj - XtOffsetOf(php_phongo_utcdatetime_t, std));
}

#endif /* PHONGO_STRUCTS */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

