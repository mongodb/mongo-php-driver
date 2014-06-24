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


PHONGO_API zend_class_entry *php_phongo_writeresult_ce;



/* {{{ MongoDB\Write\WriteResult */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getInfo, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getWriteConcernErrors, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getWriteErrors, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_writeresult_me[] = {
	/**
	 * Returns metadata about the operation, see https://github.com/mongodb/specifications/blob/master/source/server_write_commands.rst#situational-fields
	 *
	 * @return array Additional metadata for the operation(s) (e.g. lastOp)
	 */
	PHP_ABSTRACT_ME(WriteResult, getInfo, ai_WriteResult_getInfo)

	/**
	 * Returns the Server object that this result originated
	 *
	 * @return Server Server from which the result originated
	 */
	PHP_ABSTRACT_ME(WriteResult, getServer, ai_WriteResult_getServer)

	/**
	 * Returns all WriteConcern Errors that occurred
	 *
	 * @return WriteConcernError[]
	 */
	PHP_ABSTRACT_ME(WriteResult, getWriteConcernErrors, ai_WriteResult_getWriteConcernErrors)

	/**
	 * Returns all Write Errors that occurred
	 *
	 * @return WriteError[]
	 */
	PHP_ABSTRACT_ME(WriteResult, getWriteErrors, ai_WriteResult_getWriteErrors)

	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_writeresult_free_object && php_phongo_writeresult_create_object */
static void php_phongo_writeresult_free_object(void *object TSRMLS_DC)
{
	php_phongo_writeresult_t *intern = (php_phongo_writeresult_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
}

zend_object_value php_phongo_writeresult_create_object(zend_class_entry *class_type TSRMLS_DC)
{
	zend_object_value retval;
	php_phongo_writeresult_t *intern;

	intern = (php_phongo_writeresult_t *)emalloc(sizeof(php_phongo_writeresult_t));
	memset(intern, 0, sizeof(php_phongo_writeresult_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeresult_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteResult)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Write", "WriteResult", php_phongo_writeresult_me);
	php_phongo_writeresult_ce = zend_register_internal_interface(&ce TSRMLS_CC);

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
