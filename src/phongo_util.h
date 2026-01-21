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

#ifndef PHONGO_UTIL_H
#define PHONGO_UTIL_H

#include "bson/bson.h"

const char* php_phongo_bson_type_to_string(bson_type_t type);

bool php_phongo_parse_int64(int64_t* retval, const char* data, size_t data_len);

bool phongo_split_namespace(const char* namespace, char** dbname, char** cname);

#endif /* PHONGO_UTIL_H */
