/*
 * Copyright 2016-2017 MongoDB, Inc.
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

#include <php.h>
#include <Zend/zend_interfaces.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_commandsubscriber_ce;

/* {{{ MongoDB\Driver\Monitoring\CommandSubscriber function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_CommandSubscriber_commandStarted, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\CommandStartedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_CommandSubscriber_commandSucceeded, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\CommandSucceededEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_CommandSubscriber_commandFailed, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\CommandFailedEvent, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_commandsubscriber_me[] = {
	/* clang-format off */
	ZEND_ABSTRACT_ME(CommandSubscriber, commandStarted, ai_CommandSubscriber_commandStarted)
	ZEND_ABSTRACT_ME(CommandSubscriber, commandSucceeded, ai_CommandSubscriber_commandSucceeded)
	ZEND_ABSTRACT_ME(CommandSubscriber, commandFailed, ai_CommandSubscriber_commandFailed)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

void php_phongo_commandsubscriber_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;
	(void) type;
	(void) module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "CommandSubscriber", php_phongo_commandsubscriber_me);
	php_phongo_commandsubscriber_ce = zend_register_internal_interface(&ce TSRMLS_CC);
	zend_class_implements(php_phongo_commandsubscriber_ce TSRMLS_CC, 1, php_phongo_subscriber_ce);

	return;
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
