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

#ifndef PHONGO_SERVERDESCRIPTION_H
#define PHONGO_SERVERDESCRIPTION_H

#include "mongoc/mongoc.h"

#include <php.h>

/* Note: these constants are derived from mongoc_server_description_type, since
 * since mongoc_server_description_type_t is private. */
#define PHONGO_SERVER_TYPE_UNKNOWN "Unknown"
#define PHONGO_SERVER_TYPE_STANDALONE "Standalone"
#define PHONGO_SERVER_TYPE_MONGOS "Mongos"
#define PHONGO_SERVER_TYPE_POSSIBLE_PRIMARY "PossiblePrimary"
#define PHONGO_SERVER_TYPE_RS_PRIMARY "RSPrimary"
#define PHONGO_SERVER_TYPE_RS_SECONDARY "RSSecondary"
#define PHONGO_SERVER_TYPE_RS_ARBITER "RSArbiter"
#define PHONGO_SERVER_TYPE_RS_OTHER "RSOther"
#define PHONGO_SERVER_TYPE_RS_GHOST "RSGhost"
#define PHONGO_SERVER_TYPE_LOAD_BALANCER "LoadBalancer"

/* This enum is necessary since mongoc_server_description_type_t is private and
 * we need to translate strings returned by mongoc_server_description_type() to
 * Server integer constants. */
typedef enum {
	PHONGO_SERVER_UNKNOWN           = 0,
	PHONGO_SERVER_STANDALONE        = 1,
	PHONGO_SERVER_MONGOS            = 2,
	PHONGO_SERVER_POSSIBLE_PRIMARY  = 3,
	PHONGO_SERVER_RS_PRIMARY        = 4,
	PHONGO_SERVER_RS_SECONDARY      = 5,
	PHONGO_SERVER_RS_ARBITER        = 6,
	PHONGO_SERVER_RS_OTHER          = 7,
	PHONGO_SERVER_RS_GHOST          = 8,
	PHONGO_SERVER_LOAD_BALANCER     = 9,
	PHONGO_SERVER_DESCRIPTION_TYPES = 10,
} php_phongo_server_description_type_t;

typedef struct {
	php_phongo_server_description_type_t type;
	const char*                          name;
} php_phongo_server_description_type_map_t;

extern php_phongo_server_description_type_map_t php_phongo_server_description_type_map[];

void phongo_serverdescription_init_ex(zval* return_value, mongoc_server_description_t* sd, bool copy);
#define phongo_serverdescription_init(r, sd) phongo_serverdescription_init_ex((r), (sd), true)

php_phongo_server_description_type_t php_phongo_server_description_type(mongoc_server_description_t* sd);

#endif /* PHONGO_SERVERDESCRIPTION_H */
