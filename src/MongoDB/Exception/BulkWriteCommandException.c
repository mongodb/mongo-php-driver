/*
 * Copyright 2024-present MongoDB, Inc.
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

#include "php_phongo.h"
#include "phongo_error.h"

#include "BSON/Document.h"
#include "BulkWriteCommandException.h"
#include "BulkWriteCommandException_arginfo.h"
#include "MongoDB/WriteConcernError.h"
#include "MongoDB/WriteError.h"

zend_class_entry* php_phongo_bulkwritecommandexception_ce;

/* Returns the error reply document (if any) from the failed bulk write */
static PHP_METHOD(MongoDB_Driver_Exception_BulkWriteCommandException, getErrorReply)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	zval  rv;
	zval* result = zend_read_property(php_phongo_bulkwritecommandexception_ce, Z_OBJ_P(getThis()), ZEND_STRL("errorReply"), 0, &rv);

	RETURN_ZVAL(result, 1, 0);
}

/* Returns the partial BulkWriteCommandResult (if any) from the failed bulk write. */
static PHP_METHOD(MongoDB_Driver_Exception_BulkWriteCommandException, getPartialResult)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	zval  rv;
	zval* result = zend_read_property(php_phongo_bulkwritecommandexception_ce, Z_OBJ_P(getThis()), ZEND_STRL("partialResult"), 0, &rv);

	RETURN_ZVAL(result, 1, 0);
}

/* Returns a map of write errors from the failed bulk write. */
static PHP_METHOD(MongoDB_Driver_Exception_BulkWriteCommandException, getWriteErrors)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	zval  rv;
	zval* result = zend_read_property(php_phongo_bulkwritecommandexception_ce, Z_OBJ_P(getThis()), ZEND_STRL("writeErrors"), 0, &rv);

	RETURN_ZVAL(result, 1, 0);
}

/* Returns a list of write concern errors from the failed bulk write. */
static PHP_METHOD(MongoDB_Driver_Exception_BulkWriteCommandException, getWriteConcernErrors)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	zval  rv;
	zval* result = zend_read_property(php_phongo_bulkwritecommandexception_ce, Z_OBJ_P(getThis()), ZEND_STRL("writeConcernErrors"), 0, &rv);

	RETURN_ZVAL(result, 1, 0);
}

void php_phongo_bulkwritecommandexception_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_bulkwritecommandexception_ce = register_class_MongoDB_Driver_Exception_BulkWriteCommandException(php_phongo_serverexception_ce);
}

/* Populates return_value with a list of WriteConcernError objects. Returns true
 * on success; otherwise, false is returned and an exception is thrown. */
static bool phongo_bulkwritecommandexception_get_writeconcernerrors(const bson_t* write_concern_errors, zval* return_value)
{
	bson_iter_t iter;

	array_init(return_value);

	if (bson_iter_init(&iter, write_concern_errors)) {
		while (bson_iter_next(&iter)) {
			bson_t         bson;
			uint32_t       len;
			const uint8_t* data;
			zval           write_concern_error;

			if (!BSON_ITER_HOLDS_DOCUMENT(&iter)) {
				continue;
			}

			bson_iter_document(&iter, &len, &data);

			if (!bson_init_static(&bson, data, len)) {
				continue;
			}

			if (!phongo_writeconcernerror_init(&write_concern_error, &bson)) {
				/* Exception already thrown */
				zval_ptr_dtor(&write_concern_error);
				return false;
			}

			add_next_index_zval(return_value, &write_concern_error);
		}
	}

	return true;
}

/* Populates return_value with a map of WriteError objects indexed by the offset
 * of the corresponding operation. Returns true on success; otherwise, false is
 * returned and an exception is thrown. */
static bool phongo_bulkwritecommandexception_get_writeerrors(const bson_t* write_errors, zval* return_value)
{
	bson_iter_t iter;

	array_init(return_value);

	if (bson_iter_init(&iter, write_errors)) {
		while (bson_iter_next(&iter)) {
			bson_t         bson;
			uint32_t       len;
			const uint8_t* data;
			zval           write_error;
			zend_ulong     index;

			if (!BSON_ITER_HOLDS_DOCUMENT(&iter)) {
				continue;
			}

			bson_iter_document(&iter, &len, &data);

			if (!bson_init_static(&bson, data, len)) {
				continue;
			}

			index = (zend_ulong) ZEND_STRTOUL(bson_iter_key(&iter), NULL, 10);

			if (!phongo_writeerror_init_ex(&write_error, &bson, (int32_t) index)) {
				/* Exception already thrown */
				zval_ptr_dtor(&write_error);
				return false;
			}

			add_index_zval(return_value, index, &write_error);
		}
	}

	return true;
}

void php_phongo_bulkwritecommandexception_init_props(zend_object* object, const mongoc_bulkwriteexception_t* bw_exc, zval* result)
{
	const bson_t* errorreply = mongoc_bulkwriteexception_errorreply(bw_exc);
	zval          zwriteconcernerrors, zwriteerrors;

	if (!bson_empty(errorreply)) {
		zval zerrorreply;

		/* Manually copy the bson_t to satisfy phongo_document_new. This can be
		 * changed once PHPC-2535 is addressed. */
		phongo_document_new(&zerrorreply, bson_copy(errorreply), false);
		zend_update_property(php_phongo_bulkwritecommandexception_ce, object, ZEND_STRL("errorReply"), &zerrorreply);
		zval_ptr_dtor(&zerrorreply);
	}

	if (result && Z_TYPE_P(result) == IS_OBJECT && instanceof_function(Z_OBJCE_P(result), php_phongo_bulkwritecommandresult_ce)) {
		zend_update_property(php_phongo_bulkwritecommandexception_ce, object, ZEND_STRL("partialResult"), result);
	}

	/* Note: get_writeconcernerrors and get_writeerrors could throw if BSON
	 * decoding fails, but that risk similarly exists for decoding a command
	 * result in phongo_throw_exception_from_bson_error_t_and_reply. */
	if (phongo_bulkwritecommandexception_get_writeconcernerrors(mongoc_bulkwriteexception_writeconcernerrors(bw_exc), &zwriteconcernerrors)) {
		zend_update_property(php_phongo_bulkwritecommandexception_ce, object, ZEND_STRL("writeConcernErrors"), &zwriteconcernerrors);
	}

	if (phongo_bulkwritecommandexception_get_writeerrors(mongoc_bulkwriteexception_writeerrors(bw_exc), &zwriteerrors)) {
		zend_update_property(php_phongo_bulkwritecommandexception_ce, object, ZEND_STRL("writeErrors"), &zwriteerrors);
	}

	zval_ptr_dtor(&zwriteconcernerrors);
	zval_ptr_dtor(&zwriteerrors);
}
