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

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_error.h"

#include "MongoDB/WriteError.h"
#include "WriteError_arginfo.h"

zend_class_entry* phongo_writeerror_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_WriteError)

/* Returns the MongoDB error code */
static PHP_METHOD(MongoDB_Driver_WriteError, getCode)
{
	PHONGO_INTERN_FROM_THIS(writeerror);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->code);
}

/* Returns the index of the operation in the BulkWrite to which this WriteError
   corresponds. */
static PHP_METHOD(MongoDB_Driver_WriteError, getIndex)
{
	PHONGO_INTERN_FROM_THIS(writeerror);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->index);
}

/* Returns the actual error message from the server */
static PHP_METHOD(MongoDB_Driver_WriteError, getMessage)
{
	PHONGO_INTERN_FROM_THIS(writeerror);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRING(intern->message);
}

/* Returns additional metadata for the error */
static PHP_METHOD(MongoDB_Driver_WriteError, getInfo)
{
	PHONGO_INTERN_FROM_THIS(writeerror);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!Z_ISUNDEF(intern->info)) {
		RETURN_ZVAL(&intern->info, 1, 0);
	}
}

/* MongoDB\Driver\WriteError object handlers */
static zend_object_handlers phongo_handler_writeerror;

static void phongo_writeerror_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(writeerror, object);

	zend_object_std_dtor(&intern->std);

	if (intern->message) {
		efree(intern->message);
	}

	if (!Z_ISUNDEF(intern->info)) {
		zval_ptr_dtor(&intern->info);
	}
}

static zend_object* phongo_writeerror_create_object(zend_class_entry* class_type)
{
	phongo_writeerror_t* intern = zend_object_alloc(sizeof(phongo_writeerror_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &phongo_handler_writeerror;

	return &intern->std;
}

static HashTable* phongo_writeerror_get_debug_info(zend_object* object, int* is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(writeerror, object);

	zval retval = ZVAL_STATIC_INIT;

	*is_temp = 1;

	array_init_size(&retval, 3);
	ADD_ASSOC_STRING(&retval, "message", intern->message);
	ADD_ASSOC_LONG_EX(&retval, "code", intern->code);
	ADD_ASSOC_LONG_EX(&retval, "index", intern->index);
	if (!Z_ISUNDEF(intern->info)) {
		Z_ADDREF(intern->info);
		ADD_ASSOC_ZVAL_EX(&retval, "info", &intern->info);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "info");
	}

	return Z_ARRVAL(retval);
}

void phongo_writeerror_init_ce(INIT_FUNC_ARGS)
{
	phongo_writeerror_ce                = register_class_MongoDB_Driver_WriteError();
	phongo_writeerror_ce->create_object = phongo_writeerror_create_object;

	memcpy(&phongo_handler_writeerror, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_writeerror.get_debug_info = phongo_writeerror_get_debug_info;
	phongo_handler_writeerror.free_obj       = phongo_writeerror_free_object;
	phongo_handler_writeerror.offset         = XtOffsetOf(phongo_writeerror_t, std);
}

bool phongo_writeerror_init(zval* return_value, const bson_t* bson)
{
	return phongo_writeerror_init_ex(return_value, bson, 0);
}

/* Initializes a new WriteError in return_value using the BSON document. Returns
 * true on success; otherwise, false is returned and an exception is thrown.
 *
 * This function supports documents from both mongoc_bulk_operation_execute and
 * mongoc_bulkwriteexception_t (returned by mongoc_bulkwrite_execute). When
 * initializing from mongoc_bulkwriteexception_t, an index should be explicitly
 * provided since the BSON document will not have an "index" field. */
bool phongo_writeerror_init_ex(zval* return_value, const bson_t* bson, int32_t index)
{
	bson_iter_t          iter;
	phongo_writeerror_t* intern;

	object_init_ex(return_value, phongo_writeerror_ce);

	intern        = Z_WRITEERROR_OBJ_P(return_value);
	intern->code  = 0;
	intern->index = index;

	if (bson_iter_init_find(&iter, bson, "code") && BSON_ITER_HOLDS_INT32(&iter)) {
		intern->code = bson_iter_int32(&iter);
	}

	// Additionally check for field name used by mongoc_bulkwriteexception_t
	if ((bson_iter_init_find(&iter, bson, "errmsg") && BSON_ITER_HOLDS_UTF8(&iter)) ||
		(bson_iter_init_find(&iter, bson, "message") && BSON_ITER_HOLDS_UTF8(&iter))) {
		uint32_t    errmsg_len;
		const char* err_msg = bson_iter_utf8(&iter, &errmsg_len);

		intern->message = estrndup(err_msg, errmsg_len);
	}

	// Additionally check for field name used by mongoc_bulkwriteexception_t
	if ((bson_iter_init_find(&iter, bson, "errInfo") && BSON_ITER_HOLDS_DOCUMENT(&iter)) ||
		(bson_iter_init_find(&iter, bson, "details") && BSON_ITER_HOLDS_DOCUMENT(&iter))) {
		uint32_t       len;
		const uint8_t* data = NULL;

		bson_iter_document(&iter, &len, &data);

		if (!phongo_bson_data_to_zval(data, len, &intern->info)) {
			/* Exception already thrown */
			zval_ptr_dtor(&intern->info);
			ZVAL_UNDEF(&intern->info);

			return false;
		}
	}

	/* If the WriteError is initialized from mongoc_bulkwriteexception_t, an
	 * index should already have been specified. */
	if (!intern->index && bson_iter_init_find(&iter, bson, "index") && BSON_ITER_HOLDS_INT32(&iter)) {
		intern->index = bson_iter_int32(&iter);
	}

	return true;
}
