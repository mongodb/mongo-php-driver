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
#include <ext/standard/php_var.h>

#if PHP_VERSION_ID >= 70000
# include <Zend/zend_smart_str.h>
#else
# include <ext/standard/php_smart_str.h>
#endif

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

/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"

#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "PHONGO"

#define PHONGO_DEBUG_INI "mongodb.debug"
#define PHONGO_DEBUG_INI_DEFAULT ""

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

zend_object_handlers *phongo_get_std_object_handlers(void)
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

zend_bool phongo_writeconcernerror_init(zval *return_value, bson_t *bson TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter;
	php_phongo_writeconcernerror_t *writeconcernerror;

	writeconcernerror = Z_WRITECONCERNERROR_OBJ_P(return_value);

	if (bson_iter_init_find(&iter, bson, "code") && BSON_ITER_HOLDS_INT32(&iter)) {
		writeconcernerror->code = bson_iter_int32(&iter);
	}
	if (bson_iter_init_find(&iter, bson, "errmsg") && BSON_ITER_HOLDS_UTF8(&iter)) {
		uint32_t errmsg_len;
		const char *err_msg = bson_iter_utf8(&iter, &errmsg_len);

		writeconcernerror->message = estrndup(err_msg, errmsg_len);
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
		uint32_t errmsg_len;
		const char *err_msg = bson_iter_utf8(&iter, &errmsg_len);

		writeerror->message = estrndup(err_msg, errmsg_len);
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

	if (bulk_write->executed) {
		phongo_throw_exception(PHONGO_ERROR_WRITE_FAILED TSRMLS_CC, "BulkWrite objects may only be executed once and this instance has already been executed");
		return false;
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

	cursor = mongoc_collection_find_with_opts(collection, query->filter, query->opts, phongo_read_preference_from_zval(zreadPreference TSRMLS_CC));
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
		ADD_ASSOC_STRING(retval, "level", level);
	}
} /* }}} */

/* Prepare tagSets for BSON encoding by converting each array in the set to an
 * object. This ensures that empty arrays will serialize as empty documents.
 *
 * php_phongo_read_preference_tags_are_valid() handles actual validation of the
 * tag set structure. */
void php_phongo_read_preference_prep_tagsets(zval *tagSets TSRMLS_DC) /* {{{ */
{
	HashTable     *ht_data;

	if (Z_TYPE_P(tagSets) != IS_ARRAY) {
		return;
	}

	ht_data = HASH_OF(tagSets);

#if PHP_VERSION_ID >= 70000
	{
		zval *tagSet;

		ZEND_HASH_FOREACH_VAL(ht_data, tagSet) {
			if (Z_TYPE_P(tagSet) == IS_ARRAY) {
				convert_to_object(tagSet);
			}
		} ZEND_HASH_FOREACH_END();
	}
#else
	{
		HashPosition   pos;
		zval         **tagSet;

		for (zend_hash_internal_pointer_reset_ex(ht_data, &pos);
		     zend_hash_get_current_data_ex(ht_data, (void **) &tagSet, &pos) == SUCCESS;
		     zend_hash_move_forward_ex(ht_data, &pos)) {
			if (Z_TYPE_PP(tagSet) == IS_ARRAY) {
				convert_to_object(*tagSet);
			}
		}
	}
#endif

	return;
} /* }}} */

/* Checks if tags is valid to set on a mongoc_read_prefs_t. It may be null or an
 * array of one or more documents. */
bool php_phongo_read_preference_tags_are_valid(const bson_t *tags) /* {{{ */
{
	bson_iter_t iter;

	if (bson_empty0(tags)) {
		return true;
	}

	if (!bson_iter_init(&iter, tags)) {
		return false;
	}

	while (bson_iter_next(&iter)) {
		if (!BSON_ITER_HOLDS_DOCUMENT(&iter) && !BSON_ITER_HOLDS_ARRAY(&iter)) {
			return false;
		}
	}

	return true;
} /* }}} */

void php_phongo_read_preference_to_zval(zval *retval, const mongoc_read_prefs_t *read_prefs) /* {{{ */
{
	const bson_t *tags = mongoc_read_prefs_get_tags(read_prefs);
	mongoc_read_mode_t mode = mongoc_read_prefs_get_mode(read_prefs);

	array_init_size(retval, 3);

	switch (mode) {
		case MONGOC_READ_PRIMARY: ADD_ASSOC_STRING(retval, "mode", "primary"); break;
		case MONGOC_READ_PRIMARY_PREFERRED: ADD_ASSOC_STRING(retval, "mode", "primaryPreferred"); break;
		case MONGOC_READ_SECONDARY: ADD_ASSOC_STRING(retval, "mode", "secondary"); break;
		case MONGOC_READ_SECONDARY_PREFERRED: ADD_ASSOC_STRING(retval, "mode", "secondaryPreferred"); break;
		case MONGOC_READ_NEAREST: ADD_ASSOC_STRING(retval, "mode", "nearest"); break;
		default: /* Do nothing */
			break;
	}

	if (!bson_empty0(tags)) {
		/* Use PHONGO_TYPEMAP_NATIVE_ARRAY for the root type since tags is an
		 * array; however, inner documents and arrays can use the default. */
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		phongo_bson_to_zval_ex(bson_get_data(tags), tags->len, &state);
#if PHP_VERSION_ID >= 70000
		ADD_ASSOC_ZVAL_EX(retval, "tags", &state.zchild);
#else
		ADD_ASSOC_ZVAL_EX(retval, "tags", state.zchild);
#endif
	}

	if (mongoc_read_prefs_get_max_staleness_seconds(read_prefs) != MONGOC_NO_MAX_STALENESS) {
		ADD_ASSOC_LONG_EX(retval, "maxStalenessSeconds", mongoc_read_prefs_get_max_staleness_seconds(read_prefs));
	}
} /* }}} */

void php_phongo_write_concern_to_zval(zval *retval, const mongoc_write_concern_t *write_concern) /* {{{ */
{
	const char *wtag = mongoc_write_concern_get_wtag(write_concern);
	const int32_t w = mongoc_write_concern_get_w(write_concern);
	const int32_t wtimeout = mongoc_write_concern_get_wtimeout(write_concern);

	array_init_size(retval, 4);

	if (wtag) {
		ADD_ASSOC_STRING(retval, "w", wtag);
	} else if (mongoc_write_concern_get_wmajority(write_concern)) {
		ADD_ASSOC_STRING(retval, "w", PHONGO_WRITE_CONCERN_W_MAJORITY);
	} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
		ADD_ASSOC_LONG_EX(retval, "w", w);
	}

	if (mongoc_write_concern_journal_is_set(write_concern)) {
		ADD_ASSOC_BOOL_EX(retval, "j", mongoc_write_concern_get_journal(write_concern));
	}

	if (wtimeout != 0) {
		ADD_ASSOC_LONG_EX(retval, "wtimeout", wtimeout);
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
			    !strcasecmp(key, "wtimeoutms") ||
			    !strcasecmp(key, "maxstalenessseconds") ||
			    !strcasecmp(key, "appname")
			) {
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
	    !bson_iter_init_find_case(&iter, options, "readpreferencetags") &&
	    !bson_iter_init_find_case(&iter, options, "maxstalenessseconds")
	) {
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

		if (!bson_init_static(&tags, data, len)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Could not initialize BSON structure for read preference tags");
			mongoc_read_prefs_destroy(new_rp);

			return false;
		}

		if (!php_phongo_read_preference_tags_are_valid(&tags)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Read preference tags must be an array of zero or more documents");
			mongoc_read_prefs_destroy(new_rp);

			return false;
		}

		mongoc_read_prefs_set_tags(new_rp, &tags);
	}

	if (mongoc_read_prefs_get_mode(new_rp) == MONGOC_READ_PRIMARY &&
	    !bson_empty(mongoc_read_prefs_get_tags(new_rp))) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Primary read preference mode conflicts with tags");
		mongoc_read_prefs_destroy(new_rp);

		return false;
	}

	/* Handle maxStalenessSeconds, and make sure it is not combined with primary
	 * readPreference */
	if (bson_iter_init_find_case(&iter, options, "maxstalenessseconds") && BSON_ITER_HOLDS_INT32(&iter)) {
		int32_t max_staleness_seconds = bson_iter_int32(&iter);

		if (max_staleness_seconds != MONGOC_NO_MAX_STALENESS) {

			if (max_staleness_seconds < MONGOC_SMALLEST_MAX_STALENESS_SECONDS) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected maxStalenessSeconds to be >= %d, %" PRId32 " given", MONGOC_SMALLEST_MAX_STALENESS_SECONDS, max_staleness_seconds);
				mongoc_read_prefs_destroy(new_rp);

				return false;
			}

			/* max_staleness_seconds is fetched as an INT32, so there is no need to check
			 * if it exists INT32_MAX as we do in the ReadPreference constructor. */

			if (mongoc_read_prefs_get_mode(new_rp) == MONGOC_READ_PRIMARY) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Primary read preference mode conflicts with maxStalenessSeconds");
				mongoc_read_prefs_destroy(new_rp);

				return false;
			}
		}

		mongoc_read_prefs_set_max_staleness_seconds(new_rp, max_staleness_seconds);
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

static inline char *php_phongo_fetch_ssl_opt_string(zval *zoptions, const char *key, int key_len)
{
	int        plen;
	zend_bool  pfree;
	char      *pval, *value;

	pval = php_array_fetchl_string(zoptions, key, key_len, &plen, &pfree);
	value = pfree ? pval : estrndup(pval, plen);

	return value;
}

static mongoc_ssl_opt_t *php_phongo_make_ssl_opt(zval *zoptions TSRMLS_DC)
{
	mongoc_ssl_opt_t *ssl_opt;

	if (!zoptions) {
		return NULL;
	}

#if defined(MONGOC_ENABLE_SSL_SECURE_CHANNEL) || defined(MONGOC_ENABLE_SSL_SECURE_TRANSPORT)
	if (php_array_existsc(zoptions, "ca_dir")) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "\"ca_dir\" option is not supported by Secure Channel and Secure Transport");
		return NULL;
	}

	if (php_array_existsc(zoptions, "capath")) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "\"capath\" option is not supported by Secure Channel and Secure Transport");
		return NULL;
	}
#endif

#if defined(MONGOC_ENABLE_SSL_LIBRESSL) || defined(MONGOC_ENABLE_SSL_SECURE_TRANSPORT)
	if (php_array_existsc(zoptions, "crl_file")) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "\"crl_file\" option is not supported by LibreSSL and Secure Transport");
		return NULL;
	}
#endif

	ssl_opt = ecalloc(1, sizeof(mongoc_ssl_opt_t));

	/* Check canonical option names first and fall back to SSL context options
	 * for backwards compatibility. */
	if (php_array_existsc(zoptions, "allow_invalid_hostname")) {
		ssl_opt->allow_invalid_hostname = php_array_fetchc_bool(zoptions, "allow_invalid_hostname");
	}

	if (php_array_existsc(zoptions, "weak_cert_validation")) {
		ssl_opt->weak_cert_validation = php_array_fetchc_bool(zoptions, "weak_cert_validation");
	} else if (php_array_existsc(zoptions, "allow_self_signed")) {
		ssl_opt->weak_cert_validation = php_array_fetchc_bool(zoptions, "allow_self_signed");
	}

	if (php_array_existsc(zoptions, "pem_file")) {
		ssl_opt->pem_file = php_phongo_fetch_ssl_opt_string(zoptions, ZEND_STRL("pem_file"));
	} else if (php_array_existsc(zoptions, "local_cert")) {
		ssl_opt->pem_file = php_phongo_fetch_ssl_opt_string(zoptions, ZEND_STRL("local_cert"));
	}

	if (php_array_existsc(zoptions, "pem_pwd")) {
		ssl_opt->pem_pwd = php_phongo_fetch_ssl_opt_string(zoptions, ZEND_STRL("pem_pwd"));
	} else if (php_array_existsc(zoptions, "passphrase")) {
		ssl_opt->pem_pwd = php_phongo_fetch_ssl_opt_string(zoptions, ZEND_STRL("passphrase"));
	}

	if (php_array_existsc(zoptions, "ca_file")) {
		ssl_opt->ca_file = php_phongo_fetch_ssl_opt_string(zoptions, ZEND_STRL("ca_file"));
	} else if (php_array_existsc(zoptions, "cafile")) {
		ssl_opt->ca_file = php_phongo_fetch_ssl_opt_string(zoptions, ZEND_STRL("cafile"));
	}

	if (php_array_existsc(zoptions, "ca_dir")) {
		ssl_opt->ca_dir = php_phongo_fetch_ssl_opt_string(zoptions, ZEND_STRL("ca_dir"));
	} else if (php_array_existsc(zoptions, "capath")) {
		ssl_opt->ca_dir = php_phongo_fetch_ssl_opt_string(zoptions, ZEND_STRL("capath"));
	}

	if (php_array_existsc(zoptions, "crl_file")) {
		ssl_opt->crl_file = php_phongo_fetch_ssl_opt_string(zoptions, ZEND_STRL("crl_file"));
	}

	return ssl_opt;
}

static void php_phongo_free_ssl_opt(mongoc_ssl_opt_t *ssl_opt)
{
	if (ssl_opt->pem_file) {
		str_efree(ssl_opt->pem_file);
	}

	if (ssl_opt->pem_pwd) {
		str_efree(ssl_opt->pem_pwd);
	}

	if (ssl_opt->ca_file) {
		str_efree(ssl_opt->ca_file);
	}

	if (ssl_opt->ca_dir) {
		str_efree(ssl_opt->ca_dir);
	}

	if (ssl_opt->crl_file) {
		str_efree(ssl_opt->crl_file);
	}

	efree(ssl_opt);
}

/* Creates a hash for a client by concatenating the URI string with serialized
 * options arrays. On success, a persistent string is returned (i.e. pefree()
 * should be used to free it) and hash_len will be set to the string's length.
 * On error, an exception will have been thrown and NULL will be returned. */
static char *php_phongo_manager_make_client_hash(const char *uri_string, zval *options, zval *driverOptions, size_t *hash_len TSRMLS_DC)
{
	char                 *hash = NULL;
	smart_str             var_buf = {0};
	php_serialize_data_t  var_hash;

#if PHP_VERSION_ID >= 70000
	zval                  args;

	array_init_size(&args, 3);
	ADD_ASSOC_STRING(&args, "uri", uri_string);

	if (options) {
		ADD_ASSOC_ZVAL_EX(&args, "options", options);
		Z_ADDREF_P(options);
	} else {
		ADD_ASSOC_NULL_EX(&args, "options");
	}

	if (driverOptions) {
		ADD_ASSOC_ZVAL_EX(&args, "driverOptions", driverOptions);
		Z_ADDREF_P(driverOptions);
	} else {
		ADD_ASSOC_NULL_EX(&args, "driverOptions");
	}

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&var_buf, &args, &var_hash);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	if (!EG(exception)) {
		*hash_len = ZSTR_LEN(var_buf.s);
		hash = pestrndup(ZSTR_VAL(var_buf.s), *hash_len, 1);
	}

	zval_ptr_dtor(&args);
#else
	zval                 *args;

	MAKE_STD_ZVAL(args);
	array_init_size(args, 3);
	ADD_ASSOC_STRING(args, "uri", uri_string);

	if (options) {
		ADD_ASSOC_ZVAL_EX(args, "options", options);
		Z_ADDREF_P(options);
	} else {
		ADD_ASSOC_NULL_EX(args, "options");
	}

	if (driverOptions) {
		ADD_ASSOC_ZVAL_EX(args, "driverOptions", driverOptions);
		Z_ADDREF_P(driverOptions);
	} else {
		ADD_ASSOC_NULL_EX(args, "driverOptions");
	}

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&var_buf, &args, &var_hash TSRMLS_CC);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	if (!EG(exception)) {
		*hash_len = var_buf.len;
		hash = pestrndup(var_buf.c, *hash_len, 1);
	}

	zval_ptr_dtor(&args);
#endif

	smart_str_free(&var_buf);

	return hash;
}

static mongoc_client_t *php_phongo_make_mongo_client(const mongoc_uri_t *uri, mongoc_ssl_opt_t *ssl_opt TSRMLS_DC) /* {{{ */
{
	const char      *mongoc_version, *bson_version;
	mongoc_client_t *client;

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
		return NULL;
	}

	if (mongoc_uri_get_ssl(uri) && ssl_opt) {
		mongoc_client_set_ssl_opts(client, ssl_opt);
	}

	return client;
} /* }}} */

void phongo_manager_init(php_phongo_manager_t *manager, const char *uri_string, zval *options, zval *driverOptions TSRMLS_DC) /* {{{ */
{
	char             *hash = NULL;
	size_t            hash_len = 0;
	bson_t            bson_options = BSON_INITIALIZER;
	mongoc_uri_t     *uri = NULL;
	mongoc_ssl_opt_t *ssl_opt = NULL;
	bson_iter_t       iter;

#if PHP_VERSION_ID >= 70000
	zval             *client_ptr;
	zval              new_client_ptr;
#else
	mongoc_client_t **client_ptr;
#endif

	if (!(hash = php_phongo_manager_make_client_hash(uri_string, options, driverOptions, &hash_len TSRMLS_CC))) {
		/* Exception should already have been thrown and there is nothing to free */
		return;
	}

#if PHP_VERSION_ID >= 70000
	if ((client_ptr = zend_hash_str_find(&MONGODB_G(clients), hash, hash_len)) && Z_TYPE_P(client_ptr) == IS_PTR) {
		MONGOC_DEBUG("Found client for hash: %s\n", hash);
		manager->client = (mongoc_client_t *)Z_PTR_P(client_ptr);
		goto cleanup;
	}
#else
	if (zend_hash_find(&MONGODB_G(clients), hash, hash_len + 1, (void**) &client_ptr) == SUCCESS) {
		MONGOC_DEBUG("Found client for hash: %s\n", hash);
		manager->client = *client_ptr;
		goto cleanup;
	}
#endif

	if (options) {
		phongo_zval_to_bson(options, PHONGO_BSON_NONE, &bson_options, NULL TSRMLS_CC);
	}

	/* An exception may be thrown during BSON conversion */
	if (EG(exception)) {
		goto cleanup;
	}

	if (!(uri = php_phongo_make_uri(uri_string, &bson_options))) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Failed to parse MongoDB URI: '%s'", uri_string);
		goto cleanup;
	}

	if (!php_phongo_apply_rc_options_to_uri(uri, &bson_options TSRMLS_CC) ||
	    !php_phongo_apply_rp_options_to_uri(uri, &bson_options TSRMLS_CC) ||
	    !php_phongo_apply_wc_options_to_uri(uri, &bson_options TSRMLS_CC)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (bson_iter_init_find_case(&iter, &bson_options, "appname") && BSON_ITER_HOLDS_UTF8(&iter)) {
		const char *str = bson_iter_utf8(&iter, NULL);

		if (!mongoc_uri_set_appname(uri, str)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Invalid appname value: '%s'", str);
			goto cleanup;
		}
	}

	ssl_opt = php_phongo_make_ssl_opt(driverOptions TSRMLS_CC);

	/* An exception may be thrown during SSL option creation */
	if (EG(exception)) {
		goto cleanup;
	}

	manager->client = php_phongo_make_mongo_client(uri, ssl_opt TSRMLS_CC);

	if (!manager->client) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to create Manager from URI: '%s'", uri_string);
		goto cleanup;
	}

	MONGOC_DEBUG("Created client hash: %s\n", hash);
#if PHP_VERSION_ID >= 70000
	ZVAL_PTR(&new_client_ptr, manager->client);
	zend_hash_str_update(&MONGODB_G(clients), hash, hash_len, &new_client_ptr);
#else
	zend_hash_update(&MONGODB_G(clients), hash, hash_len + 1, &manager->client, sizeof(mongoc_client_t *), NULL);
#endif

cleanup:
	if (hash) {
		pefree(hash, 1);
	}

	bson_destroy(&bson_options);

	if (uri) {
		mongoc_uri_destroy(uri);
	}

	if (ssl_opt) {
		php_phongo_free_ssl_opt(ssl_opt);
	}
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
	return pemalloc(num_bytes, 1);
} /* }}} */

static void* php_phongo_calloc(size_t num_members, size_t num_bytes) /* {{{ */
{
	return pecalloc(num_members, num_bytes, 1);
} /* }}} */

static void* php_phongo_realloc(void *mem, size_t num_bytes) { /* {{{ */
	return perealloc(mem, num_bytes, 1);
} /* }}} */

static void php_phongo_free(void *mem) /* {{{ */
{
	if (mem) {
		pefree(mem, 1);
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

#if PHP_VERSION_ID >= 70000
static void php_phongo_client_dtor(zval *zv)
{
	mongoc_client_destroy((mongoc_client_t *) Z_PTR_P(zv));
}
#else
static void php_phongo_client_dtor(void *client)
{
	mongoc_client_destroy(*((mongoc_client_t **) client));
}
#endif

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
	memset(mongodb_globals, 0, sizeof(zend_mongodb_globals));
	mongodb_globals->bsonMemVTable = bsonMemVTable;
	/* Initialize HashTable for persistent clients */
	zend_hash_init_ex(&mongodb_globals->clients, 0, NULL, php_phongo_client_dtor, 1, 0);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(mongodb)
{
	char *php_version_string;

	(void)type; /* We don't care if we are loaded via dl() or extension= */

	REGISTER_INI_ENTRIES();

	/* Initialize libmongoc */
	mongoc_init();

	/* Set handshake options */
	php_version_string = malloc(4 + sizeof(PHP_VERSION) + 1);
	snprintf(php_version_string, 4 + sizeof(PHP_VERSION) + 1, "PHP %s", PHP_VERSION);
	mongoc_handshake_data_append("ext-mongodb:PHP", MONGODB_VERSION_S, php_version_string);
	free(php_version_string);

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

	REGISTER_STRING_CONSTANT("MONGODB_VERSION", (char *)MONGODB_VERSION_S, CONST_CS | CONST_PERSISTENT);
	REGISTER_STRING_CONSTANT("MONGODB_STABILITY", (char *)MONGODB_STABILITY_S, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(mongodb)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */

	/* Destroy HashTable for persistent clients. The HashTable destructor will
	 * destroy any mongoc_client_t objects contained within. */
	zend_hash_destroy(&MONGODB_G(clients));

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
	ZEND_NS_FE("MongoDB\\BSON", fromPHP, ai_bson_fromPHP)
	ZEND_NS_FE("MongoDB\\BSON", toPHP,   ai_bson_toPHP)
	ZEND_NS_FE("MongoDB\\BSON", toJSON,    ai_bson_toJSON)
	ZEND_NS_FE("MongoDB\\BSON", fromJSON,  ai_bson_fromJSON)
	PHP_FE_END
};
/* }}} */

static const zend_module_dep mongodb_deps[] = {
	ZEND_MOD_REQUIRED("date")
	ZEND_MOD_REQUIRED("json")
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_REQUIRED("standard")
	ZEND_MOD_END
};

/* {{{ mongodb_module_entry
 */
zend_module_entry mongodb_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	mongodb_deps,
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
