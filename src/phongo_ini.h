/*
 * Copyright 2021-present MongoDB, Inc.
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

#ifndef PHONGO_INI_H
#define PHONGO_INI_H

#include <php.h>
#include <stdio.h>

void phongo_log_disable(FILE* stream);
void phongo_display_ini_entries(ZEND_MODULE_INFO_FUNC_ARGS);
void phongo_register_ini_entries(INIT_FUNC_ARGS);
void phongo_unregister_ini_entries(SHUTDOWN_FUNC_ARGS);

#endif /* PHONGO_INI_H */
