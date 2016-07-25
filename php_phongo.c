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
/* Stream wrapper */
#include <main/php_streams.h>
#include <main/php_network.h>
/* Debug log writing */
#include <main/php_open_temporary_file.h>
/* For formating timestamp in the log */
#include <ext/date/php_date.h>
/* String manipulation */
#include <Zend/zend_string.h>
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
#if PHP_VERSION_ID >= 70000
#if defined(ZTS) && defined(COMPILE_DL_MONGODB)
	ZEND_TSRMLS_CACHE_DEFINE();
#endif
#endif

php_phongo_server_description_type_map_t
php_phongo_server_description_type_map[PHONGO_SERVER_DESCRIPTION_TYPES] = {
	{ PHONGO_SERVER_UNKNOWN, "Unknown" },
	{ PHONGO_SERVER_STANDALONE, "Standalone" },
	{ PHONGO_SERVER_MONGOS, "Mongos" },
	{ PHONGO_SERVER_POSSIBLE_PRIMARY, "PossiblePrimary" },
	{ PHONGO_SERVER_RS_PRIMARY, "RSPrimary" },
	{ PHONGO_SERVER_RS_SECONDARY, "RSSecondary" },
	{ PHONGO_SERVER_RS_ARBITER, "RSArbiter" },
	{ PHONGO_SERVER_RS_OTHER, "RSOther" },
	{ PHONGO_SERVER_RS_GHOST, "RSGhost" },
};

/* {{{ phongo_std_object_handlers */
zend_object_handlers phongo_std_object_handlers;

PHONGO_API zend_object_handlers *phongo_get_std_object_handlers(void)
{
	return &phongo_std_object_handlers;
}
/* }}} */

/* Forward declarations */
static bool phongo_split_namespace(const char *namespace, char **dbname, char **cname);

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
		case MONGOC_ERROR_SERVER_SELECTION_FAILURE:
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
void phongo_throw_exception(php_phongo_error_domain_t domain TSRMLS_DC, const char *format, ...)
{
	va_list args;
	char *message;
	int message_len;

	va_start(args, format);
	message_len = vspprintf(&message, 0, format, args);
	zend_throw_exception(phongo_exception_from_phongo_domain(domain), message, 0 TSRMLS_CC);
	efree(message);
	va_end(args);
}
void phongo_throw_exception_from_bson_error_t(bson_error_t *error TSRMLS_DC)
{
	zend_throw_exception(phongo_exception_from_mongoc_domain(error->domain, error->code), error->message, error->code TSRMLS_CC);
}
static void php_phongo_log(mongoc_log_level_t log_level, const char *log_domain, const char *message, void *user_data)
{
	PHONGO_TSRMLS_FETCH_FROM_CTX(user_data);
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
			phongo_char *dt;

			time(&t);
			dt = php_format_date((char *)"Y-m-d\\TH:i:sP", strlen("Y-m-d\\TH:i:sP"), t, 0 TSRMLS_CC);

			fprintf(MONGODB_G(debug_fd), "[%s] %10s: %-8s> %s\n", ZSTR_VAL(dt), log_domain, mongoc_log_level_str(log_level), message);
			fflush(MONGODB_G(debug_fd));
			efree(dt);
		} break;
	}
}

/* }}} */

/* {{{ Init objects */
static void phongo_cursor_init(zval *return_value, zval *manager, mongoc_cursor_t *cursor, zval *readPreference TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *intern;

	object_init_ex(return_value, php_phongo_cursor_ce);

	intern = Z_CURSOR_OBJ_P(return_value);
	intern->cursor = cursor;
	intern->server_id = mongoc_cursor_get_hint(cursor);
	intern->client = Z_MANAGER_OBJ_P(manager)->client;

#if PHP_VERSION_ID >= 70000
	ZVAL_COPY(&intern->manager, manager);
#else
	Z_ADDREF_P(manager);
	intern->manager = manager;
#endif

	if (readPreference) {
#if PHP_VERSION_ID >= 70000
		ZVAL_ZVAL(&intern->read_preference, readPreference, 1, 0);
#else
		Z_ADDREF_P(readPreference);
		intern->read_preference = readPreference;
#endif
	}
} /* }}} */

static void phongo_cursor_init_for_command(zval *return_value, zval *manager, mongoc_cursor_t *cursor, const char *db, zval *command, zval *readPreference TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *intern;

	phongo_cursor_init(return_value, manager, cursor, readPreference TSRMLS_CC);
	intern = Z_CURSOR_OBJ_P(return_value);

	intern->database = estrdup(db);

#if PHP_VERSION_ID >= 70000
	ZVAL_ZVAL(&intern->command, command, 1, 0);
#else
	Z_ADDREF_P(command);
	intern->command = command;
#endif
} /* }}} */

static void phongo_cursor_init_for_query(zval *return_value, zval *manager, mongoc_cursor_t *cursor, const char *namespace, zval *query, zval *readPreference TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *intern;

	phongo_cursor_init(return_value, manager, cursor, readPreference TSRMLS_CC);
	intern = Z_CURSOR_OBJ_P(return_value);

	/* namespace has already been validated by phongo_execute_query() */
	phongo_split_namespace(namespace, &intern->database, &intern->collection);

#if PHP_VERSION_ID >= 70000
	ZVAL_ZVAL(&intern->query, query, 1, 0);
#else
	Z_ADDREF_P(query);
	intern->query = query;
#endif
} /* }}} */

void phongo_server_init(zval *return_value, zval *manager, int server_id TSRMLS_DC) /* {{{ */
{
	php_phongo_server_t *server;

	object_init_ex(return_value, php_phongo_server_ce);

	server = Z_SERVER_OBJ_P(return_value);
	server->server_id = server_id;
	server->client = Z_MANAGER_OBJ_P(manager)->client;

#if PHP_VERSION_ID >= 70000
	ZVAL_COPY(&server->manager, manager);
#else
	Z_ADDREF_P(manager);
	server->manager = manager;
#endif
}
/* }}} */

void phongo_readconcern_init(zval *return_value, const mongoc_read_concern_t *read_concern TSRMLS_DC) /* {{{ */
{
	php_phongo_readconcern_t *intern;

	object_init_ex(return_value, php_phongo_readconcern_ce);

	intern = Z_READCONCERN_OBJ_P(return_value);
	intern->read_concern = mongoc_read_concern_copy(read_concern);
}
/* }}} */

void phongo_readpreference_init(zval *return_value, const mongoc_read_prefs_t *read_prefs TSRMLS_DC) /* {{{ */
{
	php_phongo_readpreference_t *intern;

	object_init_ex(return_value, php_phongo_readpreference_ce);

	intern = Z_READPREFERENCE_OBJ_P(return_value);
	intern->read_preference = mongoc_read_prefs_copy(read_prefs);
}
/* }}} */

void phongo_writeconcern_init(zval *return_value, const mongoc_write_concern_t *write_concern TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcern_t *intern;

	object_init_ex(return_value, php_phongo_writeconcern_ce);

	intern = Z_WRITECONCERN_OBJ_P(return_value);
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
			uint32_t len = 0;
			const uint8_t *data = NULL;

			if (! (BSON_ITER_HOLDS_DOCUMENT (&iter) || BSON_ITER_HOLDS_ARRAY (&iter))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected modifiers to be array or object, %d given", bson_iter_type(&iter));
				return false;
			}

			bson_iter_document(&iter, &len, &data);
			if (len) {
				bson_t tmp;

				bson_init_static(&tmp, data, len);
				bson_copy_to_excluding_noinit(&tmp, query->query, "not-used-value", NULL);
				bson_destroy (&tmp);
			}
		}

		if (bson_iter_init_find(&iter, options, "projection")) {
			uint32_t len = 0;
			const uint8_t *data = NULL;

			if (! (BSON_ITER_HOLDS_DOCUMENT (&iter) || BSON_ITER_HOLDS_ARRAY (&iter))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected projection to be array or object, %d given", bson_iter_type(&iter));
				return false;
			}

			bson_iter_document(&iter, &len, &data);
			if (len) {
				query->selector = bson_new_from_data(data, len);
			}
		}

		if (bson_iter_init_find(&iter, options, "sort")) {
			uint32_t len = 0;
			const uint8_t *data = NULL;

			if (! (BSON_ITER_HOLDS_DOCUMENT (&iter) || BSON_ITER_HOLDS_ARRAY (&iter))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected sort to be array or object, %d given", bson_iter_type(&iter));
				return false;
			}

			phongo_bson_iter_as_document(&iter, &len, &data);
			if (len) {
				bson_t tmp;

				bson_init_static(&tmp, data, len);
				bson_append_document(query->query, "$orderby", -1, &tmp);
				bson_destroy(&tmp);
			}
		}
	}

	BSON_APPEND_DOCUMENT(query->query, "$query", filter);
	return true;
} /* }}} */

zend_bool phongo_writeconcernerror_init(zval *return_value, bson_t *bson TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	php_phongo_writeconcernerror_t *writeconcernerror;

	writeconcernerror = Z_WRITECONCERNERROR_OBJ_P(return_value);

	if (bson_iter_init_find(&iter, bson, "code") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeconcernerror->code = bson_iter_int32(&iter);
	}
	if (bson_iter_init_find(&iter, bson, "errmsg") && BSON_ITER_HOLDS_UTF8(&iter)) {
		writeconcernerror->message = bson_iter_dup_utf8(&iter, NULL);
	}
	if (bson_iter_init_find(&iter, bson, "errInfo") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
		uint32_t               len;
		const uint8_t         *data;

		bson_iter_document(&iter, &len, &data);

		if (!data) {
			return false;
		}

		if (!phongo_bson_to_zval(data, len, &writeconcernerror->info)) {
			zval_ptr_dtor(&writeconcernerror->info);
#if PHP_VERSION_ID >= 70000
			ZVAL_UNDEF(&writeconcernerror->info);
#else
			writeconcernerror->info = NULL;
#endif

			return false;
		}
	}

	return true;
} /* }}} */

zend_bool phongo_writeerror_init(zval *return_value, bson_t *bson TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	php_phongo_writeerror_t *writeerror;

	writeerror = Z_WRITEERROR_OBJ_P(return_value);

	if (bson_iter_init_find(&iter, bson, "code") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeerror->code = bson_iter_int32(&iter);
	}
	if (bson_iter_init_find(&iter, bson, "errmsg") && BSON_ITER_HOLDS_UTF8(&iter)) {
		writeerror->message = bson_iter_dup_utf8(&iter, NULL);
	}
	if (bson_iter_init_find(&iter, bson, "errInfo")) {
		bson_t                 info;

		bson_init(&info);
		bson_append_iter(&info, NULL, 0, &iter);

		if (!phongo_bson_to_zval(bson_get_data(&info), info.len, &writeerror->info)) {
			zval_ptr_dtor(&writeerror->info);
#if PHP_VERSION_ID >= 70000
			ZVAL_UNDEF(&writeerror->info);
#else
			writeerror->info = NULL;
#endif

			return false;
		}
	}
	if (bson_iter_init_find(&iter, bson, "index") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeerror->index = bson_iter_int32(&iter);
	}

	return true;
} /* }}} */

php_phongo_writeresult_t *phongo_writeresult_init(zval *return_value, bson_t *reply, zval *manager, int server_id TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t *writeresult;

	object_init_ex(return_value, php_phongo_writeresult_ce);

	writeresult = Z_WRITERESULT_OBJ_P(return_value);
	writeresult->reply     = bson_copy(reply);
	writeresult->server_id = server_id;
	writeresult->client = Z_MANAGER_OBJ_P(manager)->client;

#if PHP_VERSION_ID >= 70000
	ZVAL_COPY(&writeresult->manager, manager);
#else
	Z_ADDREF_P(manager);
	writeresult->manager = manager;
#endif

	return writeresult;
} /* }}} */
/* }}} */

/* {{{ CRUD */
/* Splits a namespace name into the database and collection names, allocated with estrdup. */
static bool phongo_split_namespace(const char *namespace, char **dbname, char **cname) /* {{{ */
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

bool phongo_execute_write(zval *manager, const char *namespace, php_phongo_bulkwrite_t  *bulk_write, const mongoc_write_concern_t *write_concern, int server_id, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	mongoc_client_t *client;
	bson_error_t error;
	int success;
	bson_t reply = BSON_INITIALIZER;
	mongoc_bulk_operation_t *bulk = bulk_write->bulk;
	php_phongo_writeresult_t *writeresult;

	client = Z_MANAGER_OBJ_P(manager)->client;

	/* Since BulkWrite objects can currently be executed multiple times, ensure
	 * that the database and collection name are freed before we overwrite them.
	 * This may be removed once PHPC-676 is implemented. */
	if (bulk_write->database) {
		efree(bulk_write->database);
	}

	if (bulk_write->collection) {
		efree(bulk_write->collection);
	}

	if (!phongo_split_namespace(namespace, &bulk_write->database, &bulk_write->collection)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s: %s", "Invalid namespace provided", namespace);
		return false;
	}

	mongoc_bulk_operation_set_database(bulk, bulk_write->database);
	mongoc_bulk_operation_set_collection(bulk, bulk_write->collection);
	mongoc_bulk_operation_set_client(bulk, client);

	/* If a write concern was not specified, libmongoc will use the client's
	 * write concern; however, we should still fetch it for the write result. */
	if (write_concern) {
		mongoc_bulk_operation_set_write_concern(bulk, write_concern);
	} else {
		write_concern = mongoc_client_get_write_concern(client);
	}

	if (server_id > 0) {
		mongoc_bulk_operation_set_hint(bulk, server_id);
	}

	success = mongoc_bulk_operation_execute(bulk, &reply, &error);
	bulk_write->executed = true;

	/* Write succeeded and the user doesn't care for the results */
	if (success && !return_value_used) {
		bson_destroy(&reply);
		return true;
	}

	/* Check for connection related exceptions */
	if (EG(exception)) {
		bson_destroy(&reply);
		return false;
	}

	writeresult = phongo_writeresult_init(return_value, &reply, manager, mongoc_bulk_operation_get_hint(bulk) TSRMLS_CC);
	writeresult->write_concern = mongoc_write_concern_copy(write_concern);

	/* The Write failed */
	if (!success) {
		if (error.domain == MONGOC_ERROR_COMMAND || error.domain == MONGOC_ERROR_WRITE_CONCERN) {
			phongo_throw_exception(PHONGO_ERROR_WRITE_FAILED TSRMLS_CC, "%s", error.message);
			phongo_add_exception_prop(ZEND_STRL("writeResult"), return_value TSRMLS_CC);
		} else {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
		}
	}

	bson_destroy(&reply);

	return success;
} /* }}} */

/* Advance the cursor and return whether there is an error. On error, the cursor
 * will be destroyed and an exception will be thrown. */
static bool phongo_advance_cursor_and_check_for_error(mongoc_cursor_t *cursor TSRMLS_DC)
{
	const bson_t *doc;

	if (!mongoc_cursor_next(cursor, &doc)) {
		bson_error_t error;

		/* Check for connection related exceptions */
		if (EG(exception)) {
			mongoc_cursor_destroy(cursor);
			return false;
		}

		/* Could simply be no docs, which is not an error */
		if (mongoc_cursor_error(cursor, &error)) {
			phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
			mongoc_cursor_destroy(cursor);
			return false;
		}
	}

	return true;
}

int phongo_execute_query(zval *manager, const char *namespace, zval *zquery, zval *zreadPreference, int server_id, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	mongoc_client_t *client;
	const php_phongo_query_t *query;
	mongoc_cursor_t *cursor;
	char *dbname;
	char *collname;
	mongoc_collection_t *collection;

	client = Z_MANAGER_OBJ_P(manager)->client;

	if (!phongo_split_namespace(namespace, &dbname, &collname)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s: %s", "Invalid namespace provided", namespace);
		return false;
	}
	collection = mongoc_client_get_collection(client, dbname, collname);
	efree(dbname);
	efree(collname);

	query = Z_QUERY_OBJ_P(zquery);

	if (query->read_concern) {
		mongoc_collection_set_read_concern(collection, query->read_concern);
	}

	cursor = mongoc_collection_find(collection, query->flags, query->skip, query->limit, query->batch_size, query->query, query->selector, phongo_read_preference_from_zval(zreadPreference TSRMLS_CC));
	mongoc_collection_destroy(collection);

	/* mongoc issues a warning we need to catch somehow */
	if (!cursor) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED TSRMLS_CC, "%s", "FIXME: Couldn't create cursor...");
		return false;
	}

	if (server_id > 0 && !mongoc_cursor_set_hint(cursor, server_id)) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED TSRMLS_CC, "%s", "Could not set cursor server_id");
		return false;
	}

	if (!phongo_advance_cursor_and_check_for_error(cursor TSRMLS_CC)) {
		return false;
	}

	if (!return_value_used) {
		mongoc_cursor_destroy(cursor);
		return true;
	}

	phongo_cursor_init_for_query(return_value, manager, cursor, namespace, zquery, zreadPreference TSRMLS_CC);
	return true;
} /* }}} */

int phongo_execute_command(zval *manager, const char *db, zval *zcommand, zval *zreadPreference, int server_id, zval *return_value, int return_value_used TSRMLS_DC) /* {{{ */
{
	mongoc_client_t *client;
	const php_phongo_command_t *command;
	mongoc_cursor_t *cursor;
	bson_iter_t iter;

	client = Z_MANAGER_OBJ_P(manager)->client;
	command = Z_COMMAND_OBJ_P(zcommand);

	cursor = mongoc_client_command(client, db, MONGOC_QUERY_NONE, 0, 1, 0, command->bson, NULL, phongo_read_preference_from_zval(zreadPreference TSRMLS_CC));

	if (server_id > 0 && !mongoc_cursor_set_hint(cursor, server_id)) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED TSRMLS_CC, "%s", "Could not set cursor server_id");
		return false;
	}

	if (!phongo_advance_cursor_and_check_for_error(cursor TSRMLS_CC)) {
		return false;
	}

	if (!return_value_used) {
		mongoc_cursor_destroy(cursor);
		return true;
	}

	if (bson_iter_init_find(&iter, mongoc_cursor_current(cursor), "cursor") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
		mongoc_cursor_t *cmd_cursor;

		/* According to mongoc_cursor_new_from_command_reply(), the reply bson_t
		 * is ultimately destroyed on both success and failure. Use bson_copy()
		 * to create a writable copy of the const bson_t we fetched above. */
		cmd_cursor = mongoc_cursor_new_from_command_reply(client, bson_copy(mongoc_cursor_current(cursor)), mongoc_cursor_get_hint(cursor));
		mongoc_cursor_destroy(cursor);

		if (!phongo_advance_cursor_and_check_for_error(cmd_cursor TSRMLS_CC)) {
			return false;
		}

		phongo_cursor_init_for_command(return_value, manager, cmd_cursor, db, zcommand, zreadPreference TSRMLS_CC);
		return true;
	}

	phongo_cursor_init_for_command(return_value, manager, cursor, db, zcommand, zreadPreference TSRMLS_CC);
	return true;
} /* }}} */

/* }}} */

/* {{{ Stream vtable */
void phongo_stream_destroy(mongoc_stream_t *stream_wrap) /* {{{ */
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream_wrap;

	if (base_stream->stream) {
		MONGOC_DEBUG("Not destroying RSRC#%d", PHONGO_STREAM_ID(base_stream->stream));
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
#if PHP_VERSION_ID < 70000
		PHONGO_TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);
#endif

		MONGOC_DEBUG("Destroying RSRC#%d", PHONGO_STREAM_ID(base_stream->stream));
		php_stream_free(base_stream->stream, PHP_STREAM_FREE_CLOSE_PERSISTENT | PHP_STREAM_FREE_RSRC_DTOR);
		base_stream->stream = NULL;
	}

	efree(base_stream);
} /* }}} */

int phongo_stream_close(mongoc_stream_t *stream_wrap) /* {{{ */
{
	php_phongo_stream_socket *base_stream = (php_phongo_stream_socket *)stream_wrap;

	MONGOC_DEBUG("Closing RSRC#%d", PHONGO_STREAM_ID(base_stream->stream));
	if (base_stream->stream) {
#if PHP_VERSION_ID < 70000
		TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);
#endif

		MONGOC_DEBUG("Destroying RSRC#%d", PHONGO_STREAM_ID(base_stream->stream));
		php_stream_free(base_stream->stream, PHP_STREAM_FREE_CLOSE_PERSISTENT | PHP_STREAM_FREE_RSRC_DTOR);
		base_stream->stream = NULL;
	}

	return 0;
} /* }}} */

void php_phongo_set_timeout(php_phongo_stream_socket *base_stream, int32_t timeout_msec) /* {{{ */
{
	struct timeval rtimeout = {0, 0};
	PHONGO_TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

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
	PHONGO_TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);


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
	PHONGO_TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

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
	PHONGO_TSRMLS_FETCH_FROM_CTX(base_stream->tsrm_ls);

	return PHP_STREAM_OPTION_RETURN_OK != php_stream_set_option(base_stream->stream, PHP_STREAM_OPTION_CHECK_LIVENESS, 0, NULL);
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
static int php_phongo_verify_hostname(const char *hostname, X509 *cert TSRMLS_DC)
{
	if (php_mongodb_matches_san_list(cert, hostname) == SUCCESS) {
		return SUCCESS;
	}

	if (php_mongodb_matches_common_name(cert, hostname TSRMLS_CC) == SUCCESS) {
		return SUCCESS;
	}

	return FAILURE;
}

int php_phongo_peer_verify(php_stream *stream, X509 *cert, const char *hostname, bson_error_t *error TSRMLS_DC)
{
	zval **verify_peer_name;

	/* This option is available since PHP 5.6.0 */
	if (php_stream_context_get_option(PHP_STREAM_CONTEXT(stream), "ssl", "verify_peer_name", &verify_peer_name) == SUCCESS && zend_is_true(*verify_peer_name)) {
		zval **zhost = NULL;
		const char *peer;

		if (php_stream_context_get_option(PHP_STREAM_CONTEXT(stream), "ssl", "peer_name", &zhost) == SUCCESS) {
			convert_to_string_ex(zhost);
			peer = Z_STRVAL_PP(zhost);
		} else {
			peer = hostname;
		}

#ifdef HAVE_OPENSSL_EXT
		if (php_phongo_verify_hostname(peer, cert TSRMLS_CC) == FAILURE) {
			bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Remote certificate SubjectAltName or CN does not match '%s'", hostname);
			return false;
		}
#else
		bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Cannot verify remote certificate SubjectAltName or CN. Please ensure that extension is compiled against PHP with OpenSSL or disable the \"verify_peer_name\" SSL context option.");
		return false;
#endif
	}

	return true;
}
#endif

bool php_phongo_ssl_verify(php_stream *stream, const char *hostname, bson_error_t *error TSRMLS_DC)
{
#if PHP_VERSION_ID >= 70000
	zval *zcert;
	zval *verify_expiry;
#else
	zval **zcert;
	zval **verify_expiry;
#endif
	X509 *cert;

	if (!PHP_STREAM_CONTEXT(stream)) {
		return true;
	}

#if PHP_VERSION_ID >= 70000
	if (!((zcert = php_stream_context_get_option(PHP_STREAM_CONTEXT(stream), "ssl", "peer_certificate")) != NULL && Z_TYPE_P(zcert) == IS_RESOURCE)) {
#else
	if (!(php_stream_context_get_option(PHP_STREAM_CONTEXT(stream), "ssl", "peer_certificate", &zcert) == SUCCESS && Z_TYPE_PP(zcert) == IS_RESOURCE)) {
#endif
		bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Could not capture certificate of %s", hostname);
		return false;
	}

#if PHP_VERSION_ID >= 70000
	cert = (X509 *)x509_from_zval(zcert TSRMLS_CC);
#else
	cert = (X509 *)x509_from_zval(*zcert TSRMLS_CC);
#endif
	if (!cert) {
		bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Could not get certificate of %s", hostname);
		return false;
	}

#if PHP_VERSION_ID < 50600
	if (!php_phongo_peer_verify(stream, cert, hostname, error TSRMLS_CC)) {
		return false;
	}
#endif

#if PHP_VERSION_ID >= 70000
	if ((verify_expiry = php_stream_context_get_option(PHP_STREAM_CONTEXT(stream), "ssl", "verify_expiry")) != NULL && zend_is_true(verify_expiry)) {
#else
	if (php_stream_context_get_option(PHP_STREAM_CONTEXT(stream), "ssl", "verify_expiry", &verify_expiry) == SUCCESS && zend_is_true(*verify_expiry)) {
#endif
#ifdef HAVE_OPENSSL_EXT
		time_t current     = time(NULL);
		time_t valid_from  = php_mongodb_asn1_time_to_time_t(X509_get_notBefore(cert) TSRMLS_CC);
		time_t valid_until = php_mongodb_asn1_time_to_time_t(X509_get_notAfter(cert) TSRMLS_CC);

		if (valid_from > current) {
			bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Certificate is not valid yet on %s", hostname);
			return false;
		}
		if (current > valid_until) {
			bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Certificate has expired on %s", hostname);
			return false;
		}
#else
		bson_set_error(error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Cannot verify certificate expiration. Please ensure that extension is compiled against PHP with OpenSSL or disable the \"verify_expiry\" SSL context option.");
		return false;
#endif
	}

	return true;
}

mongoc_stream_t* phongo_stream_initiator(const mongoc_uri_t *uri, const mongoc_host_list_t *host, void *user_data, bson_error_t *error) /* {{{ */
{
	zend_error_handling error_handling;
	php_phongo_stream_socket *base_stream = NULL;
	php_stream *stream = NULL;
	const bson_t *options;
	bson_iter_t iter;
	struct timeval timeout = {0, 0};
	struct timeval *timeoutp = NULL;
	char *uniqid;
	const char *persistent_id;
	phongo_char *errmsg = NULL;
	int errcode;
	char *dsn;
	int dsn_len;
	TSRMLS_FETCH();

	ENTRY;

	switch (host->family) {
#if defined(AF_INET6)
		case AF_INET6:
			dsn_len = spprintf(&dsn, 0, "tcp://[%s]:%d", host->host, host->port);
			break;
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

	if (bson_iter_init_find_case (&iter, options, "connecttimeoutms") && BSON_ITER_HOLDS_INT32 (&iter)) {
		int32_t connecttimeoutms = MONGOC_DEFAULT_CONNECTTIMEOUTMS;

		if (!(connecttimeoutms = bson_iter_int32(&iter))) {
			connecttimeoutms = MONGOC_DEFAULT_CONNECTTIMEOUTMS;
		}

		timeout.tv_sec = connecttimeoutms / 1000;
		timeout.tv_usec = (connecttimeoutms % 1000) * 1000;

		timeoutp = &timeout;
		MONGOC_DEBUG("Applying connectTimeoutMS: %d", connecttimeoutms);
	}

	spprintf(&uniqid, 0, "%s:%d[%s]", host->host, host->port, mongoc_uri_get_string(uri));

	/* Do not persist SSL streams to avoid errors attempting to reinitialize SSL
	 * on subsequent requests (see: PHPC-720) */
	persistent_id = mongoc_uri_get_ssl(uri) ? NULL : uniqid;

	MONGOC_DEBUG("Connecting to '%s'", uniqid);
	zend_replace_error_handling(EH_SUPPRESS, NULL, &error_handling TSRMLS_CC);
	stream = php_stream_xport_create(dsn, dsn_len, 0, STREAM_XPORT_CLIENT | STREAM_XPORT_CONNECT, persistent_id, timeoutp, (php_stream_context *)user_data, &errmsg, &errcode);
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (!stream) {
		bson_set_error (error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_CONNECT, "Failed connecting to '%s:%d': %s", host->host, host->port, ZSTR_VAL(errmsg));
		efree(dsn);
		efree(uniqid);
		if (errmsg) {
			phongo_char_free(errmsg);
		}
		RETURN(NULL);
	}
	php_stream_auto_cleanup(stream);

	MONGOC_DEBUG("Created: RSRC#%d as '%s'", PHONGO_STREAM_ID(stream), uniqid);
	efree(uniqid);

	if (mongoc_uri_get_ssl(uri)) {
		zend_replace_error_handling(EH_THROW, php_phongo_sslconnectionexception_ce, &error_handling TSRMLS_CC);

		MONGOC_DEBUG("Enabling SSL (stream will not be persisted)");

		/* Capture the server certificate so we can do further verification */
		if (PHP_STREAM_CONTEXT(stream)) {
			zval capture;
			ZVAL_BOOL(&capture, 1);
			php_stream_context_set_option(PHP_STREAM_CONTEXT(stream), "ssl", "capture_peer_cert", &capture);
		}

		if (php_stream_xport_crypto_setup(stream, PHONGO_CRYPTO_METHOD, NULL TSRMLS_CC) < 0) {
			zend_restore_error_handling(&error_handling TSRMLS_CC);
			php_stream_free(stream, PHP_STREAM_FREE_CLOSE_PERSISTENT | PHP_STREAM_FREE_RSRC_DTOR);
			bson_set_error (error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_INVALID_TYPE, "Failed to setup crypto, is the OpenSSL extension loaded?");
			efree(dsn);
			RETURN(NULL);
		}

		if (php_stream_xport_crypto_enable(stream, 1 TSRMLS_CC) < 0) {
			zend_restore_error_handling(&error_handling TSRMLS_CC);
			php_stream_free(stream, PHP_STREAM_FREE_CLOSE_PERSISTENT | PHP_STREAM_FREE_RSRC_DTOR);
			bson_set_error (error, MONGOC_ERROR_STREAM, MONGOC_ERROR_STREAM_INVALID_TYPE, "Failed to setup crypto, is the server running with SSL?");
			efree(dsn);
			RETURN(NULL);
		}

		if (!php_phongo_ssl_verify(stream, host->host, error TSRMLS_CC)) {
			zend_restore_error_handling(&error_handling TSRMLS_CC);
			php_stream_pclose(stream);
			efree(dsn);
			RETURN(NULL);
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
		php_phongo_writeconcern_t *intern = Z_WRITECONCERN_OBJ_P(zwrite_concern);

		if (intern) {
			return intern->write_concern;
		}
	}

	return NULL;
} /* }}} */

const mongoc_read_concern_t* phongo_read_concern_from_zval(zval *zread_concern TSRMLS_DC) /* {{{ */
{
	if (zread_concern) {
		php_phongo_readconcern_t *intern = Z_READCONCERN_OBJ_P(zread_concern);

		if (intern) {
			return intern->read_concern;
		}
	}

	return NULL;
} /* }}} */

const mongoc_read_prefs_t* phongo_read_preference_from_zval(zval *zread_preference TSRMLS_DC) /* {{{ */
{
	if (zread_preference) {
		php_phongo_readpreference_t *intern = Z_READPREFERENCE_OBJ_P(zread_preference);

		if (intern) {
			return intern->read_preference;
		}
	}

	return NULL;
} /* }}} */
/* }}} */

/* {{{ phongo zval from mongoc types */
void php_phongo_cursor_id_new_from_id(zval *object, int64_t cursorid TSRMLS_DC) /* {{{ */
{
	php_phongo_cursorid_t     *intern;

	object_init_ex(object, php_phongo_cursorid_ce);

	intern = Z_CURSORID_OBJ_P(object);
	intern->id = cursorid;
} /* }}} */

void php_phongo_objectid_new_from_oid(zval *object, const bson_oid_t *oid TSRMLS_DC) /* {{{ */
{
	php_phongo_objectid_t     *intern;

	object_init_ex(object, php_phongo_objectid_ce);

	intern = Z_OBJECTID_OBJ_P(object);
	bson_oid_to_string(oid, intern->oid);
	intern->initialized = true;
} /* }}} */

php_phongo_server_description_type_t php_phongo_server_description_type(mongoc_server_description_t *sd)
{
	const char* name = mongoc_server_description_type(sd);
	int i;

	for (i = 0; i < PHONGO_SERVER_DESCRIPTION_TYPES; i++) {
		if (!strcmp(name, php_phongo_server_description_type_map[i].name)) {
			return php_phongo_server_description_type_map[i].type;
		}
	}

	return PHONGO_SERVER_UNKNOWN;
}

void php_phongo_server_to_zval(zval *retval, mongoc_server_description_t *sd) /* {{{ */
{
	mongoc_host_list_t *host = mongoc_server_description_host(sd);
	const bson_t       *is_master = mongoc_server_description_ismaster(sd);
	bson_iter_t         iter;

	array_init(retval);

	ADD_ASSOC_STRING(retval, "host", host->host);
	ADD_ASSOC_LONG_EX(retval, "port", host->port);
	ADD_ASSOC_LONG_EX(retval, "type", php_phongo_server_description_type(sd));
	ADD_ASSOC_BOOL_EX(retval, "is_primary", !strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_PRIMARY].name));
	ADD_ASSOC_BOOL_EX(retval, "is_secondary", !strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_SECONDARY].name));
	ADD_ASSOC_BOOL_EX(retval, "is_arbiter", !strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_ARBITER].name));
	ADD_ASSOC_BOOL_EX(retval, "is_hidden", bson_iter_init_find_case(&iter, is_master, "hidden") && bson_iter_as_bool(&iter));
	ADD_ASSOC_BOOL_EX(retval, "is_passive", bson_iter_init_find_case(&iter, is_master, "passive") && bson_iter_as_bool(&iter));

	if (bson_iter_init_find(&iter, is_master, "tags") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
		const uint8_t         *bytes;
		uint32_t       	       len;
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

		/* Use native arrays for debugging output */
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		bson_iter_document(&iter, &len, &bytes);
		phongo_bson_to_zval_ex(bytes, len, &state);

#if PHP_VERSION_ID >= 70000
		ADD_ASSOC_ZVAL_EX(retval, "tags", &state.zchild);
#else
		ADD_ASSOC_ZVAL_EX(retval, "tags", state.zchild);
#endif
	}

	{
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;
		/* Use native arrays for debugging output */
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		phongo_bson_to_zval_ex(bson_get_data(is_master), is_master->len, &state);

#if PHP_VERSION_ID >= 70000
		ADD_ASSOC_ZVAL_EX(retval, "last_is_master", &state.zchild);
#else
		ADD_ASSOC_ZVAL_EX(retval, "last_is_master", state.zchild);
#endif
	}
	ADD_ASSOC_LONG_EX(retval, "round_trip_time", (phongo_long) mongoc_server_description_round_trip_time(sd));

} /* }}} */

void php_phongo_read_concern_to_zval(zval *retval, const mongoc_read_concern_t *read_concern) /* {{{ */
{
	const char *level = mongoc_read_concern_get_level(read_concern);

	array_init_size(retval, 1);

	if (level) {
		ADD_ASSOC_STRING(retval, "level", (char *)level);
	} else {
		ADD_ASSOC_NULL_EX(retval, "level");
	}
} /* }}} */

void php_phongo_read_preference_to_zval(zval *retval, const mongoc_read_prefs_t *read_prefs) /* {{{ */
{
	const bson_t *tags = mongoc_read_prefs_get_tags(read_prefs);

	array_init_size(retval, 2);

	ADD_ASSOC_LONG_EX(retval, "mode", mongoc_read_prefs_get_mode(read_prefs));

	if (tags->len) {
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;
		/* Use native arrays for debugging output */
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		phongo_bson_to_zval_ex(bson_get_data(tags), tags->len, &state);
#if PHP_VERSION_ID >= 70000
		ADD_ASSOC_ZVAL_EX(retval, "tags", &state.zchild);
#else
		ADD_ASSOC_ZVAL_EX(retval, "tags", state.zchild);
#endif
	} else {
		ADD_ASSOC_NULL_EX(retval, "tags");
	}
} /* }}} */

void php_phongo_write_concern_to_zval(zval *retval, const mongoc_write_concern_t *write_concern) /* {{{ */
{
	const char *wtag = mongoc_write_concern_get_wtag(write_concern);
	const int32_t w = mongoc_write_concern_get_w(write_concern);

	array_init_size(retval, 4);

	if (wtag) {
		ADD_ASSOC_STRING(retval, "w", (char *)wtag);
	} else if (mongoc_write_concern_get_wmajority(write_concern)) {
		ADD_ASSOC_STRING(retval, "w", (char *)PHONGO_WRITE_CONCERN_W_MAJORITY);
	} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
		ADD_ASSOC_LONG_EX(retval, "w", w);
	} else {
		ADD_ASSOC_NULL_EX(retval, "w");
	}

	ADD_ASSOC_BOOL_EX(retval, "wmajority", mongoc_write_concern_get_wmajority(write_concern));
	ADD_ASSOC_LONG_EX(retval, "wtimeout", mongoc_write_concern_get_wtimeout(write_concern));

	if (mongoc_write_concern_journal_is_set(write_concern)) {
		ADD_ASSOC_BOOL_EX(retval, "journal", mongoc_write_concern_get_journal(write_concern));
	} else {
		ADD_ASSOC_NULL_EX(retval, "journal");
	}
} /* }}} */
/* }}} */


static mongoc_uri_t *php_phongo_make_uri(const char *uri_string, bson_t *options) /* {{{ */
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
#if PHP_VERSION_ID >= 70000
	zval                     *tmp;

#define SET_STRING_CTX(name) \
		if (driverOptions && php_array_exists(driverOptions, name)) { \
			zval ztmp; \
			zend_bool ctmp_free; \
			int ctmp_len; \
			char *ctmp; \
			ctmp = php_array_fetchl_string(driverOptions, name, sizeof(name)-1, &ctmp_len, &ctmp_free); \
			ZVAL_STRING(&ztmp, ctmp); \
			if (ctmp_free) { \
				str_efree(ctmp); \
			} \
			php_stream_context_set_option(ctx, "ssl", name, &ztmp); \
			zval_ptr_dtor(&ztmp); \
		}
#define SET_BOOL_CTX(name, defaultvalue) \
		{ \
			zval ztmp; \
			if (driverOptions && php_array_exists(driverOptions, name)) { \
				ZVAL_BOOL(&ztmp, php_array_fetchl_bool(driverOptions, ZEND_STRL(name))); \
				php_stream_context_set_option(ctx, "ssl", name, &ztmp); \
			} \
			else if ((tmp = php_stream_context_get_option(ctx, "ssl", name)) == NULL) { \
				ZVAL_BOOL(&ztmp, defaultvalue); \
				php_stream_context_set_option(ctx, "ssl", name, &ztmp); \
			} \
		}
#else
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
#endif

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

static bool php_phongo_apply_rc_options_to_uri(mongoc_uri_t *uri, bson_t *options TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	mongoc_read_concern_t *new_rc;
	const mongoc_read_concern_t *old_rc;

	if (!(old_rc = mongoc_uri_get_read_concern(uri))) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED TSRMLS_CC, "mongoc_uri_t does not have a read concern");

		return false;
	}

	/* Return early if there are no options to apply */
	if (bson_empty0(options)) {
		return true;
	}

	if (!bson_iter_init_find_case(&iter, options, "readconcernlevel")) {
		return true;
	}

	new_rc = mongoc_read_concern_copy(old_rc);

	if (bson_iter_init_find_case(&iter, options, "readconcernlevel") && BSON_ITER_HOLDS_UTF8(&iter)) {
		const char *str = bson_iter_utf8(&iter, NULL);

		mongoc_read_concern_set_level(new_rc, str);
	}

	mongoc_uri_set_read_concern(uri, new_rc);
	mongoc_read_concern_destroy(new_rc);

	return true;
} /* }}} */

static bool php_phongo_apply_rp_options_to_uri(mongoc_uri_t *uri, bson_t *options TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	mongoc_read_prefs_t *new_rp;
	const mongoc_read_prefs_t *old_rp;

	if (!(old_rp = mongoc_uri_get_read_prefs_t(uri))) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED TSRMLS_CC, "mongoc_uri_t does not have a read preference");

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

	mongoc_uri_set_read_prefs_t(uri, new_rp);
	mongoc_read_prefs_destroy(new_rp);

	return true;
} /* }}} */

static bool php_phongo_apply_wc_options_to_uri(mongoc_uri_t *uri, bson_t *options TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	int32_t wtimeoutms;
	mongoc_write_concern_t *new_wc;
	const mongoc_write_concern_t *old_wc;

	if (!(old_wc = mongoc_uri_get_write_concern(uri))) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED TSRMLS_CC, "mongoc_uri_t does not have a write concern");

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
	if (!mongoc_write_concern_is_valid(new_wc)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Write concern is not valid");
		mongoc_write_concern_destroy(new_wc);

		return false;
	}

	mongoc_uri_set_write_concern(uri, new_wc);
	mongoc_write_concern_destroy(new_wc);

	return true;
} /* }}} */

static mongoc_client_t *php_phongo_make_mongo_client(php_phongo_manager_t *manager, const mongoc_uri_t *uri, zval *driverOptions TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID >= 70000
	zval                      *zdebug = NULL;
	zval                      *zcontext = NULL;
#else
	zval                     **zdebug = NULL;
	zval                     **zcontext = NULL;
#endif
	php_stream_context        *ctx = NULL;
	const char                *mech, *mongoc_version, *bson_version;
	mongoc_client_t           *client;

	ENTRY;

#if PHP_VERSION_ID >= 70000
	if (driverOptions && (zdebug = zend_hash_str_find(Z_ARRVAL_P(driverOptions), "debug", sizeof("debug")-1)) != NULL) {
		zend_string *key = zend_string_init(PHONGO_DEBUG_INI, sizeof(PHONGO_DEBUG_INI)-1, 0);
		zend_string *value_str = zval_get_string(zdebug);
		zend_alter_ini_entry_ex(key, value_str, PHP_INI_USER, PHP_INI_STAGE_RUNTIME, 0);
		zend_string_release(key);
		zend_string_release(value_str);
	}
#else
	if (driverOptions && zend_hash_find(Z_ARRVAL_P(driverOptions), "debug", strlen("debug") + 1, (void**)&zdebug) == SUCCESS) {
		convert_to_string(*zdebug);

		zend_alter_ini_entry_ex((char *)PHONGO_DEBUG_INI, sizeof(PHONGO_DEBUG_INI), Z_STRVAL_PP(zdebug), Z_STRLEN_PP(zdebug), PHP_INI_USER, PHP_INI_STAGE_RUNTIME, 0 TSRMLS_CC);
	}
#endif

#if PHP_VERSION_ID >= 70000
	if (driverOptions && (zcontext = zend_hash_str_find(Z_ARRVAL_P(driverOptions), "context", sizeof("context")-1)) != NULL) {
		ctx = php_stream_context_from_zval(zcontext, 0);
#else
	if (driverOptions && zend_hash_find(Z_ARRVAL_P(driverOptions), "context", strlen("context") + 1, (void**)&zcontext) == SUCCESS) {
		ctx = php_stream_context_from_zval(*zcontext, 0);
#endif
	} else {
		zval *tmp = NULL; /* PHP 5.x requires an lvalue */
		ctx = php_stream_context_from_zval(tmp, 0);
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
#if PHP_VERSION_ID >= 70000
		zval *pem;
#else
		zval **pem;
#endif

#if PHP_VERSION_ID >= 70000
		if ((pem = php_stream_context_get_option(ctx, "ssl", "local_cert")) != NULL) {
			zend_string *s = zval_get_string(pem);
#else
		if (SUCCESS == php_stream_context_get_option(ctx, "ssl", "local_cert", &pem)) {
			convert_to_string_ex(pem);
#endif
			/* mongoc_client_set_ssl_opts() copies mongoc_ssl_opt_t shallowly;
			 * its strings must be kept valid for the life of mongoc_client_t */
			manager->pem_file = ecalloc(1, MAXPATHLEN);

#if PHP_VERSION_ID >= 70000
			if (VCWD_REALPATH(ZSTR_VAL(s), manager->pem_file)) {
#else
			if (VCWD_REALPATH(Z_STRVAL_PP(pem), manager->pem_file)) {
#endif
				mongoc_ssl_opt_t ssl_options = {0};

				ssl_options.pem_file = manager->pem_file;
				mongoc_client_set_ssl_opts(client, &ssl_options);
			}
#if PHP_VERSION_ID >= 70000
			zend_string_release(s);
#endif
		}
	}

	mongoc_client_set_stream_initiator(client, phongo_stream_initiator, ctx);

	RETURN(client);
} /* }}} */

bool phongo_manager_init(php_phongo_manager_t *manager, const char *uri_string, bson_t *bson_options, zval *driverOptions TSRMLS_DC) /* {{{ */
{
	mongoc_uri_t             *uri;

	if (!(uri = php_phongo_make_uri(uri_string, bson_options))) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Failed to parse MongoDB URI: '%s'", uri_string);
		return false;
	}

	if (!php_phongo_apply_rc_options_to_uri(uri, bson_options TSRMLS_CC) ||
	    !php_phongo_apply_rp_options_to_uri(uri, bson_options TSRMLS_CC) ||
	    !php_phongo_apply_wc_options_to_uri(uri, bson_options TSRMLS_CC)) {
		/* Exception should already have been thrown */
		mongoc_uri_destroy(uri);
		return false;
	}

	manager->client = php_phongo_make_mongo_client(manager, uri, driverOptions TSRMLS_CC);
	mongoc_uri_destroy(uri);

	if (!manager->client) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to create Manager from URI: '%s'", uri_string);
		return false;
	}

	/* Keep a reference to driverOptions, since it may be referenced later for
	 * lazy stream initialization. */
	if (driverOptions) {
#if PHP_VERSION_ID >= 70000
		ZVAL_COPY(&manager->driverOptions, driverOptions);
#else
		Z_ADDREF_P(driverOptions);
		manager->driverOptions = driverOptions;
#endif
	}

	return true;
} /* }}} */

void php_phongo_new_utcdatetime_from_epoch(zval *object, int64_t msec_since_epoch TSRMLS_DC) /* {{{ */
{
	php_phongo_utcdatetime_t     *intern;

	object_init_ex(object, php_phongo_utcdatetime_ce);

	intern = Z_UTCDATETIME_OBJ_P(object);
	intern->milliseconds = msec_since_epoch;
	intern->initialized = true;
} /* }}} */

void php_phongo_new_timestamp_from_increment_and_timestamp(zval *object, uint32_t increment, uint32_t timestamp TSRMLS_DC) /* {{{ */
{
	php_phongo_timestamp_t     *intern;

	object_init_ex(object, php_phongo_timestamp_ce);

	intern = Z_TIMESTAMP_OBJ_P(object);
	intern->increment = increment;
	intern->timestamp = timestamp;
	intern->initialized = true;
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

	intern = Z_JAVASCRIPT_OBJ_P(object);
	intern->code = estrndup(code, code_len);
	intern->code_len = code_len;
	intern->scope = scope ? bson_copy(scope) : NULL;
} /* }}} */
void php_phongo_new_binary_from_binary_and_type(zval *object, const char *data, size_t data_len, bson_subtype_t type TSRMLS_DC) /* {{{ */
{
	php_phongo_binary_t     *intern;

	object_init_ex(object, php_phongo_binary_ce);

	intern = Z_BINARY_OBJ_P(object);
	intern->data = estrndup(data, data_len);
	intern->data_len = data_len;
	intern->type = (uint8_t) type;
} /* }}} */

void php_phongo_new_decimal128(zval *object, const bson_decimal128_t *decimal TSRMLS_DC) /* {{{ */
{
	php_phongo_decimal128_t *intern;

	object_init_ex(object, php_phongo_decimal128_ce);

	intern = Z_DECIMAL128_OBJ_P(object);
	memcpy(&intern->decimal, decimal, sizeof(bson_decimal128_t));
	intern->initialized = true;
} /* }}} */

void php_phongo_new_regex_from_regex_and_options(zval *object, const char *pattern, const char *flags TSRMLS_DC) /* {{{ */
{
	php_phongo_regex_t     *intern;

	object_init_ex(object, php_phongo_regex_ce);

	intern = Z_REGEX_OBJ_P(object);
	intern->pattern_len = strlen(pattern);
	intern->pattern = estrndup(pattern, intern->pattern_len);
	intern->flags_len = strlen(flags);
	intern->flags = estrndup(flags, intern->flags_len);
} /* }}} */

static void php_phongo_cursor_free_current(php_phongo_cursor_t *cursor) /* {{{ */
{
	if (!Z_ISUNDEF(cursor->visitor_data.zchild)) {
		zval_ptr_dtor(&cursor->visitor_data.zchild);
#if PHP_VERSION_ID >= 70000
		ZVAL_UNDEF(&cursor->visitor_data.zchild);
#else
		cursor->visitor_data.zchild = NULL;
#endif
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

	if (!Z_ISUNDEF(cursor_it->intern.data)) {
#if PHP_VERSION_ID >= 70000
		zval_ptr_dtor(&cursor_it->intern.data);
#else
		zval_ptr_dtor((zval**)&cursor_it->intern.data);
		cursor_it->intern.data = NULL;
#endif
	}

#if PHP_VERSION_ID < 70000
	efree(cursor_it);
#endif
} /* }}} */

static int php_phongo_cursor_iterator_valid(zend_object_iterator *iter TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *cursor = ((php_phongo_cursor_iterator *)iter)->cursor;

	if (!Z_ISUNDEF(cursor->visitor_data.zchild)) {
		return SUCCESS;
	}

	return FAILURE;
} /* }}} */

#if PHP_VERSION_ID < 50500
static int php_phongo_cursor_iterator_get_current_key(zend_object_iterator *iter, char **str_key, uint *str_key_len, ulong *int_key TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *cursor = ((php_phongo_cursor_iterator *)iter)->cursor;

	*int_key = (ulong) cursor->current;
	return HASH_KEY_IS_LONG;
} /* }}} */
#else
static void php_phongo_cursor_iterator_get_current_key(zend_object_iterator *iter, zval *key TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *cursor = ((php_phongo_cursor_iterator *)iter)->cursor;

	ZVAL_LONG(key, cursor->current);
} /* }}} */
#endif

#if PHP_VERSION_ID < 70000
static void php_phongo_cursor_iterator_get_current_data(zend_object_iterator *iter, zval ***data TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_t *cursor = ((php_phongo_cursor_iterator *)iter)->cursor;

	*data = &cursor->visitor_data.zchild;
} /* }}} */
#else
static zval* php_phongo_cursor_iterator_get_current_data(zend_object_iterator *iter) /* {{{ */
{
	php_phongo_cursor_t *cursor = ((php_phongo_cursor_iterator *)iter)->cursor;

	return &cursor->visitor_data.zchild;
} /* }}} */
#endif

static void php_phongo_cursor_iterator_move_forward(zend_object_iterator *iter TSRMLS_DC) /* {{{ */
{
	php_phongo_cursor_iterator *cursor_it = (php_phongo_cursor_iterator *)iter;
	php_phongo_cursor_t        *cursor = cursor_it->cursor;
	const bson_t               *doc;

	php_phongo_cursor_free_current(cursor);
	cursor->current++;

	if (mongoc_cursor_next(cursor->cursor, &doc)) {
		phongo_bson_to_zval_ex(bson_get_data(doc), doc->len, &cursor->visitor_data);
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

	if (cursor->current > 0) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC TSRMLS_CC, "Cursors cannot rewind after starting iteration");
		return;
	}

	php_phongo_cursor_free_current(cursor);

	doc = mongoc_cursor_current(cursor->cursor);

	if (doc) {
		phongo_bson_to_zval_ex(bson_get_data(doc), doc->len, &cursor->visitor_data);
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
	php_phongo_cursor_iterator *cursor_it = NULL;
	php_phongo_cursor_t *cursor = Z_CURSOR_OBJ_P(object);

	if (by_ref) {
		zend_error(E_ERROR, "An iterator cannot be used with foreach by reference");
	}

	if (cursor->got_iterator) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC TSRMLS_CC, "Cursors cannot yield multiple iterators");
		return NULL;
	}

	cursor->got_iterator = 1;

	cursor_it = ecalloc(1, sizeof(php_phongo_cursor_iterator));
#if PHP_VERSION_ID >= 70000
	zend_iterator_init(&cursor_it->intern);
#endif

#if PHP_VERSION_ID >= 70000
	ZVAL_COPY(&cursor_it->intern.data, object);
#else
	Z_ADDREF_P(object);
	cursor_it->intern.data  = (void*)object;
#endif
	cursor_it->intern.funcs = &php_phongo_cursor_iterator_funcs;
	cursor_it->cursor = cursor;
	/* cursor_it->current should already be allocated to zero */

	php_phongo_cursor_free_current(cursor_it->cursor);

	return &cursor_it->intern;
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

	if (!new_value || (new_value && !ZSTR_VAL(new_value)[0])
		|| strcasecmp("0", ZSTR_VAL(new_value)) == 0
		|| strcasecmp("off", ZSTR_VAL(new_value)) == 0
		|| strcasecmp("no", ZSTR_VAL(new_value)) == 0
		|| strcasecmp("false", ZSTR_VAL(new_value)) == 0
	   ) {
		mongoc_log_trace_disable();
		mongoc_log_set_handler(NULL, NULL);

#if PHP_VERSION_ID >= 70000
		return OnUpdateString(entry, new_value, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);
#else
		return OnUpdateString(entry, new_value, new_value_length, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);
#endif
	}


	if (strcasecmp(ZSTR_VAL(new_value), "stderr") == 0) {
		MONGODB_G(debug_fd) = stderr;
	} else if (strcasecmp(ZSTR_VAL(new_value), "stdout") == 0) {
		MONGODB_G(debug_fd) = stdout;
	} else if (
		strcasecmp("1", ZSTR_VAL(new_value)) == 0
		|| strcasecmp("on", ZSTR_VAL(new_value)) == 0
		|| strcasecmp("yes", ZSTR_VAL(new_value)) == 0
		|| strcasecmp("true", ZSTR_VAL(new_value)) == 0
	) {
		tmp_dir = NULL;
	} else {
		tmp_dir = ZSTR_VAL(new_value);
	}

	if (!MONGODB_G(debug_fd)) {
		time_t t;
		int fd = -1;
		char *prefix;
		int len;
		phongo_char *filename;

		time(&t);
		len = spprintf(&prefix, 0, "PHONGO-%ld", t);

		fd = php_open_temporary_fd(tmp_dir, prefix, &filename TSRMLS_CC);
		if (fd != -1) {
			const char *path = ZSTR_VAL(filename);
			MONGODB_G(debug_fd) = VCWD_FOPEN(path, "a");
		}
		efree(filename);
		efree(prefix);
		close(fd);
	}

	mongoc_log_trace_enable();
	mongoc_log_set_handler(php_phongo_log, ctx);

#if PHP_VERSION_ID >= 70000
	return OnUpdateString(entry, new_value, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);
#else
	return OnUpdateString(entry, new_value, new_value_length, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);
#endif
}


/* {{{ INI entries */
PHP_INI_BEGIN()
#if PHP_VERSION_ID >= 70000
	STD_PHP_INI_ENTRY(PHONGO_DEBUG_INI,      PHONGO_DEBUG_INI_DEFAULT,   PHP_INI_ALL, OnUpdateDebug, debug,      zend_mongodb_globals,    mongodb_globals)
#else
	{ 0, PHP_INI_ALL, (char *)PHONGO_DEBUG_INI, sizeof(PHONGO_DEBUG_INI), OnUpdateDebug, (void *) XtOffsetOf(zend_mongodb_globals, debug), (void *) &mglo, NULL, (char *)PHONGO_DEBUG_INI_DEFAULT, sizeof(PHONGO_DEBUG_INI_DEFAULT)-1, NULL, 0, 0, 0, NULL },
#endif
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
#if PHP_VERSION_ID >= 70000
#if defined(COMPILE_DL_MONGODB) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
#endif
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
	PHP_MINIT(ReadConcern)(INIT_FUNC_ARGS_PASSTHRU);
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
	PHP_MINIT(Decimal128)(INIT_FUNC_ARGS_PASSTHRU);
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
