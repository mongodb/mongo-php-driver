/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2015 MongoDB, Inc.                                         |
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
  | Copyright (c) 2014-2015 MongoDB, Inc.                                     |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>
#include <mongoc-write-concern-private.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_writeresult_ce;

zend_object_handlers php_phongo_handler_writeresult;

/* {{{ proto integer WriteResult::getInsertedCount()
   Returns the number of documents that were inserted */
PHP_METHOD(WriteResult, getInsertedCount)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_LONG(intern->write_result.nInserted);
}
/* }}} */
/* {{{ proto integer WriteResult::getMatchedCount()
   Returns the number of documents that matched the update criteria */
PHP_METHOD(WriteResult, getMatchedCount)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_LONG(intern->write_result.nMatched);
}
/* }}} */
/* {{{ proto integer|null WriteResult::getModifiedCount()
   Returns the number of documents that were actually modified by an update */
PHP_METHOD(WriteResult, getModifiedCount)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (intern->write_result.omit_nModified) {
		RETURN_NULL();
	}

	RETURN_LONG(intern->write_result.nModified);
}
/* }}} */
/* {{{ proto integer WriteResult::getDeletedCount()
   Returns the number of documents that were deleted */
PHP_METHOD(WriteResult, getDeletedCount)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_LONG(intern->write_result.nRemoved);
}
/* }}} */
/* {{{ proto integer WriteResult::getUpsertedCount()
   Returns the number of documents that were upserted */
PHP_METHOD(WriteResult, getUpsertedCount)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_LONG(intern->write_result.nUpserted);
}
/* }}} */
/* {{{ proto MongoDB\Driver\Server WriteResult::getServer()
   Returns the Server from which the result originated */
PHP_METHOD(WriteResult, getServer)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	phongo_server_init(return_value, intern->client, intern->server_id TSRMLS_CC);
}
/* }}} */
/* {{{ proto array WriteResult::getUpsertedIds()
   Returns the identifiers generated by the server for upsert operations. */
PHP_METHOD(WriteResult, getUpsertedIds)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	array_init(return_value);

	if (!bson_empty0(&intern->write_result.upserted)) {
		bson_iter_t iter;

		bson_iter_init(&iter, &intern->write_result.upserted);

		while (bson_iter_next(&iter)) {
			int32_t index;
			bson_iter_t outer;
			zval *zid = NULL;

			if (!BSON_ITER_HOLDS_DOCUMENT(&iter) || !bson_iter_recurse(&iter, &outer)) {
				continue;
			}
			if (!bson_iter_find(&outer, "index") || !BSON_ITER_HOLDS_INT32(&outer)) {
				continue;
			}

			index = bson_iter_int32(&outer);

			if (!bson_iter_find(&outer, "_id")) {
				continue;
			}

			if (BSON_ITER_HOLDS_OID(&outer)) {
				MAKE_STD_ZVAL(zid);

				php_phongo_objectid_new_from_oid(zid, bson_iter_oid(&outer) TSRMLS_CC);
				add_index_zval(return_value, index, zid);
			} else if (BSON_ITER_HOLDS_INT32(&outer)) {
				int32_t val = bson_iter_int32(&outer);

				add_index_long(return_value, index, val);
			} else if (BSON_ITER_HOLDS_INT64(&outer)) {
				int64_t val = bson_iter_int64(&outer);

				ADD_INDEX_INT64(return_value, index, val);
			}
		}
	}
}
/* }}} */
/* {{{ proto WriteConcernError WriteResult::getWriteConcernError()
   Return any write concern error that occurred */
PHP_METHOD(WriteResult, getWriteConcernError)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	if (!bson_empty0(&intern->write_result.writeConcernErrors)) {
		bson_iter_t iter;

		bson_iter_init(&iter, &intern->write_result.writeConcernErrors);

		while (bson_iter_next(&iter)) {
			bson_t cbson;
			uint32_t len;
			const uint8_t *data;

			if (!BSON_ITER_HOLDS_DOCUMENT(&iter)) {
				continue;
			}

			bson_iter_document(&iter, &len, &data);

			if (!bson_init_static(&cbson, data, len)) {
				continue;
			}

			object_init_ex(return_value, php_phongo_writeconcernerror_ce);

			if (!phongo_writeconcernerror_init(return_value, &cbson TSRMLS_CC)) {
				zval_ptr_dtor(&return_value);
			}

			return;
		}
	}
}
/* }}} */
/* {{{ proto WriteError[] WriteResult::getWriteErrors()
   Returns any write errors that occurred */
PHP_METHOD(WriteResult, getWriteErrors)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	array_init(return_value);

	if (!bson_empty0(&intern->write_result.writeErrors)) {
		bson_iter_t iter;

		bson_iter_init(&iter, &intern->write_result.writeErrors);

		while (bson_iter_next(&iter)) {
			bson_t cbson;
			uint32_t len;
			const uint8_t *data;
			zval *writeerror = NULL;

			if (!BSON_ITER_HOLDS_DOCUMENT(&iter)) {
				continue;
			}

			bson_iter_document(&iter, &len, &data);

			if (!bson_init_static(&cbson, data, len)) {
				continue;
			}

			MAKE_STD_ZVAL(writeerror);
			object_init_ex(writeerror, php_phongo_writeerror_ce);

			if (!phongo_writeerror_init(writeerror, &cbson TSRMLS_CC)) {
				zval_ptr_dtor(&writeerror);
				continue;
			}

			add_next_index_zval(return_value, writeerror);
		}
	}
}
/* }}} */
/* {{{ proto boolean WriteResult::isAcknowledged()
   Returns the number of documents that were upserted */
PHP_METHOD(WriteResult, isAcknowledged)
{
	php_phongo_writeresult_t *intern;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_BOOL(_mongoc_write_concern_needs_gle(intern->write_concern));
}
/* }}} */

/**
 * Result returned by Server and Manager executeBulkWrite() methods.
 *
 * This class may be constructed internally if it will encapsulate a libmongoc
 * data structure.
 */
/* {{{ MongoDB\Driver\WriteResult */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getInsertedCount, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getMatchedCount, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getModifiedCount, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getDeletedCount, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getUpsertedCount, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getServer, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getUpsertedIds, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getWriteConcernError, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_getWriteErrors, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_isAcknowledged, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_writeresult_me[] = {
	PHP_ME(Server, __construct, NULL, ZEND_ACC_FINAL|ZEND_ACC_PRIVATE)
	PHP_ME(WriteResult, getInsertedCount, ai_WriteResult_getInsertedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getMatchedCount, ai_WriteResult_getMatchedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getModifiedCount, ai_WriteResult_getModifiedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getDeletedCount, ai_WriteResult_getDeletedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getUpsertedCount, ai_WriteResult_getUpsertedCount, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getServer, ai_WriteResult_getServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getUpsertedIds, ai_WriteResult_getUpsertedIds, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getWriteConcernError, ai_WriteResult_getWriteConcernError, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getWriteErrors, ai_WriteResult_getWriteErrors, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, isAcknowledged, ai_WriteResult_isAcknowledged, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_writeresult_t object handlers */
static void php_phongo_writeresult_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t *intern = (php_phongo_writeresult_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	_mongoc_write_result_destroy(&intern->write_result);

	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}

	efree(intern);
} /* }}} */

zend_object_value php_phongo_writeresult_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_writeresult_t *intern = NULL;

	intern = (php_phongo_writeresult_t *)ecalloc(1, sizeof *intern);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	_mongoc_write_result_init(&intern->write_result);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeresult_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_writeresult;

	return retval;
} /* }}} */

HashTable *php_phongo_writeresult_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t *intern;
	zval                      retval = zval_used_for_init;
	php_phongo_bson_state     state = PHONGO_BSON_STATE_INITIALIZER;
	bson_iter_t iter;

	intern = (php_phongo_writeresult_t *)zend_object_store_get_object(object TSRMLS_CC);
	*is_temp = 1;
	array_init_size(&retval, 9);

	add_assoc_long_ex(&retval, ZEND_STRS("nInserted"), intern->write_result.nInserted);
	add_assoc_long_ex(&retval, ZEND_STRS("nMatched"), intern->write_result.nMatched);
	if (intern->write_result.omit_nModified) {
		add_assoc_null_ex(&retval, ZEND_STRS("nModified"));
	} else {
		add_assoc_long_ex(&retval, ZEND_STRS("nModified"), intern->write_result.nModified);
	}
	add_assoc_long_ex(&retval, ZEND_STRS("nRemoved"), intern->write_result.nRemoved);
	add_assoc_long_ex(&retval, ZEND_STRS("nUpserted"), intern->write_result.nUpserted);

	/* Use native arrays for debugging output */
	state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
	state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

	MAKE_STD_ZVAL(state.zchild);
	bson_to_zval(bson_get_data(&intern->write_result.upserted), intern->write_result.upserted.len, &state);
	add_assoc_zval_ex(&retval, ZEND_STRS("upsertedIds"), state.zchild);

	MAKE_STD_ZVAL(state.zchild);
	bson_to_zval(bson_get_data(&intern->write_result.writeErrors), intern->write_result.writeErrors.len, &state);
	add_assoc_zval_ex(&retval, ZEND_STRS("writeErrors"), state.zchild);

	if (!bson_empty0(&intern->write_result.writeConcernErrors) &&
			bson_iter_init(&iter, &intern->write_result.writeConcernErrors) &&
			bson_iter_next(&iter) &&
			BSON_ITER_HOLDS_DOCUMENT(&iter)) {
		uint32_t len;
		const uint8_t *data;

		bson_iter_document(&iter, &len, &data);

		MAKE_STD_ZVAL(state.zchild);
		bson_to_zval(data, len, &state);
		add_assoc_zval_ex(&retval, ZEND_STRS("writeConcernError"), state.zchild);
	} else {
		add_assoc_null_ex(&retval, ZEND_STRS("writeConcernError"));
	}

	if (intern->write_concern) {
		zval *write_concern = NULL;
		MAKE_STD_ZVAL(write_concern);
		php_phongo_write_concern_to_zval(write_concern, intern->write_concern);
		add_assoc_zval_ex(&retval, ZEND_STRS("writeConcern"), write_concern);
	} else {
		add_assoc_null_ex(&retval, ZEND_STRS("writeConcern"));
	}

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteResult)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteResult", php_phongo_writeresult_me);
	php_phongo_writeresult_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_writeresult_ce->create_object = php_phongo_writeresult_create_object;
	PHONGO_CE_INIT(php_phongo_writeresult_ce);

	memcpy(&php_phongo_handler_writeresult, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeresult.get_debug_info = php_phongo_writeresult_get_debug_info;

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
