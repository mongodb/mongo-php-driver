/*
 * Copyright 2017-present MongoDB, Inc.
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

#ifndef PHP_MONGODB_DRIVER_SESSION_H
#define PHP_MONGODB_DRIVER_SESSION_H

#include "mongoc/mongoc.h"

#include <php.h>

#define PHONGO_TRANSACTION_NONE "none"
#define PHONGO_TRANSACTION_STARTING "starting"
#define PHONGO_TRANSACTION_IN_PROGRESS "in_progress"
#define PHONGO_TRANSACTION_COMMITTED "committed"
#define PHONGO_TRANSACTION_ABORTED "aborted"

mongoc_transaction_opt_t* php_mongodb_session_parse_transaction_options(zval* txnOptions);

void phongo_session_init(zval* return_value, zval* manager, mongoc_client_session_t* client_session);

#endif /* PHP_MONGODB_DRIVER_SESSION_H */
