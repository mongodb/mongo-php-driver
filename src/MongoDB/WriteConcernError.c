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

#include "MongoDB/WriteConcernError.h"
#include "WriteConcernError_arginfo.h"

zend_class_entry* phongo_writeconcernerror_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_WriteConcernError)

/* Returns the MongoDB error code */
static PHP_METHOD(MongoDB_Driver_WriteConcernError, getCode)
{
	PHONGO_INTERN_FROM_THIS(writeconcernerror);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->code);
}

/* Returns additional metadata for the error */
static PHP_METHOD(MongoDB_Driver_WriteConcernError, getInfo)
{
	PHONGO_INTERN_FROM_THIS(writeconcernerror);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!Z_ISUNDEF(intern->info)) {
		RETURN_ZVAL(&intern->info, 1, 0);
	}
}

/* Returns the actual error message from the server */
static PHP_METHOD(MongoDB_Driver_WriteConcernError, getMessage)
{
	PHONGO_INTERN_FROM_THIS(writeconcernerror);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->message) {
		RETURN_STRING("");
	}

	RETURN_STRING(intern->message);
}

/* MongoDB\Driver\WriteConcernError object handlers */
static zend_object_handlers phongo_handler_writeconcernerror;

static void phongo_writeconcernerror_free_object(zend_object* object)
{
	phongo_writeconcernerror_t* intern = Z_OBJ_WRITECONCERNERROR(object);

	zend_object_std_dtor(&intern->std);

	if (intern->message) {
		efree(intern->message);
	}

	if (!Z_ISUNDEF(intern->info)) {
		zval_ptr_dtor(&intern->info);
	}
}

static zend_object* phongo_writeconcernerror_create_object(zend_class_entry* class_type)
{
	phongo_writeconcernerror_t* intern = zend_object_alloc(sizeof(phongo_writeconcernerror_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &phongo_handler_writeconcernerror;

	return &intern->std;
}

static HashTable* phongo_writeconcernerror_get_debug_info(zend_object* object, int* is_temp)
{
	phongo_writeconcernerror_t* intern;
	zval                        retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_OBJ_WRITECONCERNERROR(object);

	array_init_size(&retval, 3);
	ADD_ASSOC_STRING(&retval, "message", intern->message ? intern->message : "");
	ADD_ASSOC_LONG_EX(&retval, "code", intern->code);
	if (!Z_ISUNDEF(intern->info)) {
		Z_ADDREF(intern->info);
		ADD_ASSOC_ZVAL_EX(&retval, "info", &intern->info);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "info");
	}

	return Z_ARRVAL(retval);
}

void phongo_writeconcernerror_init_ce(INIT_FUNC_ARGS)
{
	phongo_writeconcernerror_ce                = register_class_MongoDB_Driver_WriteConcernError();
	phongo_writeconcernerror_ce->create_object = phongo_writeconcernerror_create_object;

	memcpy(&phongo_handler_writeconcernerror, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_writeconcernerror.get_debug_info = phongo_writeconcernerror_get_debug_info;
	phongo_handler_writeconcernerror.free_obj       = phongo_writeconcernerror_free_object;
	phongo_handler_writeconcernerror.offset         = XtOffsetOf(phongo_writeconcernerror_t, std);
}

/* Initializes a new WriteConcernError in return_value using the BSON document.
 * Returns true on success; otherwise, false is returned and an exception is
 * thrown.
 *
 * This function supports documents from both mongoc_bulk_operation_execute and
 * mongoc_bulkwriteexception_t (returned by mongoc_bulkwrite_execute). */
bool phongo_writeconcernerror_init(zval* return_value, const bson_t* bson)
{
	bson_iter_t                 iter;
	phongo_writeconcernerror_t* intern;

	object_init_ex(return_value, phongo_writeconcernerror_ce);

	intern       = Z_WRITECONCERNERROR_OBJ_P(return_value);
	intern->code = 0;

	if (bson_iter_init_find(&iter, bson, "code") && BSON_ITER_HOLDS_INT32(&iter)) {
		intern->code = bson_iter_int32(&iter);
	}

	// Additionally check for field name used by mongoc_bulkwriteexception_t
	if ((bson_iter_init_find(&iter, bson, "errmsg") && BSON_ITER_HOLDS_UTF8(&iter)) ||
		(bson_iter_init_find(&iter, bson, "message") && BSON_ITER_HOLDS_UTF8(&iter))) {
		uint32_t    len;
		const char* message = bson_iter_utf8(&iter, &len);

		intern->message = estrndup(message, len);
	}

	// Additionally check for field name used by mongoc_bulkwriteexception_t
	if ((bson_iter_init_find(&iter, bson, "errInfo") && BSON_ITER_HOLDS_DOCUMENT(&iter)) ||
		(bson_iter_init_find(&iter, bson, "details") && BSON_ITER_HOLDS_DOCUMENT(&iter))) {
		uint32_t       len;
		const uint8_t* data = NULL;

		bson_iter_document(&iter, &len, &data);

		if (!phongo_bson_data_to_zval(data, len, &intern->info)) {
			zval_ptr_dtor(&intern->info);
			ZVAL_UNDEF(&intern->info);

			return false;
		}
	}

	return true;
}
