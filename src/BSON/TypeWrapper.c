/*
 * Copyright 2017 MongoDB, Inc.
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <php.h>

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry *php_phongo_typewrapper_ce;

/* {{{ MongoDB\BSON\TypeWrapper function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_TypeWrapper_createFromBSONType, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, type, MongoDB\\BSON\\Type, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_TypeWrapper_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_typewrapper_me[] = {
	ZEND_FENTRY(createFromBSONType, NULL, ai_TypeWrapper_createFromBSONType, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT|ZEND_ACC_STATIC)
	ZEND_ABSTRACT_ME(TypeWrapper, toBSONType, ai_TypeWrapper_void)
	PHP_FE_END
};
/* }}} */

void php_phongo_typewrapper_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "TypeWrapper", php_phongo_typewrapper_me);
	php_phongo_typewrapper_ce = zend_register_internal_interface(&ce TSRMLS_CC);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
