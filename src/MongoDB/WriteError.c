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

PHONGO_PROPERTY_GETTER(MongoDB_Driver_WriteError, getCode, writeerror, "code")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_WriteError, getIndex, writeerror, "index")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_WriteError, getMessage, writeerror, "message")
PHONGO_PROPERTY_GETTER(MongoDB_Driver_WriteError, getInfo, writeerror, "info")

static bool phongo_writeerror_update_properties(zend_object* object, const bson_t* bson, int32_t index)
{
	bson_iter_t iter;

	if (bson_iter_init_find(&iter, bson, "code") && BSON_ITER_HOLDS_INT32(&iter)) {
		zend_update_property_long(phongo_writeerror_ce, object, ZEND_STRL("code"), bson_iter_int32(&iter));
	} else {
		zend_update_property_long(phongo_writeerror_ce, object, ZEND_STRL("code"), 0);
	}

	// Additionally check for field name used by mongoc_bulkwriteexception_t
	if ((bson_iter_init_find(&iter, bson, "errmsg") && BSON_ITER_HOLDS_UTF8(&iter)) ||
		(bson_iter_init_find(&iter, bson, "message") && BSON_ITER_HOLDS_UTF8(&iter))) {
		uint32_t    errmsg_len;
		const char* message = bson_iter_utf8(&iter, &errmsg_len);

		zend_update_property_string(phongo_writeerror_ce, object, ZEND_STRL("message"), message ? message : "");
	} else {
		zend_update_property_string(phongo_writeerror_ce, object, ZEND_STRL("message"), "");
	}

	// Additionally check for field name used by mongoc_bulkwriteexception_t
	if ((bson_iter_init_find(&iter, bson, "errInfo") && BSON_ITER_HOLDS_DOCUMENT(&iter)) ||
		(bson_iter_init_find(&iter, bson, "details") && BSON_ITER_HOLDS_DOCUMENT(&iter))) {
		uint32_t       len;
		const uint8_t* data = NULL;
		zval           zinfo;

		bson_iter_document(&iter, &len, &data);

		if (!phongo_bson_data_to_zval(data, len, &zinfo)) {
			/* Exception already thrown */
			zval_ptr_dtor(&zinfo);

			return false;
		}

		zend_update_property(phongo_writeerror_ce, object, ZEND_STRL("info"), &zinfo);
		zval_ptr_dtor(&zinfo);
	} else {
		zend_update_property_null(phongo_writeerror_ce, object, ZEND_STRL("info"));
	}

	/* If the WriteError is initialized from mongoc_bulkwriteexception_t, an
	 * index should already have been specified. */
	if (!index && bson_iter_init_find(&iter, bson, "index") && BSON_ITER_HOLDS_INT32(&iter)) {
		index = bson_iter_int32(&iter);
	}

	zend_update_property_long(phongo_writeerror_ce, object, ZEND_STRL("index"), index);

	return true;
}

void phongo_writeerror_init_ce(INIT_FUNC_ARGS)
{
	phongo_writeerror_ce = register_class_MongoDB_Driver_WriteError();
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
	PHONGO_OBJECT_INIT_EX(writeerror, return_value);

	return phongo_writeerror_update_properties(object, bson, index);
}
