/*
 * Copyright 2017 MongoDB, Inc.
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

mongoc_transaction_opt_t* php_mongodb_session_parse_transaction_options(zval* txnOptions TSRMLS_DC);

#endif /* PHP_MONGODB_DRIVER_SESSION_H */
