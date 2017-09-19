/*
 * Copyright 2014-2017 MongoDB, Inc.
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

/* External libs */
#include "bson.h"
#include "mongoc.h"

#define phpext_mongodb_ptr &mongodb_module_entry
extern zend_module_entry mongodb_module_entry;

/* FIXME: Its annoying to bump version. Move into phongo_version.h.in */
#define PHP_MONGODB_VERSION "1.2.11"
#define PHP_MONGODB_STABILITY "stable"

#ifdef PHP_WIN32
#	define PHONGO_API __declspec(dllexport)
#	include "ext/standard/flock_compat.h"
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHONGO_API __attribute__ ((visibility("default")))
#else
#	define PHONGO_API
#endif

/* Structure for persisted libmongoc clients. The PID is included to ensure that
 * processes do not destroy clients created by other processes (relevant for
 * forking). We avoid using pid_t for Windows compatibility. */
typedef struct {
	mongoc_client_t *client;
	int              pid;
} php_phongo_pclient_t;

ZEND_BEGIN_MODULE_GLOBALS(mongodb)
	char *debug;
	FILE *debug_fd;
	bson_mem_vtable_t bsonMemVTable;
	HashTable pclients;
ZEND_END_MODULE_GLOBALS(mongodb)

#if PHP_VERSION_ID >= 70000
# define MONGODB_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(mongodb, v)
# if defined(ZTS) && defined(COMPILE_DL_MONGODB)
ZEND_TSRMLS_CACHE_EXTERN()
# endif
#else
# ifdef ZTS
#  define MONGODB_G(v) TSRMG(mongodb_globals_id, zend_mongodb_globals *, v)
#  define mglo mongodb_globals_id
# else
#  define MONGODB_G(v) (mongodb_globals.v)
#  define mglo mongodb_globals
# endif
#endif

#define PHONGO_WRITE_CONCERN_W_MAJORITY "majority"

#include "php_phongo_classes.h"

/* This enum is necessary since mongoc_server_description_type_t is private and
 * we need to translate strings returned by mongoc_server_description_type() to
 * Server integer constants. */
typedef enum {
	PHONGO_SERVER_UNKNOWN           = 0,
	PHONGO_SERVER_STANDALONE        = 1,
	PHONGO_SERVER_MONGOS            = 2,
	PHONGO_SERVER_POSSIBLE_PRIMARY  = 3,
	PHONGO_SERVER_RS_PRIMARY        = 4,
	PHONGO_SERVER_RS_SECONDARY      = 5,
	PHONGO_SERVER_RS_ARBITER        = 6,
	PHONGO_SERVER_RS_OTHER          = 7,
	PHONGO_SERVER_RS_GHOST          = 8,
	PHONGO_SERVER_DESCRIPTION_TYPES = 9,
} php_phongo_server_description_type_t;

typedef struct {
	php_phongo_server_description_type_t  type;
	const char                           *name;
} php_phongo_server_description_type_map_t;

extern php_phongo_server_description_type_map_t php_phongo_server_description_type_map[];

typedef enum {
	PHONGO_ERROR_INVALID_ARGUMENT    = 1,
	PHONGO_ERROR_RUNTIME             = 2,
	PHONGO_ERROR_UNEXPECTED_VALUE    = 8,
	PHONGO_ERROR_MONGOC_FAILED       = 3,
	PHONGO_ERROR_WRITE_FAILED        = 5,
	PHONGO_ERROR_CONNECTION_FAILED   = 7,
	PHONGO_ERROR_LOGIC               = 9
} php_phongo_error_domain_t;

zend_class_entry* phongo_exception_from_mongoc_domain(uint32_t /* mongoc_error_domain_t */ domain, uint32_t /* mongoc_error_code_t */ code);
zend_class_entry* phongo_exception_from_phongo_domain(php_phongo_error_domain_t domain);
void phongo_throw_exception(php_phongo_error_domain_t domain TSRMLS_DC, const char *format, ...)
#if PHP_VERSION_ID < 70000
# ifndef PHP_WIN32
#  ifdef ZTS
	 __attribute__ ((format(printf, 3, 4)))
#  else
	 __attribute__ ((format(printf, 2, 3)))
#  endif
# endif
#endif
;
void phongo_throw_exception_from_bson_error_t(bson_error_t *error TSRMLS_DC);

zend_object_handlers *phongo_get_std_object_handlers(void);

void                     phongo_server_init          (zval *return_value, zval *manager, int server_id TSRMLS_DC);
void                     phongo_readconcern_init     (zval *return_value, const mongoc_read_concern_t *read_concern TSRMLS_DC);
void                     phongo_readpreference_init  (zval *return_value, const mongoc_read_prefs_t *read_prefs TSRMLS_DC);
void                     phongo_writeconcern_init    (zval *return_value, const mongoc_write_concern_t *write_concern TSRMLS_DC);
mongoc_bulk_operation_t* phongo_bulkwrite_init       (zend_bool ordered);
bool                     phongo_execute_write        (zval *manager, const char *namespace, php_phongo_bulkwrite_t  *bulk_write, const mongoc_write_concern_t *write_concern, int server_id, zval *return_value, int return_value_used TSRMLS_DC);
int                      phongo_execute_command      (zval *manager, const char *db, zval *zcommand, zval *zreadPreference, int server_id, zval *return_value, int return_value_used TSRMLS_DC);
int                      phongo_execute_query        (zval *manager, const char *namespace, zval *zquery, zval *zreadPreference, int server_id, zval *return_value, int return_value_used TSRMLS_DC);

const mongoc_read_concern_t*  phongo_read_concern_from_zval   (zval *zread_concern TSRMLS_DC);
const mongoc_read_prefs_t*    phongo_read_preference_from_zval(zval *zread_preference TSRMLS_DC);
const mongoc_write_concern_t* phongo_write_concern_from_zval  (zval *zwrite_concern TSRMLS_DC);

php_phongo_server_description_type_t php_phongo_server_description_type(mongoc_server_description_t *sd);

void php_phongo_read_preference_prep_tagsets(zval *tagSets TSRMLS_DC);
bool php_phongo_read_preference_tags_are_valid(const bson_t *tags);

void php_phongo_server_to_zval(zval *retval, mongoc_server_description_t *sd);
void php_phongo_read_concern_to_zval(zval *retval, const mongoc_read_concern_t *read_concern);
void php_phongo_read_preference_to_zval(zval *retval, const mongoc_read_prefs_t *read_prefs);
void php_phongo_write_concern_to_zval(zval *retval, const mongoc_write_concern_t *write_concern);
void php_phongo_cursor_to_zval(zval *retval, const mongoc_cursor_t *cursor);

void phongo_manager_init(php_phongo_manager_t *manager, const char *uri_string, zval *options, zval *driverOptions TSRMLS_DC);
void php_phongo_objectid_new_from_oid(zval *object, const bson_oid_t *oid TSRMLS_DC);
void php_phongo_cursor_id_new_from_id(zval *object, int64_t cursorid TSRMLS_DC);
void php_phongo_new_utcdatetime_from_epoch(zval *object, int64_t msec_since_epoch TSRMLS_DC);
void php_phongo_new_timestamp_from_increment_and_timestamp(zval *object, uint32_t increment, uint32_t timestamp TSRMLS_DC);
void php_phongo_new_javascript_from_javascript(int init, zval *object, const char *code, size_t code_len TSRMLS_DC);
void php_phongo_new_javascript_from_javascript_and_scope(int init, zval *object, const char *code, size_t code_len, const bson_t *scope TSRMLS_DC);
void php_phongo_new_binary_from_binary_and_type(zval *object, const char *data, size_t data_len, bson_subtype_t type TSRMLS_DC);
void php_phongo_new_decimal128(zval *object, const bson_decimal128_t *decimal TSRMLS_DC);
void php_phongo_new_regex_from_regex_and_options(zval *object, const char *pattern, const char *flags TSRMLS_DC);

zend_bool phongo_writeerror_init(zval *return_value, bson_t *bson TSRMLS_DC);
zend_bool phongo_writeconcernerror_init(zval *return_value, bson_t *bson TSRMLS_DC);

#if PHP_VERSION_ID >= 70000
#define PHONGO_CE_FINAL(ce) do {    \
	ce->ce_flags |= ZEND_ACC_FINAL; \
} while(0);
#else
#define PHONGO_CE_FINAL(ce) do {          \
	ce->ce_flags |= ZEND_ACC_FINAL_CLASS; \
} while(0);
#endif

#define PHONGO_CE_DISABLE_SERIALIZATION(ce) do {   \
    ce->serialize   = zend_class_serialize_deny;   \
    ce->unserialize = zend_class_unserialize_deny; \
} while(0);

#ifdef PHP_DEBUG
void _phongo_debug_bson(bson_t *bson);
#else
	#define _phongo_debug_bson(bson)
#endif

PHP_MINIT_FUNCTION(bson);

#endif /* PHONGO_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
