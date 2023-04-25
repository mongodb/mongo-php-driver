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

#ifndef PHONGO_BSON_ENCODE_H
#define PHONGO_BSON_ENCODE_H

#include "bson/bson.h"

#include <php.h>

typedef enum {
	PHONGO_BSON_NONE      = 0x00,
	PHONGO_BSON_ADD_ID    = 0x01,
	PHONGO_BSON_RETURN_ID = 0x02
} php_phongo_bson_flags_t;

void php_phongo_zval_to_bson(zval* data, php_phongo_bson_flags_t flags, bson_t* bson, bson_t** bson_out);
bool phongo_zval_to_bson_value(zval* data, bson_value_t* value);
void php_phongo_bson_append_zval(bson_t* bson, const char* key, long key_len, zval* value);

#endif /* PHONGO_BSON_ENCODE_H */
