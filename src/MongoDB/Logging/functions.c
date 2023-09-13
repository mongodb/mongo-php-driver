/*
 * Copyright 2016-present MongoDB, Inc.
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

#include <php.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_log.h"

#define PHONGO_LOG_DOMAIN "php"

/* Registers a global logger */
PHP_FUNCTION(MongoDB_Driver_Logging_addLogger)
{
	zval* logger;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(logger, php_phongo_logger_ce)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_log_add_logger(logger);
}

/* Log a message through libmongoc */
PHP_FUNCTION(MongoDB_Driver_Logging_log)
{
	zend_long level;
	char*     message;
	size_t    message_len;

	PHONGO_PARSE_PARAMETERS_START(2, 2)
	Z_PARAM_LONG(level)
	Z_PARAM_STRING(message, message_len)
	PHONGO_PARSE_PARAMETERS_END();

	/* Note: MONGOC_LOG_LEVEL_TRACE is intentionally unsupported. Trace logging
	 * is very verbose and often includes multi-line output, which takes the
	 * form of multiple log messages. Therefore, it is only reported via streams
	 * (i.e. mongodb.debug INI) and not to registered Logger objects.
	 *
	 * Additionally, the log() function exists mainly for internal usage by
	 * by PHPLIB, which does not need to log trace-level messages. */
	if (level < MONGOC_LOG_LEVEL_ERROR || level > MONGOC_LOG_LEVEL_DEBUG) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected level to be >= %d and <= %d, %" PHONGO_LONG_FORMAT " given", MONGOC_LOG_LEVEL_ERROR, MONGOC_LOG_LEVEL_DEBUG, level);
		return;
	}

	if (strlen(message) != message_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Log messages cannot contain null bytes. Unexpected null byte after \"%s\".", message);
		return;
	}

	mongoc_log(level, PHONGO_LOG_DOMAIN, "%s", message);
}

/* Unregisters a global logger */
PHP_FUNCTION(MongoDB_Driver_Logging_removeLogger)
{
	zval* logger;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(logger, php_phongo_logger_ce)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_log_remove_logger(logger);
}
