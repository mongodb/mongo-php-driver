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

#include "phongo.h"
#include "phongo_error.h"

#include "MongoDB/Server.h"
#include "MongoDB/WriteConcern.h"
#include "MongoDB/WriteConcernError.h"
#include "MongoDB/WriteError.h"
#include "WriteResult_arginfo.h"

#define PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED(method)                                                                                                     \
	if (!mongoc_write_concern_is_acknowledged(intern->write_concern)) {                                                                                   \
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "MongoDB\\Driver\\WriteResult::" method "() should not be called for an unacknowledged write result"); \
	}

#define PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(iter, bson, key)                \
	if (bson_iter_init_find((iter), (bson), (key)) && BSON_ITER_HOLDS_INT32((iter))) { \
		RETURN_LONG(bson_iter_int32((iter)));                                          \
	}                                                                                  \
	RETURN_LONG(0);

#define PHONGO_WRITERESULT_UPDATE_PROP(prop, field)                                                                                                          \
	if (mongoc_write_concern_is_acknowledged(intern->write_concern) && bson_iter_init_find(&iter, intern->reply, (field)) && BSON_ITER_HOLDS_INT32(&iter)) { \
		zend_update_property_long(phongo_writeresult_ce, &intern->std, ZEND_STRL(prop), bson_iter_int32(&iter));                                             \
	} else {                                                                                                                                                 \
		zend_update_property_null(phongo_writeresult_ce, &intern->std, ZEND_STRL(prop));                                                                     \
	}

zend_class_entry* phongo_writeresult_ce;

/* Populates return_value with a WriteConcernError object (if available).
 * Returns true on success; otherwise, false is returned and an exception is
 * thrown. */
static bool phongo_writeresult_get_writeconcernerror(phongo_writeresult_t* intern, zval* return_value)
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
				/* Exception already thrown */
				zval_ptr_dtor(&writeconcernerror);
				return false;
			}

			ZVAL_ZVAL(return_value, &writeconcernerror, 1, 1);

			return true;
		}
	}

	return true;
}

/* Populates return_value with a list of WriteError objects. Returns true on
 * success; otherwise, false is returned and an exception is thrown. */
static bool phongo_writeresult_get_writeerrors(phongo_writeresult_t* intern, zval* return_value)
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
				/* Exception already thrown */
				zval_ptr_dtor(&writeerror);
				return false;
			}

			add_next_index_zval(return_value, &writeerror);
		}
	}

	return true;
}

static bool phongo_writeresult_get_error_replies(phongo_writeresult_t* intern, zval* return_value)
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
			phongo_bson_data_to_zval(data, len, &error_reply);

			add_next_index_zval(return_value, &error_reply);
		}
	}

	return true;
}

static void phongo_writeresult_get_upserted_ids(phongo_writeresult_t* intern, zval* return_value)
{
	bson_iter_t iter, child;

	array_init(return_value);

	if (bson_iter_init_find(&iter, intern->reply, "upserted") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		while (bson_iter_next(&child)) {
			uint32_t          data_len;
			const uint8_t*    data = NULL;
			phongo_bson_state state;

			/* Use PHONGO_TYPEMAP_NATIVE_ARRAY for the root type so we can
			 * easily access the "index" and "_id" fields. */
			PHONGO_BSON_INIT_STATE(state);
			state.map.root.type = PHONGO_TYPEMAP_NATIVE_ARRAY;

			if (!BSON_ITER_HOLDS_DOCUMENT(&child)) {
				continue;
			}

			bson_iter_document(&child, &data_len, &data);

			if (phongo_bson_data_to_zval_ex(data, data_len, &state)) {
				zval* zid = php_array_fetchc(&state.zchild, "_id");
				add_index_zval(return_value, php_array_fetchc_long(&state.zchild, "index"), zid);
				zval_add_ref(zid);
			}

			zval_ptr_dtor(&state.zchild);
		}
	}
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_WriteResult)

/* Returns the number of documents that were inserted */
static PHP_METHOD(MongoDB_Driver_WriteResult, getInsertedCount)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	bson_iter_t iter;

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getInsertedCount");

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nInserted");
}

/* Returns the number of documents that matched the update criteria */
static PHP_METHOD(MongoDB_Driver_WriteResult, getMatchedCount)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	bson_iter_t iter;

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getMatchedCount");

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nMatched");
}

/* Returns the number of documents that were actually modified by an update */
static PHP_METHOD(MongoDB_Driver_WriteResult, getModifiedCount)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	bson_iter_t iter;

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getModifiedCount");

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nModified");
}

/* Returns the number of documents that were deleted */
static PHP_METHOD(MongoDB_Driver_WriteResult, getDeletedCount)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	bson_iter_t iter;

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getDeletedCount");

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nRemoved");
}

/* Returns the number of documents that were upserted */
static PHP_METHOD(MongoDB_Driver_WriteResult, getUpsertedCount)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	bson_iter_t iter;

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getUpsertedCount");

	PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32(&iter, intern->reply, "nUpserted");
}

/* Returns the Server from which the result originated */
static PHP_METHOD(MongoDB_Driver_WriteResult, getServer)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_server_init(return_value, &intern->manager, intern->server_id);
}

/* Returns the identifiers generated by the server for upsert operations. */
static PHP_METHOD(MongoDB_Driver_WriteResult, getUpsertedIds)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED("getUpsertedIds");

	phongo_writeresult_get_upserted_ids(intern, return_value);
}

/* Return any write concern error that occurred */
static PHP_METHOD(MongoDB_Driver_WriteResult, getWriteConcernError)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_writeresult_get_writeconcernerror(intern, return_value);
}

/* Returns any write errors that occurred */
static PHP_METHOD(MongoDB_Driver_WriteResult, getWriteErrors)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_writeresult_get_writeerrors(intern, return_value);
}

static PHP_METHOD(MongoDB_Driver_WriteResult, getErrorReplies)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_writeresult_get_error_replies(intern, return_value);
}

/* Returns whether the write operation was acknowledged (based on the write
   concern). */
static PHP_METHOD(MongoDB_Driver_WriteResult, isAcknowledged)
{
	PHONGO_INTERN_FROM_THIS(writeresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(mongoc_write_concern_is_acknowledged(intern->write_concern));
}

/* MongoDB\Driver\WriteResult object handlers */
static zend_object_handlers phongo_handler_writeresult;

static void phongo_writeresult_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(writeresult, object);

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

static zend_object* phongo_writeresult_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(writeresult, class_type);

	intern->std.handlers = &phongo_handler_writeresult;

	return &intern->std;
}

static void phongo_writeresult_update_properties(phongo_writeresult_t* intern)
{
	bson_iter_t iter;

	PHONGO_WRITERESULT_UPDATE_PROP("insertedCount", "nInserted");
	PHONGO_WRITERESULT_UPDATE_PROP("matchedCount", "nMatched");
	PHONGO_WRITERESULT_UPDATE_PROP("modifiedCount", "nModified");
	PHONGO_WRITERESULT_UPDATE_PROP("deletedCount", "nRemoved");
	PHONGO_WRITERESULT_UPDATE_PROP("upsertedCount", "nUpserted");

	{
		zval upsertedIds;

		phongo_writeresult_get_upserted_ids(intern, &upsertedIds);
		zend_update_property(phongo_writeresult_ce, &intern->std, ZEND_STRL("upsertedIds"), &upsertedIds);
		zval_ptr_dtor(&upsertedIds);
	}

	{
		zval writeerrors;
		phongo_writeresult_get_writeerrors(intern, &writeerrors);
		zend_update_property(phongo_writeresult_ce, &intern->std, ZEND_STRL("writeErrors"), &writeerrors);
		zval_ptr_dtor(&writeerrors);
	}

	{
		zval writeconcernerror;
		phongo_writeresult_get_writeconcernerror(intern, &writeconcernerror);
		zend_update_property(phongo_writeresult_ce, &intern->std, ZEND_STRL("writeConcernError"), &writeconcernerror);
		zval_ptr_dtor(&writeconcernerror);
	}

	if (intern->write_concern) {
		zval write_concern;
		phongo_writeconcern_init(&write_concern, intern->write_concern);
		zend_update_property(phongo_writeresult_ce, &intern->std, ZEND_STRL("writeConcern"), &write_concern);
		zval_ptr_dtor(&write_concern);
	} else {
		zend_update_property_null(phongo_writeresult_ce, &intern->std, ZEND_STRL("writeConcern"));
	}

	{
		zval error_replies;
		phongo_writeresult_get_error_replies(intern, &error_replies);
		zend_update_property(phongo_writeresult_ce, &intern->std, ZEND_STRL("errorReplies"), &error_replies);
		zval_ptr_dtor(&error_replies);
	}

	{
		zval server;
		phongo_server_init(&server, &intern->manager, intern->server_id);
		zend_update_property(phongo_writeresult_ce, &intern->std, ZEND_STRL("server"), &server);
		zval_ptr_dtor(&server);
	}
}

void phongo_writeresult_init_ce(INIT_FUNC_ARGS)
{
	phongo_writeresult_ce                = register_class_MongoDB_Driver_WriteResult();
	phongo_writeresult_ce->create_object = phongo_writeresult_create_object;

	memcpy(&phongo_handler_writeresult, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_writeresult.free_obj = phongo_writeresult_free_object;
	phongo_handler_writeresult.offset   = XtOffsetOf(phongo_writeresult_t, std);
}

void phongo_writeresult_init(zval* return_value, bson_t* reply, zval* manager, uint32_t server_id, const mongoc_write_concern_t* write_concern)
{
	phongo_writeresult_t* writeresult;

	object_init_ex(return_value, phongo_writeresult_ce);

	writeresult                = Z_WRITERESULT_OBJ_P(return_value);
	writeresult->reply         = bson_copy(reply);
	writeresult->server_id     = server_id;
	writeresult->write_concern = mongoc_write_concern_copy(write_concern);

	ZVAL_ZVAL(&writeresult->manager, manager, 1, 0);

	phongo_writeresult_update_properties(writeresult);
}

#undef PHONGO_WRITERESULT_CHECK_ACKNOWLEDGED
#undef PHONGO_WRITERESULT_RETURN_LONG_FROM_BSON_INT32
#undef PHONGO_WRITERESULT_UPDATE_PROP
