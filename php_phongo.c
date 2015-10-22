/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2015 MongoDB, Inc.                                         |
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
  | Copyright (c) 2014-2015 MongoDB, Inc.                                     |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include "bson.h"
#include "mongoc.h"
#include "mongoc-cursor-cursorid-private.h"
#include "mongoc-read-prefs-private.h"
#include "mongoc-bulk-operation-private.h"
#include "mongoc-write-concern-private.h"
#include "mongoc-uri-private.h"
#include "mongoc-trace.h"


/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <ext/standard/file.h>
#include <Zend/zend_hash.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_exceptions.h>
#include <ext/spl/spl_iterators.h>
#include <ext/spl/spl_exceptions.h>
/* For formating timestamp in the log */
#include <ext/date/php_date.h>
/* Stream wrapper */
#include <main/php_streams.h>
#include <main/php_network.h>
/* Debug log writing */
#include <main/php_open_temporary_file.h>
/* PHP array helpers */
#include "php_array_api.h"

/* For our stream verifications */
#include <openssl/x509.h>

/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"
#include "php-ssl.h"

#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "PHONGO"

#define PHONGO_DEBUG_INI "mongodb.debug"
#define PHONGO_DEBUG_INI_DEFAULT ""
#define PHONGO_STREAM_BUFFER_SIZE 4096

ZEND_DECLARE_MODULE_GLOBALS(mongodb)

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
			return php_phongo_invalidargumentexception_ce;
		case PHONGO_ERROR_LOGIC:
			return php_phongo_logicexception_ce;
		case PHONGO_ERROR_RUNTIME:
			return php_phongo_runtimeexception_ce;
		case PHONGO_ERROR_UNEXPECTED_VALUE:
			return php_phongo_unexpectedvalueexception_ce;
		case PHONGO_ERROR_MONGOC_FAILED:
			return php_phongo_runtimeexception_ce;
		case PHONGO_ERROR_WRITE_FAILED:
			return php_phongo_bulkwriteexception_ce;
		case PHONGO_ERROR_CONNECTION_FAILED:
			return php_phongo_connectionexception_ce;
	}

	MONGOC_ERROR("Resolving unknown phongo error domain: %d", domain);
	return php_phongo_runtimeexception_ce;
}
zend_class_entry* phongo_exception_from_mongoc_domain(uint32_t /* mongoc_error_domain_t */ domain, uint32_t /* mongoc_error_code_t */ code)
{
	switch(code) {
		case 50: /* ExceededTimeLimit */
			return php_phongo_executiontimeoutexception_ce;
		case MONGOC_ERROR_STREAM_SOCKET:
			return php_phongo_connectiontimeoutexception_ce;
		case MONGOC_ERROR_CLIENT_AUTHENTICATE:
			return php_phongo_authenticationexception_ce;

		case MONGOC_ERROR_STREAM_INVALID_TYPE:
		case MONGOC_ERROR_STREAM_INVALID_STATE:
		case MONGOC_ERROR_STREAM_NAME_RESOLUTION:
		case MONGOC_ERROR_STREAM_CONNECT:
		case MONGOC_ERROR_STREAM_NOT_ESTABLISHED:
			return php_phongo_connectionexception_ce;
		case MONGOC_ERROR_CLIENT_NOT_READY:
		case MONGOC_ERROR_CLIENT_TOO_BIG:
		case MONGOC_ERROR_CLIENT_TOO_SMALL:
		case MONGOC_ERROR_CLIENT_GETNONCE:
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
			return php_phongo_runtimeexception_ce;
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
			return php_phongo_runtimeexception_ce;
	}
}
PHONGO_API zval* phongo_throw_exception(php_phongo_error_domain_t domain TSRMLS_DC, const char *format, ...)
{
	zval *return_value;
	va_list args;
	char *message;
	int message_len;

	va_start(args, format);
	message_len = vspprintf(&message, 0, format, args);
	return_value = zend_throw_exception(phongo_exception_from_phongo_domain(domain), message, 0 TSRMLS_CC);
	efree(message);
	va_end(args);


	return return_value;
}
PHONGO_API zval* phongo_throw_exception_from_bson_error_t(bson_error_t *error TSRMLS_DC)
{
	return zend_throw_exception(phongo_exception_from_mongoc_domain(error->domain, error->code), error->message, error->code TSRMLS_CC);
}
static void php_phongo_log(mongoc_log_level_t log_level, const char *log_domain, const char *message, void *user_data)
{
	TSRMLS_FETCH_FROM_CTX(user_data);
	(void)user_data;

	switch(log_level) {
	case MONGOC_LOG_LEVEL_ERROR:
	case MONGOC_LOG_LEVEL_CRITICAL:
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED TSRMLS_CC, "%s", message);
		return;

	case MONGOC_LOG_LEVEL_WARNING:
	case MONGOC_LOG_LEVEL_MESSAGE:
	case MONGOC_LOG_LEVEL_INFO:
	case MONGOC_LOG_LEVEL_DEBUG:
	case MONGOC_LOG_LEVEL_TRACE:
		{
			time_t t;
			char *dt = NULL;

			time(&t);
			dt = php_format_date((char *)"Y-m-d\\TH:i:sP", strlen("Y-m-d\\TH:i:sP"), t, 0 TSRMLS_CC);

			fprintf(MONGODB_G(debug_fd), "[%s] %10s: %-8s> %s\n", dt, log_domain, mongoc_log_level_str(log_level), message);
			fflush(MONGODB_G(debug_fd));
			efree(dt);
		} break;
	}
}

/* }}} */

/* {{{ Init objects */
static void phongo_cursor_init(zval *return_value, mongoc_cursor_t *cursor, mongoc_client_t *client TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *intern;

	object_init_ex(return_value, php_phongo_cursor_ce);

	intern = (php_phongo_cursor_t *)zend_object_store_get_object(return_value TSRMLS_CC);
	intern->cursor = cursor;
	intern->server_id = mongoc_cursor_get_hint(cursor);
	intern->client = client;
} /* }}} */

void phongo_server_init(zval *return_value, mongoc_client_t *client, int server_id TSRMLS_DC) /* {{{ */
{
	php_phongo_server_t *server;

	object_init_ex(return_value, php_phongo_server_ce);

	server = (php_phongo_server_t *)zend_object_store_get_object(return_value TSRMLS_CC);
	server->client = client;
	server->server_id = server_id;
}
/* }}} */

void phongo_readpreference_init(zval *return_value, const mongoc_read_prefs_t *read_prefs TSRMLS_DC) /* {{{ */
{
	php_phongo_readpreference_t *intern;

	object_init_ex(return_value, php_phongo_readpreference_ce);

	intern = (php_phongo_readpreference_t *)zend_object_store_get_object(return_value TSRMLS_CC);
	intern->read_preference = mongoc_read_prefs_copy(read_prefs);
}
/* }}} */

void phongo_writeconcern_init(zval *return_value, const mongoc_write_concern_t *write_concern TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcern_t *intern;

	object_init_ex(return_value, php_phongo_writeconcern_ce);

	intern = (php_phongo_writeconcern_t *)zend_object_store_get_object(return_value TSRMLS_CC);
	intern->write_concern = mongoc_write_concern_copy(write_concern);
}
/* }}} */

int32_t phongo_bson_find_as_int32(bson_t *bson, const char *key, int32_t fallback)
{
	bson_iter_t iter;

	if (bson_iter_init_find(&iter, bson, key) && BSON_ITER_HOLDS_INT32(&iter)) {
		return bson_iter_int32(&iter);
	}

	return fallback;
}

bool phongo_bson_find_as_bool(bson_t *bson, const char *key, bool fallback)
{
	bson_iter_t iter;

	if (bson_iter_init_find(&iter, bson, key) && BSON_ITER_HOLDS_BOOL(&iter)) {
		return bson_iter_bool(&iter);
	}

	return fallback;
}

void phongo_bson_iter_as_document(const bson_iter_t  *iter, uint32_t *document_len, const uint8_t **document)
{
   *document = NULL;
   *document_len = 0;

   if (BSON_ITER_HOLDS_DOCUMENT(iter) || BSON_ITER_HOLDS_ARRAY(iter)) {
      memcpy (document_len, (iter->raw + iter->d1), sizeof (*document_len));
      *document_len = BSON_UINT32_FROM_LE (*document_len);
      *document = (iter->raw + iter->d1);
   }
}

bool phongo_query_init(php_phongo_query_t *query, bson_t *filter, bson_t *options TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;

	if (options) {
		query->batch_size = phongo_bson_find_as_int32(options, "batchSize", 0);
		query->limit = phongo_bson_find_as_int32(options, "limit", 0);
		query->skip = phongo_bson_find_as_int32(options, "skip", 0);

		query->flags = 0;
		query->flags |= phongo_bson_find_as_bool(options, "tailable", false)        ? MONGOC_QUERY_TAILABLE_CURSOR   : 0;
		query->flags |= phongo_bson_find_as_bool(options, "slaveOk", false)         ? MONGOC_QUERY_SLAVE_OK          : 0;
		query->flags |= phongo_bson_find_as_bool(options, "oplogReplay", false)     ? MONGOC_QUERY_OPLOG_REPLAY      : 0;
		query->flags |= phongo_bson_find_as_bool(options, "noCursorTimeout", false) ? MONGOC_QUERY_NO_CURSOR_TIMEOUT : 0;
		query->flags |= phongo_bson_find_as_bool(options, "awaitData", false)       ? MONGOC_QUERY_AWAIT_DATA        : 0;
		query->flags |= phongo_bson_find_as_bool(options, "exhaust", false)         ? MONGOC_QUERY_EXHAUST           : 0;
		query->flags |= phongo_bson_find_as_bool(options, "partial", false)         ? MONGOC_QUERY_PARTIAL           : 0;


		if (bson_iter_init_find(&iter, options, "modifiers")) {
			bson_t tmp;
			uint32_t len = 0;
			const uint8_t *data = NULL;

			if (! (BSON_ITER_HOLDS_DOCUMENT (&iter) || BSON_ITER_HOLDS_ARRAY (&iter))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected modifiers to be array or object, %d given", bson_iter_type(&iter));
				return false;
			}

			bson_iter_document(&iter, &len, &data);
			bson_init_static(&tmp, data, len);
			bson_copy_to_excluding_noinit(&tmp, query->query, "nadastrada", NULL);
			bson_destroy (&tmp);
		}

		if (bson_iter_init_find(&iter, options, "projection")) {
			uint32_t len = 0;
			const uint8_t *data = NULL;

			if (! (BSON_ITER_HOLDS_DOCUMENT (&iter) || BSON_ITER_HOLDS_ARRAY (&iter))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected projection to be array or object, %d given", bson_iter_type(&iter));
				return false;
			}

			bson_iter_document(&iter, &len, &data);
			query->selector = bson_new_from_data(data, len);
		}

		if (bson_iter_init_find(&iter, options, "sort")) {
			bson_t tmp;
			uint32_t len = 0;
			const uint8_t *data = NULL;

			if (! (BSON_ITER_HOLDS_DOCUMENT (&iter) || BSON_ITER_HOLDS_ARRAY (&iter))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected sort to be array or object, %d given", bson_iter_type(&iter));
				return false;
			}

			phongo_bson_iter_as_document(&iter, &len, &data);
			bson_init_static(&tmp, data, len);
			bson_append_document(query->query, "$orderby", -1, &tmp);
			bson_destroy(&tmp);
		}
	}

	BSON_APPEND_DOCUMENT(query->query, "$query", filter);
	return true;
} /* }}} */

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
		uint32_t               len;
		const uint8_t         *data;
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

		bson_iter_document(&iter, &len, &data);

		if (!data) {
			return false;
		}

		MAKE_STD_ZVAL(writeconcernerror->info);
		state.zchild = writeconcernerror->info;

		if (!bson_to_zval(data, len, &state)) {
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
	if (bson_iter_init_find(&iter, bson, "errInfo")) {
		bson_t                 info;
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

		MAKE_STD_ZVAL(writeerror->info);
		state.zchild = writeerror->info;

		bson_init(&info);
		bson_append_iter(&info, NULL, 0, &iter);

		if (!bson_to_zval(bson_get_data(&info), info.len, &state)) {
			zval_ptr_dtor(&writeerror->info);
			writeerror->info = NULL;
			return false;
		}
	}
	if (bson_iter_init_find(&iter, bson, "index") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeerror->index = bson_iter_int32(&iter);
	}

	return true;
} /* }}} */

php_phongo_writeresult_t *phongo_writeresult_init(zval *return_value, mongoc_write_result_t *write_result, mongoc_client_t *client, int server_id TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t *writeresult;

	object_init_ex(return_value, php_phongo_writeresult_ce);

	writeresult = (php_phongo_writeresult_t *)zend_object_store_get_object(return_value TSRMLS_CC);
	writeresult->client    = client;
	writeresult->server_id = server_id;

	/* Copy write_results or else it'll get destroyed with the bulk destruction */
#define SCP(field) writeresult->write_result.field = write_result->field
	SCP(omit_nModified);
	SCP(nInserted);
	SCP(nMatched);
	SCP(nModified);
	SCP(nRemoved);
	SCP(nUpserted);

	bson_copy_to(&write_result->upserted,           &writeresult->write_result.upserted);
	SCP(n_writeConcernErrors);
	bson_copy_to(&write_result->writeConcernErrors, &writeresult->write_result.writeConcernErrors);
	bson_copy_to(&write_result->writeErrors,        &writeresult->write_result.writeErrors);
	SCP(upsert_append_count);
#undef SCP

	return writeresult;

} /* }}} */
/* }}} */

/* {{{ CRUD */
/* Splits a namespace name into the database and collection names, allocated with estrdup. */
bool phongo_split_namespace(const char *namespace, char **dbname, char **cname) /* {{{ */
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

mongoc_bulk_operation_t *phongo_bulkwrite_init(zend_bool ordered) { /* {{{ */
	return mongoc_bulk_operation_new(ordered);
} /* }}} */

bool phongo_execute_write(mongoc_client_t *client, const char *namespace, mongoc_bulk_operation_t *bulk, const mongoc_write_concern_t *write_concern, int server_id, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	bson_error_t error;
	char *dbname;
	char *collname;
	int success;
	php_phongo_writeresult_t *writeresult;

	if (!phongo_split_namespace(namespace, &dbname, &collname)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s: %s", "Invalid namespace provided", namespace);
		return false;
	}

	mongoc_bulk_operation_set_database(bulk, dbname);
	mongoc_bulk_operation_set_collection(bulk, collname);
	mongoc_bulk_operation_set_client(bulk, client);

	/* If a write concern was not specified, libmongoc will use the client's
	 * write concern; however, we should still fetch it for the write result. */
	if (write_concern) {
		mongoc_bulk_operation_set_write_concern(bulk, write_concern);
	} else {
		write_concern = mongoc_client_get_write_concern(client);
	}

	efree(dbname);
	efree(collname);

	if (server_id > 0) {
		mongoc_bulk_operation_set_hint(bulk, server_id);
	}

	success = mongoc_bulk_operation_execute(bulk, NULL, &error);

	/* Write succeeded and the user doesn't care for the results */
	if (success && !return_value_used) {
		return true;
	}

	/* Check for connection related exceptions */
	if (EG(exception)) {
		return false;
	}

	writeresult = phongo_writeresult_init(return_value, &bulk->result, client, bulk->hint TSRMLS_CC);
	writeresult->write_concern = mongoc_write_concern_copy(write_concern);

	/* The Write failed */
	if (!success) {
		/* The Command itself failed */
		if (bson_empty0(&writeresult->write_result.writeErrors) && bson_empty0(&writeresult->write_result.writeConcernErrors)) {
			/* FIXME: Maybe we can look at write_result.error and not pass error at all? */
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
		} else {
			zval *ex;

			ex = phongo_throw_exception(PHONGO_ERROR_WRITE_FAILED TSRMLS_CC, "BulkWrite error");
			zend_update_property(Z_OBJCE_P(ex), ex, ZEND_STRL("writeResult"), return_value TSRMLS_CC);
		}
		return false;
	}
	return true;
} /* }}} */

int phongo_execute_query(mongoc_client_t *client, const char *namespace, const php_phongo_query_t *query, const mongoc_read_prefs_t *read_preference, int server_id, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	const bson_t *doc = NULL;
	mongoc_cursor_t *cursor;
	char *dbname;
	char *collname;
	mongoc_collection_t *collection;

	if (!phongo_split_namespace(namespace, &dbname, &collname)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s: %s", "Invalid namespace provided", namespace);
		return false;
	}
	collection = mongoc_client_get_collection(client, dbname, collname);
	efree(dbname);
	efree(collname);

	cursor = mongoc_collection_find(collection, query->flags, query->skip, query->limit, query->batch_size, query->query, query->selector, read_preference);
	mongoc_collection_destroy(collection);

	/* mongoc issues a warning we need to catch somehow */
	if (!cursor) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED TSRMLS_CC, "%s", "FIXME: Couldn't create cursor...");
		return false;
	}

	if (server_id > 0) {
		cursor->hint = server_id;
	}
	if (!mongoc_cursor_next(cursor, &doc)) {
		bson_error_t error;

		/* Could simply be no docs, which is not an error */
		if (mongoc_cursor_error(cursor, &error)) {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			mongoc_cursor_destroy(cursor);
			return false;
		}

	}

	if (!return_value_used) {
		mongoc_cursor_destroy(cursor);
		return true;
	}

	phongo_cursor_init(return_value, cursor, client TSRMLS_CC);
	return true;
} /* }}} */

int phongo_execute_command(mongoc_client_t *client, const char *db, const bson_t *command, const mongoc_read_prefs_t *read_preference, int server_id, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_iter_t iter;
	bson_iter_t child;


	cursor = mongoc_client_command(client, db, MONGOC_QUERY_NONE, 0, 1, 0, command, NULL, read_preference);
	if (server_id > 0) {
		cursor->hint = server_id;
	}

	if (!mongoc_cursor_next(cursor, &doc)) {
		bson_error_t error;

		if (mongoc_cursor_error(cursor, &error)) {
			mongoc_cursor_destroy(cursor);
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			return false;
		}
	}

	if (!return_value_used) {
		mongoc_cursor_destroy(cursor);
		return true;
	}

	/* This code is adapated from _mongoc_cursor_cursorid_prime(), but we avoid
	 * advancing the cursor, since we are already positioned at the first result
	 * after the error checking above. */
	if (bson_iter_init_find(&iter, doc, "cursor") && BSON_ITER_HOLDS_DOCUMENT(&iter) && bson_iter_recurse(&iter, &child)) {
		mongoc_cursor_cursorid_t *cid;

		_mongoc_cursor_cursorid_init(cursor);
		cursor->limit = 0;

		cid = cursor->iface_data;
		cid->has_cursor = true;

		while (bson_iter_next(&child)) {
			if (BSON_ITER_IS_KEY(&child, "id")) {
				cursor->rpc.reply.cursor_id = bson_iter_as_int64(&child);
			} else if (BSON_ITER_IS_KEY(&child, "ns")) {
				const char *ns;

				ns = bson_iter_utf8(&child, &cursor->nslen);
				bson_strncpy(cursor->ns, ns, sizeof cursor->ns);
			} else if (BSON_ITER_IS_KEY(&child, "firstBatch")) {
				if (BSON_ITER_HOLDS_ARRAY(&child) && bson_iter_recurse(&child, &cid->first_batch_iter)) {
					cid->in_first_batch = true;
				}
			}
		}

		cursor->is_command = false;

		/* The cursor's current element is the command's response document.
		 * Advance once so that the cursor is positioned at the first document
		 * within the command cursor's result set.
		 */
		mongoc_cursor_next(cursor, &doc);
	}

	phongo_cursor_init(return_value, cursor, client TSRMLS_CC);
	return true;
} /* }}} */

/* }}} */

/* {{{ Stream vtable */
void phongo_stream_destroy(mongoc_stream_t *stream_wrap) /* {{{ */
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream_wrap;

	if (base_stream->stream) {
		MONGOC_DEBUG("Not destroying RSRC#%d", base_stream->stream->rsrc_id);
	} else {
		MONGOC_DEBUG("Wrapped stream already destroyed");
	}
	/*
	 * DON'T DO ANYTHING TO THE INTERNAL base_stream->stream
	 * The stream should not be closed during normal dtor -- as we want it to
	 * survive until next request.
	 * We only clean it up on failure and (implicitly) MSHUTDOWN
	 */

	efree(base_stream);
} /* }}} */
void phongo_stream_failed(mongoc_stream_t *stream_wrap) /* {{{ */
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream_wrap;

	if (base_stream->stream) {
		TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

		MONGOC_DEBUG("Destroying RSRC#%d", base_stream->stream->rsrc_id);
		php_stream_free(base_stream->stream, PHP_STREAM_FREE_CLOSE_PERSISTENT | PHP_STREAM_FREE_RSRC_DTOR);
		base_stream->stream = NULL;
	}

	efree(base_stream);
} /* }}} */

int phongo_stream_close(mongoc_stream_t *stream_wrap) /* {{{ */
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream_wrap;

	MONGOC_DEBUG("Closing RSRC#%d", base_stream->stream->rsrc_id);
	if (base_stream->stream) {
		TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

		MONGOC_DEBUG("Destroying RSRC#%d", base_stream->stream->rsrc_id);
		php_stream_free(base_stream->stream, PHP_STREAM_FREE_CLOSE_PERSISTENT | PHP_STREAM_FREE_RSRC_DTOR);
		base_stream->stream = NULL;
	}

	return 0;
} /* }}} */

void php_phongo_set_timeout(php_phongo_stream_socket *base_stream, int32_t timeout_msec) /* {{{ */
{
	struct timeval rtimeout = {0, 0};
	TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

	if (timeout_msec > 0) {
		rtimeout.tv_sec = timeout_msec / 1000;
		rtimeout.tv_usec = (timeout_msec % 1000) * 1000;
	}

	php_stream_set_option(base_stream->stream, PHP_STREAM_OPTION_READ_TIMEOUT, 0, &rtimeout);
	MONGOC_DEBUG("Setting timeout to: %d", timeout_msec);
} /* }}} */

/* This is blatantr copy of _mongoc_stream_tls_writev
 * https://github.com/mongodb/mongo-c-driver/blob/4ebba3d84286df3867bad89358eb6ae956e62a59/src/mongoc/mongoc-stream-tls.c#L500
 */
ssize_t phongo_stream_writev(mongoc_stream_t *stream, mongoc_iovec_t *iov, size_t iovcnt, int32_t timeout_msec) /* {{{ */
{
	char buf[PHONGO_STREAM_BUFFER_SIZE];

	ssize_t ret = 0;
	ssize_t child_ret;
	size_t i;
	size_t iov_pos = 0;

	/* There's a bit of a dance to coalesce vectorized writes into
	 * PHONGO_STREAM_BUFFER_SIZE'd writes to avoid lots of small tls
	 * packets.
	 *
	 * The basic idea is that we want to combine writes in the buffer if they're
	 * smaller than the buffer, flushing as it gets full.  For larger writes, or
	 * the last write in the iovec array, we want to ignore the buffer and just
	 * write immediately.  We take care of doing buffer writes by re-invoking
	 * ourself with a single iovec_t, pointing at our stack buffer.
	 */
	char *buf_head = buf;
	char *buf_tail = buf;
	char *buf_end = buf + PHONGO_STREAM_BUFFER_SIZE;
	size_t bytes;

	char *to_write = NULL;
	size_t to_write_len;

	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream;
	TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);


	php_phongo_set_timeout(base_stream, timeout_msec);


	BSON_ASSERT (iov);
	BSON_ASSERT (iovcnt);

	for (i = 0; i < iovcnt; i++) {
		iov_pos = 0;

		while (iov_pos < iov[i].iov_len) {
			if (buf_head != buf_tail ||
					((i + 1 < iovcnt) &&
					 ((buf_end - buf_tail) > (iov[i].iov_len - iov_pos)))) {
				/* If we have either of:
				 *   - buffered bytes already
				 *   - another iovec to send after this one and we don't have more
				 *     bytes to send than the size of the buffer.
				 *
				 * copy into the buffer */

				bytes = BSON_MIN (iov[i].iov_len - iov_pos, buf_end - buf_tail);

				memcpy (buf_tail, iov[i].iov_base + iov_pos, bytes);
				buf_tail += bytes;
				iov_pos += bytes;

				if (buf_tail == buf_end) {
					/* If we're full, request send */

					to_write = buf_head;
					to_write_len = buf_tail - buf_head;

					buf_tail = buf_head = buf;
				}
			} else {
				/* Didn't buffer, so just write it through */

				to_write = (char *)iov[i].iov_base + iov_pos;
				to_write_len = iov[i].iov_len - iov_pos;

				iov_pos += to_write_len;
			}

			if (to_write) {
				/* We get here if we buffered some bytes and filled the buffer, or
				 * if we didn't buffer and have to send out of the iovec */

				child_ret = php_stream_write(base_stream->stream, to_write, to_write_len);

				if (child_ret < 0) {
					/* Buffer write failed, just return the error */
					return child_ret;
				}

				ret += child_ret;

				if (child_ret < to_write_len) {
					/* we timed out, so send back what we could send */

					return ret;
				}

				to_write = NULL;
			}
		}
	}

	if (buf_head != buf_tail) {
		/* If we have any bytes buffered, send */

		child_ret = php_stream_write(base_stream->stream, buf_head, buf_tail - buf_head);

		if (child_ret < 0) {
			return child_ret;
		}

		ret += child_ret;
	}


	return ret;
} /* }}} */

ssize_t phongo_stream_readv(mongoc_stream_t *stream, mongoc_iovec_t *iov, size_t iovcnt, size_t min_bytes, int32_t timeout_msec) /* {{{ */
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream;
	ssize_t ret = 0;
	ssize_t read;
	size_t cur = 0;
	TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

	php_phongo_set_timeout(base_stream, timeout_msec);

	do {
		read = php_stream_read(base_stream->stream, iov[cur].iov_base, iov[cur].iov_len);
		MONGOC_DEBUG("Reading got: %zd wanted: %zd", read, min_bytes);

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

bool phongo_stream_socket_check_closed(mongoc_stream_t *stream) /* {{{ */
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream;
	TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

	return PHP_STREAM_OPTION_RETURN_OK == php_stream_set_option(base_stream->stream, PHP_STREAM_OPTION_CHECK_LIVENESS, 0, NULL);
} /* }}} */

mongoc_stream_t* phongo_stream_get_base_stream(mongoc_stream_t *stream) /* {{{ */
{
	return (mongoc_stream_t *) stream;
} /* }}} */

ssize_t phongo_stream_poll (mongoc_stream_poll_t *streams, size_t nstreams, int32_t timeout) /* {{{ */
{
	php_pollfd *fds = NULL;
	size_t i;
	ssize_t rval = -1;
	TSRMLS_FETCH();

	fds = emalloc(sizeof(*fds) * nstreams);
	for (i = 0; i < nstreams; i++) {
		php_socket_t this_fd;

		if (php_stream_cast(((php_phongo_stream_socket *)streams[i].stream)->stream, PHP_STREAM_AS_FD_FOR_SELECT | PHP_STREAM_CAST_INTERNAL, (void*)&this_fd, 0) == SUCCESS && this_fd >= 0) {
			fds[i].fd = this_fd;
			fds[i].events = streams[i].events;
			fds[i].revents = 0;
		}
	}

	rval = php_poll2(fds, nstreams, timeout);

	if (rval > 0) {
		for (i = 0; i < nstreams; i++) {
			streams[i].revents = fds[i].revents;
		}
	}

	efree(fds);

	return rval;
} /* }}} */

#if PHP_VERSION_ID < 50600
int php_mongo_verify_hostname(const char *hostname, X509 *cert TSRMLS_DC)
{
	if (php_mongo_matches_san_list(cert, hostname) == SUCCESS) {
		return SUCCESS;
	}

	if (php_mongo_matches_common_name(cert, hostname TSRMLS_CC) == SUCCESS) {
		return SUCCESS;
	}

	return FAILURE;
}

int php_phongo_peer_verify(php_stream *stream, X509 *cert, const char *hostname, bson_error_t *error TSRMLS_DC)
{
	zval **verify_peer_name;

	/* This option is available since PHP 5.6.0 */
	if (php_stream_context_get_option(stream->context, "ssl", "verify_peer_name", &verify_peer_name) == SUCCESS && zend_is_true(*verify_peer_name)) {
		zval **zhost = NULL;
		const char *peer;

		if (php_stream_context_get_option(stream->context, "ssl", "peer_name", &zhost) == SUCCESS) {
			convert_to_string_ex(zhost);
			peer = Z_STRVAL_PP(zhost);
		} else {
			peer = hostname;
		}

		if (php_mongo_verify_hostname(peer, cert TSRMLS_CC) == FAILURE) {
			bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Remote certificate SubjectAltName or CN does not match '%s'", hostname);
			return false;
		}
	}

	return true;
}
#endif

bool php_phongo_ssl_verify(php_stream *stream, const char *hostname, bson_error_t *error TSRMLS_DC)
{
	zval **zcert;
	zval **verify_expiry;
	int resource_type;
	X509 *cert;
	int type;

	if (!stream->context) {
		return true;
	}

	if (!(php_stream_context_get_option(stream->context, "ssl", "peer_certificate", &zcert) == SUCCESS && Z_TYPE_PP(zcert) == IS_RESOURCE)) {
		bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Could not capture certificate of %s", hostname);
		return false;
	}



	zend_list_find(Z_LVAL_PP(zcert), &resource_type);
	cert = (X509 *)zend_fetch_resource(zcert TSRMLS_CC, -1, "OpenSSL X.509", &type, 1, resource_type);

	if (!cert) {
		bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Could not get certificate of %s", hostname);
		return false;
	}

#if PHP_VERSION_ID < 50600
	if (!php_phongo_peer_verify(stream, cert, hostname, error TSRMLS_CC)) {
		return false;
	}
#endif

	if (php_stream_context_get_option(stream->context, "ssl", "verify_expiry", &verify_expiry) == SUCCESS && zend_is_true(*verify_expiry)) {
		time_t current     = time(NULL);
		time_t valid_from  = php_mongo_asn1_time_to_time_t(X509_get_notBefore(cert) TSRMLS_CC);
		time_t valid_until = php_mongo_asn1_time_to_time_t(X509_get_notAfter(cert) TSRMLS_CC);

		if (valid_from > current) {
			bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Certificate is not valid yet on %s", hostname);
			return false;
		}
		if (current > valid_until) {
			bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Certificate has expired on %s", hostname);
			return false;
		}
	}

	return true;
}

mongoc_stream_t* phongo_stream_initiator(const mongoc_uri_t *uri, const mongoc_host_list_t *host, void *user_data, bson_error_t *error) /* {{{ */
{
	php_phongo_stream_socket *base_stream = NULL;
	php_stream *stream = NULL;
	const bson_t *options;
	bson_iter_t iter;
	struct timeval timeout = {0, 0};
	struct timeval *timeoutp = NULL;
	char *uniqid;
	char *errmsg = NULL;
	int errcode;
	char *dsn;
	int dsn_len;
	TSRMLS_FETCH();

	ENTRY;

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
			RETURN(NULL);
	}

	options = mongoc_uri_get_options(uri);

	if (bson_iter_init_find (&iter, options, "connecttimeoutms") && BSON_ITER_HOLDS_INT32 (&iter)) {
		int32_t connecttimeoutms = MONGOC_DEFAULT_CONNECTTIMEOUTMS;

		if (!(connecttimeoutms = bson_iter_int32(&iter))) {
			connecttimeoutms = MONGOC_DEFAULT_CONNECTTIMEOUTMS;
		}

		timeout.tv_sec = connecttimeoutms / 1000;
		timeout.tv_usec = (connecttimeoutms % 1000) * 1000;

		timeoutp = &timeout;
	}

	spprintf(&uniqid, 0, "%s:%d[%s]", host->host, host->port, mongoc_uri_get_string(uri));

	MONGOC_DEBUG("Connecting to '%s'", uniqid);
	stream = php_stream_xport_create(dsn, dsn_len, 0, STREAM_XPORT_CLIENT | STREAM_XPORT_CONNECT | STREAM_XPORT_CONNECT_ASYNC, uniqid, timeoutp, (php_stream_context *)user_data, &errmsg, &errcode);

	if (!stream) {
		bson_set_error (error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Failed connecting to '%s:%d': %s", host->host, host->port, errmsg);
		efree(dsn);
		efree(uniqid);
		if (errmsg) {
			efree(errmsg);
		}
		RETURN(NULL);
	}
	php_stream_auto_cleanup(stream);

	MONGOC_DEBUG("Created: RSRC#%d as '%s'", stream->rsrc_id, uniqid);
	efree(uniqid);

	if (mongoc_uri_get_ssl(uri)) {
		zend_error_handling       error_handling;

		zend_replace_error_handling(EH_THROW, php_phongo_sslconnectionexception_ce, &error_handling TSRMLS_CC);

		MONGOC_DEBUG("Enabling SSL");

		/* Capture the server certificate so we can do further verification */
		if (stream->context) {
			zval capture;
			ZVAL_BOOL(&capture, 1);
			php_stream_context_set_option(stream->context, "ssl", "capture_peer_cert", &capture);
		}

		if (php_stream_xport_crypto_setup(stream, PHONGO_CRYPTO_METHOD, NULL TSRMLS_CC) < 0) {
			zend_restore_error_handling(&error_handling TSRMLS_CC);
			php_stream_free(stream, PHP_STREAM_FREE_CLOSE_PERSISTENT | PHP_STREAM_FREE_RSRC_DTOR);
			bson_set_error (error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_INVALID_TYPE, "Failed to setup crypto, is the OpenSSL extension loaded?");
			efree(dsn);
			return NULL;
		}

		if (php_stream_xport_crypto_enable(stream, 1 TSRMLS_CC) < 0) {
			zend_restore_error_handling(&error_handling TSRMLS_CC);
			php_stream_free(stream, PHP_STREAM_FREE_CLOSE_PERSISTENT | PHP_STREAM_FREE_RSRC_DTOR);
			bson_set_error (error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_INVALID_TYPE, "Failed to setup crypto, is the server running with SSL?");
			efree(dsn);
			return NULL;
		}

		if (!php_phongo_ssl_verify(stream, host->host, error TSRMLS_CC)) {
			zend_restore_error_handling(&error_handling TSRMLS_CC);
			php_stream_pclose(stream);
			efree(dsn);
			return NULL;
		}

		zend_restore_error_handling(&error_handling TSRMLS_CC);
	}
	efree(dsn);


	/* We only need the context really for SSL initialization, safe to remove now */
	php_stream_context_set(stream, NULL);

	base_stream = ecalloc(1, sizeof(php_phongo_stream_socket));
	base_stream->stream = stream;
	base_stream->uri = uri;
	base_stream->host = host;
	TSRMLS_SET_CTX(base_stream->tsrm_ls);

	/* flush missing, doesn't seem to be used */
	base_stream->vtable.type = 100;
	base_stream->vtable.destroy = phongo_stream_destroy;
	base_stream->vtable.failed  = phongo_stream_failed;
	base_stream->vtable.close = phongo_stream_close;
	base_stream->vtable.writev = phongo_stream_writev;
	base_stream->vtable.readv = phongo_stream_readv;
	base_stream->vtable.setsockopt = phongo_stream_setsockopt;
	base_stream->vtable.check_closed = phongo_stream_socket_check_closed;
	base_stream->vtable.get_base_stream = phongo_stream_get_base_stream;
	base_stream->vtable.poll = phongo_stream_poll;

	if (host->family != AF_UNIX) {
		int flag = 1;

		if (phongo_stream_setsockopt((mongoc_stream_t *)base_stream, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int))) {
			MONGOC_WARNING("setsockopt TCP_NODELAY failed");
		}
	}

	RETURN((mongoc_stream_t *)base_stream);
} /* }}} */

/* }}} */

/* {{{ mongoc types from from_zval */
const mongoc_write_concern_t* phongo_write_concern_from_zval(zval *zwrite_concern TSRMLS_DC) /* {{{ */
{
	if (zwrite_concern) {
		php_phongo_writeconcern_t *intern = (php_phongo_writeconcern_t *)zend_object_store_get_object(zwrite_concern TSRMLS_CC);

		if (intern) {
			return intern->write_concern;
		}
	}

	return NULL;
} /* }}} */

const mongoc_read_prefs_t* phongo_read_preference_from_zval(zval *zread_preference TSRMLS_DC) /* {{{ */
{
	if (zread_preference) {
		php_phongo_readpreference_t *intern = (php_phongo_readpreference_t *)zend_object_store_get_object(zread_preference TSRMLS_CC);

		if (intern) {
			return intern->read_preference;
		}
	}

	return NULL;
} /* }}} */

const php_phongo_query_t* phongo_query_from_zval(zval *zquery TSRMLS_DC) /* {{{ */
{
	php_phongo_query_t *intern = (php_phongo_query_t *)zend_object_store_get_object(zquery TSRMLS_CC);

	return intern;
} /* }}} */
/* }}} */

/* {{{ phongo zval from mongoc types */
void php_phongo_cursor_id_new_from_id(zval *object, int64_t cursorid TSRMLS_DC) /* {{{ */
{
	php_phongo_cursorid_t     *intern;

	object_init_ex(object, php_phongo_cursorid_ce);

	intern = (php_phongo_cursorid_t *)zend_object_store_get_object(object TSRMLS_CC);
	intern->id = cursorid;
} /* }}} */

void php_phongo_objectid_new_from_oid(zval *object, const bson_oid_t *oid TSRMLS_DC) /* {{{ */
{
	php_phongo_objectid_t     *intern;

	object_init_ex(object, php_phongo_objectid_ce);

	intern = (php_phongo_objectid_t *)zend_object_store_get_object(object TSRMLS_CC);
	bson_oid_to_string(oid, intern->oid);
} /* }}} */

void php_phongo_server_to_zval(zval *retval, const mongoc_server_description_t *sd) /* {{{ */
{
	array_init(retval);

	add_assoc_string_ex(retval, ZEND_STRS("host"), (char *)sd->host.host, 1);
	add_assoc_long_ex(retval, ZEND_STRS("port"), sd->host.port);
	add_assoc_long_ex(retval, ZEND_STRS("type"), sd->type);
	add_assoc_bool_ex(retval, ZEND_STRS("is_primary"), sd->type == MONGOC_SERVER_RS_PRIMARY);
	add_assoc_bool_ex(retval, ZEND_STRS("is_secondary"), sd->type == MONGOC_SERVER_RS_SECONDARY);
	add_assoc_bool_ex(retval, ZEND_STRS("is_arbiter"), sd->type == MONGOC_SERVER_RS_ARBITER);
	{
		bson_iter_t iter;
		zend_bool b = bson_iter_init_find_case(&iter, &sd->last_is_master, "hidden") && bson_iter_as_bool(&iter);

		add_assoc_bool_ex(retval, ZEND_STRS("is_hidden"), b);
	}
	{
		bson_iter_t iter;
		zend_bool b = bson_iter_init_find_case(&iter, &sd->last_is_master, "passive") && bson_iter_as_bool(&iter);

		add_assoc_bool_ex(retval, ZEND_STRS("is_passive"), b);
	}
	if (sd->tags.len) {
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;
		/* Use native arrays for debugging output */
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		MAKE_STD_ZVAL(state.zchild);
		bson_to_zval(bson_get_data(&sd->tags), sd->tags.len, &state);
		add_assoc_zval_ex(retval, ZEND_STRS("tags"), state.zchild);
	}
	{
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;
		/* Use native arrays for debugging output */
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		MAKE_STD_ZVAL(state.zchild);
		bson_to_zval(bson_get_data(&sd->last_is_master), sd->last_is_master.len, &state);
		add_assoc_zval_ex(retval, ZEND_STRS("last_is_master"), state.zchild);
	}
	add_assoc_long_ex(retval, ZEND_STRS("round_trip_time"), sd->round_trip_time);

} /* }}} */

void php_phongo_read_preference_to_zval(zval *retval, const mongoc_read_prefs_t *read_prefs) /* {{{ */
{

	array_init_size(retval, 2);

	add_assoc_long_ex(retval, ZEND_STRS("mode"), read_prefs->mode);
	if (read_prefs->tags.len) {
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;
		/* Use native arrays for debugging output */
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		MAKE_STD_ZVAL(state.zchild);
		bson_to_zval(bson_get_data(&read_prefs->tags), read_prefs->tags.len, &state);
		add_assoc_zval_ex(retval, ZEND_STRS("tags"), state.zchild);
	} else {
		add_assoc_null_ex(retval, ZEND_STRS("tags"));
	}
} /* }}} */

void php_phongo_write_concern_to_zval(zval *retval, const mongoc_write_concern_t *write_concern) /* {{{ */
{
	const char *wtag = mongoc_write_concern_get_wtag(write_concern);
	const int32_t w = mongoc_write_concern_get_w(write_concern);

	array_init_size(retval, 4);

	if (wtag) {
		add_assoc_string_ex(retval, ZEND_STRS("w"), (char *)wtag, 1);
	} else if (mongoc_write_concern_get_wmajority(write_concern)) {
		add_assoc_string_ex(retval, ZEND_STRS("w"), (char *)PHONGO_WRITE_CONCERN_W_MAJORITY, 1);
	} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
		add_assoc_long_ex(retval, ZEND_STRS("w"), w);
	} else {
		add_assoc_null_ex(retval, ZEND_STRS("w"));
	}

	add_assoc_bool_ex(retval, ZEND_STRS("wmajority"), mongoc_write_concern_get_wmajority(write_concern));
	add_assoc_long_ex(retval, ZEND_STRS("wtimeout"), mongoc_write_concern_get_wtimeout(write_concern));

	if (write_concern->journal != MONGOC_WRITE_CONCERN_JOURNAL_DEFAULT) {
		add_assoc_bool_ex(retval, ZEND_STRS("journal"), mongoc_write_concern_get_journal(write_concern));
	} else {
		add_assoc_null_ex(retval, ZEND_STRS("journal"));
	}
} /* }}} */

void php_phongo_cursor_to_zval(zval *retval, php_phongo_cursor_t *cursor) /* {{{ */
{

	array_init_size(retval, 4);

	if (cursor->cursor) {
		zval *zcursor = NULL;

		MAKE_STD_ZVAL(zcursor);
		array_init_size(zcursor, 19);

		add_assoc_long_ex(zcursor, ZEND_STRS("stamp"), cursor->cursor->stamp);

#define _ADD_BOOL(z, field) add_assoc_bool_ex(z, ZEND_STRS(#field), cursor->cursor->field)
		_ADD_BOOL(zcursor, is_command);
		_ADD_BOOL(zcursor, sent);
		_ADD_BOOL(zcursor, done);
		_ADD_BOOL(zcursor, failed);
		_ADD_BOOL(zcursor, end_of_event);
		_ADD_BOOL(zcursor, in_exhaust);
		_ADD_BOOL(zcursor, redir_primary);
		_ADD_BOOL(zcursor, has_fields);
#undef _ADD_BOOL

		/* Avoid using PHONGO_TYPEMAP_NATIVE_ARRAY for decoding query, selector,
		 * and current documents so that users can differentiate BSON arrays
		 * and documents. */
		{
			php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

			MAKE_STD_ZVAL(state.zchild);
			bson_to_zval(bson_get_data(&cursor->cursor->query), cursor->cursor->query.len, &state);
			add_assoc_zval_ex(zcursor, ZEND_STRS("query"), state.zchild);
		}
		{
			php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

			MAKE_STD_ZVAL(state.zchild);
			bson_to_zval(bson_get_data(&cursor->cursor->fields), cursor->cursor->fields.len, &state);
			add_assoc_zval_ex(zcursor, ZEND_STRS("fields"), state.zchild);
		}
		{
			zval *read_preference = NULL;

			MAKE_STD_ZVAL(read_preference);
			php_phongo_read_preference_to_zval(read_preference, cursor->cursor->read_prefs);
			add_assoc_zval_ex(zcursor, ZEND_STRS("read_preference"), read_preference);
		}

#define _ADD_INT(z, field) add_assoc_long_ex(z, ZEND_STRS(#field), cursor->cursor->field)
		_ADD_INT(zcursor, flags);
		_ADD_INT(zcursor, skip);
		_ADD_INT(zcursor, limit);
		_ADD_INT(zcursor, count);
		_ADD_INT(zcursor, batch_size);
#undef _ADD_INT

		add_assoc_string_ex(zcursor, ZEND_STRS("ns"), cursor->cursor->ns, 1);
		if (cursor->cursor->current) {
			php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

			MAKE_STD_ZVAL(state.zchild);
			bson_to_zval(bson_get_data(cursor->cursor->current), cursor->cursor->current->len, &state);
			add_assoc_zval_ex(zcursor, ZEND_STRS("current_doc"), state.zchild);
		}
		add_assoc_zval_ex(retval, ZEND_STRS("cursor"), zcursor);
	} else {
		add_assoc_null_ex(retval, ZEND_STRS("cursor"));
	}

	add_assoc_long_ex(retval, ZEND_STRS("server_id"), cursor->server_id);

} /* }}} */
/* }}} */


mongoc_uri_t *php_phongo_make_uri(const char *uri_string, bson_t *options) /* {{{ */
{
	bson_iter_t   iter;
	mongoc_uri_t *uri;

	uri = mongoc_uri_new(uri_string);
	MONGOC_DEBUG("Connection string: '%s'", uri_string);

	if (!uri) {
		return NULL;
	}

	if (options && bson_iter_init(&iter, options)) {
		while (bson_iter_next (&iter)) {
			const char *key = bson_iter_key(&iter);

			/* Skip read preference and write concern options, as those must be
			 * processed after the mongoc_client_t is constructed. */
			if (!strcasecmp(key, "journal") ||
			    !strcasecmp(key, "readpreference") ||
			    !strcasecmp(key, "readpreferencetags") ||
			    !strcasecmp(key, "safe") ||
			    !strcasecmp(key, "slaveok") ||
			    !strcasecmp(key, "w") ||
			    !strcasecmp(key, "wtimeoutms")) {
				continue;
			}

			if (mongoc_uri_option_is_bool(key)) {
				mongoc_uri_set_option_as_bool (uri, key, bson_iter_as_bool(&iter));
			}
			else if (mongoc_uri_option_is_int32(key) && BSON_ITER_HOLDS_INT32(&iter)) {
				mongoc_uri_set_option_as_int32 (uri, key, bson_iter_int32 (&iter));
			}
			else if (mongoc_uri_option_is_utf8(key) && BSON_ITER_HOLDS_UTF8(&iter)) {
				mongoc_uri_set_option_as_utf8(uri, key, bson_iter_utf8 (&iter, NULL));
			}
			else if (BSON_ITER_HOLDS_ARRAY(&iter) && !strcasecmp(key, "hosts")) {
				bson_iter_t sub;

				bson_iter_recurse(&iter, &sub);
				while (bson_iter_next (&sub)) {
					if (BSON_ITER_HOLDS_UTF8(&sub)) {
						mongoc_uri_parse_host(uri, bson_iter_utf8(&sub, NULL));
					}
				}
			}
			else if (BSON_ITER_HOLDS_UTF8(&iter)) {
				const char *value = bson_iter_utf8 (&iter, NULL);

				if (!strcasecmp(key, "username")) {
					mongoc_uri_set_username(uri, value);
				} else if (!strcasecmp(key, "password")) {
					mongoc_uri_set_password(uri, value);
				} else if (!strcasecmp(key, "database")) {
					mongoc_uri_set_database(uri, value);
				} else if (!strcasecmp(key, "authsource")) {
					mongoc_uri_set_auth_source(uri, value);
				}
			}
		}
	}

	return uri;
} /* }}} */

void php_phongo_populate_default_ssl_ctx(php_stream_context *ctx, zval *driverOptions) /* {{{ */
{
	zval                     **tmp;

#define SET_STRING_CTX(name) \
		if (driverOptions && php_array_exists(driverOptions, name)) { \
			zval ztmp; \
			zend_bool ctmp_free; \
			int ctmp_len; \
			char *ctmp; \
			ctmp = php_array_fetchl_string(driverOptions, name, sizeof(name)-1, &ctmp_len, &ctmp_free); \
			ZVAL_STRING(&ztmp, ctmp, ctmp_free); \
			php_stream_context_set_option(ctx, "ssl", name, &ztmp); \
		}

#define SET_BOOL_CTX(name, defaultvalue) \
		{ \
			zval ztmp; \
			if (driverOptions && php_array_exists(driverOptions, name)) { \
				ZVAL_BOOL(&ztmp, php_array_fetchl_bool(driverOptions, ZEND_STRL(name))); \
				php_stream_context_set_option(ctx, "ssl", name, &ztmp); \
			} \
			else if (php_stream_context_get_option(ctx, "ssl", name, &tmp) == FAILURE) { \
				ZVAL_BOOL(&ztmp, defaultvalue); \
				php_stream_context_set_option(ctx, "ssl", name, &ztmp); \
			} \
		}

		SET_BOOL_CTX("verify_peer", 1);
		SET_BOOL_CTX("verify_peer_name", 1);
		SET_BOOL_CTX("verify_hostname", 1);
		SET_BOOL_CTX("verify_expiry", 1);
		SET_BOOL_CTX("allow_self_signed", 0);

		SET_STRING_CTX("peer_name");
		SET_STRING_CTX("local_pk");
		SET_STRING_CTX("local_cert");
		SET_STRING_CTX("cafile");
		SET_STRING_CTX("capath");
		SET_STRING_CTX("passphrase");
		SET_STRING_CTX("ciphers");
#undef SET_BOOL_CTX
#undef SET_STRING_CTX
} /* }}} */

bool php_phongo_apply_rp_options_to_client(mongoc_client_t *client, bson_t *options TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	mongoc_read_prefs_t *new_rp;
	const mongoc_read_prefs_t *old_rp;

	if (!(old_rp = mongoc_client_get_read_prefs(client))) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED TSRMLS_CC, "Client does not have a read preference");

		return false;
	}

	/* Return early if there are no options to apply */
	if (bson_empty0(options)) {
		return true;
	}

	if (!bson_iter_init_find_case(&iter, options, "slaveok") &&
	    !bson_iter_init_find_case(&iter, options, "readpreference") &&
	    !bson_iter_init_find_case(&iter, options, "readpreferencetags")) {
		return true;
	}

	new_rp = mongoc_read_prefs_copy(old_rp);

	if (bson_iter_init_find_case(&iter, options, "slaveok") && BSON_ITER_HOLDS_BOOL(&iter)) {
		mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_SECONDARY_PREFERRED);
	}

	if (bson_iter_init_find_case(&iter, options, "readpreference") && BSON_ITER_HOLDS_UTF8(&iter)) {
		const char *str = bson_iter_utf8(&iter, NULL);

		if (0 == strcasecmp("primary", str)) {
			mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_PRIMARY);
		} else if (0 == strcasecmp("primarypreferred", str)) {
			mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_PRIMARY_PREFERRED);
		} else if (0 == strcasecmp("secondary", str)) {
			mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_SECONDARY);
		} else if (0 == strcasecmp("secondarypreferred", str)) {
			mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_SECONDARY_PREFERRED);
		} else if (0 == strcasecmp("nearest", str)) {
			mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_NEAREST);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Unsupported readPreference value: '%s'", str);
			mongoc_read_prefs_destroy(new_rp);

			return false;
		}
	}

	if (bson_iter_init_find_case(&iter, options, "readpreferencetags") && BSON_ITER_HOLDS_ARRAY(&iter)) {
		bson_t tags;
		uint32_t len;
		const uint8_t *data;

		bson_iter_array(&iter, &len, &data);

		if (bson_init_static(&tags, data, len)) {
			mongoc_read_prefs_set_tags(new_rp, &tags);
		}
	}

	if (mongoc_read_prefs_get_mode(new_rp) == MONGOC_READ_PRIMARY &&
	    !bson_empty(mongoc_read_prefs_get_tags(new_rp))) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Primary read preference mode conflicts with tags");
		mongoc_read_prefs_destroy(new_rp);

		return false;
	}

	/* This may be redundant in light of the last check (primary with tags), but
	 * we'll check anyway in case additional validation is implemented. */
	if (!mongoc_read_prefs_is_valid(new_rp)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Read preference is not valid");
		mongoc_read_prefs_destroy(new_rp);

		return false;
	}

	mongoc_client_set_read_prefs(client, new_rp);
	mongoc_read_prefs_destroy(new_rp);

	return true;
} /* }}} */

bool php_phongo_apply_wc_options_to_client(mongoc_client_t *client, bson_t *options TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	int32_t wtimeoutms;
	mongoc_write_concern_t *new_wc;
	const mongoc_write_concern_t *old_wc;

	if (!(old_wc = mongoc_client_get_write_concern(client))) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED TSRMLS_CC, "Client does not have a write concern");

		return false;
	}

	/* Return early if there are no options to apply */
	if (bson_empty0(options)) {
		return true;
	}

	if (!bson_iter_init_find_case(&iter, options, "journal") &&
	    !bson_iter_init_find_case(&iter, options, "safe") &&
	    !bson_iter_init_find_case(&iter, options, "w") &&
	    !bson_iter_init_find_case(&iter, options, "wtimeoutms")) {
		return true;
	}

	wtimeoutms = mongoc_write_concern_get_wtimeout(old_wc);

	new_wc = mongoc_write_concern_copy(old_wc);

	if (bson_iter_init_find_case(&iter, options, "safe") && BSON_ITER_HOLDS_BOOL(&iter)) {
		mongoc_write_concern_set_w(new_wc, bson_iter_bool(&iter) ? 1 : MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED);
	}

	if (bson_iter_init_find_case(&iter, options, "wtimeoutms") && BSON_ITER_HOLDS_INT32(&iter)) {
		wtimeoutms = bson_iter_int32(&iter);
	}

	if (bson_iter_init_find_case(&iter, options, "journal") && BSON_ITER_HOLDS_BOOL(&iter)) {
		mongoc_write_concern_set_journal(new_wc, bson_iter_bool(&iter));
	}

	if (bson_iter_init_find_case(&iter, options, "w")) {
		if (BSON_ITER_HOLDS_INT32(&iter)) {
			int32_t value = bson_iter_int32(&iter);

			switch (value) {
				case MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED:
				case MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED:
					mongoc_write_concern_set_w(new_wc, value);
					break;

				default:
					if (value > 0) {
						mongoc_write_concern_set_w(new_wc, value);
						break;
					}
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Unsupported w value: %d", value);
					mongoc_write_concern_destroy(new_wc);

					return false;
			}
		} else if (BSON_ITER_HOLDS_UTF8(&iter)) {
			const char *str = bson_iter_utf8(&iter, NULL);

			if (0 == strcasecmp(PHONGO_WRITE_CONCERN_W_MAJORITY, str)) {
				mongoc_write_concern_set_wmajority(new_wc, wtimeoutms);
			} else {
				mongoc_write_concern_set_wtag(new_wc, str);
			}
		}
	}

	/* Only set wtimeout if it's still applicable; otherwise, clear it. */
	if (mongoc_write_concern_get_w(new_wc) > 1 ||
		mongoc_write_concern_get_wmajority(new_wc) ||
		mongoc_write_concern_get_wtag(new_wc)) {
		mongoc_write_concern_set_wtimeout(new_wc, wtimeoutms);
	} else {
		mongoc_write_concern_set_wtimeout(new_wc, 0);
	}

	if (mongoc_write_concern_get_journal(new_wc)) {
		int32_t w = mongoc_write_concern_get_w(new_wc);

		if (w == MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED || w == MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Journal conflicts with w value: %d", w);
			mongoc_write_concern_destroy(new_wc);

			return false;
		}
	}

	/* This may be redundant in light of the last check (unacknowledged w with
	   journal), but we'll check anyway in case additional validation is
	   implemented. */
	if (!_mongoc_write_concern_is_valid(new_wc)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Write concern is not valid");
		mongoc_write_concern_destroy(new_wc);

		return false;
	}

	mongoc_client_set_write_concern(client, new_wc);
	mongoc_write_concern_destroy(new_wc);

	return true;
} /* }}} */

mongoc_client_t *php_phongo_make_mongo_client(const mongoc_uri_t *uri, zval *driverOptions TSRMLS_DC) /* {{{ */
{
	zval                     **tmp;
	php_stream_context        *ctx;
	const char                *mech, *mongoc_version, *bson_version;
	mongoc_client_t           *client;

	ENTRY;


	if (driverOptions && zend_hash_find(Z_ARRVAL_P(driverOptions), "debug", strlen("debug") + 1, (void**)&tmp) == SUCCESS) {
		convert_to_string(*tmp);

		zend_alter_ini_entry_ex((char *)PHONGO_DEBUG_INI, sizeof(PHONGO_DEBUG_INI), Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp), PHP_INI_USER, PHP_INI_STAGE_RUNTIME, 0 TSRMLS_CC);
	}

	if (driverOptions && zend_hash_find(Z_ARRVAL_P(driverOptions), "context", strlen("context") + 1, (void**)&tmp) == SUCCESS) {
		ctx = php_stream_context_from_zval(*tmp, 0);
	} else {
		GET_DEFAULT_CONTEXT();
	}

	if (mongoc_uri_get_ssl(uri)) {
		php_phongo_populate_default_ssl_ctx(ctx, driverOptions);
	}

#ifdef HAVE_SYSTEM_LIBMONGOC
	mongoc_version = mongoc_get_version();
#else
	mongoc_version = "bundled";
#endif

#ifdef HAVE_SYSTEM_LIBBSON
	bson_version = bson_get_version();
#else
	bson_version = "bundled";
#endif

	MONGOC_DEBUG("Creating Manager, phongo-%s[%s] - mongoc-%s(%s), libbson-%s(%s), php-%s",
		MONGODB_VERSION_S,
		MONGODB_STABILITY_S,
		MONGOC_VERSION_S,
		mongoc_version,
		BSON_VERSION_S,
		bson_version,
		PHP_VERSION
	);
	client = mongoc_client_new_from_uri(uri);

	if (!client) {
		RETURN(NULL);
	}


	mech = mongoc_uri_get_auth_mechanism(uri);

	/* Check if we are doing X509 auth, in which case extract the username (subject) from the cert if no username is provided */
	if (mech && !strcasecmp(mech, "MONGODB-X509") && !mongoc_uri_get_username(uri)) {
		zval **pem;

		if (SUCCESS == php_stream_context_get_option(ctx, "ssl", "local_cert", &pem)) {
			char filename[MAXPATHLEN];

			convert_to_string_ex(pem);
			if (VCWD_REALPATH(Z_STRVAL_PP(pem), filename)) {
				mongoc_ssl_opt_t  ssl_options;

				ssl_options.pem_file = filename;
				mongoc_client_set_ssl_opts(client, &ssl_options);
			}
		}
	}

	mongoc_client_set_stream_initiator(client, phongo_stream_initiator, ctx);

	RETURN(client);
} /* }}} */

void php_phongo_new_utcdatetime_from_epoch(zval *object, int64_t msec_since_epoch TSRMLS_DC) /* {{{ */
{
	php_phongo_utcdatetime_t     *intern;

	object_init_ex(object, php_phongo_utcdatetime_ce);

	intern = (php_phongo_utcdatetime_t *)zend_object_store_get_object(object TSRMLS_CC);
	intern->milliseconds = msec_since_epoch;
} /* }}} */

void php_phongo_new_datetime_from_utcdatetime(zval *object, int64_t milliseconds TSRMLS_DC) /* {{{ */
{
	php_date_obj             *datetime_obj;
	char                     *sec;
	int                       sec_len;

	object_init_ex(object, php_date_get_date_ce());

#ifdef WIN32
	sec_len = spprintf(&sec, 0, "@%I64d", (int64_t) milliseconds / 1000);
#else
	sec_len = spprintf(&sec, 0, "@%lld", (long long int) milliseconds / 1000);
#endif

	datetime_obj = zend_object_store_get_object(object TSRMLS_CC);
	php_date_initialize(datetime_obj, sec, sec_len, NULL, NULL, 0 TSRMLS_CC);
	efree(sec);
	datetime_obj->time->f = milliseconds % 1000;
} /* }}} */
void php_phongo_new_timestamp_from_increment_and_timestamp(zval *object, int32_t increment, int32_t timestamp TSRMLS_DC) /* {{{ */
{
	php_phongo_timestamp_t     *intern;

	object_init_ex(object, php_phongo_timestamp_ce);

	intern = (php_phongo_timestamp_t *)zend_object_store_get_object(object TSRMLS_CC);
	intern->increment = increment;
	intern->timestamp = timestamp;
} /* }}} */
void php_phongo_new_javascript_from_javascript(int init, zval *object, const char *code, size_t code_len TSRMLS_DC) /* {{{ */
{
	php_phongo_new_javascript_from_javascript_and_scope(init, object, code, code_len, NULL TSRMLS_CC);
} /* }}} */
void php_phongo_new_javascript_from_javascript_and_scope(int init, zval *object, const char *code, size_t code_len, const bson_t *scope TSRMLS_DC) /* {{{ */
{
	php_phongo_javascript_t     *intern;

	if (init) {
		object_init_ex(object, php_phongo_javascript_ce);
	}

	intern = (php_phongo_javascript_t *)zend_object_store_get_object(object TSRMLS_CC);
	intern->javascript = estrndup(code, code_len);
	intern->javascript_len = code_len;
	intern->document = scope ? bson_copy(scope) : NULL;
} /* }}} */
void php_phongo_new_binary_from_binary_and_type(zval *object, const char *data, size_t data_len, bson_subtype_t type TSRMLS_DC) /* {{{ */
{
	php_phongo_binary_t     *intern;

	object_init_ex(object, php_phongo_binary_ce);

	intern = (php_phongo_binary_t *)zend_object_store_get_object(object TSRMLS_CC);
	intern->data = estrndup(data, data_len);
	intern->data_len = data_len;
	intern->type = type;
} /* }}} */
void php_phongo_new_regex_from_regex_and_options(zval *object, const char *pattern, const char *flags TSRMLS_DC) /* {{{ */
{
	php_phongo_regex_t     *intern;

	object_init_ex(object, php_phongo_regex_ce);

	intern = (php_phongo_regex_t *)zend_object_store_get_object(object TSRMLS_CC);
	intern->pattern_len = strlen(pattern);
	intern->pattern = estrndup(pattern, intern->pattern_len);
	intern->flags_len = strlen(flags);
	intern->flags = estrndup(flags, intern->flags_len);
} /* }}} */

static void php_phongo_cursor_free_current(php_phongo_cursor_t *cursor) /* {{{ */
{
	if (cursor->visitor_data.zchild) {
		zval_ptr_dtor(&cursor->visitor_data.zchild);
		cursor->visitor_data.zchild = NULL;
	}
} /* }}} */

void php_phongo_cursor_free(php_phongo_cursor_t *cursor) /* {{{ */
{
	if (cursor->cursor) {
		mongoc_cursor_destroy(cursor->cursor);
		cursor->cursor = NULL;
	}

	php_phongo_cursor_free_current(cursor);
} /* }}} */

/* {{{ Iterator */
static void php_phongo_cursor_iterator_dtor(zend_object_iterator *iter TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_iterator *cursor_it = (php_phongo_cursor_iterator *)iter;

	if (cursor_it->intern.data) {
		zval_ptr_dtor((zval**)&cursor_it->intern.data);
		cursor_it->intern.data = NULL;
	}

	efree(cursor_it);
} /* }}} */

static int php_phongo_cursor_iterator_valid(zend_object_iterator *iter TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *cursor = ((php_phongo_cursor_iterator *)iter)->cursor;

	if (cursor->visitor_data.zchild) {
		return SUCCESS;
	}

	return FAILURE;
} /* }}} */

#if PHP_VERSION_ID < 50500
static int php_phongo_cursor_iterator_get_current_key(zend_object_iterator *iter, char **str_key, uint *str_key_len, ulong *int_key TSRMLS_DC) /* {{{ */
{
	*int_key = (ulong) ((php_phongo_cursor_iterator *)iter)->current;
	return HASH_KEY_IS_LONG;
} /* }}} */
#else
static void php_phongo_cursor_iterator_get_current_key(zend_object_iterator *iter, zval *key TSRMLS_DC) /* {{{ */
{
	ZVAL_LONG(key, ((php_phongo_cursor_iterator *)iter)->current);
} /* }}} */
#endif

static void php_phongo_cursor_iterator_get_current_data(zend_object_iterator *iter, zval ***data TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *cursor = ((php_phongo_cursor_iterator *)iter)->cursor;

	*data = &cursor->visitor_data.zchild;
} /* }}} */

static void php_phongo_cursor_iterator_move_forward(zend_object_iterator *iter TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_iterator *cursor_it = (php_phongo_cursor_iterator *)iter;
	php_phongo_cursor_t        *cursor = cursor_it->cursor;
	const bson_t               *doc;

	php_phongo_cursor_free_current(cursor);
	cursor_it->current++;

	if (mongoc_cursor_next(cursor->cursor, &doc)) {
		MAKE_STD_ZVAL(cursor->visitor_data.zchild);
		bson_to_zval(bson_get_data(doc), doc->len, &cursor->visitor_data);
	} else {
		bson_error_t error;

		if (mongoc_cursor_error(cursor->cursor, &error)) {
			/* Intentionally not destroying the cursor as it will happen
			 * naturally now that there are no more results */
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
		}
	}
} /* }}} */

static void php_phongo_cursor_iterator_rewind(zend_object_iterator *iter TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_iterator *cursor_it = (php_phongo_cursor_iterator *)iter;
	php_phongo_cursor_t        *cursor = cursor_it->cursor;
	const bson_t               *doc;

	if (cursor_it->current > 0) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC TSRMLS_CC, "Cursors cannot rewind after starting iteration");
		return;
	}

	php_phongo_cursor_free_current(cursor);

	doc = mongoc_cursor_current(cursor->cursor);

	if (doc) {
		MAKE_STD_ZVAL(cursor->visitor_data.zchild);
		bson_to_zval(bson_get_data(doc), doc->len, &cursor->visitor_data);
	}
} /* }}} */

/* iterator handler table */
zend_object_iterator_funcs php_phongo_cursor_iterator_funcs = {
	php_phongo_cursor_iterator_dtor,
	php_phongo_cursor_iterator_valid,
	php_phongo_cursor_iterator_get_current_data,
	php_phongo_cursor_iterator_get_current_key,
	php_phongo_cursor_iterator_move_forward,
	php_phongo_cursor_iterator_rewind,
	NULL /* invalidate_current is not used */
};

zend_object_iterator *php_phongo_cursor_get_iterator(zend_class_entry *ce, zval *object, int by_ref TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *cursor = zend_object_store_get_object(object TSRMLS_CC);
	php_phongo_cursor_iterator *cursor_it = NULL;

	if (by_ref) {
		zend_error(E_ERROR, "An iterator cannot be used with foreach by reference");
	}

	if (cursor->got_iterator) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC TSRMLS_CC, "Cursors cannot yield multiple iterators");
		return NULL;
	}

	cursor->got_iterator = 1;

	cursor_it = ecalloc(1, sizeof(php_phongo_cursor_iterator));

	Z_ADDREF_P(object);
	cursor_it->intern.data  = (void*)object;
	cursor_it->intern.funcs = &php_phongo_cursor_iterator_funcs;
	cursor_it->cursor = cursor;
	/* cursor_it->current should already be allocated to zero */

	php_phongo_cursor_free_current(cursor_it->cursor);

	return (zend_object_iterator*)cursor_it;
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
	if (mem) {
		efree(mem);
	}
} /* }}} */

/* }}} */

#ifdef PHP_DEBUG
/* LCOV_EXCL_START */
void _phongo_debug_bson(bson_t *bson)
{
	char   *str;
	size_t  str_len;

	str = bson_as_json(bson, &str_len);

	php_printf("JSON: %s\n", str);
	bson_free(str);
}
/* LCOV_EXCL_STOP */
#endif

/* {{{ M[INIT|SHUTDOWN] R[INIT|SHUTDOWN] G[INIT|SHUTDOWN] MINFO INI */

ZEND_INI_MH(OnUpdateDebug)
{
	void ***ctx = NULL;
	char *tmp_dir = NULL;

	TSRMLS_SET_CTX(ctx);

	/* Close any previously open log files */
	if (MONGODB_G(debug_fd)) {
		if (MONGODB_G(debug_fd) != stderr && MONGODB_G(debug_fd) != stdout) {
			fclose(MONGODB_G(debug_fd));
		}
		MONGODB_G(debug_fd) = NULL;
	}

	if (!new_value_length
		|| strcasecmp("0", new_value) == 0
		|| strcasecmp("off", new_value) == 0
		|| strcasecmp("no", new_value) == 0
		|| strcasecmp("false", new_value) == 0
	   ) {
		mongoc_log_trace_disable();
		mongoc_log_set_handler(NULL, NULL);

		return OnUpdateString(entry, new_value, new_value_length, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);
	}


	if (strcasecmp(new_value, "stderr") == 0) {
		MONGODB_G(debug_fd) = stderr;
	} else if (strcasecmp(new_value, "stdout") == 0) {
		MONGODB_G(debug_fd) = stdout;
	} else if (
		strcasecmp("1", new_value) == 0
		|| strcasecmp("on", new_value) == 0
		|| strcasecmp("yes", new_value) == 0
		|| strcasecmp("true", new_value) == 0
	) {
		tmp_dir = NULL;
	} else {
		tmp_dir = new_value;
	}

	if (!MONGODB_G(debug_fd)) {
		time_t t;
		int fd = -1;
		char *prefix;
		int len;
		char *filename;

		time(&t);
		len = spprintf(&prefix, 0, "PHONGO-%ld", t);

		fd = php_open_temporary_fd(tmp_dir, prefix, &filename TSRMLS_CC);
		if (fd != -1) {
			MONGODB_G(debug_fd) = VCWD_FOPEN(filename, "a");
		}
		efree(filename);
		efree(prefix);
		close(fd);
	}

	mongoc_log_trace_enable();
	mongoc_log_set_handler(php_phongo_log, ctx);

	return OnUpdateString(entry, new_value, new_value_length, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);
}


/* {{{ INI entries */
PHP_INI_BEGIN()
	{ 0, PHP_INI_ALL, (char *)PHONGO_DEBUG_INI, sizeof(PHONGO_DEBUG_INI), OnUpdateDebug, (void *) XtOffsetOf(zend_mongodb_globals, debug), (void *) &mglo, NULL, (char *)PHONGO_DEBUG_INI_DEFAULT, sizeof(PHONGO_DEBUG_INI_DEFAULT)-1, NULL, 0, 0, 0, NULL },
PHP_INI_END()
/* }}} */

/* {{{ PHP_GINIT_FUNCTION */
PHP_GINIT_FUNCTION(mongodb)
{
	bson_mem_vtable_t bsonMemVTable = {
		php_phongo_malloc,
		php_phongo_calloc,
		php_phongo_realloc,
		php_phongo_free,
	};
	mongodb_globals->debug_fd = NULL;
	mongodb_globals->bsonMemVTable = bsonMemVTable;

}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(mongodb)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */


	REGISTER_INI_ENTRIES();

	/* Initialize libmongoc */
	mongoc_init();
	/* Initialize libbson */
	bson_mem_set_vtable(&MONGODB_G(bsonMemVTable));

	/* Prep default object handlers to be used when we register the classes */
	memcpy(&phongo_std_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_std_object_handlers.clone_obj            = NULL;
	/*
	phongo_std_object_handlers.get_debug_info       = NULL;
	phongo_std_object_handlers.compare_objects      = NULL;
	phongo_std_object_handlers.cast_object          = NULL;
	phongo_std_object_handlers.count_elements       = NULL;
	phongo_std_object_handlers.get_closure          = NULL;
	*/

	PHP_MINIT(bson)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(Command)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Cursor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(CursorId)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Manager)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Query)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ReadPreference)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Server)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(BulkWrite)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteConcern)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteConcernError)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteError)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteResult)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(Exception)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(LogicException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(RuntimeException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UnexpectedValueException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(InvalidArgumentException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ConnectionException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(AuthenticationException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(SSLConnectionException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(WriteException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(BulkWriteException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ExecutionTimeoutException)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ConnectionTimeoutException)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(Type)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Serializable)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Unserializable)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Persistable)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Binary)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Javascript)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(MaxKey)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(MinKey)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(ObjectID)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Regex)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(Timestamp)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UTCDateTime)(INIT_FUNC_ARGS_PASSTHRU);

	REGISTER_STRING_CONSTANT("MONGODB_VERSION", (char *)MONGODB_VERSION_S, CONST_CS | CONST_PERSISTENT);
	REGISTER_STRING_CONSTANT("MONGODB_STABILITY", (char *)MONGODB_STABILITY_S, CONST_CS | CONST_PERSISTENT);
	REGISTER_STRING_CONSTANT("BSON_NAMESPACE", (char *)BSON_NAMESPACE, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(mongodb)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */

	bson_mem_restore_vtable();
	/* Cleanup after libmongoc */
	mongoc_cleanup();

	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_GSHUTDOWN_FUNCTION */
PHP_GSHUTDOWN_FUNCTION(mongodb)
{
	mongodb_globals->debug = NULL;
	if (mongodb_globals->debug_fd) {
		fclose(mongodb_globals->debug_fd);
		mongodb_globals->debug_fd = NULL;
	}
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(mongodb)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "mongodb support", "enabled");
	php_info_print_table_row(2, "mongodb version", MONGODB_VERSION_S);
	php_info_print_table_row(2, "mongodb stability", MONGODB_STABILITY_S);
#ifdef HAVE_SYSTEM_LIBMONGOC
	php_info_print_table_row(2, "libmongoc headers version", MONGOC_VERSION_S);
	php_info_print_table_row(2, "libmongoc library version", mongoc_get_version());
#else
	/* Bundled libraries, buildtime = runtime */
	php_info_print_table_row(2, "libmongoc version", MONGOC_VERSION_S);
#endif
#ifdef HAVE_SYSTEM_LIBBSON
	php_info_print_table_row(2, "libbson headers version", BSON_VERSION_S);
	php_info_print_table_row(2, "libbson library version", bson_get_version());
#else
	php_info_print_table_row(2, "libbson version", BSON_VERSION_S);
#endif
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */
/* }}} */


/* {{{ mongodb_functions[]
*/
ZEND_BEGIN_ARG_INFO_EX(ai_bson_fromPHP, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_bson_toPHP, 0, 0, 1)
	ZEND_ARG_INFO(0, bson)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_bson_toJSON, 0, 0, 1)
	ZEND_ARG_INFO(0, bson)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_bson_fromJSON, 0, 0, 1)
	ZEND_ARG_INFO(0, json)
ZEND_END_ARG_INFO();

const zend_function_entry mongodb_functions[] = {
	ZEND_NS_FE(BSON_NAMESPACE, fromPHP, ai_bson_fromPHP)
	ZEND_NS_FE(BSON_NAMESPACE, toPHP,   ai_bson_toPHP)
	ZEND_NS_FE(BSON_NAMESPACE, toJSON,    ai_bson_toJSON)
	ZEND_NS_FE(BSON_NAMESPACE, fromJSON,  ai_bson_fromJSON)
	PHP_FE_END
};
/* }}} */

/* {{{ mongodb_module_entry
 */
zend_module_entry mongodb_module_entry = {
	STANDARD_MODULE_HEADER,
	"mongodb",
	mongodb_functions,
	PHP_MINIT(mongodb),
	PHP_MSHUTDOWN(mongodb),
	NULL /* PHP_RINIT(mongodb)*/,
	NULL /* PHP_RSHUTDOWN(mongodb)*/,
	PHP_MINFO(mongodb),
	MONGODB_VERSION,
	PHP_MODULE_GLOBALS(mongodb),
	PHP_GINIT(mongodb),
	PHP_GSHUTDOWN(mongodb),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_MONGODB
ZEND_GET_MODULE(mongodb)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
