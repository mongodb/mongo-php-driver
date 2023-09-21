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

#ifndef PHONGO_LOG_H
#define PHONGO_LOG_H

#include <stdio.h>

#include <php.h>

bool phongo_log_add_logger(zval* logger);
bool phongo_log_remove_logger(zval* logger);
void phongo_log_set_stream(FILE* stream);

#endif /* PHONGO_LOG_H */
