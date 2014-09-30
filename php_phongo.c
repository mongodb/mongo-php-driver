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


/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include "Zend/zend_hash.h"
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
#include "php_array.h"

#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "PHONGO"

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
		case PHONGO_ERROR_INVALID_ARGUMENT:
			return spl_ce_InvalidArgumentException;
		case PHONGO_ERROR_RUNTIME:
			return spl_ce_RuntimeException;
		case PHONGO_ERROR_MONGOC_FAILED:
			return spl_ce_RuntimeException;
		case PHONGO_ERROR_WRITE_FAILED:
			return php_phongo_writeexception_ce;
	}

	mongoc_log(MONGOC_LOG_LEVEL_ERROR, MONGOC_LOG_DOMAIN, "Resolving unknown exception domain!!!");
	return spl_ce_RuntimeException;
}
zend_class_entry* phongo_exception_from_mongoc_domain(uint32_t /* mongoc_error_domain_t */ domain, uint32_t /* mongoc_error_code_t */ code)
{
	switch(code) {
		case MONGOC_ERROR_STREAM_INVALID_TYPE:
		case MONGOC_ERROR_STREAM_INVALID_STATE:
		case MONGOC_ERROR_STREAM_NAME_RESOLUTION:
		case MONGOC_ERROR_STREAM_SOCKET:
		case MONGOC_ERROR_STREAM_CONNECT:
		case MONGOC_ERROR_STREAM_NOT_ESTABLISHED:
		case MONGOC_ERROR_CLIENT_NOT_READY:
		case MONGOC_ERROR_CLIENT_TOO_BIG:
		case MONGOC_ERROR_CLIENT_TOO_SMALL:
		case MONGOC_ERROR_CLIENT_GETNONCE:
		case MONGOC_ERROR_CLIENT_AUTHENTICATE:
		case MONGOC_ERROR_CLIENT_NO_ACCEPTABLE_PEER:
		case MONGOC_ERROR_CLIENT_IN_EXHAUST:
		case MONGOC_ERROR_PROTOCOL_INVALID_REPLY:
		case MONGOC_ERROR_PROTOCOL_BAD_WIRE_VERSION:
		case MONGOC_ERROR_CURSOR_INVALID_CURSOR:
		case MONGOC_ERROR_QUERY_FAILURE:
		/*case MONGOC_ERROR_PROTOCOL_ERROR:*/
		case MONGOC_ERROR_BSON_INVALID:
		case MONGOC_ERROR_MATCHER_INVALID:
		case MONGOC_ERROR_NAMESPACE_INVALID:
		case MONGOC_ERROR_COMMAND_INVALID_ARG:
		case MONGOC_ERROR_COLLECTION_INSERT_FAILED:
		case MONGOC_ERROR_GRIDFS_INVALID_FILENAME:
		case MONGOC_ERROR_QUERY_COMMAND_NOT_FOUND:
		case MONGOC_ERROR_QUERY_NOT_TAILABLE:
			return spl_ce_RuntimeException;
	}
	switch (domain) {
		case MONGOC_ERROR_CLIENT:
		case MONGOC_ERROR_STREAM:
		case MONGOC_ERROR_PROTOCOL:
		case MONGOC_ERROR_CURSOR:
		case MONGOC_ERROR_QUERY:
		case MONGOC_ERROR_INSERT:
		case MONGOC_ERROR_SASL:
		case MONGOC_ERROR_BSON:
		case MONGOC_ERROR_MATCHER:
		case MONGOC_ERROR_NAMESPACE:
		case MONGOC_ERROR_COMMAND:
		case MONGOC_ERROR_COLLECTION:
		case MONGOC_ERROR_GRIDFS:
			/* FIXME: We don't have the Exceptions mocked yet.. */
#if 0
			return phongo_ce_mongo_connection_exception;
#endif
		default:
			return spl_ce_RuntimeException;
	}
}
PHONGO_API zval* phongo_throw_exception(php_phongo_error_domain_t domain, const char *message TSRMLS_DC)
{
	return zend_throw_exception(phongo_exception_from_phongo_domain(domain), message, 0 TSRMLS_CC);
}
PHONGO_API zval* phongo_throw_exception_from_bson_error_t(bson_error_t *error TSRMLS_DC)
{
	return zend_throw_exception(phongo_exception_from_mongoc_domain(error->domain, error->code), error->message, error->code TSRMLS_CC);
}
static void php_phongo_log(mongoc_log_level_t log_level, const char *log_domain, const char *message, void *user_data)
{
	TSRMLS_FETCH_FROM_CTX(user_data);

	switch(log_level) {
	case MONGOC_LOG_LEVEL_ERROR:
	case MONGOC_LOG_LEVEL_CRITICAL:
		return phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED, message TSRMLS_CC);

	case MONGOC_LOG_LEVEL_WARNING:
	case MONGOC_LOG_LEVEL_MESSAGE:
	case MONGOC_LOG_LEVEL_INFO:
	case MONGOC_LOG_LEVEL_DEBUG:
	case MONGOC_LOG_LEVEL_TRACE:
		{
			int fd = 0;
			char *dt = NULL;

			if (!PHONGO_G(debug_log)) {
				return;
			}
			if (strcasecmp(PHONGO_G(debug_log), "off") == 0) {
				return;
			}
			if (strcasecmp(PHONGO_G(debug_log), "0") == 0) {
				return;
			}

#define PHONGO_DEBUG_LOG_FORMAT "[%s] %10s: %-8s> %s\n"

			dt = php_format_date("Y-m-d\\TH:i:sP", strlen("Y-m-d\\TH:i:sP"), time(NULL), 0 TSRMLS_CC);
			if (strcasecmp(PHONGO_G(debug_log), "stderr") == 0) {
				fprintf(stderr, PHONGO_DEBUG_LOG_FORMAT, dt, log_domain, mongoc_log_level_str(log_level), message);
			} else if (strcasecmp(PHONGO_G(debug_log), "stdout") == 0) {
				php_printf(PHONGO_DEBUG_LOG_FORMAT, dt, log_domain, mongoc_log_level_str(log_level), message);
			} else if ((fd = VCWD_OPEN_MODE(PHONGO_G(debug_log), O_CREAT | O_APPEND | O_WRONLY, 0644)) != -1) {
				char *tmp;
				int len;

				len = spprintf(&tmp, 0, PHONGO_DEBUG_LOG_FORMAT, dt, log_domain, mongoc_log_level_str(log_level), message);
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
void phongo_log_writer(mongoc_stream_t *stream, int32_t timeout_msec, ssize_t sent, size_t iovcnt)
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream;

	mongoc_log(MONGOC_LOG_LEVEL_MESSAGE, MONGOC_LOG_DOMAIN, "Wrote %zd bytes to '%s:%d' in %zd iterations", sent, base_stream->host->host, base_stream->host->port, iovcnt);
}
/* }}} */

/* {{{ Init objects */
void phongo_result_init(zval *return_value, zend_class_entry *result_class, mongoc_cursor_t *cursor, const bson_t *bson, int server_hint TSRMLS_DC) /* {{{ */
{
	php_phongo_result_t *result;

	object_init_ex(return_value, result_class);

	result = (php_phongo_result_t *)zend_object_store_get_object(return_value TSRMLS_CC);
	if (cursor) {
		result->cursor = cursor;
		result->hint = mongoc_cursor_get_hint(cursor);
	} else {
		result->hint = server_hint;
	}
	result->firstBatch = (bson_t *)bson;
} /* }}} */

void phongo_server_init(zval *return_value, int hint, mongoc_host_list_t *host TSRMLS_DC) /* {{{ */
{
	php_phongo_server_t *server;

	object_init_ex(return_value, php_phongo_server_ce);

	server = (php_phongo_server_t *)zend_object_store_get_object(return_value TSRMLS_CC);
	server->hint = hint;
	server->host = host;
}
/* }}} */

zend_bool phongo_writeconcernerror_init(zval *return_value, bson_t *bson TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	php_phongo_writeconcernerror_t *writeconcernerror;

	writeconcernerror = (php_phongo_writeconcernerror_t *)zend_object_store_get_object(return_value TSRMLS_CC);

	if (bson_iter_init_find(&iter, bson, "code") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeconcernerror->code = bson_iter_int32(&iter);
	}
	if (bson_iter_init_find(&iter, bson, "errmsg") && BSON_ITER_HOLDS_UTF8(&iter)) {
		writeconcernerror->message = bson_iter_dup_utf8(&iter, NULL);
	}
	if (bson_iter_init_find(&iter, bson, "errInfo") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
		uint32_t len;
		const uint8_t *data;

		bson_iter_document(&iter, &len, &data);

		if (!data) {
			return false;
		}

		MAKE_STD_ZVAL(writeconcernerror->info);

		if (!bson_to_zval(data, len, writeconcernerror->info)) {
			zval_ptr_dtor(&writeconcernerror->info);
			writeconcernerror->info = NULL;

			return false;
		}
	}

	return true;
} /* }}} */

zend_bool phongo_writeerror_init(zval *return_value, bson_t *bson TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	php_phongo_writeerror_t *writeerror;

	writeerror = (php_phongo_writeerror_t *)zend_object_store_get_object(return_value TSRMLS_CC);

	if (bson_iter_init_find(&iter, bson, "code") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeerror->code = bson_iter_int32(&iter);
	}
	if (bson_iter_init_find(&iter, bson, "errmsg") && BSON_ITER_HOLDS_UTF8(&iter)) {
		writeerror->message = bson_iter_dup_utf8(&iter, NULL);
	}
	if (bson_iter_init_find(&iter, bson, "index") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeerror->index = bson_iter_int32(&iter);
	}

	return true;
} /* }}} */

void phongo_writeresult_init(zval *return_value, const bson_t *bson, int server_hint TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter, ar;
	php_phongo_writeresult_t *writeresult;

	phongo_result_init(return_value, php_phongo_writeresult_ce, NULL, bson, server_hint TSRMLS_CC);
	writeresult = (php_phongo_writeresult_t *)zend_object_store_get_object(return_value TSRMLS_CC);

	if (bson_iter_init_find(&iter, bson, "nUpserted") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeresult->nUpserted = bson_iter_int32(&iter);
	}
	if (bson_iter_init_find(&iter, bson, "nMatched") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeresult->nMatched = bson_iter_int32(&iter);
	}
	if (bson_iter_init_find(&iter, bson, "nRemoved") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeresult->nRemoved = bson_iter_int32(&iter);
	}
	if (bson_iter_init_find(&iter, bson, "nInserted") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeresult->nInserted = bson_iter_int32(&iter);
	}
	if (bson_iter_init_find(&iter, bson, "nModified") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeresult->nModified = bson_iter_int32(&iter);
	}

	if (bson_iter_init_find(&iter, bson, "upserted") &&
		BSON_ITER_HOLDS_ARRAY(&iter) &&
		bson_iter_recurse(&iter, &ar)) {

		MAKE_STD_ZVAL(writeresult->upsertedIds);
		array_init(writeresult->upsertedIds);

		/* TODO: bson_to_zval requires an array or document, so we'll convert
		 * the entire upsert document to a zval. Ideally, we would recurse
		 * within the document, use bson_iter_value() to access the "_id" field,
		 * and then convert that alone. The "index" field could be read directly
		 * as an integer and used with add_index_zval(). */
		while (bson_iter_next(&ar)) {
			uint32_t index, len;
			const uint8_t *data;
			zval *upsert, *id;

			if (!BSON_ITER_HOLDS_DOCUMENT(&ar)) {
				continue;
			}

			bson_iter_document(&ar, &len, &data);

			MAKE_STD_ZVAL(upsert);

			if (!bson_to_zval(data, len, upsert)) {
				zval_ptr_dtor(&upsert);
				continue;
			}

			index = php_array_fetchc_long(upsert, "index");
			id = php_array_fetchc(upsert, "_id");

			add_index_zval(writeresult->upsertedIds, index, id);
			zval_add_ref(&id);

			zval_ptr_dtor(&upsert);
		}
	}

	if (bson_iter_init_find(&iter, bson, "writeErrors") &&
		BSON_ITER_HOLDS_ARRAY(&iter) &&
		bson_iter_recurse(&iter, &ar)) {

		MAKE_STD_ZVAL(writeresult->writeErrors);
		array_init(writeresult->writeErrors);

		while (bson_iter_next(&ar)) {
			bson_t cbson;
			uint32_t len;
			const uint8_t *data;
			zval *writeerror;

			if (!BSON_ITER_HOLDS_DOCUMENT(&ar)) {
				continue;
			}

			bson_iter_document(&ar, &len, &data);

			if (!bson_init_static(&cbson, data, len)) {
				continue;
			}

			MAKE_STD_ZVAL(writeerror);
			object_init_ex(writeerror, php_phongo_writeerror_ce);

			if (!phongo_writeerror_init(writeerror, &cbson TSRMLS_CC)) {
				zval_ptr_dtor(&writeerror);
				continue;
			}

			add_next_index_zval(writeresult->writeErrors, writeerror);
		}
	}

	if (bson_iter_init_find(&iter, bson, "writeConcernErrors") &&
		BSON_ITER_HOLDS_ARRAY(&iter) &&
		bson_iter_recurse(&iter, &ar)) {

		MAKE_STD_ZVAL(writeresult->writeErrors);
		array_init(writeresult->writeErrors);

		while (bson_iter_next(&ar)) {
			bson_t cbson;
			uint32_t len;
			const uint8_t *data;
			zval *writeconcernerror;

			if (!BSON_ITER_HOLDS_DOCUMENT(&ar)) {
				continue;
			}

			bson_iter_document(&ar, &len, &data);

			if (!bson_init_static(&cbson, data, len)) {
				continue;
			}

			MAKE_STD_ZVAL(writeconcernerror);
			object_init_ex(writeconcernerror, php_phongo_writeconcernerror_ce);

			if (!phongo_writeconcernerror_init(writeconcernerror, &cbson TSRMLS_CC)) {
				zval_ptr_dtor(&writeconcernerror);
				continue;
			}

			add_next_index_zval(writeresult->writeConcernErrors, writeconcernerror);
		}
	}
} /* }}} */
/* }}} */

/* {{{ CRUD */
/* Splits a namespace name into the database and collection names, allocated with estrdup. */
bool phongo_split_namespace(char *namespace, char **dbname, char **cname) /* {{{ */
{
	char *dot = strchr(namespace, '.');

	if (!dot) {
		return false;
	}

	if (cname) {
		*cname = estrdup(namespace + (dot - namespace) + 1);
	}
	if (dbname) {
		*dbname = estrndup(namespace, dot - namespace);
	}

	return true;
} /* }}} */

mongoc_bulk_operation_t *phongo_writebatch_init(zend_bool ordered) { /* {{{ */
	return mongoc_bulk_operation_new(ordered);
} /* }}} */

int phongo_execute_single_insert(mongoc_client_t *client, char *namespace, bson_t *doc, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	bool retval = false;
	mongoc_bulk_operation_t *batch;

	batch = phongo_writebatch_init(true);
	mongoc_bulk_operation_insert(batch, doc);

	retval = phongo_execute_write(client, namespace, batch, 0, return_value, return_value_used TSRMLS_CC);
	mongoc_bulk_operation_destroy(batch);

	return retval;
} /* }}} */

int phongo_execute_single_update(mongoc_client_t *client, char *namespace, bson_t *query, bson_t *update, mongoc_update_flags_t flags, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	bool retval = false;
	mongoc_bulk_operation_t *batch;

	batch = phongo_writebatch_init(true);
	if (flags & MONGOC_UPDATE_MULTI_UPDATE) {
		mongoc_bulk_operation_update_one(batch, query, update, !!(flags & MONGOC_UPDATE_UPSERT));
	} else {
		mongoc_bulk_operation_update(batch, query, update, !!(flags & MONGOC_UPDATE_UPSERT));
	}
	retval = phongo_execute_write(client, namespace, batch, 0, return_value, return_value_used TSRMLS_CC);
	mongoc_bulk_operation_destroy(batch);

	return retval;
} /* }}} */

int phongo_execute_single_delete(mongoc_client_t *client, char *namespace, bson_t *query, mongoc_delete_flags_t flags, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	bool retval = false;
	mongoc_bulk_operation_t *batch;

	batch = phongo_writebatch_init(true);
	if (flags & MONGOC_DELETE_SINGLE_REMOVE) {
		mongoc_bulk_operation_remove_one(batch, query);
	} else {
		mongoc_bulk_operation_remove(batch, query);
	}

	retval = phongo_execute_write(client, namespace, batch, 0, return_value, return_value_used TSRMLS_CC);
	mongoc_bulk_operation_destroy(batch);

	return retval;
} /* }}} */

bool phongo_execute_write(mongoc_client_t *client, char *namespace, mongoc_bulk_operation_t *batch, int server_hint, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	bson_error_t error;
	bson_t *reply = NULL;
	char *database;
	char *collection;
	int hint;

	if (!phongo_split_namespace(namespace, &database, &collection)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid namespace provided" TSRMLS_CC);
		return false;
	}

	mongoc_bulk_operation_set_database(batch, database);
	mongoc_bulk_operation_set_collection(batch, collection);
	mongoc_bulk_operation_set_client(batch, client);

	if (server_hint) {
		mongoc_bulk_operation_set_hint(batch, server_hint);
	}

	if (return_value_used) {
		reply = bson_new();
	}

	hint = mongoc_bulk_operation_execute(batch, reply, &error);

	if (!hint) {
		zval *e = phongo_throw_exception(PHONGO_ERROR_WRITE_FAILED, error.message TSRMLS_CC);
		/* zend_update_property_long(Z_OBJCE_P(e), e, ZEND_STRL("code"), error.code TSRMLS_CC) */

		if (reply) {
			phongo_writeresult_init(return_value, reply, hint TSRMLS_CC);
			zend_update_property(Z_OBJCE_P(e), e, ZEND_STRL("writeResult"), return_value TSRMLS_CC);
			bson_destroy(reply);
		}

		return false;
	}

	if (!return_value_used) {
		return true;
	}

	phongo_writeresult_init(return_value, reply, hint TSRMLS_CC);
	bson_destroy(reply);

	return true;
} /* }}} */

int phongo_execute_query(mongoc_client_t *client, char *namespace, php_phongo_query_t *query, mongoc_read_prefs_t *read_preference, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	const bson_t *doc = NULL;
	mongoc_cursor_t *cursor;
	char *dbname;
	char *collname;
	mongoc_collection_t *collection;

	if (!phongo_split_namespace(namespace, &dbname, &collname)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid namespace provided" TSRMLS_CC);
		return false;
	}
	collection = mongoc_client_get_collection(client, dbname, collname);

	cursor = mongoc_collection_find(collection, query->flags, query->skip, query->limit, query->batch_size, query->query, query->selector, read_preference);

	if (!mongoc_cursor_next(cursor, &doc)) {
		bson_error_t error;

		if (mongoc_cursor_error(cursor, &error)) {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			return false;
		}
	}

	if (!return_value_used) {
		return true;
	}

	phongo_result_init(return_value, php_phongo_queryresult_ce, cursor, doc, 0 TSRMLS_CC);
	return true;
} /* }}} */

int phongo_execute_command(mongoc_client_t *client, char *db, bson_t *command, mongoc_read_prefs_t *read_preference, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	mongoc_cursor_t *cursor;
	const bson_t *doc;


	cursor = mongoc_client_command(client, db, MONGOC_QUERY_NONE, 0, 1, 0, command, NULL, read_preference);

	if (!mongoc_cursor_next(cursor, &doc)) {
		bson_error_t error;

		if (mongoc_cursor_error(cursor, &error)) {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			return false;
		}
	}

	if (!return_value_used) {
		return true;
	}

	phongo_result_init(return_value, php_phongo_commandresult_ce, cursor, doc, 0 TSRMLS_CC);
	return true;
} /* }}} */

/* }}} */

/* {{{ Stream vtable */
void phongo_stream_destroy(mongoc_stream_t *stream) /* {{{ */
{
} /* }}} */

int phongo_stream_close(mongoc_stream_t *stream) /* {{{ */
{
	return -1;
} /* }}} */

void php_phongo_set_timeout(php_phongo_stream_socket *base_stream, int32_t timeout_msec) /* {{{ */
{
	TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

	if (timeout_msec > 0) {
		struct timeval rtimeout = {0, 0};

		rtimeout.tv_sec = timeout_msec / 1000;
		rtimeout.tv_usec = (timeout_msec % 1000) * 1000;

		php_stream_set_option(base_stream->stream, PHP_STREAM_OPTION_READ_TIMEOUT, 0, &rtimeout);
		mongoc_log(MONGOC_LOG_LEVEL_DEBUG, MONGOC_LOG_DOMAIN, "Setting timeout to: %d", timeout_msec);
	} else if (timeout_msec == 0) {
		mongoc_log(MONGOC_LOG_LEVEL_DEBUG, MONGOC_LOG_DOMAIN, "Setting timeout to 0");
		php_stream_set_option(base_stream->stream, PHP_STREAM_OPTION_READ_TIMEOUT, 0, NULL);
	}
} /* }}} */

ssize_t phongo_stream_writev(mongoc_stream_t *stream, mongoc_iovec_t *iov, size_t iovcnt, int32_t timeout_msec) /* {{{ */
{
	size_t     i = 0;
	ssize_t sent = 0;
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream;
	TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

	php_phongo_set_timeout(base_stream, timeout_msec);

	for (i = 0; i < iovcnt; i++) {
		sent += php_stream_write(base_stream->stream, iov[i].iov_base, iov[i].iov_len);
	}
	if (base_stream->log.writer) {
		base_stream->log.writer(stream, timeout_msec, sent, iovcnt);
	}

	return sent;
} /* }}} */

ssize_t phongo_stream_readv(mongoc_stream_t *stream, mongoc_iovec_t *iov, size_t iovcnt, size_t min_bytes, int32_t timeout_msec) /* {{{ */
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream;
	ssize_t ret = 0;
	ssize_t read;
	size_t cur = 0;
	TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

	do {
		read = php_stream_read(base_stream->stream, iov[cur].iov_base, iov[cur].iov_len);
		mongoc_log(MONGOC_LOG_LEVEL_DEBUG, MONGOC_LOG_DOMAIN, "Reading got: %ld wanted: %ld", read, min_bytes);

		if (read <= 0) {
			if (ret >= (ssize_t)min_bytes) {
				break;
			}
			return -1;
		}

		ret += read;

		while ((cur < iovcnt) && (read >= (ssize_t)iov[cur].iov_len)) {
			read -= iov[cur++].iov_len;
		}

		if (cur == iovcnt) {
			break;
		}

		if (ret >= (ssize_t)min_bytes) {
			break;
		}

		iov[cur].iov_base = ((char *)iov[cur].iov_base) + read;
		iov[cur].iov_len -= read;
	} while(1);

	return ret;
} /* }}} */

int phongo_stream_setsockopt(mongoc_stream_t *stream, int level, int optname, void *optval, socklen_t optlen) /* {{{ */
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream;
	int socket = ((php_netstream_data_t *)base_stream->stream->abstract)->socket;

	return setsockopt (socket, level, optname, optval, optlen);
} /* }}} */

mongoc_stream_t* phongo_stream_get_base_stream(mongoc_stream_t *stream) /* {{{ */
{
	return (mongoc_stream_t *) stream;
} /* }}} */

php_phongo_stream_logger phongo_stream_logger = {
	phongo_log_writer,
};

mongoc_stream_t* phongo_stream_initiator(const mongoc_uri_t *uri, const mongoc_host_list_t *host, void *user_data, bson_error_t *error) /* {{{ */
{
	php_phongo_stream_socket *base_stream = NULL;
	php_stream *stream;
	char *errmsg;
	int errcode;
	char *dsn;
	int dsn_len;
	TSRMLS_FETCH_FROM_CTX(user_data);


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

	base_stream = ecalloc(1, sizeof(php_phongo_stream_socket));
	base_stream->stream = stream;
	base_stream->uri_options = mongoc_uri_get_options(uri);
	base_stream->host = host;
	base_stream->log = phongo_stream_logger;
	/* flush missing, doesn't seem to be used */
	base_stream->vtable.type = 42;
	base_stream->vtable.destroy = phongo_stream_destroy;
	base_stream->vtable.close = phongo_stream_close;
	base_stream->vtable.writev = phongo_stream_writev;
	base_stream->vtable.readv = phongo_stream_readv;
	base_stream->vtable.setsockopt = phongo_stream_setsockopt;
	base_stream->vtable.get_base_stream = phongo_stream_get_base_stream;

	if (host->family != AF_UNIX) {
		int flag = 1;

		if (phongo_stream_setsockopt((mongoc_stream_t *)base_stream, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int))) {
			mongoc_log(MONGOC_LOG_LEVEL_WARNING, MONGOC_LOG_DOMAIN, "setsockopt TCP_NODELAY failed");
		}
	}

	return (mongoc_stream_t *) base_stream;
} /* }}} */

/* }}} */

mongoc_read_prefs_t* phongo_read_preference_from_zval(zval *zread_preference TSRMLS_DC) /* {{{ */
{
	if (zread_preference) {
		php_phongo_readpreference_t *intern = (php_phongo_readpreference_t *)zend_object_store_get_object(zread_preference TSRMLS_CC);

		if (intern) {
			return intern->read_preference;
		}
	}

	return NULL;
} /* }}} */

php_phongo_query_t* phongo_query_from_zval(zval *zquery TSRMLS_DC) /* {{{ */
{
	php_phongo_query_t *intern = (php_phongo_query_t *)zend_object_store_get_object(zquery TSRMLS_CC);

	return intern;
} /* }}} */

php_phongo_query_t* phongo_query_init(php_phongo_query_t *query, zval *filter, zval *options TSRMLS_DC) /* {{{ */
{
	zval *zquery;

	if (filter && !(Z_TYPE_P(filter) == IS_ARRAY || Z_TYPE_P(filter) == IS_OBJECT)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Expected filter to be array or object, %s given", zend_get_type_by_const(Z_TYPE_P(filter)));
		return NULL;
	}

	MAKE_STD_ZVAL(zquery);
	array_init(zquery);

	if (options) {
		/* TODO: Ensure batchSize, limit, and skip are 32-bit. Should we ensure
		 * that cursorFlags is a valid mongoc_query_flags_t combination? */
		query->batch_size = php_array_fetchc_long(options, "batchSize");
		query->flags = php_array_fetchc_long(options, "cursorFlags");
		query->limit = php_array_fetchc_long(options, "limit");
		query->skip = php_array_fetchc_long(options, "skip");

		if (php_array_existsc(options, "modifiers")) {
			zval *modifiers = php_array_fetchc(options, "modifiers");

			if (modifiers && !(Z_TYPE_P(modifiers) == IS_ARRAY || Z_TYPE_P(modifiers) == IS_OBJECT)) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Expected modifiers to be array or object, %s given", zend_get_type_by_const(Z_TYPE_P(modifiers)));
				zval_ptr_dtor(&zquery);
				return NULL;
			}

			convert_to_array_ex(&modifiers);
			zend_hash_merge(HASH_OF(zquery), HASH_OF(modifiers), (void (*)(void*))zval_add_ref, NULL, sizeof(zval *), 1);
		}

		if (php_array_existsc(options, "projection")) {
			zval *projection = php_array_fetchc(options, "projection");

			if (projection && !(Z_TYPE_P(projection) == IS_ARRAY || Z_TYPE_P(projection) == IS_OBJECT)) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Expected projection to be array or object, %s given", zend_get_type_by_const(Z_TYPE_P(projection)));
				zval_ptr_dtor(&zquery);
				return NULL;
			}

			convert_to_array_ex(&projection);
			query->selector = bson_new();
			zval_to_bson(projection, PHONGO_BSON_NONE, query->selector, NULL TSRMLS_CC);
		}

		if (php_array_existsc(options, "sort")) {
			zval *sort = php_array_fetchc(options, "sort");

			if (sort && !(Z_TYPE_P(sort) == IS_ARRAY || Z_TYPE_P(sort) == IS_OBJECT)) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Expected sort to be array or object, %s given", zend_get_type_by_const(Z_TYPE_P(sort)));
				zval_ptr_dtor(&zquery);
				return NULL;
			}

			convert_to_array_ex(&sort);
			add_assoc_zval_ex(zquery, ZEND_STRS("$orderby"), sort);
		}
	}

	add_assoc_zval_ex(zquery, ZEND_STRS("$query"), filter);

	query->query = bson_new();
	zval_to_bson(zquery, PHONGO_BSON_NONE, query->query, NULL TSRMLS_CC);
	zval_ptr_dtor(&zquery);

	return query;
} /* }}} */

void php_phongo_objectid_new_from_oid(zval *object, const bson_oid_t *oid TSRMLS_DC) /* {{{ */
{
	php_phongo_objectid_t     *intern;

	object_init_ex(object, php_phongo_objectid_ce);

	intern = (php_phongo_objectid_t *)zend_object_store_get_object(object TSRMLS_CC);
	intern->oid = ecalloc(1, sizeof(bson_oid_t));
	bson_oid_copy(oid, intern->oid);
} /* }}} */

/* {{{ Iterator */
typedef struct {
	zend_object_iterator iterator;
	php_phongo_result_t  *intern;
	zval                 *current;
} phongo_cursor_it;

static void phongo_cursor_it_dtor(zend_object_iterator *iter TSRMLS_DC) /* {{{ */
{
	phongo_cursor_it    *cursor_it = (phongo_cursor_it *)iter;

	if (cursor_it->current) {
		zval_ptr_dtor(&cursor_it->current);
	}

	zval_ptr_dtor((zval**)&cursor_it->iterator.data);
	efree(cursor_it);
} /* }}} */

static int phongo_cursor_it_valid(zend_object_iterator *iter TSRMLS_DC) /* {{{ */
{
	return ((phongo_cursor_it *)iter)->current ? SUCCESS : FAILURE;
} /* }}} */

static void phongo_cursor_it_get_current_data(zend_object_iterator *iter, zval ***data TSRMLS_DC) /* {{{ */
{
	*data = &((phongo_cursor_it *)iter)->current;
} /* }}} */

static void phongo_cursor_it_move_forward(zend_object_iterator *iter TSRMLS_DC) /* {{{ */
{
	phongo_cursor_it    *cursor_it = (phongo_cursor_it *)iter;
	php_phongo_result_t *intern    = cursor_it->intern;
	const bson_t *doc;

	if (cursor_it->current) {
		zval_ptr_dtor(&cursor_it->current);
		cursor_it->current = NULL;
	}

	if (mongoc_cursor_next(intern->cursor, &doc)) {
		MAKE_STD_ZVAL(cursor_it->current);
		bson_to_zval(bson_get_data(doc), doc->len, cursor_it->current);
	} else {
		cursor_it->current = NULL;
	}

} /* }}} */

static void phongo_cursor_it_rewind(zend_object_iterator *iter TSRMLS_DC) /* {{{ */
{
	phongo_cursor_it    *cursor_it = (phongo_cursor_it *)iter;
	php_phongo_result_t *intern    = cursor_it->intern;

	/* firstBatch is empty when the query simply didn't return any results */
	if (intern->firstBatch) {
		MAKE_STD_ZVAL(cursor_it->current);
		bson_to_zval(bson_get_data(intern->firstBatch), intern->firstBatch->len, cursor_it->current);
	}
} /* }}} */

/* iterator handler table */
zend_object_iterator_funcs phongo_cursor_it_funcs = {
	phongo_cursor_it_dtor,
	phongo_cursor_it_valid,
	phongo_cursor_it_get_current_data,
	NULL /*phongo_cursor_it_get_current_key */,
	phongo_cursor_it_move_forward,
	phongo_cursor_it_rewind
};
zend_object_iterator_funcs zend_interface_iterator_funcs_iterator_default = {
	phongo_cursor_it_dtor,
	zend_user_it_valid,
	zend_user_it_get_current_data,
	zend_user_it_get_current_key,
	zend_user_it_move_forward,
	zend_user_it_rewind,
	zend_user_it_invalidate_current
};

zend_object_iterator *phongo_result_get_iterator(zend_class_entry *ce, zval *object, int by_ref TSRMLS_DC) /* {{{ */
{
	php_phongo_result_t    *intern = (php_phongo_result_t *)zend_object_store_get_object(object TSRMLS_CC);

	if (by_ref) {
		zend_error(E_ERROR, "An iterator cannot be used with foreach by reference");
	}

	/* If we have a custom iterator */
	if (intern->ce_get_iterator != NULL) {
		zend_user_iterator *iterator;

		zval_ptr_dtor(&object);
		object_init_ex(object, intern->ce_get_iterator);

		iterator = emalloc(sizeof(zend_user_iterator));

		Z_ADDREF_P(object);
		iterator->it.data = (void*)object;
		iterator->it.funcs = &zend_interface_iterator_funcs_iterator_default;
		iterator->ce = Z_OBJCE_P(object);
		iterator->value = NULL;
		return (zend_object_iterator*)iterator;
	} else {
		phongo_cursor_it       *cursor_it;

		cursor_it = ecalloc(1, sizeof(phongo_cursor_it));

		Z_ADDREF_P(object);
		cursor_it->iterator.data = (void*)object;
		cursor_it->iterator.funcs = &phongo_cursor_it_funcs;
		cursor_it->intern = intern;

		return (zend_object_iterator*)cursor_it;
	}
} /* }}} */
/* }}} */


/* {{{ Memory allocation wrappers */
static void* php_phongo_malloc(size_t num_bytes) /* {{{ */
{
	return emalloc(num_bytes);
} /* }}} */

static void* php_phongo_calloc(size_t num_members, size_t num_bytes) /* {{{ */
{
	return ecalloc(num_members, num_bytes);
} /* }}} */

static void* php_phongo_realloc(void *mem, size_t num_bytes) { /* {{{ */
	return erealloc(mem, num_bytes);
} /* }}} */

static void php_phongo_free(void *mem) /* {{{ */
{
	return efree(mem);
} /* }}} */

/* }}} */


/* {{{ M[INIT|SHUTDOWN] R[INIT|SHUTDOWN] G[INIT|SHUTDOWN] MINFO INI */

/* {{{ INI entries */
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("phongo.debug_log", (char *)"stderr", PHP_INI_ALL, OnUpdateString, debug_log, zend_phongo_globals, phongo_globals)
PHP_INI_END()
/* }}} */

/* {{{ PHP_GINIT_FUNCTION */
PHP_GINIT_FUNCTION(phongo)
{
	bson_mem_vtable_t bsonMemVTable = {
		php_phongo_malloc,
		php_phongo_calloc,
		php_phongo_realloc,
		php_phongo_free,
	};
	phongo_globals->debug_log = NULL;
	phongo_globals->bsonMemVTable = bsonMemVTable;

}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(phongo)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	void ***ctx = NULL;
	TSRMLS_SET_CTX(ctx);
	REGISTER_INI_ENTRIES();

	/* Initialize libbson */
	bson_mem_set_vtable(&PHONGO_G(bsonMemVTable));
	/* Initialize libmongoc */
	mongoc_init();
	mongoc_log_set_handler(php_phongo_log, ctx);

	/* Prep default object handlers to be used when we register the classes */
	memcpy(&phongo_std_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_std_object_handlers.clone_obj = NULL;
	phongo_std_object_handlers.read_property = NULL;
	phongo_std_object_handlers.write_property = NULL;
	phongo_std_object_handlers.get_debug_info = NULL;

	PHP_MINIT(Command)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(CommandResult)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Cursor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(CursorId)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(GeneratedId)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Manager)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Query)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(QueryResult)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ReadPreference)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Server)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteBatch)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteConcern)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteConcernError)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteError)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteResult)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(Type)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Binary)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(DBRef)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Int32)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Int64)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Javascript)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Log)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(MaxKey)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(MinKey)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ObjectID)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Pool)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Regex)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Timestamp)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UTCDatetime)(INIT_FUNC_ARGS_PASSTHRU);

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
	ZEND_NS_FE("BSON", fromArray, NULL)
	ZEND_NS_FE("BSON", toArray,   NULL)
	ZEND_NS_FE("BSON", toJSON,    NULL)
	ZEND_NS_FE("BSON", fromJSON,  NULL)
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
