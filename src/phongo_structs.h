/*
 * Copyright 2015-present MongoDB, Inc.
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

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <php.h>

#include "phongo_bson.h"

typedef struct {
	mongoc_bulk_operation_t* bulk;
	size_t                   num_ops;
	bool                     ordered;
	int                      bypass;
	bson_t*                  let;
	bson_value_t*            comment;
	char*                    database;
	char*                    collection;
	bool                     executed;
	zval                     session;
	zend_object              std;
} phongo_bulkwrite_t;

typedef struct {
	mongoc_bulkwrite_t*     bw;
	int                     bypass;
	bson_value_t*           comment;
	bson_t*                 let;
	size_t                  num_ops;
	bool                    ordered;
	zval                    session;
	bool                    verbose;
	mongoc_write_concern_t* write_concern;
	zend_object             std;
} phongo_bulkwritecommand_t;

typedef struct {
	bool        is_acknowledged;
	int64_t     inserted_count;
	int64_t     matched_count;
	int64_t     modified_count;
	int64_t     upserted_count;
	int64_t     deleted_count;
	bson_t*     insert_results;
	bson_t*     update_results;
	bson_t*     delete_results;
	bson_t*     write_errors;
	bson_t*     write_concern_errors;
	bson_t*     error_reply;
	zval        manager;
	zend_object std;
} phongo_bulkwritecommandresult_t;

typedef struct {
	mongoc_client_encryption_t* client_encryption;
	zval                        key_vault_client_manager;
	char*                       key_vault_namespace;
	zend_object                 std;
} phongo_clientencryption_t;

typedef struct {
	bson_t*     bson;
	uint32_t    max_await_time_ms;
	uint32_t    batch_size;
	zend_object std;
} phongo_command_t;

typedef struct {
	mongoc_cursor_t*  cursor;
	zval              manager;
	int               created_by_pid;
	uint32_t          server_id;
	bool              advanced;
	phongo_bson_state visitor_data;
	long              current;
	char*             database;
	char*             collection;
	zval              query;
	zval              command;
	zval              read_preference;
	zval              session;
	zend_object       std;
} phongo_cursor_t;

typedef struct {
	mongoc_client_t* client;
	int              created_by_pid;
	char*            client_hash;
	size_t           client_hash_len;
	bool             use_persistent_client;
	zval             enc_fields_map;
	zval             key_vault_client_manager;
	HashTable*       subscribers;
	zend_object      std;
} phongo_manager_t;

typedef struct {
	bson_t*                filter;
	bson_t*                opts;
	mongoc_read_concern_t* read_concern;
	uint32_t               max_await_time_ms;
	zend_object            std;
} phongo_query_t;

typedef struct {
	mongoc_read_concern_t* read_concern;
	zend_object            std;
} phongo_readconcern_t;

typedef struct {
	mongoc_read_prefs_t* read_preference;
	zend_object          std;
} phongo_readpreference_t;

typedef struct {
	zval        manager;
	int         created_by_pid;
	uint32_t    server_id;
	zend_object std;
} phongo_server_t;

typedef struct {
	mongoc_server_api_t* server_api;
	HashTable*           properties;
	zend_object          std;
} phongo_serverapi_t;

typedef struct {
	mongoc_server_description_t* server_description;
	HashTable*                   properties;
	zend_object                  std;
} phongo_serverdescription_t;

typedef struct {
	mongoc_client_session_t* client_session;
	zval                     manager;
	int                      created_by_pid;
	zend_object              std;
} phongo_session_t;

typedef struct {
	mongoc_topology_description_t* topology_description;
	HashTable*                     properties;
	zend_object                    std;
} phongo_topologydescription_t;

typedef struct {
	mongoc_write_concern_t* write_concern;
	zend_object             std;
} phongo_writeconcern_t;

typedef struct {
	mongoc_write_concern_t* write_concern;
	bson_t*                 reply;
	zval                    manager;
	uint32_t                server_id;
	zend_object             std;
} phongo_writeresult_t;

typedef struct {
	char*       data;
	size_t      data_len;
	uint8_t     type;
	HashTable*  properties;
	zend_object std;
} phongo_binary_t;

typedef struct {
	bson_t*     bson;
	HashTable*  properties;
	zend_object std;
} phongo_packedarray_t;

typedef struct {
	bson_t*     bson;
	HashTable*  properties;
	zend_object std;
} phongo_document_t;

typedef struct {
	zval        bson;
	bson_iter_t iter;
	bool        valid;
	bool        is_array;
	size_t      key;
	zval        current;
	HashTable*  properties;
	zend_object std;
} phongo_iterator_t;

typedef struct {
	char*       ref;
	size_t      ref_len;
	char        id[25];
	HashTable*  properties;
	zend_object std;
} phongo_dbpointer_t;

typedef struct {
	bool              initialized;
	bson_decimal128_t decimal;
	HashTable*        properties;
	zend_object       std;
} phongo_decimal128_t;

typedef struct {
	bool        initialized;
	int64_t     integer;
	HashTable*  properties;
	zend_object std;
} phongo_int64_t;

typedef struct {
	char*       code;
	size_t      code_len;
	bson_t*     scope;
	HashTable*  properties;
	zend_object std;
} phongo_javascript_t;

typedef struct {
	bool        initialized;
	char        oid[25];
	HashTable*  properties;
	zend_object std;
} phongo_objectid_t;

typedef struct {
	char*       pattern;
	int         pattern_len;
	char*       flags;
	int         flags_len;
	HashTable*  properties;
	zend_object std;
} phongo_regex_t;

typedef struct {
	char*       symbol;
	size_t      symbol_len;
	HashTable*  properties;
	zend_object std;
} phongo_symbol_t;

typedef struct {
	bool        initialized;
	uint32_t    increment;
	uint32_t    timestamp;
	HashTable*  properties;
	zend_object std;
} phongo_timestamp_t;

typedef struct {
	zend_object std;
} phongo_undefined_t;

typedef struct {
	bool        initialized;
	int64_t     milliseconds;
	HashTable*  properties;
	zend_object std;
} phongo_utcdatetime_t;

typedef struct {
	bson_oid_t                   topology_id;
	mongoc_host_list_t           host;
	mongoc_server_description_t* new_server_description;
	mongoc_server_description_t* old_server_description;
	zend_object                  std;
} phongo_serverchangedevent_t;

typedef struct {
	bson_oid_t                     topology_id;
	mongoc_topology_description_t* new_topology_description;
	mongoc_topology_description_t* old_topology_description;
	zend_object                    std;
} phongo_topologychangedevent_t;

#endif /* PHONGO_STRUCTS */
