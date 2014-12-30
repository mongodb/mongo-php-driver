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
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"
#include <ext/spl/spl_exceptions.h>


PHONGO_API zend_class_entry *php_phongo_writeexception_ce;

/* {{{ proto MongoDB\Driver\WriteResult WriteException::getWriteResult()
   Returns the WriteResult from the failed write operation. */
PHP_METHOD(WriteException, getWriteResult)
{
	zend_error_handling       error_handling;
	zval *writeresult;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	writeresult = zend_read_property(php_phongo_writeexception_ce, getThis(), ZEND_STRL("writeResult"), 0 TSRMLS_CC);

	RETURN_ZVAL(writeresult, 1, 0);
}
/* }}} */

/**
 * Value object for write concern used in issuing write operations.
 */
/* {{{ MongoDB\Driver\WriteException */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteException_getWriteResult, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_writeexception_me[] = {
	PHP_ME(WriteException, getWriteResult, ai_WriteException_getWriteResult, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteException)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteException", php_phongo_writeexception_me);
	php_phongo_writeexception_ce = zend_register_internal_class_ex(&ce, php_phongo_runtimeexception_ce, NULL TSRMLS_CC);

	zend_declare_property_null(php_phongo_writeexception_ce, ZEND_STRL("writeResult"), ZEND_ACC_PROTECTED TSRMLS_CC);

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
