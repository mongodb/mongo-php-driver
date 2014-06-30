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


PHONGO_API zend_class_entry *php_phongo_updateresult_ce;

/* {{{ proto integer UpdateResult::getNumMatched()
   Returns the number of documents matching the criteria */
PHP_METHOD(UpdateResult, getNumMatched)
{
	php_phongo_updateresult_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_updateresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto integer UpdateResult::getNumModified()
   Returns the number of documents that got physically modified */
PHP_METHOD(UpdateResult, getNumModified)
{
	php_phongo_updateresult_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_updateresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto integer UpdateResult::getNumUpserted()
   Returns the number of new documentes */
PHP_METHOD(UpdateResult, getNumUpserted)
{
	php_phongo_updateresult_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_updateresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto GeneratedId[] UpdateResult::getUpserts()
   Returns the GeneratedIds of the upserted documents */
PHP_METHOD(UpdateResult, getUpserts)
{
	php_phongo_updateresult_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_updateresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto array UpdateResult::getInfo()
   Returns metadata about the operation, see https://github.com/mongodb/specifications/blob/master/source/server_write_commands.rst#situational-fields */
PHP_METHOD(UpdateResult, getInfo)
{
	php_phongo_updateresult_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_updateresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Server UpdateResult::getServer()
   Returns the Server object that this result originated */
PHP_METHOD(UpdateResult, getServer)
{
	php_phongo_updateresult_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_updateresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto WriteConcernError[] UpdateResult::getWriteConcernErrors()
   Returns all WriteConcern Errors that occurred */
PHP_METHOD(UpdateResult, getWriteConcernErrors)
{
	php_phongo_updateresult_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_updateresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto WriteError[] UpdateResult::getWriteErrors()
   Returns all Write Errors that occurred */
PHP_METHOD(UpdateResult, getWriteErrors)
{
	php_phongo_updateresult_t *intern;
	zend_error_handling	error_handling;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_updateresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */

/**
 * Result returned by Server and Manager executeWrite() methods processing an
 * UpdateBatch.
 *
 * This class may be constructed internally if it will encapsulate a libmongoc
 * data structure.
 */
/* {{{ MongoDB\Write\UpdateResult */

ZEND_BEGIN_ARG_INFO_EX(ai_UpdateResult_getNumMatched, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_UpdateResult_getNumModified, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_UpdateResult_getNumUpserted, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_UpdateResult_getUpserts, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_UpdateResult_getInfo, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_UpdateResult_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_UpdateResult_getWriteConcernErrors, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_UpdateResult_getWriteErrors, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_updateresult_me[] = {
	PHP_ME(UpdateResult, getNumMatched, ai_UpdateResult_getNumMatched, ZEND_ACC_PUBLIC)
	PHP_ME(UpdateResult, getNumModified, ai_UpdateResult_getNumModified, ZEND_ACC_PUBLIC)
	PHP_ME(UpdateResult, getNumUpserted, ai_UpdateResult_getNumUpserted, ZEND_ACC_PUBLIC)
	PHP_ME(UpdateResult, getUpserts, ai_UpdateResult_getUpserts, ZEND_ACC_PUBLIC)
	PHP_ME(UpdateResult, getInfo, ai_UpdateResult_getInfo, ZEND_ACC_PUBLIC)
	PHP_ME(UpdateResult, getServer, ai_UpdateResult_getServer, ZEND_ACC_PUBLIC)
	PHP_ME(UpdateResult, getWriteConcernErrors, ai_UpdateResult_getWriteConcernErrors, ZEND_ACC_PUBLIC)
	PHP_ME(UpdateResult, getWriteErrors, ai_UpdateResult_getWriteErrors, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_updateresult_free_object && php_phongo_updateresult_create_object */
static void php_phongo_updateresult_free_object(void *object TSRMLS_DC)
{
	php_phongo_updateresult_t *intern = (php_phongo_updateresult_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
}

zend_object_value php_phongo_updateresult_create_object(zend_class_entry *class_type TSRMLS_DC)
{
	zend_object_value retval;
	php_phongo_updateresult_t *intern;

	intern = (php_phongo_updateresult_t *)emalloc(sizeof(php_phongo_updateresult_t));
	memset(intern, 0, sizeof(php_phongo_updateresult_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_updateresult_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(UpdateResult)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Write", "UpdateResult", php_phongo_updateresult_me);
	ce.create_object = php_phongo_updateresult_create_object;
	php_phongo_updateresult_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_updateresult_ce.ce_flags |= ZEND_ACC_FINAL_CLASS
	zend_class_implements(php_phongo_updateresult_ce TSRMLS_CC, 1, php_phongo_writeresult_ce);


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
