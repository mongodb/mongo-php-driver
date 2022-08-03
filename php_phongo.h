/*
 * Copyright 2014-present MongoDB, Inc.
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

#ifndef PHONGO_H
#define PHONGO_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Include headers for getpid(), which is used by PHONGO_SET_CREATED_BY_PID.
 * This is based on PHP's ext/standard/pageinfo.c includes for getmypid. */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef PHP_WIN32
#include <process.h>
#endif

#include "phongo_version.h"
#include "phongo_compat.h"
#include "phongo_classes.h"
#include "phongo_structs.h"

/* Define a common logging domain for the extension. Individual files may
 * override the domain after including this header (e.g. phongo_bson.c). */
#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "PHONGO"

#define phpext_mongodb_ptr &mongodb_module_entry
extern zend_module_entry mongodb_module_entry;

ZEND_BEGIN_MODULE_GLOBALS(mongodb)
	char*      debug;
	FILE*      debug_fd;
	HashTable  persistent_clients;
	HashTable* request_clients;
	HashTable* subscribers;
	HashTable* managers;
ZEND_END_MODULE_GLOBALS(mongodb)

#define MONGODB_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(mongodb, v)
#if defined(ZTS) && defined(COMPILE_DL_MONGODB)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

zend_object_handlers* phongo_get_std_object_handlers(void);

#define PHONGO_CE_FINAL(ce)             \
	do {                                \
		ce->ce_flags |= ZEND_ACC_FINAL; \
	} while (0)

#if PHP_VERSION_ID < 80100
#define PHONGO_CE_DISABLE_SERIALIZATION(ce)            \
	do {                                               \
		ce->serialize   = zend_class_serialize_deny;   \
		ce->unserialize = zend_class_unserialize_deny; \
	} while (0)
#else
#define PHONGO_CE_DISABLE_SERIALIZATION(ce)        \
	do {                                           \
		ce->ce_flags |= ZEND_ACC_NOT_SERIALIZABLE; \
	} while (0)
#endif

#define PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, size) \
	do {                                                                  \
		if (is_temp) {                                                    \
			ALLOC_HASHTABLE(props);                                       \
			zend_hash_init((props), (size), NULL, ZVAL_PTR_DTOR, 0);      \
		} else if ((intern)->properties) {                                \
			(props) = (intern)->properties;                               \
		} else {                                                          \
			ALLOC_HASHTABLE(props);                                       \
			zend_hash_init((props), (size), NULL, ZVAL_PTR_DTOR, 0);      \
			(intern)->properties = (props);                               \
		}                                                                 \
	} while (0)

#define PHONGO_GET_PROPERTY_HASH_FREE_PROPS(is_temp, props) \
	do {                                                    \
		if (is_temp) {                                      \
			zend_hash_destroy((props));                     \
			FREE_HASHTABLE(props);                          \
		}                                                   \
	} while (0)

#define PHONGO_ZVAL_CLASS_OR_TYPE_NAME(zv) (Z_TYPE(zv) == IS_OBJECT ? ZSTR_VAL(Z_OBJCE(zv)->name) : zend_get_type_by_const(Z_TYPE(zv)))
#define PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(zvp) PHONGO_ZVAL_CLASS_OR_TYPE_NAME(*(zvp))

#define PHONGO_ZVAL_EXCEPTION_NAME(e) (ZSTR_VAL(e->ce->name))

#define PHONGO_SET_CREATED_BY_PID(intern)          \
	do {                                           \
		(intern)->created_by_pid = (int) getpid(); \
	} while (0)

#define PHONGO_DISABLED_CONSTRUCTOR(classname)                                         \
	static PHP_METHOD(classname, __construct)                                          \
	{                                                                                  \
		PHONGO_PARSE_PARAMETERS_NONE();                                                \
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Accessing private constructor"); \
	}

#define PHONGO_DISABLED_WAKEUP(classname)                                                             \
	static PHP_METHOD(classname, __wakeup)                                                            \
	{                                                                                                 \
		PHONGO_PARSE_PARAMETERS_NONE();                                                               \
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "MongoDB\\Driver objects cannot be serialized"); \
	}

/* Shared function entries for disabling constructors and unserialize() */
PHP_FUNCTION(MongoDB_disabled___construct);
PHP_FUNCTION(MongoDB_disabled___wakeup);

#endif /* PHONGO_H */
