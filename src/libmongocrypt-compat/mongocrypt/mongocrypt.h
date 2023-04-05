/*
 * Copyright 2020-present MongoDB, Inc.
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

#ifndef PHONGO_COMPAT_MONGOCRYPT_H
#define PHONGO_COMPAT_MONGOCRYPT_H

/* This header forwards includes for "mongocrypt/mongocrypt.h" to the desired
 * header in the bundled libmongocrypt submodule. This is used when compiling
 * bundled versions of libmongoc and libmongocrypt and is necessary because
 * libmongocrypt's source directory is not named "mongocrypt/".
 *
 * This path is relative to the PHPC project directory to avoid ambiguity. */
#include "src/libmongocrypt/src/mongocrypt.h"

#endif /* PHONGO_COMPAT_MONGOCRYPT_H */
