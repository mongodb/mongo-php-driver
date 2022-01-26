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

#include <php.h>

#include "php_phongo.h"
#include "phongo_apm.h"
#include "phongo_error.h"

ZEND_EXTERN_MODULE_GLOBALS(mongodb)

/* {{{ proto void MongoDB\Driver\Monitoring\addSubscriber(MongoDB\Driver\Monitoring\Subscriber $subscriber)
   Registers a global event subscriber */
PHP_FUNCTION(MongoDB_Driver_Monitoring_addSubscriber)
{
	zval* subscriber;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(subscriber, php_phongo_subscriber_ce)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_apm_add_subscriber(MONGODB_G(subscribers), subscriber);
} /* }}} */

/* {{{ proto void MongoDB\Driver\Monitoring\removeSubscriber(MongoDB\Driver\Monitoring\Subscriber $subscriber)
   Unregisters a global event subscriber */
PHP_FUNCTION(MongoDB_Driver_Monitoring_removeSubscriber)
{
	zval* subscriber;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(subscriber, php_phongo_subscriber_ce)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_apm_remove_subscriber(MONGODB_G(subscribers), subscriber);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
