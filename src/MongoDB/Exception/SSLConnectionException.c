/*
 * Copyright 2014-2017 MongoDB, Inc.
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

zend_class_entry* php_phongo_sslconnectionexception_ce;

/* {{{ MongoDB\Driver\Exception\SSLConnectionException function entries */
static zend_function_entry php_phongo_sslconnectionexception_me[] = {
	PHP_FE_END
};
/* }}} */

void php_phongo_sslconnectionexception_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "SSLConnectionException", php_phongo_sslconnectionexception_me);
#if PHP_VERSION_ID >= 70000
	php_phongo_sslconnectionexception_ce = zend_register_internal_class_ex(&ce, php_phongo_connectionexception_ce);
#else
	php_phongo_sslconnectionexception_ce = zend_register_internal_class_ex(&ce, php_phongo_connectionexception_ce, NULL TSRMLS_CC);
#endif
	PHONGO_CE_FINAL(php_phongo_sslconnectionexception_ce);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
