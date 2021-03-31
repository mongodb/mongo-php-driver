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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

ZEND_EXTERN_MODULE_GLOBALS(mongodb)

/* {{{ proto void MongoDB\Driver\Monitoring\addSubscriber(MongoDB\Driver\Monitoring\Subscriber $subscriber)
   Adds a monitoring subscriber to the set of subscribers */
PHP_FUNCTION(MongoDB_Driver_Monitoring_addSubscriber)
{
	zval* subscriber;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(subscriber, php_phongo_subscriber_ce)
	PHONGO_PARSE_PARAMETERS_END();

	/* The HashTable should never be NULL, as it's initialized during RINIT and
	 * destroyed during RSHUTDOWN. This is simply a defensive guard. */
	if (!MONGODB_G(subscribers)) {
		return;
	}

	/* NOP if the subscriber was already registered */
	if (zend_hash_index_exists(MONGODB_G(subscribers), Z_OBJ_HANDLE_P(subscriber))) {
		return;
	}

	zend_hash_index_update(MONGODB_G(subscribers), Z_OBJ_HANDLE_P(subscriber), subscriber);
	Z_ADDREF_P(subscriber);
} /* }}} */

/* {{{ proto void MongoDB\Driver\Monitoring\removeSubscriber(MongoDB\Driver\Monitoring\Subscriber $subscriber)
   Removes a monitoring subscriber from the set of subscribers */
PHP_FUNCTION(MongoDB_Driver_Monitoring_removeSubscriber)
{
	zval* subscriber;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(subscriber, php_phongo_subscriber_ce)
	PHONGO_PARSE_PARAMETERS_END();

	/* The HashTable should never be NULL, as it's initialized during RINIT and
	 * destroyed during RSHUTDOWN. This is simply a defensive guard. */
	if (!MONGODB_G(subscribers)) {
		return;
	}

	/* Note: the HashTable specifies ZVAL_PTR_DTOR as its element destructor so
	 * there is no need to decrement the subscriber's reference count here. */
	zend_hash_index_del(MONGODB_G(subscribers), Z_OBJ_HANDLE_P(subscriber));
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
