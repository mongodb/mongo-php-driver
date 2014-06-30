/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2014 MongoDB, Inc.                                         |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2014, MongoDB, Inc.                                         |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* YCM */
#include <strings.h>
/* External libs */
#include <bson.h>
#include <mongoc.h>
#define MONGOC_INSIDE
#include <src/libmongoc/src/mongoc/mongoc-stream-private.h>
#undef MONGOC_INSIDE

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include "Zend/zend_interfaces.h"
#include "Zend/zend_exceptions.h"
#include "ext/spl/spl_iterators.h"
#include "ext/spl/spl_exceptions.h"
/* For formating timestamp in the log */
#include <ext/date/php_date.h>
/* Stream wrapper */
#include <main/php_streams.h>
#include <main/php_network.h>
/* Our Compatability header */
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"

ZEND_DECLARE_MODULE_GLOBALS(phongo)

/* {{{ phongo_std_object_handlers */
zend_object_handlers phongo_std_object_handlers;

PHONGO_API zend_object_handlers *phongo_get_std_object_handlers(void)
{
	return &phongo_std_object_handlers;
}
/* }}} */

/* {{{ Error reporting and logging */
zend_class_entry* phongo_exception_from_phongo_domain(php_phongo_error_domain_t domain)
{
	switch (domain) {
		case PHONGO_INVALID_ARGUMENT:
			return spl_ce_InvalidArgumentException;
		case PHONGO_RUNETIME_ERROR:
			return spl_ce_RuntimeException;
	}
}
zend_class_entry* phongo_exception_from_mongoc_domain(mongoc_error_domain_t domain)
{
	switch (domain) {
		case MONGOC_ERROR_STREAM:
			/* FIXME: We don't have the Exceptions mocked yet.. */
#if 0
			return phongo_ce_mongo_connection_exception;
#endif
		default:
			return spl_ce_RuntimeException;
	}
}
PHONGO_API void phongo_throw_exception(php_phongo_error_domain_t domain TSRMLS_DC, char *message)
{
	zend_throw_exception(phongo_exception_from_phongo_domain(domain) TSRMLS_CC, message, 0);
}
PHONGO_API void phongo_throw_exception_from_bson_error_t(bson_error_t *error TSRMLS_DC)
{
	zend_throw_exception(phongo_exception_from_mongoc_domain(error->domain) TSRMLS_CC, error->message, error->code);
}
static void php_phongo_log(mongoc_log_level_t log_level, const char *log_domain, const char *message, void *user_data)
{
	(void)log_domain;

	if (!PHONGO_G(debug_log)) {
		return;
	}
	if (strcasecmp(PHONGO_G(debug_log), "off") == 0) {
		return;
	}
	if (strcasecmp(PHONGO_G(debug_log), "0") == 0) {
		return;
	}

	switch(log_level) {
	case MONGOC_LOG_LEVEL_ERROR:
	case MONGOC_LOG_LEVEL_CRITICAL:
	case MONGOC_LOG_LEVEL_WARNING:
	case MONGOC_LOG_LEVEL_MESSAGE:
	case MONGOC_LOG_LEVEL_INFO:
	case MONGOC_LOG_LEVEL_DEBUG:
	case MONGOC_LOG_LEVEL_TRACE:
		{
			int fd = 0;
			char *dt = NULL;
			TSRMLS_FETCH_FROM_CTX(user_data);

			dt = php_format_date("Y-m-d\\TH:i:sP", strlen("Y-m-d\\TH:i:sP"), time(NULL), 0 TSRMLS_CC);
			/*char *dt = php_format_date((char *)"Y-m-d\\TH:i:sP", strlen("Y-m-d\\TH:i:sP"), time(NULL), 1 TSRMLS_CC);*/
			if (strcasecmp(PHONGO_G(debug_log), "stderr") == 0) {
				fprintf(stderr, "[%s - %8s] %s\n", dt, mongoc_log_level_str(log_level), message);
			} else if (strcasecmp(PHONGO_G(debug_log), "stdout") == 0) {
				php_printf("[%s - %8s] %s\n", dt, mongoc_log_level_str(log_level), message);
			} else if ((fd = VCWD_OPEN_MODE(PHONGO_G(debug_log), O_CREAT | O_APPEND | O_WRONLY, 0644)) != -1) {
				char *tmp;
				int len;

				len = spprintf(&tmp, 0, "[%s - %8s] %s\n", dt, mongoc_log_level_str(log_level), message);
#ifdef PHP_WIN32
				php_flock(fd, 2);
#endif
				write(fd, tmp, len);
				efree(tmp);
				close(fd);
			}
			efree(dt);
		} break;
	}
}
/* }}} */

/* {{{ CRUD */
mongoc_collection_t* phongo_get_collection_from_namespace(mongoc_client_t *client, char *namespace, int namespace_len)
{
	char *dbname = estrndup(namespace, strchr(namespace, '.') - namespace);
	char *collname = estrdup(namespace + (strchr(namespace, '.') - namespace) + 1);
	mongoc_collection_t *collection = mongoc_client_get_collection(client, dbname, collname);
	efree(dbname);
	efree(collname);
	return collection;
}
HashTable* phongo_batch_get_documents(zval *batch, zval *retval)
{
	int i;

	array_init(retval);

	for (i=0; i<5;i++) {
		zval *entry;

		MAKE_STD_ZVAL(entry);
		array_init(entry);
		add_assoc_stringl(entry, "MyKey", "MyValue", sizeof("MyValue")-1, 1);
		add_assoc_stringl(entry, "MyKey2", "MyValue2", sizeof("MyValue2")-1, 1);
		add_assoc_stringl(entry, "MyKey3", "MyValue3", sizeof("MyValue3")-1, 1);

		add_next_index_zval(retval, entry);
	}

	return Z_ARRVAL_P(retval);
}
int phongo_crud_insert(mongoc_client_t *client, mongoc_collection_t *collection, bson_t *doc, zval *return_value, int return_value_used TSRMLS_DC)
{
	bson_error_t error;
	bson_t reply;
	mongoc_bulk_operation_t *bulk = mongoc_collection_create_bulk_operation (collection, true, NULL);


	mongoc_bulk_operation_insert(bulk, doc);

	if (!mongoc_bulk_operation_execute(bulk, &reply, &error)) {
		phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
		return false;
	}


	if (!return_value_used) {
		return true;
	}

	bson_to_zval(bson_get_data(&reply), reply.len, return_value);
	return true;
}
int phongo_execute_query(mongoc_client_t *client, mongoc_collection_t *collection, bson_t *query, mongoc_cursor_t **out_cursor, zval *return_value, int return_value_used TSRMLS_DC)
{
	bson_error_t error;
    const bson_t *doc;



	*out_cursor = mongoc_collection_find (collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

    if (!mongoc_cursor_next(*out_cursor, &doc)) {
		mongoc_cursor_error(*out_cursor, &error);
		phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
		return false;
	}

	if (!return_value_used) {
		return true;
	}

	bson_to_zval(bson_get_data(doc), doc->len, return_value);
	return true;
}
/* Throws exception from bson_error_t */
int phongo_execute_write(mongoc_client_t *client, mongoc_collection_t *collection, zval *batch, zval *return_value, int return_value_used TSRMLS_DC)
{
	mongoc_bulk_operation_t *bulk = mongoc_collection_create_bulk_operation (collection, true, NULL);
	bson_error_t error;
	bson_t reply;

	HashPosition pointer;
	zval **entry;
	char *key;
	zend_uint index_key_len;
	ulong uindex;
	zval *retval;
	HashTable *hindex;
	
	MAKE_STD_ZVAL(retval);
	hindex = phongo_batch_get_documents(batch, retval);

	for (
			zend_hash_internal_pointer_reset_ex(hindex, &pointer);
			zend_hash_get_current_data_ex(hindex, (void**)&entry, &pointer) == SUCCESS;
			zend_hash_move_forward_ex(hindex, &pointer)
		) {
		bson_t *bson = bson_new();

		if (zend_hash_get_current_key_ex(hindex, &key, &index_key_len, &uindex, 0, &pointer) != HASH_KEY_IS_LONG) {
			continue;
		}

		php_phongo_bson_encode_array(bson, *entry TSRMLS_CC);
		mongoc_bulk_operation_insert(bulk, bson);
		bson_destroy(bson);
	}

	zval_ptr_dtor(&retval);
	if (!mongoc_bulk_operation_execute(bulk, &reply, &error)) {
		phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
		return false;
	}

	if (!return_value_used) {
		return true;
	}

	bson_to_zval(bson_get_data(&reply), reply.len, return_value);
	return true;
}
int phongo_execute_command(mongoc_client_t *client, mongoc_database_t *db, bson_t *command, zval *read_preference, zval *return_value, int return_value_used TSRMLS_DC)
{
	bson_error_t error;
	bson_t reply;

	if (!mongoc_database_command_simple(db, command, NULL, &reply, &error)) {
		phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
		return false;
	}

	if (!return_value_used) {
		return true;
	}

	bson_to_zval(bson_get_data(&reply), reply.len, return_value);
	return true;
}

/* }}} */

void phongo_stream_destroy(mongoc_stream_t *stream)
{
}
int phongo_stream_close(mongoc_stream_t *stream)
{
	return -1;
}
void php_phongo_set_timeout(php_phongo_stream_socket *base_stream, int32_t timeout_msec)
{
	if (timeout_msec) {
		struct timeval rtimeout = {0, 0};

		rtimeout.tv_sec = timeout_msec / 1000;
		rtimeout.tv_usec = (timeout_msec % 1000) * 1000;
		rtimeout.tv_sec = 1;

		php_stream_set_option(base_stream->stream, PHP_STREAM_OPTION_READ_TIMEOUT, 0, &rtimeout);
	}
}
ssize_t phongo_stream_writev(mongoc_stream_t *stream, mongoc_iovec_t *iov, size_t iovcnt, int32_t timeout_msec)
{
	size_t     i = 0;
	ssize_t sent = 0;

	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream;
	php_phongo_set_timeout(base_stream, timeout_msec);

	for (i = 0; i < iovcnt; i++) {
		sent += php_stream_write(base_stream->stream, iov[i].iov_base, iov[i].iov_len);
	}
	return sent;
}
ssize_t phongo_stream_readv(mongoc_stream_t *stream, mongoc_iovec_t *iov, size_t iovcnt, size_t min_bytes, int32_t timeout_msec)
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream;
	php_phongo_set_timeout(base_stream, timeout_msec);

	return php_stream_read(base_stream->stream, iov->iov_base, iov->iov_len);
}
int phongo_stream_setsockopt(mongoc_stream_t *stream, int level, int optname, void *optval, socklen_t optlen)
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream;
	int socket = ((php_netstream_data_t *)base_stream->stream->abstract)->socket;
	int retval = setsockopt (socket, level, optname, optval, optlen);
	return retval;
}
mongoc_stream_t* phongo_stream_get_base_stream(mongoc_stream_t *stream)
{
	return (mongoc_stream_t *) stream;
}
mongoc_stream_t* phongo_stream_initiator(const mongoc_uri_t *uri, const mongoc_host_list_t *host, void *user_data, bson_error_t *error)
{
	php_phongo_stream_socket *base_stream = NULL;
	php_stream *stream;
	char *errmsg;
	int errcode;
	char *dsn;
	int dsn_len;


	switch (host->family) {
#if defined(AF_INET6)
		case AF_INET6:
#endif
		case AF_INET:
			dsn_len = spprintf(&dsn, 0, "tcp://%s:%d", host->host, host->port);
			break;

		case AF_UNIX:
			dsn_len = spprintf(&dsn, 0, "unix://%s", host->host);
			break;

		default:
			bson_set_error (error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_INVALID_TYPE, "Invalid address family: 0x%02x", host->family);
			return NULL;
	}
	stream = php_stream_xport_create(dsn, dsn_len, 0, STREAM_XPORT_CLIENT | STREAM_XPORT_CONNECT, (char *)"persistent id", /*options->connectTimeoutMS*/0, (php_stream_context *)NULL, &errmsg, &errcode);
	efree(dsn);
	if (!stream) {
		bson_set_error (error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Failed connecting to '%s:%d': %s", host->host, host->port, errmsg);
		return NULL;
	}

	if (host->family != AF_UNIX) {
		int socket = ((php_netstream_data_t*)stream->abstract)->socket;
		int flag = 1;

		setsockopt(socket, IPPROTO_TCP,  TCP_NODELAY, (char *) &flag, sizeof(int));
	}

	base_stream = ecalloc(1, sizeof(php_phongo_stream_socket));
	base_stream->stream = stream;
	base_stream->vtable.type = 42;
	base_stream->vtable.close = phongo_stream_close;
	base_stream->vtable.destroy = phongo_stream_destroy;
	base_stream->vtable.readv = phongo_stream_readv;
	base_stream->vtable.writev = phongo_stream_writev;
	base_stream->vtable.setsockopt = phongo_stream_setsockopt;
	base_stream->vtable.get_base_stream = phongo_stream_get_base_stream;

	return (mongoc_stream_t *) base_stream;
}
/* {{{ M[INIT|SHUTDOWN] R[INIT|SHUTDOWN] G[INIT|SHUTDOWN] MINFO INI */

/* {{{ INI entries */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("phongo.debug_log", (char *)"stderr", PHP_INI_ALL, OnUpdateString, debug_log, zend_phongo_globals, phongo_globals)
PHP_INI_END()
/* }}} */

/* {{{ PHP_GINIT_FUNCTION */
PHP_GINIT_FUNCTION(phongo)
{
	phongo_globals->debug_log = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(phongo)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	void ***ctx = NULL;
	TSRMLS_SET_CTX(ctx);

	REGISTER_INI_ENTRIES();

	/* Initialize libmongoc */
	mongoc_init();
	mongoc_log_set_handler(php_phongo_log, ctx);

	/* Prep default object handlers to be used when we register the classes */
	memcpy(&phongo_std_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_std_object_handlers.clone_obj = NULL;
	phongo_std_object_handlers.read_property = NULL;
	phongo_std_object_handlers.write_property = NULL;
	phongo_std_object_handlers.get_debug_info = NULL;

	PHP_MINIT(bson)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(CursorId)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(GeneratedId)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(Cursor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(CommandCursor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(QueryCursor)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(WriteResult)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(DeleteResult)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(InsertResult)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UpdateResult)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(CommandResult)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(WriteBatch)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(DeleteBatch)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(InsertBatch)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UpdateBatch)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(Manager)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Server)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(Command)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Query)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(ReadPreference)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(WriteConcernError)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteError)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(phongo)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	(void)module_number; /* Really doesn't matter which module number we are */

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(phongo)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	(void)module_number; /* Really doesn't matter which module number we are */

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(phongo)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */

	/* Cleanup after libmongoc */
	mongoc_cleanup();

	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_GSHUTDOWN_FUNCTION */
PHP_GSHUTDOWN_FUNCTION(phongo)
{
	phongo_globals->debug_log = NULL;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(phongo)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "phongo support", "enabled");
	php_info_print_table_row(2, "phongo", PHONGO_VERSION_S);
	php_info_print_table_row(2, "libmongoc", MONGOC_VERSION_S);
	php_info_print_table_row(2, "libbson", BSON_VERSION_S);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */
/* }}} */

/* {{{ phongo_functions[]
*/
const zend_function_entry phongo_functions[] = {
	PHP_FE(bson_encode,     NULL)
	PHP_FE(bson_decode,     NULL)
	PHP_FE(bson_to_json,    NULL)
	PHP_FE_END
};
/* }}} */
/* {{{ phongo_module_entry
 */
zend_module_entry phongo_module_entry = {
	STANDARD_MODULE_HEADER,
	"phongo",
	phongo_functions,
	PHP_MINIT(phongo),
	PHP_MSHUTDOWN(phongo),
	PHP_RINIT(phongo),
	PHP_RSHUTDOWN(phongo),
	PHP_MINFO(phongo),
	PHONGO_VERSION,
	PHP_MODULE_GLOBALS(phongo),
	PHP_GINIT(phongo),
	PHP_GSHUTDOWN(phongo),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_PHONGO
ZEND_GET_MODULE(phongo)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
