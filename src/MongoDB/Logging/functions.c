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

/* Registers a global logger */
PHP_FUNCTION(MongoDB_Driver_Logging_addLogger)
{
	zval* logger;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(logger, php_phongo_logger_ce)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_log_add_logger(logger);
}

/* Log a message */
PHP_FUNCTION(MongoDB_Driver_Logging_log)
{
	zend_long level;
	char*     domain;
	size_t    domain_len;
	char*     message;
	size_t    message_len;

	PHONGO_PARSE_PARAMETERS_START(3, 3)
	Z_PARAM_LONG(level)
	Z_PARAM_STRING(domain, domain_len)
	Z_PARAM_STRING(message, message_len)
	PHONGO_PARSE_PARAMETERS_END();

	/* TODO: throw if level is invalid */
	/* TODO: Consider throwing if domain or message contain null bytes */
	mongoc_log(level, domain, "%s", message);
}

/* Log a message */
PHP_FUNCTION(MongoDB_Driver_Logging_levelToString)
{
	zend_long level;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_LONG(level)
	PHONGO_PARSE_PARAMETERS_END();

	/* TODO: throw if level is invalid, instead of returning "UNKNOWN" */
	RETURN_STRING(mongoc_log_level_str(level));
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
