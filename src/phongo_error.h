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

#ifndef PHONGO_ERROR_H
#define PHONGO_ERROR_H

#include "bson/bson.h"
#include "mongoc/mongoc.h"

typedef enum {
	PHONGO_ERROR_INVALID_ARGUMENT  = 1,
	PHONGO_ERROR_RUNTIME           = 2,
	PHONGO_ERROR_UNEXPECTED_VALUE  = 8,
	PHONGO_ERROR_MONGOC_FAILED     = 3,
	PHONGO_ERROR_CONNECTION_FAILED = 7,
	PHONGO_ERROR_LOGIC             = 9
} php_phongo_error_domain_t;

void              phongo_add_exception_prop(const char* prop, int prop_len, zval* value);
zend_class_entry* phongo_exception_from_mongoc_domain(mongoc_error_domain_t domain, mongoc_error_code_t code);
zend_class_entry* phongo_exception_from_phongo_domain(php_phongo_error_domain_t domain);
void              phongo_exception_add_error_labels(const bson_t* reply);
void              phongo_throw_exception(php_phongo_error_domain_t domain, const char* format, ...);
void              phongo_throw_exception_from_bson_error_t(bson_error_t* error);
void              phongo_throw_exception_from_bson_error_t_and_reply(bson_error_t* error, const bson_t* reply);

#ifndef MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION
void phongo_throw_exception_no_cse(php_phongo_error_domain_t domain, const char* message);
#endif /* MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION */

#endif /* PHONGO_ERROR_H */
