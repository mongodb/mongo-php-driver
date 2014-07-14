/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2014 Hannes Magnusson                                  |
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
#include <bson.h>
#include <mongoc.h>
#define MONGOC_INSIDE
#include "src/libmongoc/src/mongoc/mongoc-stream-private.h"
#undef MONGOC_INSIDE

#define phpext_phongo_ptr &phongo_module_entry
extern zend_module_entry phongo_module_entry;

#define PHONGO_VERSION_S "3.0.0"
#define PHONGO_VERSION   PHONGO_VERSION_S

#ifdef PHP_WIN32
#	define PHONGO_API __declspec(dllexport)
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

ZEND_BEGIN_MODULE_GLOBALS(phongo)
	char *debug_log;
	//bson_mem_vtable_t bsonMemVTable;
ZEND_END_MODULE_GLOBALS(phongo)

#ifdef ZTS
#	define PHONGO_G(v) TSRMG(phongo_globals_id, zend_phongo_globals *, v)
#else
#	define PHONGO_G(v) (phongo_globals.v)
#endif

#include "php_phongo_classes.h"


typedef enum {
	PHONGO_ERROR_INVALID_ARGUMENT = 1,
	PHONGO_ERROR_RUNETIME         = 2,
	PHONGO_ERROR_MONGOC_FAILED    = 3
} php_phongo_error_domain_t;

typedef struct
{
	void (*writer)(mongoc_stream_t *stream, int32_t timeout_msec, ssize_t sent, size_t iovcnt);
} php_phongo_stream_logger;

typedef struct
{
	mongoc_stream_t     vtable;
	php_stream         *stream;
	const bson_t       *uri_options;
	const mongoc_host_list_t *host;
	php_phongo_stream_logger log;
#if ZTS
	void ***tsrm_ls;
#endif
} php_phongo_stream_socket;


PHONGO_API zend_class_entry* phongo_exception_from_mongoc_domain(uint32_t /* mongoc_error_domain_t */ domain, uint32_t /* mongoc_error_code_t */ code);
PHONGO_API zend_class_entry* phongo_exception_from_phongo_domain(php_phongo_error_domain_t domain);
PHONGO_API void phongo_throw_exception(php_phongo_error_domain_t domain, const char *message TSRMLS_DC);

PHONGO_API zend_object_handlers *phongo_get_std_object_handlers(void);

int                      phongo_execute_single_insert(mongoc_client_t *client, char *namespace, bson_t *doc, zval *return_value, int return_value_used TSRMLS_DC);
int                      phongo_execute_single_insert(mongoc_client_t *client, char *namespace, bson_t *doc, zval *return_value, int return_value_used TSRMLS_DC);
int                      phongo_execute_single_update(mongoc_client_t *client, char *namespace, bson_t *query, bson_t *update, bool upsert, zval *return_value, int return_value_used TSRMLS_DC);
int                      phongo_execute_single_delete(mongoc_client_t *client, char *namespace, bson_t *query, mongoc_delete_flags_t flags, zval *return_value, int return_value_used TSRMLS_DC);
bool                     phongo_execute_write(mongoc_client_t *client, mongoc_bulk_operation_t *batch, int server_id, char *namespace, zval *return_value, int return_value_used TSRMLS_DC);
int                      phongo_execute_command(mongoc_client_t *client, char *db, bson_t *command, mongoc_read_prefs_t *read_preference, zval *return_value, int return_value_used TSRMLS_DC);
int                      phongo_execute_query(mongoc_client_t *client, char *namespace, php_phongo_query_t *query, mongoc_read_prefs_t *read_preference, zval *return_value, int return_value_used TSRMLS_DC);
mongoc_stream_t*         phongo_stream_initiator(const mongoc_uri_t *uri, const mongoc_host_list_t *host, void *user_data, bson_error_t *error);
zend_object_iterator*    phongo_result_get_iterator(zend_class_entry *ce, zval *object, int by_ref TSRMLS_DC);
mongoc_bulk_operation_t* phongo_writebatch_init(zend_bool ordered);
mongoc_read_prefs_t*     phongo_read_preference_from_zval(zval *object TSRMLS_DC);

php_phongo_query_t*      php_phongo_query_init(php_phongo_query_t *query, zval *zquery, zval *selector, int flags, int skip, int limit TSRMLS_DC);

void phongo_server_init(zval *return_value, int hint, mongoc_host_list_t *host TSRMLS_DC);

php_phongo_query_t* phongo_query_from_zval(zval *zquery TSRMLS_DC);



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
