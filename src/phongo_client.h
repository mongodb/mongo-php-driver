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

#ifndef PHONGO_CLIENT_H
#define PHONGO_CLIENT_H

#include <php.h>

#include "phongo_classes.h"

const char* phongo_crypt_shared_version(void);

void phongo_manager_init(phongo_manager_t* manager, const char* uri_string, zval* options, zval* driverOptions);

void phongo_client_reset_once(phongo_manager_t* manager, int pid);
bool phongo_client_register(phongo_manager_t* manager);
bool phongo_client_unregister(phongo_manager_t* manager);

bool phongo_manager_register(phongo_manager_t* manager);
bool phongo_manager_unregister(phongo_manager_t* manager);

void phongo_pclient_destroy_ptr(zval* ptr);

#define PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, manager) \
	do {                                                    \
		int pid = (int) getpid();                           \
		if ((intern)->created_by_pid != pid) {              \
			phongo_client_reset_once((manager), pid);       \
		}                                                   \
	} while (0)

#endif /* PHONGO_CLIENT_H */
