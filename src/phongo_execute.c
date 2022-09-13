/*
 * Copyright 2022-present MongoDB, Inc.
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
#include <Zend/zend_exceptions.h>

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_execute.h"
#include "phongo_util.h"

#include "MongoDB/Cursor.h"
#include "MongoDB/ReadPreference.h"
#include "MongoDB/Session.h"
#include "MongoDB/WriteResult.h"

static bson_t* create_wrapped_command_envelope(const char* db, bson_t* reply)
{
	bson_t* tmp;
	size_t  max_ns_len = strlen(db) + 5 + 1; /* db + ".$cmd" + '\0' */
	char*   ns         = emalloc(max_ns_len);

	snprintf(ns, max_ns_len, "%s.$cmd", db);
	tmp = BCON_NEW("cursor", "{", "id", BCON_INT64(0), "ns", BCON_UTF8(ns), "firstBatch", "[", BCON_DOCUMENT(reply), "]", "}");
	efree(ns);

	return tmp;
}

static zval* phongo_create_implicit_session(zval* manager)
{
	mongoc_client_session_t* cs;
	zval*                    zsession;

	cs = mongoc_client_start_session(Z_MANAGER_OBJ_P(manager)->client, NULL, NULL);

	if (!cs) {
		return NULL;
	}

	zsession = ecalloc(1, sizeof(zval));

	phongo_session_init(zsession, manager, cs);

	return zsession;
}

/* Parses the "readConcern" option for an execute method. If mongoc_opts is not
 * NULL, the option will be appended. On error, false is returned and an
 * exception is thrown. */
static bool phongo_parse_read_concern(zval* options, bson_t* mongoc_opts)
{
	zval*                  option = NULL;
	mongoc_read_concern_t* read_concern;

	if (!options) {
		return true;
	}

	if (Z_TYPE_P(options) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected options to be array, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(options));
		return false;
	}

	option = php_array_fetchc(options, "readConcern");

	if (!option) {
		return true;
	}

	if (Z_TYPE_P(option) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(option), php_phongo_readconcern_ce)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"readConcern\" option to be %s, %s given", ZSTR_VAL(php_phongo_readconcern_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(option));
		return false;
	}

	read_concern = Z_READCONCERN_OBJ_P(option)->read_concern;

	if (mongoc_opts && !mongoc_read_concern_append(read_concern, mongoc_opts)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"readConcern\" option");
		return false;
	}

	return true;
}

/* Parses the "readPreference" option for an execute method. If zreadPreference
 * is not NULL, it will be assigned to the option. On error, false is returned
 * and an exception is thrown. */
bool phongo_parse_read_preference(zval* options, zval** zreadPreference)
{
	zval* option = NULL;

	if (!options) {
		return true;
	}

	if (Z_TYPE_P(options) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected options to be array, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(options));
		return false;
	}

	option = php_array_fetchc(options, "readPreference");

	if (!option) {
		return true;
	}

	if (Z_TYPE_P(option) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(option), php_phongo_readpreference_ce)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"readPreference\" option to be %s, %s given", ZSTR_VAL(php_phongo_readpreference_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(option));
		return false;
	}

	if (zreadPreference) {
		*zreadPreference = option;
	}

	return true;
}

/* Parses the "session" option for an execute method. The client object should
 * correspond to the Manager executing the operation and will be used to ensure
 * that the session is correctly associated with that client. If mongoc_opts is
 * not NULL, the option will be appended. If zsession is not NULL, it will be
 * assigned to the option. On error, false is returned and an exception is
 * thrown. */
bool phongo_parse_session(zval* options, mongoc_client_t* client, bson_t* mongoc_opts, zval** zsession)
{
	zval*                          option = NULL;
	const mongoc_client_session_t* client_session;

	if (!options) {
		return true;
	}

	if (Z_TYPE_P(options) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected options to be array, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(options));
		return false;
	}

	option = php_array_fetchc(options, "session");

	if (!option) {
		return true;
	}

	if (Z_TYPE_P(option) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(option), php_phongo_session_ce)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"session\" option to be %s, %s given", ZSTR_VAL(php_phongo_session_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(option));
		return false;
	}

	client_session = Z_SESSION_OBJ_P(option)->client_session;

	if (client != mongoc_client_session_get_client(client_session)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Cannot use Session started from a different Manager");
		return false;
	}

	if (mongoc_opts && !mongoc_client_session_append(client_session, mongoc_opts, NULL)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"session\" option");
		return false;
	}

	if (zsession) {
		*zsession = option;
	}

	return true;
}

/* Parses the "writeConcern" option for an execute method. If mongoc_opts is not
 * NULL, the option will be appended. If zwriteConcern is not NULL, it will be
 * assigned to the option. On error, false is returned and an exception is
 * thrown. */
static bool phongo_parse_write_concern(zval* options, bson_t* mongoc_opts, zval** zwriteConcern)
{
	zval*                   option = NULL;
	mongoc_write_concern_t* write_concern;

	if (!options) {
		return true;
	}

	if (Z_TYPE_P(options) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected options to be array, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(options));
		return false;
	}

	option = php_array_fetchc(options, "writeConcern");

	if (!option) {
		return true;
	}

	if (Z_TYPE_P(option) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(option), php_phongo_writeconcern_ce)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"writeConcern\" option to be %s, %s given", ZSTR_VAL(php_phongo_writeconcern_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(option));
		return false;
	}

	write_concern = Z_WRITECONCERN_OBJ_P(option)->write_concern;

	if (mongoc_opts && !mongoc_write_concern_append(write_concern, mongoc_opts)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"writeConcern\" option");
		return false;
	}

	if (zwriteConcern) {
		*zwriteConcern = option;
	}

	return true;
}

bool phongo_execute_bulk_write(zval* manager, const char* namespace, php_phongo_bulkwrite_t* bulk_write, zval* options, uint32_t server_id, zval* return_value)
{
	mongoc_client_t*              client = NULL;
	bson_error_t                  error  = { 0 };
	int                           success;
	bson_t                        reply = BSON_INITIALIZER;
	mongoc_bulk_operation_t*      bulk  = bulk_write->bulk;
	php_phongo_writeresult_t*     writeresult;
	zval*                         zwriteConcern = NULL;
	zval*                         zsession      = NULL;
	const mongoc_write_concern_t* write_concern = NULL;

	client = Z_MANAGER_OBJ_P(manager)->client;

	if (bulk_write->executed) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "BulkWrite objects may only be executed once and this instance has already been executed");
		return false;
	}

	if (!phongo_split_namespace(namespace, &bulk_write->database, &bulk_write->collection)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s: %s", "Invalid namespace provided", namespace);
		return false;
	}

	if (!phongo_parse_session(options, client, NULL, &zsession)) {
		/* Exception should already have been thrown */
		return false;
	}

	if (!phongo_parse_write_concern(options, NULL, &zwriteConcern)) {
		/* Exception should already have been thrown */
		return false;
	}

	/* If a write concern was not specified, libmongoc will use the client's
	 * write concern; however, we should still fetch it for the write result.
	 * Additionally, we need to check if an unacknowledged write concern would
	 * conflict with an explicit session. */
	write_concern = zwriteConcern ? Z_WRITECONCERN_OBJ_P(zwriteConcern)->write_concern : mongoc_client_get_write_concern(client);

	if (zsession && !mongoc_write_concern_is_acknowledged(write_concern)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Cannot combine \"session\" option with an unacknowledged write concern");
		return false;
	}

	mongoc_bulk_operation_set_database(bulk, bulk_write->database);
	mongoc_bulk_operation_set_collection(bulk, bulk_write->collection);
	mongoc_bulk_operation_set_client(bulk, client);
	mongoc_bulk_operation_set_hint(bulk, server_id);

	if (zsession) {
		ZVAL_ZVAL(&bulk_write->session, zsession, 1, 0);
		mongoc_bulk_operation_set_client_session(bulk, Z_SESSION_OBJ_P(zsession)->client_session);
	}

	if (zwriteConcern) {
		mongoc_bulk_operation_set_write_concern(bulk, Z_WRITECONCERN_OBJ_P(zwriteConcern)->write_concern);
	}

	success              = mongoc_bulk_operation_execute(bulk, &reply, &error);
	bulk_write->executed = true;

	writeresult                = phongo_writeresult_init(return_value, &reply, manager, mongoc_bulk_operation_get_hint(bulk));
	writeresult->write_concern = mongoc_write_concern_copy(write_concern);

	/* A BulkWriteException is always thrown if mongoc_bulk_operation_execute()
	 * fails to ensure that the write result is accessible. If the error does
	 * not originate from the server (e.g. socket error), throw the appropriate
	 * exception first. It will be included in BulkWriteException's message and
	 * will also be accessible via Exception::getPrevious(). */
	if (!success) {
		if (error.domain != MONGOC_ERROR_SERVER && error.domain != MONGOC_ERROR_WRITE_CONCERN) {
			phongo_throw_exception_from_bson_error_t_and_reply(&error, &reply);
		}

		/* Argument errors occur before command execution, so there is no need
		 * to layer this InvalidArgumentException behind a BulkWriteException.
		 * In practice, this will be a "Cannot do an empty bulk write" error. */
		if (error.domain == MONGOC_ERROR_COMMAND && error.code == MONGOC_ERROR_COMMAND_INVALID_ARG) {
			goto cleanup;
		}

		if (EG(exception)) {
			char* message;

			(void) spprintf(&message, 0, "Bulk write failed due to previous %s: %s", PHONGO_ZVAL_EXCEPTION_NAME(EG(exception)), error.message);
			zend_throw_exception(php_phongo_bulkwriteexception_ce, message, 0);
			efree(message);
		} else {
			zend_throw_exception(php_phongo_bulkwriteexception_ce, error.message, error.code);
		}

		/* Ensure error labels are added to the final BulkWriteException. If a
		 * previous exception was also thrown, error labels will already have
		 * been added by phongo_throw_exception_from_bson_error_t_and_reply. */
		phongo_exception_add_error_labels(&reply);
		phongo_add_exception_prop(ZEND_STRL("writeResult"), return_value);
	}

cleanup:
	bson_destroy(&reply);

	return success;
}

bool phongo_execute_command(zval* manager, php_phongo_command_type_t type, const char* db, zval* zcommand, zval* options, uint32_t server_id, zval* return_value)
{
	mongoc_client_t*            client;
	const php_phongo_command_t* command;
	bson_iter_t                 iter;
	bson_t                      reply;
	bson_error_t                error = { 0 };
	bson_t                      opts  = BSON_INITIALIZER;
	mongoc_cursor_t*            cmd_cursor;
	zval*                       zreadPreference                 = NULL;
	zval*                       zsession                        = NULL;
	bool                        result                          = false;
	bool                        free_reply                      = false;
	bool                        free_zsession                   = false;
	bool                        is_unacknowledged_write_concern = false;

	client  = Z_MANAGER_OBJ_P(manager)->client;
	command = Z_COMMAND_OBJ_P(zcommand);

	if ((type & PHONGO_OPTION_READ_CONCERN) && !phongo_parse_read_concern(options, &opts)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if ((type & PHONGO_OPTION_READ_PREFERENCE) && !phongo_parse_read_preference(options, &zreadPreference)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!phongo_parse_session(options, client, &opts, &zsession)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (type & PHONGO_OPTION_WRITE_CONCERN) {
		zval* zwriteConcern = NULL;

		if (!phongo_parse_write_concern(options, &opts, &zwriteConcern)) {
			/* Exception should already have been thrown */
			goto cleanup;
		}

		/* Determine if the explicit or inherited write concern is
		 * unacknowledged so that we can ensure it does not conflict with an
		 * explicit or implicit session. */
		if (zwriteConcern) {
			is_unacknowledged_write_concern = !mongoc_write_concern_is_acknowledged(Z_WRITECONCERN_OBJ_P(zwriteConcern)->write_concern);
		} else if (type != PHONGO_COMMAND_RAW) {
			is_unacknowledged_write_concern = !mongoc_write_concern_is_acknowledged(mongoc_client_get_write_concern(client));
		}
	}

	if (zsession && is_unacknowledged_write_concern) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Cannot combine \"session\" option with an unacknowledged write concern");
		goto cleanup;
	}

	/* If an explicit session was not provided and the effective write concern
	 * is not unacknowledged, attempt to create an implicit client session
	 * (ignoring any errors). */
	if (!zsession && !is_unacknowledged_write_concern) {
		zsession = phongo_create_implicit_session(manager);

		if (zsession) {
			free_zsession = true;

			if (!mongoc_client_session_append(Z_SESSION_OBJ_P(zsession)->client_session, &opts, NULL)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending implicit \"sessionId\" option");
				goto cleanup;
			}
		}
	}

	if (!BSON_APPEND_INT32(&opts, "serverId", server_id)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"serverId\" option");
		goto cleanup;
	}

	/* Although "opts" already always includes the serverId option, the read
	 * preference is added to the command parts, which is relevant for mongos
	 * command construction. */
	switch (type) {
		case PHONGO_COMMAND_RAW:
			result = mongoc_client_command_with_opts(client, db, command->bson, phongo_read_preference_from_zval(zreadPreference), &opts, &reply, &error);
			break;
		case PHONGO_COMMAND_READ:
			result = mongoc_client_read_command_with_opts(client, db, command->bson, phongo_read_preference_from_zval(zreadPreference), &opts, &reply, &error);
			break;
		case PHONGO_COMMAND_WRITE:
			result = mongoc_client_write_command_with_opts(client, db, command->bson, &opts, &reply, &error);
			break;
		case PHONGO_COMMAND_READ_WRITE:
			/* We can pass NULL as readPreference, as this argument was added historically, but has no function */
			result = mongoc_client_read_write_command_with_opts(client, db, command->bson, NULL, &opts, &reply, &error);
			break;
		default:
			/* Should never happen, but if it does: exception */
			phongo_throw_exception(PHONGO_ERROR_LOGIC, "Type '%d' should never have been passed to phongo_execute_command, please file a bug report", type);
			goto cleanup;
	}

	free_reply = true;

	if (!result) {
		phongo_throw_exception_from_bson_error_t_and_reply(&error, &reply);
		goto cleanup;
	}

	/* According to mongoc_cursor_new_from_command_reply_with_opts(), the reply
	 * bson_t is ultimately destroyed on both success and failure. */
	if (bson_iter_init_find(&iter, &reply, "cursor") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
		bson_t       initial_reply = BSON_INITIALIZER;
		bson_t       cursor_opts   = BSON_INITIALIZER;
		bson_iter_t  iter;
		bson_error_t error = { 0 };

		bson_copy_to(&reply, &initial_reply);

		bson_append_int32(&cursor_opts, "serverId", -1, server_id);

		if (command->max_await_time_ms) {
			bson_append_bool(&cursor_opts, "awaitData", -1, 1);
			bson_append_int64(&cursor_opts, "maxAwaitTimeMS", -1, command->max_await_time_ms);
			bson_append_bool(&cursor_opts, "tailable", -1, 1);
		}

		if (command->batch_size) {
			bson_append_int64(&cursor_opts, "batchSize", -1, command->batch_size);
		}

		if (bson_iter_init(&iter, command->bson) && bson_iter_find(&iter, "comment")) {
			bson_append_value(&cursor_opts, "comment", -1, bson_iter_value(&iter));
		}

		if (zsession && !mongoc_client_session_append(Z_SESSION_OBJ_P(zsession)->client_session, &cursor_opts, &error)) {
			phongo_throw_exception_from_bson_error_t(&error);
			bson_destroy(&initial_reply);
			bson_destroy(&cursor_opts);
			result = false;
			goto cleanup;
		}

		cmd_cursor = mongoc_cursor_new_from_command_reply_with_opts(client, &initial_reply, &cursor_opts);
		bson_destroy(&cursor_opts);
	} else {
		bson_t  cursor_opts   = BSON_INITIALIZER;
		bson_t* wrapped_reply = create_wrapped_command_envelope(db, &reply);

		bson_append_int32(&cursor_opts, "serverId", -1, server_id);
		cmd_cursor = mongoc_cursor_new_from_command_reply_with_opts(client, wrapped_reply, &cursor_opts);
		bson_destroy(&cursor_opts);
	}

	phongo_cursor_init_for_command(return_value, manager, cmd_cursor, db, zcommand, zreadPreference, zsession);

cleanup:
	bson_destroy(&opts);

	if (free_reply) {
		bson_destroy(&reply);
	}

	if (free_zsession) {
		zval_ptr_dtor(zsession);
		efree(zsession);
	}

	return result;
}

bool phongo_execute_query(zval* manager, const char* namespace, zval* zquery, zval* options, uint32_t server_id, zval* return_value)
{
	mongoc_client_t*          client;
	const php_phongo_query_t* query;
	bson_t                    opts = BSON_INITIALIZER;
	mongoc_cursor_t*          cursor;
	char*                     dbname;
	char*                     collname;
	mongoc_collection_t*      collection;
	zval*                     zreadPreference = NULL;
	zval*                     zsession        = NULL;

	client = Z_MANAGER_OBJ_P(manager)->client;

	if (!phongo_split_namespace(namespace, &dbname, &collname)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s: %s", "Invalid namespace provided", namespace);
		return false;
	}
	collection = mongoc_client_get_collection(client, dbname, collname);
	efree(dbname);
	efree(collname);

	query = Z_QUERY_OBJ_P(zquery);

	bson_copy_to(query->opts, &opts);

	if (query->read_concern) {
		mongoc_collection_set_read_concern(collection, query->read_concern);
	}

	if (!phongo_parse_read_preference(options, &zreadPreference)) {
		/* Exception should already have been thrown */
		mongoc_collection_destroy(collection);
		bson_destroy(&opts);
		return false;
	}

	if (!phongo_parse_session(options, client, &opts, &zsession)) {
		/* Exception should already have been thrown */
		mongoc_collection_destroy(collection);
		bson_destroy(&opts);
		return false;
	}

	if (!BSON_APPEND_INT32(&opts, "serverId", server_id)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error appending \"serverId\" option");
		mongoc_collection_destroy(collection);
		bson_destroy(&opts);
		return false;
	}

	cursor = mongoc_collection_find_with_opts(collection, query->filter, &opts, phongo_read_preference_from_zval(zreadPreference));
	mongoc_collection_destroy(collection);
	bson_destroy(&opts);

	/* maxAwaitTimeMS must be set before the cursor is sent */
	if (query->max_await_time_ms) {
		mongoc_cursor_set_max_await_time_ms(cursor, query->max_await_time_ms);
	}

	/* Initialize the cursor and advance it once */
	if (!phongo_cursor_init_for_query(return_value, manager, cursor, namespace, zquery, zreadPreference, zsession)) {
		/* Exception should already have been thrown */
		mongoc_cursor_destroy(cursor);
		return false;
	}

	return true;
}
