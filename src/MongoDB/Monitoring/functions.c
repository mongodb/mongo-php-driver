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

#include "mongoc/mongoc.h"

#include <php.h>

#include "php_phongo.h"
#include "phongo_apm.h"
#include "phongo_error.h"
#include "phongo_log.h"

ZEND_EXTERN_MODULE_GLOBALS(mongodb)

#define IS_APM_SUBSCRIBER(zv)                                              \
	instanceof_function(Z_OBJCE_P(zv), php_phongo_commandsubscriber_ce) || \
		instanceof_function(Z_OBJCE_P(zv), php_phongo_sdamsubscriber_ce)

#define IS_LOG_SUBSCRIBER(zv) instanceof_function(Z_OBJCE_P(zv), php_phongo_logsubscriber_ce)

/* Registers a global event subscriber */
PHP_FUNCTION(MongoDB_Driver_Monitoring_addSubscriber)
{
	zval* subscriber;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(subscriber, php_phongo_subscriber_ce)
	PHONGO_PARSE_PARAMETERS_END();

	// TODO: Consider throwing if subscriber is unsupported (see: PHPC-2289)

	if (IS_APM_SUBSCRIBER(subscriber)) {
		phongo_apm_add_subscriber(MONGODB_G(subscribers), subscriber);
	}

	if (IS_LOG_SUBSCRIBER(subscriber)) {
		phongo_log_add_logger(subscriber);
	}
}

/* Unregisters a global event subscriber */
PHP_FUNCTION(MongoDB_Driver_Monitoring_removeSubscriber)
{
	zval* subscriber;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(subscriber, php_phongo_subscriber_ce)
	PHONGO_PARSE_PARAMETERS_END();

	if (IS_APM_SUBSCRIBER(subscriber)) {
		phongo_apm_remove_subscriber(MONGODB_G(subscribers), subscriber);
	}

	if (IS_LOG_SUBSCRIBER(subscriber)) {
		phongo_log_remove_logger(subscriber);
	}
}

/* Log a message through libmongoc (used for internal testing) */
PHP_FUNCTION(MongoDB_Driver_Monitoring_mongoc_log)
{
	zend_long level;
	char *    domain, *message;
	size_t    domain_len, message_len;

	PHONGO_PARSE_PARAMETERS_START(3, 3)
	Z_PARAM_LONG(level)
	Z_PARAM_STRING(domain, domain_len)
	Z_PARAM_STRING(message, message_len)
	PHONGO_PARSE_PARAMETERS_END();

	if (level < MONGOC_LOG_LEVEL_ERROR || level > MONGOC_LOG_LEVEL_TRACE) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected level to be >= %d and <= %d, %" PHONGO_LONG_FORMAT " given", MONGOC_LOG_LEVEL_ERROR, MONGOC_LOG_LEVEL_TRACE, level);
		return;
	}

	if (strlen(domain) != domain_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Domain cannot contain null bytes. Unexpected null byte after \"%s\".", domain);
		return;
	}

	if (strlen(message) != message_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Message cannot contain null bytes. Unexpected null byte after \"%s\".", message);
		return;
	}

	mongoc_log(level, domain, "%s", message);
}
