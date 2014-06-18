/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2014 MongoDB, Inc.                                         |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2014, MongoDB, Inc.                                         |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <php/ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <php/ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHPAPI zend_class_entry *php_phongo_writebatch_ce;



/* {{{ MongoDB\Write\WriteBatch */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteBatch_add, 0, 0, 1)
	ZEND_ARG_INFO(0, document) 
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_writebatch_me[] = {
	/**
	 * Adds a new operation to be executed
	 *
	 * @param array|object $document Operation/document to add to the batch
	 * @return DeleteBatch
	 */
	PHP_ABSTRACT_ME(WriteBatch, add, ai_WriteBatch_add)

	PHP_FE_END
};

/* }}} */


/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteBatch)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Write", "WriteBatch", php_phongo_writebatch_me);
	php_phongo_writebatch_ce = zend_register_internal_interface(&ce TSRMLS_CC);
	zend_class_implements(php_phongo_writebatch_ce TSRMLS_CC, 1, spl_ce_Countable);


	return SUCCESS;
}
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
