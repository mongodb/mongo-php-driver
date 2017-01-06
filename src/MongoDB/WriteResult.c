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
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"

#define RETURN_LONG_FROM_BSON_INT32(iter, bson, key) \
	if (bson_iter_init_find((iter), (bson), (key)) && BSON_ITER_HOLDS_INT32((iter))) { \
		RETURN_LONG(bson_iter_int32((iter))); \
	}

PHONGO_API zend_class_entry *php_phongo_writeresult_ce;

zend_object_handlers php_phongo_handler_writeresult;

static bool php_phongo_writeresult_get_writeconcernerror(php_phongo_writeresult_t *intern, zval *return_value TSRMLS_DC)
{
	bson_iter_t iter, child;
#if PHP_VERSION_ID >= 70000
	zval writeconcernerror;
#else
	zval *writeconcernerror = NULL;
#endif

	ZVAL_NULL(return_value);

	if (bson_iter_init_find(&iter, intern->reply, "writeConcernErrors") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		while (bson_iter_next(&child)) {
			bson_t cbson;
			uint32_t len;
			const uint8_t *data;

			if (!BSON_ITER_HOLDS_DOCUMENT(&child)) {
				continue;
			}

			bson_iter_document(&child, &len, &data);

			if (!bson_init_static(&cbson, data, len)) {
				continue;
			}

#if PHP_VERSION_ID >= 70000
			if (!phongo_writeconcernerror_init(&writeconcernerror, &cbson TSRMLS_CC)) {
				zval_ptr_dtor(&writeconcernerror);
				return false;
			}

			ZVAL_ZVAL(return_value, &writeconcernerror, 1, 1);
#else
			MAKE_STD_ZVAL(writeconcernerror);

			if (!phongo_writeconcernerror_init(writeconcernerror, &cbson TSRMLS_CC)) {
				zval_ptr_dtor(&writeconcernerror);
				return false;
			}

			ZVAL_ZVAL(return_value, writeconcernerror, 1, 1);
#endif

			return true;
		}
	}

	return true;
}

static bool php_phongo_writeresult_get_writeerrors(php_phongo_writeresult_t *intern, zval *return_value TSRMLS_DC)
{
	bson_iter_t iter, child;

	array_init(return_value);

	if (bson_iter_init_find(&iter, intern->reply, "writeErrors") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		while (bson_iter_next(&child)) {
			bson_t cbson;
			uint32_t len;
			const uint8_t *data;
#if PHP_VERSION_ID >= 70000
			zval writeerror;
#else
			zval *writeerror = NULL;
#endif

			if (!BSON_ITER_HOLDS_DOCUMENT(&child)) {
				continue;
			}

			bson_iter_document(&child, &len, &data);

			if (!bson_init_static(&cbson, data, len)) {
				continue;
			}

#if PHP_VERSION_ID >= 70000
			if (!phongo_writeerror_init(&writeerror, &cbson TSRMLS_CC)) {
				zval_ptr_dtor(&writeerror);
				continue;
			}

			add_next_index_zval(return_value, &writeerror);
#else
			MAKE_STD_ZVAL(writeerror);

			if (!phongo_writeerror_init(writeerror, &cbson TSRMLS_CC)) {
				zval_ptr_dtor(&writeerror);
				continue;
			}

			add_next_index_zval(return_value, writeerror);
#endif
		}
	}

	return true;
}

/* {{{ proto integer|null WriteResult::getInsertedCount()
   Returns the number of documents that were inserted */
PHP_METHOD(WriteResult, getInsertedCount)
{
	bson_iter_t iter;
	php_phongo_writeresult_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nInserted");
}
/* }}} */

/* {{{ proto integer|null WriteResult::getMatchedCount()
   Returns the number of documents that matched the update criteria */
PHP_METHOD(WriteResult, getMatchedCount)
{
	bson_iter_t iter;
	php_phongo_writeresult_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nMatched");
}
/* }}} */

/* {{{ proto integer|null WriteResult::getModifiedCount()
   Returns the number of documents that were actually modified by an update */
PHP_METHOD(WriteResult, getModifiedCount)
{
	bson_iter_t iter;
	php_phongo_writeresult_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nModified");
}
/* }}} */

/* {{{ proto integer|null WriteResult::getDeletedCount()
   Returns the number of documents that were deleted */
PHP_METHOD(WriteResult, getDeletedCount)
{
	bson_iter_t iter;
	php_phongo_writeresult_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nRemoved");
}
/* }}} */

/* {{{ proto integer|null WriteResult::getUpsertedCount()
   Returns the number of documents that were upserted */
PHP_METHOD(WriteResult, getUpsertedCount)
{
	bson_iter_t iter;
	php_phongo_writeresult_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nUpserted");
}
/* }}} */

/* {{{ proto MongoDB\Driver\Server WriteResult::getServer()
   Returns the Server from which the result originated */
PHP_METHOD(WriteResult, getServer)
{
	php_phongo_writeresult_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

#if PHP_VERSION_ID >= 70000
	phongo_server_init(return_value, &intern->manager, intern->server_id TSRMLS_CC);
#else
	phongo_server_init(return_value, intern->manager, intern->server_id TSRMLS_CC);
#endif
}
/* }}} */

/* {{{ proto array WriteResult::getUpsertedIds()
   Returns the identifiers generated by the server for upsert operations. */
PHP_METHOD(WriteResult, getUpsertedIds)
{
	bson_iter_t iter, child;
	php_phongo_writeresult_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	array_init(return_value);

	if (bson_iter_init_find(&iter, intern->reply, "upserted") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		while (bson_iter_next(&child)) {
			int32_t index;
			bson_iter_t outer;

			if (!BSON_ITER_HOLDS_DOCUMENT(&child) || !bson_iter_recurse(&child, &outer)) {
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
#if PHP_VERSION_ID >= 70000
				zval zid;

				php_phongo_objectid_new_from_oid(&zid, bson_iter_oid(&outer) TSRMLS_CC);
				add_index_zval(return_value, index, &zid);
#else
				zval *zid = NULL;
				MAKE_STD_ZVAL(zid);

				php_phongo_objectid_new_from_oid(zid, bson_iter_oid(&outer) TSRMLS_CC);
				add_index_zval(return_value, index, zid);
#endif
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
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_writeresult_get_writeconcernerror(intern, return_value TSRMLS_CC);
}
/* }}} */

/* {{{ proto WriteError[] WriteResult::getWriteErrors()
   Returns any write errors that occurred */
PHP_METHOD(WriteResult, getWriteErrors)
{
	php_phongo_writeresult_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_writeresult_get_writeerrors(intern, return_value TSRMLS_CC);
}
/* }}} */

/* {{{ proto boolean WriteResult::isAcknowledged()
   Returns whether the write operation was acknowledged (based on the write
   concern). */
PHP_METHOD(WriteResult, isAcknowledged)
{
	php_phongo_writeresult_t *intern;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_BOOL(mongoc_write_concern_is_acknowledged(intern->write_concern));
}
/* }}} */

/**
 * Result returned by Server and Manager executeBulkWrite() methods.
 *
 * This class may be constructed internally if it will encapsulate a libmongoc
 * data structure.
 */
/* {{{ MongoDB\Driver\WriteResult */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_writeresult_me[] = {
	PHP_ME(Server, __construct, ai_WriteResult_void, ZEND_ACC_FINAL|ZEND_ACC_PRIVATE)
	PHP_ME(WriteResult, getInsertedCount, ai_WriteResult_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getMatchedCount, ai_WriteResult_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getModifiedCount, ai_WriteResult_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getDeletedCount, ai_WriteResult_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getUpsertedCount, ai_WriteResult_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getServer, ai_WriteResult_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getUpsertedIds, ai_WriteResult_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getWriteConcernError, ai_WriteResult_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getWriteErrors, ai_WriteResult_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteResult, isAcknowledged, ai_WriteResult_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeup, ai_WriteResult_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_writeresult_t object handlers */
static void php_phongo_writeresult_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t *intern = Z_OBJ_WRITERESULT(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->reply) {
		bson_destroy(intern->reply);
	}

	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}

	zval_ptr_dtor(&intern->manager);

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_writeresult_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_writeresult_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_writeresult;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeresult_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_writeresult;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_writeresult_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t *intern;
#if PHP_VERSION_ID >= 70000
	zval                      retval;
#else
	zval                      retval = zval_used_for_init;
#endif
	bson_iter_t iter;

	intern = Z_WRITERESULT_OBJ_P(object);
	*is_temp = 1;
	array_init_size(&retval, 9);

#define SCP(field) \
	if (bson_iter_init_find(&iter, intern->reply, (field)) && BSON_ITER_HOLDS_INT32(&iter)) { \
		ADD_ASSOC_LONG_EX(&retval, (field), bson_iter_int32(&iter)); \
	} else { \
		ADD_ASSOC_NULL_EX(&retval, (field)); \
	}

	SCP("nInserted");
	SCP("nMatched");
	SCP("nModified");
	SCP("nRemoved");
	SCP("nUpserted");
#undef SCP

	if (bson_iter_init_find(&iter, intern->reply, "upserted") && BSON_ITER_HOLDS_ARRAY(&iter)) {
		uint32_t len;
		const uint8_t *data;
		php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;

		/* Use native arrays for debugging output */
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		bson_iter_array(&iter, &len, &data);
		phongo_bson_to_zval_ex(data, len, &state);
#if PHP_VERSION_ID >= 70000
		ADD_ASSOC_ZVAL_EX(&retval, "upsertedIds", &state.zchild);
#else
		ADD_ASSOC_ZVAL_EX(&retval, "upsertedIds", state.zchild);
#endif
	} else {
#if PHP_VERSION_ID >= 70000
		zval upsertedIds;
		array_init(&upsertedIds);
		ADD_ASSOC_ZVAL_EX(&retval, "upsertedIds", &upsertedIds);
#else
		zval *upsertedIds = NULL;
		MAKE_STD_ZVAL(upsertedIds);
		array_init(upsertedIds);
		ADD_ASSOC_ZVAL_EX(&retval, "upsertedIds", upsertedIds);
#endif
	}

	{
#if PHP_VERSION_ID >= 70000
		zval writeerrors;

		php_phongo_writeresult_get_writeerrors(intern, &writeerrors TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(&retval, "writeErrors", &writeerrors);
#else
		zval *writeerrors = NULL;

		MAKE_STD_ZVAL(writeerrors);
		php_phongo_writeresult_get_writeerrors(intern, writeerrors TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(&retval, "writeErrors", writeerrors);
#endif
	}

	{
#if PHP_VERSION_ID >= 70000
		zval writeconcernerror;

		php_phongo_writeresult_get_writeconcernerror(intern, &writeconcernerror TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(&retval, "writeConcernError", &writeconcernerror);
#else
		zval *writeconcernerror = NULL;

		MAKE_STD_ZVAL(writeconcernerror);
		php_phongo_writeresult_get_writeconcernerror(intern, writeconcernerror TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(&retval, "writeConcernError", writeconcernerror);
#endif
	}

	if (intern->write_concern) {
#if PHP_VERSION_ID >= 70000
		zval write_concern;

		phongo_writeconcern_init(&write_concern, intern->write_concern);
		ADD_ASSOC_ZVAL_EX(&retval, "writeConcern", &write_concern);
#else
		zval *write_concern = NULL;

		MAKE_STD_ZVAL(write_concern);
		phongo_writeconcern_init(write_concern, intern->write_concern TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(&retval, "writeConcern", write_concern);
#endif
	} else {
		ADD_ASSOC_NULL_EX(&retval, "writeConcern");
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
	PHONGO_CE_FINAL(php_phongo_writeresult_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_writeresult_ce);

	memcpy(&php_phongo_handler_writeresult, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeresult.get_debug_info = php_phongo_writeresult_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_writeresult.free_obj = php_phongo_writeresult_free_object;
	php_phongo_handler_writeresult.offset = XtOffsetOf(php_phongo_writeresult_t, std);
#endif

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
