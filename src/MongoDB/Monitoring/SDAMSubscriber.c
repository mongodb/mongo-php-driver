/*
 * Copyright 2021-present MongoDB, Inc.
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

#include "php_phongo.h"

zend_class_entry* php_phongo_sdamsubscriber_ce;

/* {{{ MongoDB\Driver\Monitoring\SDAMSubscriber function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_SDAMSubscriber_serverChanged, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerChangedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_SDAMSubscriber_serverClosed, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerClosedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_SDAMSubscriber_serverOpening, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerOpeningEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_SDAMSubscriber_serverHeartbeatFailed, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerHeartbeatFailedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_SDAMSubscriber_serverHeartbeatStarted, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerHeartbeatStartedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_SDAMSubscriber_serverHeartbeatSucceeded, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\ServerHeartbeatSucceededEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_SDAMSubscriber_topologyChanged, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\TopologyChangedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_SDAMSubscriber_topologyClosed, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\TopologyClosedEvent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_SDAMSubscriber_topologyOpening, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, event, MongoDB\\Driver\\Monitoring\\TopologyOpeningEvent, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_sdamsubscriber_me[] = {
	/* clang-format off */
	ZEND_ABSTRACT_ME(SDAMSubscriber, serverChanged, ai_SDAMSubscriber_serverChanged)
	ZEND_ABSTRACT_ME(SDAMSubscriber, serverClosed, ai_SDAMSubscriber_serverClosed)
	ZEND_ABSTRACT_ME(SDAMSubscriber, serverOpening, ai_SDAMSubscriber_serverOpening)
	ZEND_ABSTRACT_ME(SDAMSubscriber, serverHeartbeatFailed, ai_SDAMSubscriber_serverHeartbeatFailed)
	ZEND_ABSTRACT_ME(SDAMSubscriber, serverHeartbeatStarted, ai_SDAMSubscriber_serverHeartbeatStarted)
	ZEND_ABSTRACT_ME(SDAMSubscriber, serverHeartbeatSucceeded, ai_SDAMSubscriber_serverHeartbeatSucceeded)
	ZEND_ABSTRACT_ME(SDAMSubscriber, topologyChanged, ai_SDAMSubscriber_topologyChanged)
	ZEND_ABSTRACT_ME(SDAMSubscriber, topologyClosed, ai_SDAMSubscriber_topologyClosed)
	ZEND_ABSTRACT_ME(SDAMSubscriber, topologyOpening, ai_SDAMSubscriber_topologyOpening)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

void php_phongo_sdamsubscriber_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;
	(void) type;
	(void) module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Monitoring", "SDAMSubscriber", php_phongo_sdamsubscriber_me);
	php_phongo_sdamsubscriber_ce = zend_register_internal_interface(&ce);
	zend_class_implements(php_phongo_sdamsubscriber_ce, 1, php_phongo_subscriber_ce);

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
