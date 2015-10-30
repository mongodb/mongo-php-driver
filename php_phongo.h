/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2014-2015 Hannes Magnusson                             |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Hannes Magnusson <bjori@php.net>                             |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHONGO_H
#define PHONGO_H

/* External libs */
#include "bson.h"
#include "mongoc.h"
#include "mongoc-topology-description-private.h"

#define phpext_mongodb_ptr &mongodb_module_entry
extern zend_module_entry mongodb_module_entry;

/* FIXME: Its annoying to bump version. Move into phongo_version.h.in */
#define MONGODB_VERSION_S "1.0.0"
#define MONGODB_STABILITY_S "stable"
#define MONGODB_VERSION   MONGODB_VERSION_S

#ifdef PHP_WIN32
#	define PHONGO_API __declspec(dllexport)
#	include "ext/standard/flock_compat.h"
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHONGO_API __attribute__ ((visibility("default")))
#else
#	define PHONGO_API
#endif

#ifdef ZTS
#	include "TSRM.h"
#	define PHONGO_STREAM_CTX(x) x
#else
#	define PHONGO_STREAM_CTX(x) NULL
#endif

ZEND_BEGIN_MODULE_GLOBALS(mongodb)
	char *debug;
	FILE *debug_fd;
	bson_mem_vtable_t bsonMemVTable;
ZEND_END_MODULE_GLOBALS(mongodb)

#ifdef ZTS
#	define MONGODB_G(v) TSRMG(mongodb_globals_id, zend_mongodb_globals *, v)
#	define mglo mongodb_globals_id
#else
#	define MONGODB_G(v) (mongodb_globals.v)
#	define mglo mongodb_globals
#endif

#define PHONGO_WRITE_CONCERN_W_MAJORITY "majority"

#include "php_phongo_classes.h"

typedef enum {
	PHONGO_ERROR_INVALID_ARGUMENT    = 1,
	PHONGO_ERROR_RUNTIME             = 2,
	PHONGO_ERROR_UNEXPECTED_VALUE    = 8,
	PHONGO_ERROR_MONGOC_FAILED       = 3,
	PHONGO_ERROR_WRITE_FAILED        = 5,
	PHONGO_ERROR_CONNECTION_FAILED   = 7,
	PHONGO_ERROR_LOGIC               = 9
} php_phongo_error_domain_t;

typedef struct
{
	void (*writer)(mongoc_stream_t *stream, int32_t timeout_msec, ssize_t sent, size_t iovcnt);
} php_phongo_stream_logger;

typedef struct
{
	mongoc_stream_t           vtable;
	php_stream               *stream;
	const mongoc_uri_t       *uri;
	const mongoc_host_list_t *host;
#if ZTS
	void ***tsrm_ls;
#endif
} php_phongo_stream_socket;


PHONGO_API zend_class_entry* phongo_exception_from_mongoc_domain(uint32_t /* mongoc_error_domain_t */ domain, uint32_t /* mongoc_error_code_t */ code);
PHONGO_API zend_class_entry* phongo_exception_from_phongo_domain(php_phongo_error_domain_t domain);
PHONGO_API zval* phongo_throw_exception(php_phongo_error_domain_t domain TSRMLS_DC, const char *format, ...)
#ifndef PHP_WIN32
#ifdef ZTS
	 __attribute__ ((format(printf, 3, 4)))
#else
	 __attribute__ ((format(printf, 2, 3)))
#endif
#endif
;

PHONGO_API zend_object_handlers *phongo_get_std_object_handlers(void);

void phongo_server_init                              (zval *return_value, mongoc_client_t *client, int server_id TSRMLS_DC);
void                     phongo_readpreference_init  (zval *return_value, const mongoc_read_prefs_t *read_prefs TSRMLS_DC);
void                     phongo_writeconcern_init    (zval *return_value, const mongoc_write_concern_t *write_concern TSRMLS_DC);
bool                     phongo_query_init           (php_phongo_query_t *query, bson_t *filter, bson_t *options TSRMLS_DC);
mongoc_bulk_operation_t* phongo_bulkwrite_init       (zend_bool ordered);
bool                     phongo_execute_write        (mongoc_client_t *client, const char *namespace, mongoc_bulk_operation_t *bulk, const mongoc_write_concern_t *write_concern, int server_id, zval *return_value, int return_value_used TSRMLS_DC);
int                      phongo_execute_command      (mongoc_client_t *client, const char *db,        const bson_t *command,           const mongoc_read_prefs_t *read_preference, int server_id, zval *return_value, int return_value_used TSRMLS_DC);
int                      phongo_execute_query        (mongoc_client_t *client, const char *namespace, const php_phongo_query_t *query, const mongoc_read_prefs_t *read_preference, int server_id, zval *return_value, int return_value_used TSRMLS_DC);

mongoc_stream_t*         phongo_stream_initiator     (const mongoc_uri_t *uri, const mongoc_host_list_t *host, void *user_data, bson_error_t *error);
const mongoc_read_prefs_t*    phongo_read_preference_from_zval(zval *zread_preference TSRMLS_DC);
const mongoc_write_concern_t* phongo_write_concern_from_zval  (zval *zwrite_concern TSRMLS_DC);
const php_phongo_query_t*     phongo_query_from_zval          (zval *zquery TSRMLS_DC);

void php_phongo_server_to_zval(zval *retval, const mongoc_server_description_t *sd);
void php_phongo_read_preference_to_zval(zval *retval, const mongoc_read_prefs_t *read_prefs);
void php_phongo_write_concern_to_zval(zval *retval, const mongoc_write_concern_t *write_concern);
void php_phongo_cursor_to_zval(zval *retval, php_phongo_cursor_t *cursor);

bool php_phongo_apply_rp_options_to_client(mongoc_client_t *client, bson_t *options TSRMLS_DC);
bool php_phongo_apply_wc_options_to_client(mongoc_client_t *client, bson_t *options TSRMLS_DC);
mongoc_uri_t *php_phongo_make_uri(const char *uri_string, bson_t *options);
mongoc_client_t *php_phongo_make_mongo_client(const mongoc_uri_t *uri, zval *driverOptions TSRMLS_DC);
void php_phongo_objectid_new_from_oid(zval *object, const bson_oid_t *oid TSRMLS_DC);
void php_phongo_cursor_id_new_from_id(zval *object, int64_t cursorid TSRMLS_DC);
void php_phongo_new_utcdatetime_from_epoch(zval *object, int64_t msec_since_epoch TSRMLS_DC);
void php_phongo_new_datetime_from_utcdatetime(zval *object, int64_t milliseconds TSRMLS_DC);
void php_phongo_new_timestamp_from_increment_and_timestamp(zval *object, int32_t increment, int32_t timestamp TSRMLS_DC);
void php_phongo_new_javascript_from_javascript(int init, zval *object, const char *code, size_t code_len TSRMLS_DC);
void php_phongo_new_javascript_from_javascript_and_scope(int init, zval *object, const char *code, size_t code_len, const bson_t *scope TSRMLS_DC);
void php_phongo_new_binary_from_binary_and_type(zval *object, const char *data, size_t data_len, bson_subtype_t type TSRMLS_DC);
void php_phongo_new_regex_from_regex_and_options(zval *object, const char *pattern, const char *flags TSRMLS_DC);

zend_bool phongo_writeerror_init(zval *return_value, bson_t *bson TSRMLS_DC);
zend_bool phongo_writeconcernerror_init(zval *return_value, bson_t *bson TSRMLS_DC);

void php_phongo_cursor_free(php_phongo_cursor_t *cursor);
zend_object_iterator* php_phongo_cursor_get_iterator(zend_class_entry *ce, zval *object, int by_ref TSRMLS_DC);

#define PHONGO_CE_INIT(ce) do {                     \
	ce->ce_flags    |= ZEND_ACC_FINAL_CLASS;        \
	ce->serialize    = zend_class_serialize_deny;   \
	ce->unserialize  = zend_class_unserialize_deny; \
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
