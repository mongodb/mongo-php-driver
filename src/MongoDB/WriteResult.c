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

/* {{{ proto integer WriteResult::getNumInserted()
   Returns the number of documents that were inserted */
PHP_METHOD(WriteResult, getNumInserted)
{
	php_phongo_writeresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	RETURN_LONG(intern->nInserted);
}
/* }}} */
/* {{{ proto integer WriteResult::getNumMatched()
   Returns the number of documents that matched the update criteria */
PHP_METHOD(WriteResult, getNumMatched)
{
	php_phongo_writeresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	RETURN_LONG(intern->nMatched);
}
/* }}} */
/* {{{ proto integer WriteResult::getNumModified()
   Returns the number of documents that were actually modified by an update */
PHP_METHOD(WriteResult, getNumModified)
{
	php_phongo_writeresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	RETURN_LONG(intern->nModified);
}
/* }}} */
/* {{{ proto integer WriteResult::getNumRemoved()
   Returns the number of documents that were deleted */
PHP_METHOD(WriteResult, getNumRemoved)
{
	php_phongo_writeresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	RETURN_LONG(intern->nRemoved);
}
/* }}} */
/* {{{ proto integer WriteResult::getNumUpserted()
   Returns the number of documents that were upserted */
PHP_METHOD(WriteResult, getNumUpserted)
{
	php_phongo_writeresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	RETURN_LONG(intern->nUpserted);
}
/* }}} */
/* {{{ proto array WriteResult::getInfo()
   Returns metadata about the operation. */
PHP_METHOD(WriteResult, getInfo)
{
	php_phongo_writeresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	if (intern->info && Z_TYPE_P(intern->info) == IS_ARRAY) {
		RETURN_ZVAL(intern->info, 1, 0);
	}

	array_init(return_value);
}
/* }}} */
/* {{{ proto MongoDB\Server WriteResult::getServer()
   Returns the Server from which the result originated */
PHP_METHOD(WriteResult, getServer)
{
	php_phongo_writeresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	phongo_server_init(return_value, intern->result.hint, NULL TSRMLS_CC);
}
/* }}} */
/* {{{ proto array WriteResult::getUpsertedIds()
   Returns the identifiers generated by the server for upsert operations. */
PHP_METHOD(WriteResult, getUpsertedIds)
{
	php_phongo_writeresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	if (intern->upsertedIds && Z_TYPE_P(intern->upsertedIds) == IS_ARRAY) {
		RETURN_ZVAL(intern->upsertedIds, 1, 0);
	}

	array_init(return_value);
}
/* }}} */
/* {{{ proto WriteConcernError[] WriteResult::getWriteConcernErrors()
   Return any write concern errors that occurred */
PHP_METHOD(WriteResult, getWriteConcernErrors)
{
	php_phongo_writeresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	if (intern->writeConcernErrors && Z_TYPE_P(intern->writeConcernErrors) == IS_ARRAY) {
		RETURN_ZVAL(intern->writeConcernErrors, 1, 0);
	}

	array_init(return_value);
}
/* }}} */
/* {{{ proto WriteError[] WriteResult::getWriteErrors()
   Returns any write errors that occurred */
PHP_METHOD(WriteResult, getWriteErrors)
{
	php_phongo_writeresult_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	if (intern->writeErrors && Z_TYPE_P(intern->writeErrors) == IS_ARRAY) {
		RETURN_ZVAL(intern->writeErrors, 1, 0);
	}

	array_init(return_value);
}
/* }}} */

/**
 * Result returned by Server and Manager executeWriteBatch() methods.
 *
 * This class may be constructed internally if it will encapsulate a libmongoc
 * data structure.
 */
/* {{{ MongoDB\WriteResult */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getNumInserted, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getNumMatched, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getNumModified, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getNumRemoved, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getNumUpserted, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getInfo, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getUpsertedIds, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getWriteConcernErrors, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getWriteErrors, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_writeresult_me[] = {
	PHP_ME(WriteResult, getNumInserted, ai_WriteResult_getNumInserted, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getNumMatched, ai_WriteResult_getNumMatched, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getNumModified, ai_WriteResult_getNumModified, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getNumRemoved, ai_WriteResult_getNumRemoved, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getNumUpserted, ai_WriteResult_getNumUpserted, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getInfo, ai_WriteResult_getInfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getServer, ai_WriteResult_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getUpsertedIds, ai_WriteResult_getUpsertedIds, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getWriteConcernErrors, ai_WriteResult_getWriteConcernErrors, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getWriteErrors, ai_WriteResult_getWriteErrors, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_writeresult_t object handlers */
static void php_phongo_writeresult_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t *intern = (php_phongo_writeresult_t*)object;

	zend_object_std_dtor(&intern->result.std TSRMLS_CC);

	if (intern->info) {
		zval_ptr_dtor(&intern->info);
	}

	if (intern->upsertedIds) {
		zval_ptr_dtor(&intern->upsertedIds);
	}

	if (intern->writeConcernErrors) {
		zval_ptr_dtor(&intern->writeConcernErrors);
	}

	if (intern->writeErrors) {
		zval_ptr_dtor(&intern->writeErrors);
	}
	efree(intern);
} /* }}} */

zend_object_value php_phongo_writeresult_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_writeresult_t *intern;

	intern = (php_phongo_writeresult_t *)emalloc(sizeof(php_phongo_writeresult_t));
	memset(intern, 0, sizeof(php_phongo_writeresult_t));

	zend_object_std_init(&intern->result.std, class_type TSRMLS_CC);
	object_properties_init(&intern->result.std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeresult_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteResult)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB", "WriteResult", php_phongo_writeresult_me);
	ce.create_object = php_phongo_writeresult_create_object;
	php_phongo_writeresult_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_writeresult_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;


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
