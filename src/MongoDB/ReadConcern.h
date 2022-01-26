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

#ifndef PHONGO_READCONCERN_H
#define PHONGO_READCONCERN_H

#include "mongoc/mongoc.h"

#include <php.h>

void phongo_readconcern_init(zval* return_value, const mongoc_read_concern_t* read_concern);

const mongoc_read_concern_t* phongo_read_concern_from_zval(zval* zread_concern);

void php_phongo_read_concern_to_zval(zval* retval, const mongoc_read_concern_t* read_concern);

#endif /* PHONGO_READCONCERN_H */
