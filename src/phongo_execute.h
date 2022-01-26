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

#ifndef PHONGO_EXECUTE_H
#define PHONGO_EXECUTE_H

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <php.h>

/* This enum is used for processing options and selecting a libmongoc function
 * to use in phongo_execute_command. The values are important, as READ and WRITE
 * are also used as a bit field to determine whether readPreference,
 * readConcern, and writeConcern options are parsed. */
typedef enum {
	PHONGO_OPTION_READ_CONCERN    = 0x01,
	PHONGO_OPTION_READ_PREFERENCE = 0x02,
	PHONGO_OPTION_WRITE_CONCERN   = 0x04,
	PHONGO_COMMAND_RAW            = 0x07,
	PHONGO_COMMAND_READ           = 0x03,
	PHONGO_COMMAND_WRITE          = 0x04,
	PHONGO_COMMAND_READ_WRITE     = 0x05,
} php_phongo_command_type_t;

bool phongo_execute_bulk_write(zval* manager, const char* namespace, php_phongo_bulkwrite_t* bulk_write, zval* zwriteConcern, uint32_t server_id, zval* return_value);
bool phongo_execute_command(zval* manager, php_phongo_command_type_t type, const char* db, zval* zcommand, zval* zreadPreference, uint32_t server_id, zval* return_value);
bool phongo_execute_query(zval* manager, const char* namespace, zval* zquery, zval* zreadPreference, uint32_t server_id, zval* return_value);

bool phongo_parse_read_preference(zval* options, zval** zreadPreference);
bool phongo_parse_session(zval* options, mongoc_client_t* client, bson_t* mongoc_opts, zval** zsession);

#endif /* PHONGO_EXECUTE_H */
