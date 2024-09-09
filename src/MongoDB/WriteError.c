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

#include "php_phongo.h"
#include "phongo_error.h"

#include "MongoDB/WriteError.h"
#include "WriteError_arginfo.h"

zend_class_entry* php_phongo_writeerror_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_WriteError)

/* Returns the MongoDB error code */
static PHP_METHOD(MongoDB_Driver_WriteError, getCode)
{
	php_phongo_writeerror_t* intern;

	intern = Z_WRITEERROR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->code);
}

/* Returns the index of the operation in the BulkWrite to which this WriteError
   corresponds. */
static PHP_METHOD(MongoDB_Driver_WriteError, getIndex)
{
	php_phongo_writeerror_t* intern;

	intern = Z_WRITEERROR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->index);
}

/* Returns the actual error message from the server */
static PHP_METHOD(MongoDB_Driver_WriteError, getMessage)
{
	php_phongo_writeerror_t* intern;

	intern = Z_WRITEERROR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRING(intern->message);
}

/* Returns additional metadata for the error */
static PHP_METHOD(MongoDB_Driver_WriteError, getInfo)
{
	php_phongo_writeerror_t* intern;

	intern = Z_WRITEERROR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!Z_ISUNDEF(intern->info)) {
		RETURN_ZVAL(&intern->info, 1, 0);
	}
}

/* MongoDB\Driver\WriteError object handlers */
static zend_object_handlers php_phongo_handler_writeerror;

static void php_phongo_writeerror_free_object(zend_object* object)
{
	php_phongo_writeerror_t* intern = Z_OBJ_WRITEERROR(object);

	zend_object_std_dtor(&intern->std);

	if (intern->message) {
		efree(intern->message);
	}

	if (!Z_ISUNDEF(intern->info)) {
		zval_ptr_dtor(&intern->info);
	}
}

static zend_object* php_phongo_writeerror_create_object(zend_class_entry* class_type)
{
	php_phongo_writeerror_t* intern = zend_object_alloc(sizeof(php_phongo_writeerror_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_writeerror;

	return &intern->std;
}

static HashTable* php_phongo_writeerror_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_writeerror_t* intern;
	zval                     retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_OBJ_WRITEERROR(object);

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

void php_phongo_writeerror_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_writeerror_ce                = register_class_MongoDB_Driver_WriteError();
	php_phongo_writeerror_ce->create_object = php_phongo_writeerror_create_object;

	memcpy(&php_phongo_handler_writeerror, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeerror.get_debug_info = php_phongo_writeerror_get_debug_info;
	php_phongo_handler_writeerror.free_obj       = php_phongo_writeerror_free_object;
	php_phongo_handler_writeerror.offset         = XtOffsetOf(php_phongo_writeerror_t, std);
}

zend_bool phongo_writeerror_init(zval* return_value, bson_t* bson)
{
	bson_iter_t              iter;
	php_phongo_writeerror_t* intern;

	object_init_ex(return_value, php_phongo_writeerror_ce);

	intern        = Z_WRITEERROR_OBJ_P(return_value);
	intern->code  = 0;
	intern->index = 0;

	if (bson_iter_init_find(&iter, bson, "code") && BSON_ITER_HOLDS_INT32(&iter)) {
		intern->code = bson_iter_int32(&iter);
	}

	if (bson_iter_init_find(&iter, bson, "errmsg") && BSON_ITER_HOLDS_UTF8(&iter)) {
		uint32_t    errmsg_len;
		const char* err_msg = bson_iter_utf8(&iter, &errmsg_len);

		intern->message = estrndup(err_msg, errmsg_len);
	}

	if (bson_iter_init_find(&iter, bson, "errInfo") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
		uint32_t       len;
		const uint8_t* data = NULL;

		bson_iter_document(&iter, &len, &data);

		if (!php_phongo_bson_data_to_zval(data, len, &intern->info)) {
			zval_ptr_dtor(&intern->info);
			ZVAL_UNDEF(&intern->info);

			return false;
		}
	}

	if (bson_iter_init_find(&iter, bson, "index") && BSON_ITER_HOLDS_INT32(&iter)) {
		intern->index = bson_iter_int32(&iter);
	}

	return true;
}
