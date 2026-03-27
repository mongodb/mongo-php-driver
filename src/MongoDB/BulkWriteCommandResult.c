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
#include <Zend/zend_long.h>

#include "php_array_api.h"

#include "phongo.h"
#include "phongo_error.h"

#include "BSON/Document.h"
#include "MongoDB/WriteConcernError.h"
#include "MongoDB/WriteError.h"
#include "BulkWriteCommandResult_arginfo.h"

#define PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED(method)                                                                                                     \
	if (!intern->is_acknowledged) {                                                                                                                                  \
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "MongoDB\\Driver\\BulkWriteCommandResult::" method "() should not be called for an unacknowledged write result"); \
		return;                                                                                                                                                      \
	}

zend_class_entry* phongo_bulkwritecommandresult_ce;

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_Driver_BulkWriteCommandResult)

/* Returns the number of documents that were inserted */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getInsertedCount)
{
	PHONGO_INTERN_FROM_THIS(bulkwritecommandresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getInsertedCount");

	RETURN_LONG(intern->inserted_count);
}

/* Returns the number of documents that matched the update criteria */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getMatchedCount)
{
	PHONGO_INTERN_FROM_THIS(bulkwritecommandresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getMatchedCount");

	RETURN_LONG(intern->matched_count);
}

/* Returns the number of documents that were actually modified by an update */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getModifiedCount)
{
	PHONGO_INTERN_FROM_THIS(bulkwritecommandresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getModifiedCount");

	RETURN_LONG(intern->modified_count);
}

/* Returns the number of documents that were deleted */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getDeletedCount)
{
	PHONGO_INTERN_FROM_THIS(bulkwritecommandresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getDeletedCount");

	RETURN_LONG(intern->deleted_count);
}

/* Returns the number of documents that were upserted */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getUpsertedCount)
{
	PHONGO_INTERN_FROM_THIS(bulkwritecommandresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getUpsertedCount");

	RETURN_LONG(intern->upserted_count);
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getInsertResults)
{
	PHONGO_INTERN_FROM_THIS(bulkwritecommandresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getInsertResults");

	if (intern->insert_results) {
		phongo_document_new(return_value, intern->insert_results, true);
	}
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getUpdateResults)
{
	PHONGO_INTERN_FROM_THIS(bulkwritecommandresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getUpdateResults");

	if (intern->update_results) {
		phongo_document_new(return_value, intern->update_results, true);
	}
}

static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, getDeleteResults)
{
	PHONGO_INTERN_FROM_THIS(bulkwritecommandresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	PHONGO_BULKWRITECOMMANDRESULT_CHECK_ACKNOWLEDGED("getDeleteResults");

	if (intern->delete_results) {
		phongo_document_new(return_value, intern->delete_results, true);
	}
}

/* Returns whether the write operation was acknowledged (based on the write
   concern). */
static PHP_METHOD(MongoDB_Driver_BulkWriteCommandResult, isAcknowledged)
{
	PHONGO_INTERN_FROM_THIS(bulkwritecommandresult);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(intern->is_acknowledged);
}

/* MongoDB\Driver\BulkWriteCommandResult object handlers */
static zend_object_handlers phongo_handler_bulkwritecommandresult;

static void phongo_bulkwritecommandresult_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(bulkwritecommandresult, object);

	zend_object_std_dtor(&intern->std);

	bson_destroy(intern->insert_results);
	bson_destroy(intern->update_results);
	bson_destroy(intern->delete_results);
}

static zend_object* phongo_bulkwritecommandresult_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(bulkwritecommandresult, class_type);

	intern->std.handlers = &phongo_handler_bulkwritecommandresult;

	return &intern->std;
}

static HashTable* phongo_bulkwritecommandresult_get_debug_info(zend_object* object, int* is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(bulkwritecommandresult, object);

	zval retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	array_init_size(&retval, 12);

	ADD_ASSOC_BOOL_EX(&retval, "isAcknowledged", intern->is_acknowledged);
	ADD_ASSOC_LONG_EX(&retval, "insertedCount", intern->inserted_count);
	ADD_ASSOC_LONG_EX(&retval, "matchedCount", intern->matched_count);
	ADD_ASSOC_LONG_EX(&retval, "modifiedCount", intern->modified_count);
	ADD_ASSOC_LONG_EX(&retval, "upsertedCount", intern->upserted_count);
	ADD_ASSOC_LONG_EX(&retval, "deletedCount", intern->deleted_count);

	if (intern->insert_results) {
		zval insert_results;

		phongo_document_new(&insert_results, intern->insert_results, true);
		ADD_ASSOC_ZVAL_EX(&retval, "insertResults", &insert_results);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "insertResults");
	}

	if (intern->update_results) {
		zval update_results;

		phongo_document_new(&update_results, intern->update_results, true);
		ADD_ASSOC_ZVAL_EX(&retval, "updateResults", &update_results);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "updateResults");
	}

	if (intern->delete_results) {
		zval delete_results;

		phongo_document_new(&delete_results, intern->delete_results, true);
		ADD_ASSOC_ZVAL_EX(&retval, "deleteResults", &delete_results);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "deleteResults");
	}

	return Z_ARRVAL(retval);
}

void phongo_bulkwritecommandresult_init_ce(INIT_FUNC_ARGS)
{
	phongo_bulkwritecommandresult_ce                = register_class_MongoDB_Driver_BulkWriteCommandResult();
	phongo_bulkwritecommandresult_ce->create_object = phongo_bulkwritecommandresult_create_object;

	memcpy(&phongo_handler_bulkwritecommandresult, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_bulkwritecommandresult.get_debug_info = phongo_bulkwritecommandresult_get_debug_info;
	phongo_handler_bulkwritecommandresult.free_obj       = phongo_bulkwritecommandresult_free_object;
	phongo_handler_bulkwritecommandresult.offset         = XtOffsetOf(phongo_bulkwritecommandresult_t, std);
}

static inline bson_t* _bson_copy_or_null(const bson_t* bson)
{
	return bson ? bson_copy(bson) : NULL;
}

phongo_bulkwritecommandresult_t* phongo_bulkwritecommandresult_init(zval* return_value, mongoc_bulkwriteresult_t* bw_res)
{
	PHONGO_INTERN_INIT_EX(bulkwritecommandresult, return_value);
	intern->is_acknowledged = (bw_res != NULL);

	// Copy mongoc_bulkwriteresult_t fields
	if (bw_res) {
		intern->inserted_count = mongoc_bulkwriteresult_insertedcount(bw_res);
		intern->upserted_count = mongoc_bulkwriteresult_upsertedcount(bw_res);
		intern->matched_count  = mongoc_bulkwriteresult_matchedcount(bw_res);
		intern->modified_count = mongoc_bulkwriteresult_modifiedcount(bw_res);
		intern->deleted_count  = mongoc_bulkwriteresult_deletedcount(bw_res);

		// Result documents will null if verboseResults=false
		intern->insert_results = _bson_copy_or_null(mongoc_bulkwriteresult_insertresults(bw_res));
		intern->update_results = _bson_copy_or_null(mongoc_bulkwriteresult_updateresults(bw_res));
		intern->delete_results = _bson_copy_or_null(mongoc_bulkwriteresult_deleteresults(bw_res));
	}

	return intern;
}
