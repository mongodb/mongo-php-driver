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
#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include "phongo_version.h"
#include "phongo_compat.h"
#include "php_phongo_classes.h"

#define phpext_mongodb_ptr &mongodb_module_entry
extern zend_module_entry mongodb_module_entry;

/* Structure for persisted libmongoc clients. The PID is included to ensure that
 * processes do not destroy clients created by other processes (relevant for
 * forking). We avoid using pid_t for Windows compatibility. */
typedef struct {
	mongoc_client_t* client;
	int              created_by_pid;
	int              last_reset_by_pid;
} php_phongo_pclient_t;

ZEND_BEGIN_MODULE_GLOBALS(mongodb)
	char*             debug;
	FILE*             debug_fd;
	bson_mem_vtable_t bsonMemVTable;
	HashTable         pclients;
	HashTable*        subscribers;
ZEND_END_MODULE_GLOBALS(mongodb)

#if PHP_VERSION_ID >= 70000
#define MONGODB_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(mongodb, v)
#if defined(ZTS) && defined(COMPILE_DL_MONGODB)
ZEND_TSRMLS_CACHE_EXTERN()
#endif
#else /* PHP_VERSION_ID >= 70000 */
#ifdef ZTS
#define MONGODB_G(v) TSRMG(mongodb_globals_id, zend_mongodb_globals*, v)
#define mglo mongodb_globals_id
#else /* ZTS */
#define MONGODB_G(v) (mongodb_globals.v)
#define mglo mongodb_globals
#endif /* ZTS */
#endif /* PHP_VERSION_ID >= 70000 */

#define PHONGO_WRITE_CONCERN_W_MAJORITY "majority"

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
	php_phongo_server_description_type_t type;
	const char*                          name;
} php_phongo_server_description_type_map_t;

extern php_phongo_server_description_type_map_t php_phongo_server_description_type_map[];

typedef enum {
	PHONGO_ERROR_INVALID_ARGUMENT  = 1,
	PHONGO_ERROR_RUNTIME           = 2,
	PHONGO_ERROR_UNEXPECTED_VALUE  = 8,
	PHONGO_ERROR_MONGOC_FAILED     = 3,
	PHONGO_ERROR_CONNECTION_FAILED = 7,
	PHONGO_ERROR_LOGIC             = 9
} php_phongo_error_domain_t;

/* This constant is used for determining if a server error for an exceeded query
 * or command should select ExecutionTimeoutException. */
#define PHONGO_SERVER_ERROR_EXCEEDED_TIME_LIMIT 50

zend_class_entry* phongo_exception_from_mongoc_domain(uint32_t /* mongoc_error_domain_t */ domain, uint32_t /* mongoc_error_code_t */ code);
zend_class_entry* phongo_exception_from_phongo_domain(php_phongo_error_domain_t domain);
void              phongo_throw_exception(php_phongo_error_domain_t domain TSRMLS_DC, const char* format, ...)
#if PHP_VERSION_ID < 70000
#ifndef PHP_WIN32
#ifdef ZTS
	__attribute__((format(printf, 3, 4)))
#else
	__attribute__((format(printf, 2, 3)))
#endif /* ZTS */
#endif /* PHP_WIN32 */
#endif /* PHP_VERSION_ID < 70000 */
	;
void phongo_throw_exception_from_bson_error_t(bson_error_t* error TSRMLS_DC);
void phongo_throw_exception_from_bson_error_t_and_reply(bson_error_t* error, const bson_t* reply TSRMLS_DC);

/* This enum is used for processing options in phongo_execute_parse_options and
 * selecting a libmongoc function to use in phongo_execute_command. The values
 * are important, as READ and WRITE are also used as a bit field to determine
 * whether readPreference, readConcern, and writeConcern options are parsed. */
typedef enum {
	PHONGO_OPTION_READ_CONCERN    = 0x01,
	PHONGO_OPTION_READ_PREFERENCE = 0x02,
	PHONGO_OPTION_WRITE_CONCERN   = 0x04,
	PHONGO_COMMAND_RAW            = 0x07,
	PHONGO_COMMAND_READ           = 0x03,
	PHONGO_COMMAND_WRITE          = 0x04,
	PHONGO_COMMAND_READ_WRITE     = 0x05,
} php_phongo_command_type_t;

zend_object_handlers* phongo_get_std_object_handlers(void);

void phongo_clientencryption_init(php_phongo_clientencryption_t* ce_obj, mongoc_client_t* client, zval* options TSRMLS_DC);
void phongo_server_init(zval* return_value, mongoc_client_t* client, uint32_t server_id TSRMLS_DC);
void phongo_session_init(zval* return_value, mongoc_client_session_t* client_session TSRMLS_DC);
void phongo_readconcern_init(zval* return_value, const mongoc_read_concern_t* read_concern TSRMLS_DC);
void phongo_readpreference_init(zval* return_value, const mongoc_read_prefs_t* read_prefs TSRMLS_DC);
void phongo_writeconcern_init(zval* return_value, const mongoc_write_concern_t* write_concern TSRMLS_DC);
bool phongo_execute_bulk_write(mongoc_client_t* client, const char* namespace, php_phongo_bulkwrite_t* bulk_write, zval* zwriteConcern, uint32_t server_id, zval* return_value, int return_value_used TSRMLS_DC);
bool phongo_execute_command(mongoc_client_t* client, php_phongo_command_type_t type, const char* db, zval* zcommand, zval* zreadPreference, uint32_t server_id, zval* return_value, int return_value_used TSRMLS_DC);
bool phongo_execute_query(mongoc_client_t* client, const char* namespace, zval* zquery, zval* zreadPreference, uint32_t server_id, zval* return_value, int return_value_used TSRMLS_DC);

bool phongo_cursor_advance_and_check_for_error(mongoc_cursor_t* cursor TSRMLS_DC);

const mongoc_read_concern_t*  phongo_read_concern_from_zval(zval* zread_concern TSRMLS_DC);
const mongoc_read_prefs_t*    phongo_read_preference_from_zval(zval* zread_preference TSRMLS_DC);
const mongoc_write_concern_t* phongo_write_concern_from_zval(zval* zwrite_concern TSRMLS_DC);

php_phongo_server_description_type_t php_phongo_server_description_type(mongoc_server_description_t* sd);

bool phongo_parse_read_preference(zval* options, zval** zreadPreference TSRMLS_DC);
bool phongo_parse_session(zval* options, mongoc_client_t* client, bson_t* mongoc_opts, zval** zsession TSRMLS_DC);

zval* php_phongo_prep_legacy_option(zval* options, const char* key, bool* allocated TSRMLS_DC);
void  php_phongo_prep_legacy_option_free(zval* options TSRMLS_DC);

void php_phongo_read_preference_prep_tagsets(zval* tagSets TSRMLS_DC);
bool php_phongo_read_preference_tags_are_valid(const bson_t* tags);

bool php_phongo_server_to_zval(zval* retval, mongoc_server_description_t* sd);
void php_phongo_read_concern_to_zval(zval* retval, const mongoc_read_concern_t* read_concern);
void php_phongo_write_concern_to_zval(zval* retval, const mongoc_write_concern_t* write_concern);
void php_phongo_cursor_to_zval(zval* retval, const mongoc_cursor_t* cursor);

void phongo_manager_init(php_phongo_manager_t* manager, const char* uri_string, zval* options, zval* driverOptions TSRMLS_DC);
int  php_phongo_set_monitoring_callbacks(mongoc_client_t* client);

bool php_phongo_parse_int64(int64_t* retval, const char* data, phongo_zpp_char_len data_len);

void phongo_clientencryption_create_datakey(php_phongo_clientencryption_t* clientencryption, zval* return_value, char* kms_provider, zval* options TSRMLS_DC);
void phongo_clientencryption_encrypt(php_phongo_clientencryption_t* clientencryption, zval* zvalue, zval* zciphertext, zval* options TSRMLS_DC);
void phongo_clientencryption_decrypt(php_phongo_clientencryption_t* clientencryption, zval* zciphertext, zval* zvalue TSRMLS_DC);

zend_bool phongo_writeerror_init(zval* return_value, bson_t* bson TSRMLS_DC);
zend_bool phongo_writeconcernerror_init(zval* return_value, bson_t* bson TSRMLS_DC);

void php_phongo_client_reset_once(mongoc_client_t* client, int pid);

#if PHP_VERSION_ID >= 70000
#define PHONGO_CE_FINAL(ce)             \
	do {                                \
		ce->ce_flags |= ZEND_ACC_FINAL; \
	} while (0);
#else
#define PHONGO_CE_FINAL(ce)                   \
	do {                                      \
		ce->ce_flags |= ZEND_ACC_FINAL_CLASS; \
	} while (0);
#endif

#define PHONGO_CE_DISABLE_SERIALIZATION(ce)            \
	do {                                               \
		ce->serialize   = zend_class_serialize_deny;   \
		ce->unserialize = zend_class_unserialize_deny; \
	} while (0);

#define PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_debug, intern, props, size) \
	do {                                                                   \
		if (is_debug) {                                                    \
			ALLOC_HASHTABLE(props);                                        \
			zend_hash_init((props), (size), NULL, ZVAL_PTR_DTOR, 0);       \
		} else if ((intern)->properties) {                                 \
			(props) = (intern)->properties;                                \
		} else {                                                           \
			ALLOC_HASHTABLE(props);                                        \
			zend_hash_init((props), (size), NULL, ZVAL_PTR_DTOR, 0);       \
			(intern)->properties = (props);                                \
		}                                                                  \
	} while (0);

#define PHONGO_GET_PROPERTY_HASH_FREE_PROPS(is_debug, props) \
	do {                                                     \
		if (is_debug) {                                      \
			zend_hash_destroy((props));                      \
			FREE_HASHTABLE(props);                           \
		}                                                    \
	} while (0);

#define PHONGO_ZVAL_CLASS_OR_TYPE_NAME(zv) (Z_TYPE(zv) == IS_OBJECT ? ZSTR_VAL(Z_OBJCE(zv)->name) : zend_get_type_by_const(Z_TYPE(zv)))
#define PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(zvp) PHONGO_ZVAL_CLASS_OR_TYPE_NAME(*(zvp))

#if PHP_VERSION_ID >= 70000
#define PHONGO_ZVAL_EXCEPTION_NAME(e) (ZSTR_VAL(e->ce->name))
#else
#define PHONGO_ZVAL_EXCEPTION_NAME(e) (ZSTR_VAL(Z_OBJCE_P(e)->name))
#endif

#define PHONGO_SET_CREATED_BY_PID(intern)          \
	do {                                           \
		(intern)->created_by_pid = (int) getpid(); \
	} while (0);

#define PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern)               \
	do {                                                         \
		int pid = (int) getpid();                                \
		if ((intern)->created_by_pid != pid) {                   \
			php_phongo_client_reset_once((intern)->client, pid); \
		}                                                        \
	} while (0);

#endif /* PHONGO_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
