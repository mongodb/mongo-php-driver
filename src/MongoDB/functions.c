/*
 * Copyright 2019 MongoDB, Inc.
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

/* {{{ proto void MongoDB\Driver\reset(MongoDB\Driver\Manager $manager)
   Resets a manager after forking */
PHP_FUNCTION(MongoDB_Driver_reset)
{
	zval* zManager = NULL;
	php_phongo_manager_t* intern;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &zManager, php_phongo_manager_ce) == FAILURE) {
		return;
	}

	intern = Z_MANAGER_OBJ_P(zManager);
	mongoc_client_reset(intern->client);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
