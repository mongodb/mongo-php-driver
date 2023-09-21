/*
 * Copyright 2022-present MongoDB, Inc.
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

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <stdio.h>

#include <php.h>
#include <ext/date/php_date.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_operators.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_log.h"

ZEND_EXTERN_MODULE_GLOBALS(mongodb)

static void phongo_log_to_stream(FILE* stream, mongoc_log_level_t level, const char* domain, const char* message)
{
	struct timeval tv;
	time_t         t;
	zend_long      tu;
	zend_string*   dt;

	bson_gettimeofday(&tv);
	t  = tv.tv_sec;
	tu = tv.tv_usec;

	dt = php_format_date((char*) ZEND_STRL("Y-m-d\\TH:i:s"), t, 0);

	fprintf(stream, "[%s.%06" PHONGO_LONG_FORMAT "+00:00] %10s: %-8s> %s\n", ZSTR_VAL(dt), tu, domain, mongoc_log_level_str(level), message);
	fflush(stream);
	efree(dt);
}

/* Dispatch a log message to all registered loggers. The caller is responsible
 * for ensuring that loggers implement the correct interface. */
static void phongo_log_dispatch(mongoc_log_level_t level, const char* domain, const char* message)
{
	zval* logger;
	zval  func_name;
	zval  args[3];

	/* Trace logging is very verbose and often includes multi-line output, which
	 * takes the form of multiple log messages. Therefore, it is only reported
	 * via streams (i.e. mongodb.debug INI) and not to registered loggers. */
	if (level >= MONGOC_LOG_LEVEL_TRACE) {
		return;
	}

	ZVAL_STRING(&func_name, "log");
	ZVAL_LONG(&args[0], level);
	ZVAL_STRING(&args[1], domain);
	ZVAL_STRING(&args[2], message);

	ZEND_HASH_FOREACH_VAL_IND(MONGODB_G(loggers), logger)
	{
		zval retval;

		if (EG(exception)) {
			break;
		}

		call_user_function(NULL, logger, &func_name, &retval, 3, args);
		zval_ptr_dtor(&retval);
	}
	ZEND_HASH_FOREACH_END();

	zval_ptr_dtor(&func_name);
	zval_ptr_dtor(&args[0]);
	zval_ptr_dtor(&args[1]);
	zval_ptr_dtor(&args[2]);
}

static void phongo_log_handler(mongoc_log_level_t level, const char* domain, const char* message, void* user_data)
{
	if (MONGODB_G(debug_fd)) {
		phongo_log_to_stream(MONGODB_G(debug_fd), level, domain, message);
	}

	// Trace logs are only reported via streams and not to registered loggers
	if (level >= MONGOC_LOG_LEVEL_TRACE) {
		return;
	}

	if (MONGODB_G(loggers) && zend_hash_num_elements(MONGODB_G(loggers)) > 0) {
		phongo_log_dispatch(level, domain, message);
	}
}

/* Sets or unsets our libmongoc handler according to whether logging is enabled
 * (i.e. there is a stream or the loggers HashTable is not empty). This should
 * be called each time after updating the stream or logger HashTable. */
static void phongo_log_sync_handler(void)
{
	if (MONGODB_G(debug_fd) || (MONGODB_G(loggers) && zend_hash_num_elements(MONGODB_G(loggers)) > 0)) {
		// Trace logging is only needed if a stream is active
		if (MONGODB_G(debug_fd)) {
			mongoc_log_trace_enable();
		}

		mongoc_log_set_handler(phongo_log_handler, NULL);
	} else {
		mongoc_log_trace_disable();
		mongoc_log_set_handler(NULL, NULL);
	}
}

/* Checks args for adding/removing a logger. Returns true on success; otherwise,
 * throws an exception and returns false. */
static bool phongo_log_check_args_for_add_and_remove(HashTable* loggers, zval* logger)
{
	if (!loggers) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Logger HashTable is not initialized");
		return false;
	}

	if (!logger || Z_TYPE_P(logger) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(logger), php_phongo_logsubscriber_ce)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Logger is not an instance of %s", ZSTR_VAL(php_phongo_logsubscriber_ce->name));
		return false;
	}

	return true;
}

/* Adds a logger to the HashTable. Returns true on success (including NOP if
 * already registered); otherwise, throws an exception and returns false. */
bool phongo_log_add_logger(zval* logger)
{
	HashTable* loggers = MONGODB_G(loggers);

	if (!phongo_log_check_args_for_add_and_remove(loggers, logger)) {
		/* Exception should already have been thrown */
		return false;
	}

	/* NOP if the logger was already registered */
	if (zend_hash_index_exists(loggers, Z_OBJ_HANDLE_P(logger))) {
		return true;
	}

	zend_hash_index_update(loggers, Z_OBJ_HANDLE_P(logger), logger);
	Z_ADDREF_P(logger);

	/* Sync log handler after modifying the loggers HashTable */
	phongo_log_sync_handler();

	return true;
}

/* Removes a logger from the global HashTable. Returns true on success
 * (including NOP if never registered); otherwise, throws an exception and
 * returns false. */
bool phongo_log_remove_logger(zval* logger)
{
	HashTable* loggers = MONGODB_G(loggers);

	if (!phongo_log_check_args_for_add_and_remove(loggers, logger)) {
		/* Exception should already have been thrown */
		return false;
	}

	/* Note: the HashTable should specify ZVAL_PTR_DTOR as its element
	 * destructor so there is no need to decrement the logger's reference count
	 * here. We also don't care about whether zend_hash_index_del returns
	 * SUCCESS or FAILURE, as removing an unregistered logger is a NOP. */
	zend_hash_index_del(loggers, Z_OBJ_HANDLE_P(logger));

	/* Sync log handler after modifying the loggers HashTable */
	phongo_log_sync_handler();

	return true;
}

void phongo_log_set_stream(FILE* stream)
{
	FILE* prev_stream = MONGODB_G(debug_fd);

	/* NOP if the stream is not being changed */
	if (prev_stream == stream) {
		return;
	}

	/* Close the previous stream (excluding stderr/stdout) */
	if (prev_stream && prev_stream != stderr && prev_stream != stdout) {
		fclose(prev_stream);
	}

	MONGODB_G(debug_fd) = stream;

	phongo_log_sync_handler();
}
