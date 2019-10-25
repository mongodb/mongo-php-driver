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
#include <Zend/zend_interfaces.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_array_api.h"
#include "phongo_compat.h"
#include "php_phongo.h"
#include "php_bson.h"

#define PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(iter, bson, key)                \
	if (bson_iter_init_find((iter), (bson), (key)) && BSON_ITER_HOLDS_INT32((iter))) { \
		RETURN_LONG(bson_iter_int32((iter)));                                          \
	}

zend_class_entry* php_phongo_writeresult_ce;

static bool php_phongo_writeresult_get_writeconcernerror(php_phongo_writeresult_t* intern, zval* return_value TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter, child;
#if PHP_VERSION_ID >= 70000
	zval writeconcernerror;
#else
	zval* writeconcernerror = NULL;
#endif

	ZVAL_NULL(return_value);

	if (bson_iter_init_find(&iter, intern->reply, "writeConcernErrors") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		while (bson_iter_next(&child)) {
			bson_t         cbson;
			uint32_t       len;
			const uint8_t* data;

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
} /* }}} */

static bool php_phongo_writeresult_get_writeerrors(php_phongo_writeresult_t* intern, zval* return_value TSRMLS_DC) /* {{{ */
{
	bson_iter_t iter, child;

	array_init(return_value);

	if (bson_iter_init_find(&iter, intern->reply, "writeErrors") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		while (bson_iter_next(&child)) {
			bson_t         cbson;
			uint32_t       len;
			const uint8_t* data;
#if PHP_VERSION_ID >= 70000
			zval writeerror;
#else
			zval* writeerror = NULL;
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
} /* }}} */

/* {{{ proto integer|null MongoDB\Driver\WriteResult::getInsertedCount()
   Returns the number of documents that were inserted */
static PHP_METHOD(WriteResult, getInsertedCount)
{
	bson_iter_t               iter;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nInserted");
} /* }}} */

/* {{{ proto integer|null MongoDB\Driver\WriteResult::getMatchedCount()
   Returns the number of documents that matched the update criteria */
static PHP_METHOD(WriteResult, getMatchedCount)
{
	bson_iter_t               iter;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nMatched");
} /* }}} */

/* {{{ proto integer|null MongoDB\Driver\WriteResult::getModifiedCount()
   Returns the number of documents that were actually modified by an update */
static PHP_METHOD(WriteResult, getModifiedCount)
{
	bson_iter_t               iter;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nModified");
} /* }}} */

/* {{{ proto integer|null MongoDB\Driver\WriteResult::getDeletedCount()
   Returns the number of documents that were deleted */
static PHP_METHOD(WriteResult, getDeletedCount)
{
	bson_iter_t               iter;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nRemoved");
} /* }}} */

/* {{{ proto integer|null MongoDB\Driver\WriteResult::getUpsertedCount()
   Returns the number of documents that were upserted */
static PHP_METHOD(WriteResult, getUpsertedCount)
{
	bson_iter_t               iter;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nUpserted");
} /* }}} */

/* {{{ proto MongoDB\Driver\Server MongoDB\Driver\WriteResult::getServer()
   Returns the Server from which the result originated */
static PHP_METHOD(WriteResult, getServer)
{
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	phongo_server_init(return_value, intern->client, intern->server_id TSRMLS_CC);
} /* }}} */

/* {{{ proto array MongoDB\Driver\WriteResult::getUpsertedIds()
   Returns the identifiers generated by the server for upsert operations. */
static PHP_METHOD(WriteResult, getUpsertedIds)
{
	bson_iter_t               iter, child;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	array_init(return_value);

	if (bson_iter_init_find(&iter, intern->reply, "upserted") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		while (bson_iter_next(&child)) {
			uint32_t              data_len;
			const uint8_t*        data = NULL;
			php_phongo_bson_state state;

			/* Use PHONGO_TYPEMAP_NATIVE_ARRAY for the root type so we can
			 * easily access the "index" and "_id" fields. */
			PHONGO_BSON_INIT_STATE(state);
			state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

			if (!BSON_ITER_HOLDS_DOCUMENT(&child)) {
				continue;
			}

			bson_iter_document(&child, &data_len, &data);

			if (php_phongo_bson_to_zval_ex(data, data_len, &state)) {
#if PHP_VERSION_ID >= 70000
				zval* zid = php_array_fetchc(&state.zchild, "_id");
				add_index_zval(return_value, php_array_fetchc_long(&state.zchild, "index"), zid);
				zval_add_ref(zid);
#else
				zval* zid = php_array_fetchc(state.zchild, "_id");
				add_index_zval(return_value, php_array_fetchc_long(state.zchild, "index"), zid);
				zval_add_ref(&zid);
#endif
			}

			zval_ptr_dtor(&state.zchild);
		}
	}
} /* }}} */

/* {{{ proto WriteConcernError MongoDB\Driver\WriteResult::getWriteConcernError()
   Return any write concern error that occurred */
static PHP_METHOD(WriteResult, getWriteConcernError)
{
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_writeresult_get_writeconcernerror(intern, return_value TSRMLS_CC);
} /* }}} */

/* {{{ proto WriteError[] MongoDB\Driver\WriteResult::getWriteErrors()
   Returns any write errors that occurred */
static PHP_METHOD(WriteResult, getWriteErrors)
{
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	php_phongo_writeresult_get_writeerrors(intern, return_value TSRMLS_CC);
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\WriteResult::isAcknowledged()
   Returns whether the write operation was acknowledged (based on the write
   concern). */
static PHP_METHOD(WriteResult, isAcknowledged)
{
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_BOOL(mongoc_write_concern_is_acknowledged(intern->write_concern));
} /* }}} */

/* {{{ MongoDB\Driver\WriteResult function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_WriteResult_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_writeresult_me[] = {
	/* clang-format off */
	PHP_ME(WriteResult, getInsertedCount, ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getMatchedCount, ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getModifiedCount, ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getDeletedCount, ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getUpsertedCount, ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getServer, ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getUpsertedIds, ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getWriteConcernError, ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteResult, getWriteErrors, ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(WriteResult, isAcknowledged, ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_WriteResult_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_WriteResult_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\WriteResult object handlers */
static zend_object_handlers php_phongo_handler_writeresult;

static void php_phongo_writeresult_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t* intern = Z_OBJ_WRITERESULT(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->reply) {
		bson_destroy(intern->reply);
	}

	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_writeresult_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_writeresult_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_writeresult;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeresult_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_writeresult;

		return retval;
	}
#endif
} /* }}} */

static HashTable* php_phongo_writeresult_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_writeresult_t* intern;
	zval                      retval = ZVAL_STATIC_INIT;
	bson_iter_t               iter;

	intern   = Z_WRITERESULT_OBJ_P(object);
	*is_temp = 1;
	array_init_size(&retval, 9);

#define PHONGO_WRITERESULT_SCP(field)                                                         \
	if (bson_iter_init_find(&iter, intern->reply, (field)) && BSON_ITER_HOLDS_INT32(&iter)) { \
		ADD_ASSOC_LONG_EX(&retval, (field), bson_iter_int32(&iter));                          \
	} else {                                                                                  \
		ADD_ASSOC_NULL_EX(&retval, (field));                                                  \
	}

	PHONGO_WRITERESULT_SCP("nInserted");
	PHONGO_WRITERESULT_SCP("nMatched");
	PHONGO_WRITERESULT_SCP("nModified");
	PHONGO_WRITERESULT_SCP("nRemoved");
	PHONGO_WRITERESULT_SCP("nUpserted");
#undef PHONGO_WRITERESULT_SCP

	if (bson_iter_init_find(&iter, intern->reply, "upserted") && BSON_ITER_HOLDS_ARRAY(&iter)) {
		uint32_t              len;
		const uint8_t*        data;
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);
		bson_iter_array(&iter, &len, &data);
		if (!php_phongo_bson_to_zval_ex(data, len, &state)) {
			zval_ptr_dtor(&state.zchild);
			goto done;
		}

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
		zval* upsertedIds = NULL;
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
		zval* writeerrors = NULL;

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
		zval* writeconcernerror = NULL;

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
		zval* write_concern = NULL;

		MAKE_STD_ZVAL(write_concern);
		phongo_writeconcern_init(write_concern, intern->write_concern TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(&retval, "writeConcern", write_concern);
#endif
	} else {
		ADD_ASSOC_NULL_EX(&retval, "writeConcern");
	}

done:
	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_writeresult_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteResult", php_phongo_writeresult_me);
	php_phongo_writeresult_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_writeresult_ce->create_object = php_phongo_writeresult_create_object;
	PHONGO_CE_FINAL(php_phongo_writeresult_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_writeresult_ce);

	memcpy(&php_phongo_handler_writeresult, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeresult.get_debug_info = php_phongo_writeresult_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_writeresult.free_obj = php_phongo_writeresult_free_object;
	php_phongo_handler_writeresult.offset   = XtOffsetOf(php_phongo_writeresult_t, std);
#endif
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
