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

#ifndef PHONGO_CLIENTENCRYPTION_H
#define PHONGO_CLIENTENCRYPTION_H

#include <php.h>

#include "php_phongo.h"

void phongo_clientencryption_init(php_phongo_clientencryption_t* ce_obj, zval* manager, zval* options);

#endif /* PHONGO_CLIENTENCRYPTION_H */
