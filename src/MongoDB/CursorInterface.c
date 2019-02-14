/*
 * Copyright 2018 MongoDB, Inc.
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

zend_class_entry* php_phongo_cursor_interface_ce;

/* {{{ MongoDB\BSON\CursorInterface function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_CursorInterface_setTypeMap, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, typemap, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_CursorInterface_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_cursor_interface_me[] = {
	/* clang-format off */
	ZEND_ABSTRACT_ME(CursorInterface, getId, ai_CursorInterface_void)
	ZEND_ABSTRACT_ME(CursorInterface, getServer, ai_CursorInterface_void)
	ZEND_ABSTRACT_ME(CursorInterface, isDead, ai_CursorInterface_void)
	ZEND_ABSTRACT_ME(CursorInterface, setTypeMap, ai_CursorInterface_setTypeMap)
	ZEND_ABSTRACT_ME(CursorInterface, toArray, ai_CursorInterface_void)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

void php_phongo_cursor_interface_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "CursorInterface", php_phongo_cursor_interface_me);
	php_phongo_cursor_interface_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	zend_class_implements(php_phongo_cursor_interface_ce TSRMLS_CC, 1, zend_ce_traversable);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
