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
	HashTable* loggers;
ZEND_END_MODULE_GLOBALS(mongodb)

#define MONGODB_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(mongodb, v)
#if defined(ZTS) && defined(COMPILE_DL_MONGODB)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

zend_object_handlers* phongo_get_std_object_handlers(void);

#define PHONGO_RETURN_PROPS(is_temp, props) \
	if (!(is_temp)) { \
		GC_ADDREF(props); \
	} \
	return props;

#define PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, size) \
	do {                                                                  \
		if (!(intern)->php_properties) {                                  \
			ALLOC_HASHTABLE((intern)->php_properties);                    \
			zend_hash_init((intern)->php_properties, 0, NULL, ZVAL_PTR_DTOR, 0); \
		}                                                                 \
		if (is_temp) {                                                    \
			(props) = zend_array_dup((intern)->php_properties);           \
		} else {                                                          \
			(props) = zend_array_dup((intern)->php_properties);           \
			if ((intern)->properties) {                                   \
				HashTable *__tmp = (intern)->properties; \
				(intern)->properties = NULL; \
				zend_hash_release(__tmp);                    \
			}                                                             \
			(intern)->properties = (props);                               \
		}                                                                 \
	} while (0)

#define PHONGO_GET_PROPERTY_HASH_FREE_PROPS(is_temp, props) \
	do {                                                    \
		if (is_temp) {                                      \
			zend_hash_release((props));                     \
		}                                                   \
	} while (0)

#define PHONGO_GET_PROPERTY_HANDLERS(_name, _intern_extractor) \
	static zval* php_phongo_##_name##_read_property(zend_object *zobj, zend_string *member, int type, void **cache_slot, zval *rv) \
	{ \
		HashTable *props = _intern_extractor(zobj)->php_properties; \
		if (!props) { \
			ALLOC_HASHTABLE(props); \
			zend_hash_init(props, 0, NULL, ZVAL_PTR_DTOR, 0); \
			_intern_extractor(zobj)->php_properties = props; \
		} \
		return zend_hash_find(props, member); \
	} \
	\
	static zval *php_phongo_##_name##_write_property(zend_object *zobj, zend_string *name, zval *value, void **cache_slot) \
	{ \
		Z_TRY_ADDREF_P(value); \
		HashTable *props = _intern_extractor(zobj)->php_properties; \
		if (!props) { \
			ALLOC_HASHTABLE(props); \
			zend_hash_init(props, 0, NULL, ZVAL_PTR_DTOR, 0); \
			_intern_extractor(zobj)->php_properties = props; \
		} \
		return zend_hash_add_new(props, name, value); \
	} \
	static int php_phongo_##_name##_has_property(zend_object *zobj, zend_string *name, int has_set_exists, void **cache_slot) \
	{ \
		HashTable *props = _intern_extractor(zobj)->php_properties; \
		if (!props) { \
			ALLOC_HASHTABLE(props); \
			zend_hash_init(props, 0, NULL, ZVAL_PTR_DTOR, 0); \
			_intern_extractor(zobj)->php_properties = props; \
		} \
		zval *value = zend_hash_find(props, name); \
		if (value) { \
			if (has_set_exists == ZEND_PROPERTY_NOT_EMPTY) { \
				return zend_is_true(value); \
			} \
			if (has_set_exists < ZEND_PROPERTY_NOT_EMPTY) { \
				ZEND_ASSERT(has_set_exists == ZEND_PROPERTY_ISSET); \
				ZVAL_DEREF(value); \
				return (Z_TYPE_P(value) != IS_NULL); \
			} \
			ZEND_ASSERT(has_set_exists == ZEND_PROPERTY_EXISTS); \
			return true; \
		} \
		return false; \
	} \
	static void php_phongo_##_name##_unset_property(zend_object *zobj, zend_string *name, void **cache_slot) \
	{ \
		HashTable *props = _intern_extractor(zobj)->php_properties; \
		if (!props) { \
			ALLOC_HASHTABLE(props); \
			zend_hash_init(props, 0, NULL, ZVAL_PTR_DTOR, 0); \
			_intern_extractor(zobj)->php_properties = props; \
		} \
		zend_hash_del(props, name); \
	} \
	\
	static zval *php_phongo_##_name##_get_property_ptr_ptr(zend_object *zobj, zend_string *name, int type, void **cache_slot) \
	{ \
		HashTable *props = _intern_extractor(zobj)->php_properties; \
		if (!props) { \
			ALLOC_HASHTABLE(props); \
			zend_hash_init(props, 0, NULL, ZVAL_PTR_DTOR, 0); \
			_intern_extractor(zobj)->php_properties = props; \
		} \
		 \
		zval *value = zend_hash_find(props, name); \
		if (value) { \
			return value; \
		} \
		return zend_hash_add(props, name, &EG(uninitialized_zval)); \
	}

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

#endif /* PHONGO_H */
