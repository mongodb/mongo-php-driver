/*
 * Copyright 2017-present MongoDB, Inc.
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

zend_class_entry* php_phongo_timestamp_interface_ce;

/* {{{ MongoDB\BSON\TimestampInterface function entries */
/* clang-format off */
ZEND_BEGIN_ARG_WITH_TENTATIVE_RETURN_TYPE_INFO_EX(ai_TimestampInterface___toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_TimestampInterface_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_timestamp_interface_me[] = {
	ZEND_ABSTRACT_ME(TimestampInterface, getIncrement, ai_TimestampInterface_void)
	ZEND_ABSTRACT_ME(TimestampInterface, getTimestamp, ai_TimestampInterface_void)
	ZEND_ABSTRACT_ME(TimestampInterface, __toString, ai_TimestampInterface___toString)
	PHP_FE_END
};
/* clang-format on */
/* }}} */

void php_phongo_timestamp_interface_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "TimestampInterface", php_phongo_timestamp_interface_me);
	php_phongo_timestamp_interface_ce = zend_register_internal_interface(&ce);
} /* }}} */
