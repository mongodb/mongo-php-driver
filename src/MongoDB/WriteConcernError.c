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

#include "MongoDB/WriteConcernError.h"
#include "WriteConcernError_arginfo.h"

zend_class_entry* php_phongo_writeconcernerror_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_WriteConcernError)

/* Returns the MongoDB error code */
static PHP_METHOD(MongoDB_Driver_WriteConcernError, getCode)
{
	php_phongo_writeconcernerror_t* intern;

	intern = Z_WRITECONCERNERROR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_LONG(intern->code);
}

/* Returns additional metadata for the error */
static PHP_METHOD(MongoDB_Driver_WriteConcernError, getInfo)
{
	php_phongo_writeconcernerror_t* intern;

	intern = Z_WRITECONCERNERROR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!Z_ISUNDEF(intern->info)) {
		RETURN_ZVAL(&intern->info, 1, 0);
	}
}

/* Returns the actual error message from the server */
static PHP_METHOD(MongoDB_Driver_WriteConcernError, getMessage)
{
	php_phongo_writeconcernerror_t* intern;

	intern = Z_WRITECONCERNERROR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->message) {
		RETURN_STRING("");
	}

	RETURN_STRING(intern->message);
}

/* MongoDB\Driver\WriteConcernError object handlers */
static zend_object_handlers php_phongo_handler_writeconcernerror;

static void php_phongo_writeconcernerror_free_object(zend_object* object)
{
	php_phongo_writeconcernerror_t* intern = Z_OBJ_WRITECONCERNERROR(object);

	zend_object_std_dtor(&intern->std);

	if (intern->message) {
		efree(intern->message);
	}

	if (!Z_ISUNDEF(intern->info)) {
		zval_ptr_dtor(&intern->info);
	}
}

static zend_object* php_phongo_writeconcernerror_create_object(zend_class_entry* class_type)
{
	php_phongo_writeconcernerror_t* intern = zend_object_alloc(sizeof(php_phongo_writeconcernerror_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_writeconcernerror;

	return &intern->std;
}

static HashTable* php_phongo_writeconcernerror_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_writeconcernerror_t* intern;
	zval                            retval = ZVAL_STATIC_INIT;

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

void php_phongo_writeconcernerror_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_writeconcernerror_ce                = register_class_MongoDB_Driver_WriteConcernError();
	php_phongo_writeconcernerror_ce->create_object = php_phongo_writeconcernerror_create_object;

	memcpy(&php_phongo_handler_writeconcernerror, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeconcernerror.get_debug_info = php_phongo_writeconcernerror_get_debug_info;
	php_phongo_handler_writeconcernerror.free_obj       = php_phongo_writeconcernerror_free_object;
	php_phongo_handler_writeconcernerror.offset         = XtOffsetOf(php_phongo_writeconcernerror_t, std);
}

zend_bool phongo_writeconcernerror_init(zval* return_value, bson_t* bson)
{
	bson_iter_t                     iter;
	php_phongo_writeconcernerror_t* intern;

	object_init_ex(return_value, php_phongo_writeconcernerror_ce);

	intern       = Z_WRITECONCERNERROR_OBJ_P(return_value);
	intern->code = 0;

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

	return true;
}
