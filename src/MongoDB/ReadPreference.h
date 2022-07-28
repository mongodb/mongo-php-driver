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

#ifndef PHONGO_READPREFERENCE_H
#define PHONGO_READPREFERENCE_H

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <php.h>

#define PHONGO_READ_PRIMARY "primary"
#define PHONGO_READ_PRIMARY_PREFERRED "primaryPreferred"
#define PHONGO_READ_SECONDARY "secondary"
#define PHONGO_READ_SECONDARY_PREFERRED "secondaryPreferred"
#define PHONGO_READ_NEAREST "nearest"

void phongo_readpreference_init(zval* return_value, const mongoc_read_prefs_t* read_prefs);

const mongoc_read_prefs_t* phongo_read_preference_from_zval(zval* zread_preference);

void php_phongo_read_preference_prep_tagsets(zval* tagSets);
bool php_phongo_read_preference_tags_are_valid(const bson_t* tags);

#endif /* PHONGO_READPREFERENCE_H */
