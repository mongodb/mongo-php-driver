/*
 * Copyright 2014-present MongoDB, Inc.
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

#include "bson/bson.h"
#include "mongoc/mongoc.h"

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_error.h"

#include "MongoDB/Server.h"
#include "MongoDB/WriteConcern.h"
#include "MongoDB/WriteConcernError.h"
#include "MongoDB/WriteError.h"
#include "WriteResult_arginfo.h"

#define PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED(method)                                                                                                                                        \
	if (!mongoc_write_concern_is_acknowledged(intern->write_concern)) {                                                                                                                      \
		php_error_docref(NULL, E_DEPRECATED, "Calling MongoDB\\Driver\\WriteResult::" method "() for an unacknowledged write is deprecated and will throw an exception in ext-mongodb 2.0"); \
		RETURN_NULL();                                                                                                                                                                       \
	}

#define PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(iter, bson, key)                \
	if (bson_iter_init_find((iter), (bson), (key)) && BSON_ITER_HOLDS_INT32((iter))) { \
		RETURN_LONG(bson_iter_int32((iter)));                                          \
	}                                                                                  \
	RETURN_LONG(0);

zend_class_entry* php_phongo_writeresult_ce;

static bool php_phongo_writeresult_get_writeconcernerror(php_phongo_writeresult_t* intern, zval* return_value)
{
	bson_iter_t iter, child;
	zval        writeconcernerror;

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

			if (!phongo_writeconcernerror_init(&writeconcernerror, &cbson)) {
				zval_ptr_dtor(&writeconcernerror);
				return false;
			}

			ZVAL_ZVAL(return_value, &writeconcernerror, 1, 1);

			return true;
		}
	}

	return true;
}

static bool php_phongo_writeresult_get_writeerrors(php_phongo_writeresult_t* intern, zval* return_value)
{
	bson_iter_t iter, child;

	array_init(return_value);

	if (bson_iter_init_find(&iter, intern->reply, "writeErrors") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		while (bson_iter_next(&child)) {
			bson_t         cbson;
			uint32_t       len;
			const uint8_t* data;
			zval           writeerror;

			if (!BSON_ITER_HOLDS_DOCUMENT(&child)) {
				continue;
			}

			bson_iter_document(&child, &len, &data);

			if (!bson_init_static(&cbson, data, len)) {
				continue;
			}

			if (!phongo_writeerror_init(&writeerror, &cbson)) {
				zval_ptr_dtor(&writeerror);
				continue;
			}

			add_next_index_zval(return_value, &writeerror);
		}
	}

	return true;
}

static bool php_phongo_writeresult_get_error_replies(php_phongo_writeresult_t* intern, zval* return_value)
{
	bson_iter_t iter, child;

	array_init(return_value);

	if (bson_iter_init_find(&iter, intern->reply, "errorReplies") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		while (bson_iter_next(&child)) {
			uint32_t       len;
			const uint8_t* data;
			zval           error_reply;

			if (!BSON_ITER_HOLDS_DOCUMENT(&child)) {
				continue;
			}

			bson_iter_document(&child, &len, &data);
			php_phongo_bson_data_to_zval(data, len, &error_reply);

			add_next_index_zval(return_value, &error_reply);
		}
	}

	return true;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_WriteResult)

/* Returns the number of documents that were inserted */
static PHP_METHOD(MongoDB_Driver_WriteResult, getInsertedCount)
{
	bson_iter_t               iter;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getInsertedCount");

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nInserted");
}

/* Returns the number of documents that matched the update criteria */
static PHP_METHOD(MongoDB_Driver_WriteResult, getMatchedCount)
{
	bson_iter_t               iter;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getMatchedCount");

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nMatched");
}

/* Returns the number of documents that were actually modified by an update */
static PHP_METHOD(MongoDB_Driver_WriteResult, getModifiedCount)
{
	bson_iter_t               iter;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getModifiedCount");

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nModified");
}

/* Returns the number of documents that were deleted */
static PHP_METHOD(MongoDB_Driver_WriteResult, getDeletedCount)
{
	bson_iter_t               iter;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getDeletedCount");

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nRemoved");
}

/* Returns the number of documents that were upserted */
static PHP_METHOD(MongoDB_Driver_WriteResult, getUpsertedCount)
{
	bson_iter_t               iter;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getUpsertedCount");

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nUpserted");
}

/* Returns the Server from which the result originated */
static PHP_METHOD(MongoDB_Driver_WriteResult, getServer)
{
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_server_init(return_value, &intern->manager, intern->server_id);
}

/* Returns the identifiers generated by the server for upsert operations. */
static PHP_METHOD(MongoDB_Driver_WriteResult, getUpsertedIds)
{
	bson_iter_t               iter, child;
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getUpsertedIds");

	array_init(return_value);

	if (bson_iter_init_find(&iter, intern->reply, "upserted") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		while (bson_iter_next(&child)) {
			uint32_t              data_len;
			const uint8_t*        data = NULL;
			php_phongo_bson_state state;

			/* Use PHONGO_TYPEMAP_NATIVE_ARRAY for the root type so we can
			 * easily access the "index" and "_id" fields. */
			PHONGO_BSON_INIT_STATE(state);
			state.map.root.type = PHONGO_TYPEMAP_NATIVE_ARRAY;

			if (!BSON_ITER_HOLDS_DOCUMENT(&child)) {
				continue;
			}

			bson_iter_document(&child, &data_len, &data);

			if (php_phongo_bson_data_to_zval_ex(data, data_len, &state)) {
				zval* zid = php_array_fetchc(&state.zchild, "_id");
				add_index_zval(return_value, php_array_fetchc_long(&state.zchild, "index"), zid);
				zval_add_ref(zid);
			}

			zval_ptr_dtor(&state.zchild);
		}
	}
}

/* Return any write concern error that occurred */
static PHP_METHOD(MongoDB_Driver_WriteResult, getWriteConcernError)
{
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	php_phongo_writeresult_get_writeconcernerror(intern, return_value);
}

/* Returns any write errors that occurred */
static PHP_METHOD(MongoDB_Driver_WriteResult, getWriteErrors)
{
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	php_phongo_writeresult_get_writeerrors(intern, return_value);
}

static PHP_METHOD(MongoDB_Driver_WriteResult, getErrorReplies)
{
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	php_phongo_writeresult_get_error_replies(intern, return_value);
}

/* Returns whether the write operation was acknowledged (based on the write
   concern). */
static PHP_METHOD(MongoDB_Driver_WriteResult, isAcknowledged)
{
	php_phongo_writeresult_t* intern;

	intern = Z_WRITERESULT_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(mongoc_write_concern_is_acknowledged(intern->write_concern));
}

/* MongoDB\Driver\WriteResult object handlers */
static zend_object_handlers php_phongo_handler_writeresult;

static void php_phongo_writeresult_free_object(zend_object* object)
{
	php_phongo_writeresult_t* intern = Z_OBJ_WRITERESULT(object);

	zend_object_std_dtor(&intern->std);

	if (intern->reply) {
		bson_destroy(intern->reply);
	}

	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}

	if (!Z_ISUNDEF(intern->manager)) {
		zval_ptr_dtor(&intern->manager);
	}
}

static zend_object* php_phongo_writeresult_create_object(zend_class_entry* class_type)
{
	php_phongo_writeresult_t* intern = zend_object_alloc(sizeof(php_phongo_writeresult_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_writeresult;

	return &intern->std;
}

static HashTable* php_phongo_writeresult_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_writeresult_t* intern;
	zval                      retval = ZVAL_STATIC_INIT;
	bson_iter_t               iter;

	intern   = Z_OBJ_WRITERESULT(object);
	*is_temp = 1;
	array_init_size(&retval, 10);

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
		if (!php_phongo_bson_data_to_zval_ex(data, len, &state)) {
			zval_ptr_dtor(&state.zchild);
			goto done;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "upsertedIds", &state.zchild);
	} else {
		zval upsertedIds;
		array_init(&upsertedIds);
		ADD_ASSOC_ZVAL_EX(&retval, "upsertedIds", &upsertedIds);
	}

	{
		zval writeerrors;

		php_phongo_writeresult_get_writeerrors(intern, &writeerrors);
		ADD_ASSOC_ZVAL_EX(&retval, "writeErrors", &writeerrors);
	}

	{
		zval writeconcernerror;

		php_phongo_writeresult_get_writeconcernerror(intern, &writeconcernerror);
		ADD_ASSOC_ZVAL_EX(&retval, "writeConcernError", &writeconcernerror);
	}

	if (intern->write_concern) {
		zval write_concern;

		phongo_writeconcern_init(&write_concern, intern->write_concern);
		ADD_ASSOC_ZVAL_EX(&retval, "writeConcern", &write_concern);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "writeConcern");
	}

	{
		zval error_replies;

		php_phongo_writeresult_get_error_replies(intern, &error_replies);
		ADD_ASSOC_ZVAL_EX(&retval, "errorReplies", &error_replies);
	}

done:
	return Z_ARRVAL(retval);
}

void php_phongo_writeresult_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_writeresult_ce                = register_class_MongoDB_Driver_WriteResult();
	php_phongo_writeresult_ce->create_object = php_phongo_writeresult_create_object;

	memcpy(&php_phongo_handler_writeresult, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeresult.get_debug_info = php_phongo_writeresult_get_debug_info;
	php_phongo_handler_writeresult.free_obj       = php_phongo_writeresult_free_object;
	php_phongo_handler_writeresult.offset         = XtOffsetOf(php_phongo_writeresult_t, std);
}

php_phongo_writeresult_t* phongo_writeresult_init(zval* return_value, bson_t* reply, zval* manager, uint32_t server_id)
{
	php_phongo_writeresult_t* writeresult;

	object_init_ex(return_value, php_phongo_writeresult_ce);

	writeresult            = Z_WRITERESULT_OBJ_P(return_value);
	writeresult->reply     = bson_copy(reply);
	writeresult->server_id = server_id;

	ZVAL_ZVAL(&writeresult->manager, manager, 1, 0);

	return writeresult;
}
