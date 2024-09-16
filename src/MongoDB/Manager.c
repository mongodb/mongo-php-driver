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
#include <ext/standard/file.h>
#include <Zend/zend_interfaces.h>

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_apm.h"
#include "phongo_client.h"
#include "phongo_error.h"
#include "phongo_execute.h"
#include "phongo_util.h"

#include "MongoDB/ClientEncryption.h"
#include "MongoDB/ReadConcern.h"
#include "MongoDB/ReadPreference.h"
#include "MongoDB/Server.h"
#include "MongoDB/Session.h"
#include "MongoDB/WriteConcern.h"
#include "Manager_arginfo.h"

#define PHONGO_MANAGER_URI_DEFAULT "mongodb://127.0.0.1/"

/**
 * Manager abstracts a cluster of Server objects (i.e. socket connections).
 *
 * Typically, users will connect to a cluster using a URI, and the Manager will
 * perform tasks such as replica set discovery and create the necessary Server
 * objects. That said, it is also possible to create a Manager with an arbitrary
 * collection of Server objects using the static factory method (this can be
 * useful for testing or administration).
 *
 * Operation methods do not take socket-level options (e.g. socketTimeoutMS).
 * Those options should be specified during construction.
 */
zend_class_entry* php_phongo_manager_ce;

/* Checks if driverOptions contains a stream context resource in the "context"
 * key and incorporates any of its SSL options into the base array that did not
 * already exist (i.e. array union). The "context" key is then unset from the
 * base array.
 *
 * This handles the merging of any legacy SSL context options and also makes
 * driverOptions suitable for serialization by removing the resource zval. */
static bool php_phongo_manager_merge_context_options(zval* zdriverOptions)
{
	php_stream_context* context;
	zval *              zcontext, *zcontextOptions;

	if (!php_array_existsc(zdriverOptions, "context")) {
		return true;
	}

	zcontext = php_array_fetchc(zdriverOptions, "context");
	context  = php_stream_context_from_zval(zcontext, 1);

	if (!context) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "\"context\" driver option is not a valid Stream-Context resource");
		return false;
	}

	zcontextOptions = php_array_fetchc_array(&context->options, "ssl");

	if (!zcontextOptions) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Stream-Context resource does not contain \"ssl\" options array");
		return false;
	}

	/* When running PHP in debug mode, php_error_docref duplicates the current
	 * scope, leading to a COW violation in zend_hash_merge and
	 * zend_symtable_str_del (called by php_array_unsetc). This macro allows
	 * that violation in debug mode and is a NOOP when in non-debug. */
	HT_ALLOW_COW_VIOLATION(Z_ARRVAL_P(zdriverOptions));

	php_error_docref(NULL, E_DEPRECATED, "The \"context\" driver option is deprecated.");

	/* Perform array union (see: add_function() in zend_operators.c) */
	zend_hash_merge(Z_ARRVAL_P(zdriverOptions), Z_ARRVAL_P(zcontextOptions), zval_add_ref, 0);

	php_array_unsetc(zdriverOptions, "context");

	return true;
}

/* Prepare authMechanismProperties for BSON encoding by converting a boolean
 * value for the "CANONICALIZE_HOST_NAME" option to a string.
 *
 * Note: URI options are case-insensitive, so we must iterate through the
 * HashTable in order to detect options. */
static void php_phongo_manager_prep_authmechanismproperties(zval* properties)
{
	HashTable* ht_data;

	if (Z_TYPE_P(properties) != IS_ARRAY && Z_TYPE_P(properties) != IS_OBJECT) {
		return;
	}

	ht_data = HASH_OF(properties);

	{
		zend_string* string_key = NULL;
		zend_ulong   num_key    = 0;
		zval*        property;

		ZEND_HASH_FOREACH_KEY_VAL_IND(ht_data, num_key, string_key, property)
		{
			if (!string_key) {
				continue;
			}

			/* URI options are case-insensitive */
			if (!strcasecmp(ZSTR_VAL(string_key), "CANONICALIZE_HOST_NAME")) {
				ZVAL_DEREF(property);
				if (Z_TYPE_P(property) != IS_STRING && zend_is_true(property)) {
					SEPARATE_ZVAL_NOREF(property);
					ZVAL_NEW_STR(property, zend_string_init(ZEND_STRL("true"), 0));
				}
			}
		}
		ZEND_HASH_FOREACH_END();
	}
}

/* Prepare URI options for BSON encoding.
 *
 * Read preference tag sets must be an array of documents. In order to ensure
 * that empty arrays serialize as empty documents, array elements will be
 * converted to objects. php_phongo_read_preference_tags_are_valid() handles
 * actual validation of the tag set structure.
 *
 * Auth mechanism properties must have string values, so a boolean true value
 * for the "CANONICALIZE_HOST_NAME" property will be converted to "true".
 *
 * Note: URI options are case-insensitive, so we must iterate through the
 * HashTable in order to detect options. */
static void php_phongo_manager_prep_uri_options(zval* options)
{
	HashTable* ht_data;

	if (Z_TYPE_P(options) != IS_ARRAY) {
		return;
	}

	ht_data = HASH_OF(options);

	{
		zend_string* string_key = NULL;
		zend_ulong   num_key    = 0;
		zval*        option;

		ZEND_HASH_FOREACH_KEY_VAL_IND(ht_data, num_key, string_key, option)
		{
			if (!string_key) {
				continue;
			}

			if (!strcasecmp(ZSTR_VAL(string_key), MONGOC_URI_READPREFERENCETAGS)) {
				ZVAL_DEREF(option);
				SEPARATE_ZVAL_NOREF(option);
				php_phongo_read_preference_prep_tagsets(option);
				continue;
			}

			if (!strcasecmp(ZSTR_VAL(string_key), MONGOC_URI_AUTHMECHANISMPROPERTIES)) {
				ZVAL_DEREF(option);
				SEPARATE_ZVAL_NOREF(option);
				php_phongo_manager_prep_authmechanismproperties(option);
				continue;
			}
		}
		ZEND_HASH_FOREACH_END();
	}
}

/* Selects a server for an execute method. If "for_writes" is true, a primary
 * will be selected. Otherwise, a read preference will be used to select the
 * server. If zreadPreference is NULL, the client's read preference will be
 * used. If zsession is a session object in a sharded transaction, the session
 * will be checked whether it is pinned to a server. If so, that server will be
 * selected. Otherwise, server selection
 *
 * On success, server_id will be set and the function will return true;
 * otherwise, false is returned and an exception is thrown. */
static bool php_phongo_manager_select_server(bool for_writes, bool inherit_read_preference, zval* zreadPreference, zval* zsession, mongoc_client_t* client, uint32_t* server_id)
{
	mongoc_server_description_t* selected_server;
	const mongoc_read_prefs_t*   read_preference = NULL;
	bson_error_t                 error           = { 0 };

	if (zsession) {
		const mongoc_client_session_t* session = Z_SESSION_OBJ_P(zsession)->client_session;

		/* Attempt to fetch server pinned to session */
		if (mongoc_client_session_get_server_id(session) > 0) {
			*server_id = mongoc_client_session_get_server_id(session);

			return true;
		}
	}

	if (!for_writes) {
		if (zreadPreference) {
			read_preference = phongo_read_preference_from_zval(zreadPreference);
		} else if (inherit_read_preference) {
			read_preference = mongoc_client_get_read_prefs(client);
		}
	}

	selected_server = mongoc_client_select_server(client, for_writes, read_preference, &error);

	if (selected_server) {
		*server_id = mongoc_server_description_id(selected_server);
		mongoc_server_description_destroy(selected_server);

		return true;
	}

	/* Check for connection related exceptions */
	if (!EG(exception)) {
		phongo_throw_exception_from_bson_error_t(&error);
	}

	return false;
}

/* Constructs a new Manager */
static PHP_METHOD(MongoDB_Driver_Manager, __construct)
{
	php_phongo_manager_t* intern;
	char*                 uri_string     = NULL;
	size_t                uri_string_len = 0;
	zval*                 options        = NULL;
	zval*                 driverOptions  = NULL;

	intern = Z_MANAGER_OBJ_P(getThis());

	/* Separate the options and driverOptions zvals, since we may end up
	 * modifying them in php_phongo_manager_prep_uri_options() and
	 * php_phongo_manager_merge_context_options() below, respectively. */
	PHONGO_PARSE_PARAMETERS_START(0, 3)
	Z_PARAM_OPTIONAL
	Z_PARAM_STRING_OR_NULL(uri_string, uri_string_len)
	Z_PARAM_ARRAY_EX(options, 1, 1)
	Z_PARAM_ARRAY_EX(driverOptions, 1, 1)
	PHONGO_PARSE_PARAMETERS_END();

	if (options) {
		php_phongo_manager_prep_uri_options(options);
	}

	if (driverOptions && !php_phongo_manager_merge_context_options(driverOptions)) {
		/* Exception should already have been thrown */
		return;
	}

	phongo_manager_init(intern, uri_string ? uri_string : PHONGO_MANAGER_URI_DEFAULT, options, driverOptions);

	if (EG(exception)) {
		return;
	}

	/* Update the request-scoped Manager registry */
	if (!php_phongo_manager_register(intern)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Failed to add Manager to internal registry");
	}
}

/* Registers an event subscriber for this Manager */
static PHP_METHOD(MongoDB_Driver_Manager, addSubscriber)
{
	php_phongo_manager_t* intern;
	zval*                 subscriber;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(subscriber, php_phongo_subscriber_ce)
	PHONGO_PARSE_PARAMETERS_END();

	if (instanceof_function(Z_OBJCE_P(subscriber), php_phongo_logsubscriber_ce)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "LogSubscriber instances cannot be registered with a Manager");
	}

	intern = Z_MANAGER_OBJ_P(getThis());

	/* Lazily initialize the subscriber HashTable */
	if (!intern->subscribers) {
		ALLOC_HASHTABLE(intern->subscribers);
		zend_hash_init(intern->subscribers, 0, NULL, ZVAL_PTR_DTOR, 0);
	}

	phongo_apm_add_subscriber(intern->subscribers, subscriber);
}

/* Return a ClientEncryption instance */
static PHP_METHOD(MongoDB_Driver_Manager, createClientEncryption)
{
	zval* options;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(options)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_clientencryption_ce);

	/* An exception will be thrown on error. */
	phongo_clientencryption_init(Z_CLIENTENCRYPTION_OBJ_P(return_value), options, getThis());
}

/* Execute a Command */
static PHP_METHOD(MongoDB_Driver_Manager, executeCommand)
{
	php_phongo_manager_t* intern;
	char*                 db;
	size_t                db_len;
	zval*                 command;
	zval*                 options         = NULL;
	bool                  free_options    = false;
	zval*                 zreadPreference = NULL;
	zval*                 zsession        = NULL;
	uint32_t              server_id       = 0;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING_OR_NULL(db, db_len)
	Z_PARAM_OBJECT_OF_CLASS(command, php_phongo_command_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ZVAL_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_MANAGER_OBJ_P(getThis());

	options = php_phongo_prep_legacy_option(options, "readPreference", &free_options);

	if (!phongo_parse_session(options, intern->client, NULL, &zsession)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!phongo_parse_read_preference(options, &zreadPreference)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!php_phongo_manager_select_server(false, false, zreadPreference, zsession, intern->client, &server_id)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, intern);

	phongo_execute_command(getThis(), PHONGO_COMMAND_RAW, db, command, options, server_id, return_value);

cleanup:
	if (free_options) {
		php_phongo_prep_legacy_option_free(options);
	}
}

/* Execute a ReadCommand */
static PHP_METHOD(MongoDB_Driver_Manager, executeReadCommand)
{
	php_phongo_manager_t* intern;
	char*                 db;
	size_t                db_len;
	zval*                 command;
	zval*                 options         = NULL;
	zval*                 zreadPreference = NULL;
	uint32_t              server_id       = 0;
	zval*                 zsession        = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING_OR_NULL(db, db_len)
	Z_PARAM_OBJECT_OF_CLASS(command, php_phongo_command_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_MANAGER_OBJ_P(getThis());

	if (!phongo_parse_session(options, intern->client, NULL, &zsession)) {
		/* Exception should already have been thrown */
		return;
	}

	if (!phongo_parse_read_preference(options, &zreadPreference)) {
		/* Exception should already have been thrown */
		return;
	}

	if (!php_phongo_manager_select_server(false, true, zreadPreference, zsession, intern->client, &server_id)) {
		/* Exception should already have been thrown */
		return;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, intern);

	phongo_execute_command(getThis(), PHONGO_COMMAND_READ, db, command, options, server_id, return_value);
}

/* Execute a WriteCommand */
static PHP_METHOD(MongoDB_Driver_Manager, executeWriteCommand)
{
	php_phongo_manager_t* intern;
	char*                 db;
	size_t                db_len;
	zval*                 command;
	zval*                 options   = NULL;
	uint32_t              server_id = 0;
	zval*                 zsession  = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING_OR_NULL(db, db_len)
	Z_PARAM_OBJECT_OF_CLASS(command, php_phongo_command_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_MANAGER_OBJ_P(getThis());

	if (!phongo_parse_session(options, intern->client, NULL, &zsession)) {
		/* Exception should already have been thrown */
		return;
	}

	if (!php_phongo_manager_select_server(true, false, NULL, zsession, intern->client, &server_id)) {
		/* Exception should already have been thrown */
		return;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, intern);

	phongo_execute_command(getThis(), PHONGO_COMMAND_WRITE, db, command, options, server_id, return_value);
}

/* Execute a ReadWriteCommand */
static PHP_METHOD(MongoDB_Driver_Manager, executeReadWriteCommand)
{
	php_phongo_manager_t* intern;
	char*                 db;
	size_t                db_len;
	zval*                 command;
	zval*                 options   = NULL;
	uint32_t              server_id = 0;
	zval*                 zsession  = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING_OR_NULL(db, db_len)
	Z_PARAM_OBJECT_OF_CLASS(command, php_phongo_command_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_MANAGER_OBJ_P(getThis());

	if (!phongo_parse_session(options, intern->client, NULL, &zsession)) {
		/* Exception should already have been thrown */
		return;
	}

	if (!php_phongo_manager_select_server(true, false, NULL, zsession, intern->client, &server_id)) {
		/* Exception should already have been thrown */
		return;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, intern);

	phongo_execute_command(getThis(), PHONGO_COMMAND_READ_WRITE, db, command, options, server_id, return_value);
}

/* Execute a Query */
static PHP_METHOD(MongoDB_Driver_Manager, executeQuery)
{
	php_phongo_manager_t* intern;
	char* namespace;
	size_t   namespace_len;
	zval*    query;
	zval*    options         = NULL;
	bool     free_options    = false;
	zval*    zreadPreference = NULL;
	uint32_t server_id       = 0;
	zval*    zsession        = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING_OR_NULL(namespace, namespace_len)
	Z_PARAM_OBJECT_OF_CLASS(query, php_phongo_query_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ZVAL_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_MANAGER_OBJ_P(getThis());

	options = php_phongo_prep_legacy_option(options, "readPreference", &free_options);

	if (!phongo_parse_session(options, intern->client, NULL, &zsession)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!phongo_parse_read_preference(options, &zreadPreference)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!php_phongo_manager_select_server(false, true, zreadPreference, zsession, intern->client, &server_id)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, intern);

	phongo_execute_query(getThis(), namespace, query, options, server_id, return_value);

cleanup:
	if (free_options) {
		php_phongo_prep_legacy_option_free(options);
	}
}

/* Executes a BulkWrite (i.e. any number of insert, update, and delete ops) */
static PHP_METHOD(MongoDB_Driver_Manager, executeBulkWrite)
{
	php_phongo_manager_t* intern;
	char* namespace;
	size_t                  namespace_len;
	zval*                   zbulk;
	php_phongo_bulkwrite_t* bulk;
	zval*                   options      = NULL;
	bool                    free_options = false;
	uint32_t                server_id    = 0;
	zval*                   zsession     = NULL;

	PHONGO_PARSE_PARAMETERS_START(2, 3)
	Z_PARAM_STRING_OR_NULL(namespace, namespace_len)
	Z_PARAM_OBJECT_OF_CLASS(zbulk, php_phongo_bulkwrite_ce)
	Z_PARAM_OPTIONAL
	Z_PARAM_ZVAL_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_MANAGER_OBJ_P(getThis());
	bulk   = Z_BULKWRITE_OBJ_P(zbulk);

	options = php_phongo_prep_legacy_option(options, "writeConcern", &free_options);

	if (!phongo_parse_session(options, intern->client, NULL, &zsession)) {
		/* Exception should already have been thrown */
		return;
	}

	if (!php_phongo_manager_select_server(true, false, NULL, zsession, intern->client, &server_id)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	/* If the Server was created in a different process, reset the client so
	 * that its session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, intern);

	phongo_execute_bulk_write(getThis(), namespace, bulk, options, server_id, return_value);

cleanup:
	if (free_options) {
		php_phongo_prep_legacy_option_free(options);
	}
}

/* Returns the autoEncryption.encryptedFieldsMap driver option */
static PHP_METHOD(MongoDB_Driver_Manager, getEncryptedFieldsMap)
{
	php_phongo_manager_t* intern;

	intern = Z_MANAGER_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!Z_ISUNDEF(intern->enc_fields_map)) {
		RETURN_ZVAL(&intern->enc_fields_map, 1, 0);
	}
}

/* Returns the ReadConcern associated with this Manager */
static PHP_METHOD(MongoDB_Driver_Manager, getReadConcern)
{
	php_phongo_manager_t* intern;

	intern = Z_MANAGER_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_readconcern_init(return_value, mongoc_client_get_read_concern(intern->client));
}

/* Returns the ReadPreference associated with this Manager */
static PHP_METHOD(MongoDB_Driver_Manager, getReadPreference)
{
	php_phongo_manager_t* intern;

	intern = Z_MANAGER_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_readpreference_init(return_value, mongoc_client_get_read_prefs(intern->client));
}

/* Returns the Servers associated with this Manager */
static PHP_METHOD(MongoDB_Driver_Manager, getServers)
{
	php_phongo_manager_t*         intern;
	mongoc_server_description_t** sds;
	size_t                        i, n = 0;

	intern = Z_MANAGER_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	sds = mongoc_client_get_server_descriptions(intern->client, &n);
	array_init_size(return_value, n);

	for (i = 0; i < n; i++) {
		zval obj;

		phongo_server_init(&obj, getThis(), mongoc_server_description_id(sds[i]));
		add_next_index_zval(return_value, &obj);
	}

	mongoc_server_descriptions_destroy_all(sds, n);
}

/* Returns the WriteConcern associated with this Manager */
static PHP_METHOD(MongoDB_Driver_Manager, getWriteConcern)
{
	php_phongo_manager_t* intern;

	intern = Z_MANAGER_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_writeconcern_init(return_value, mongoc_client_get_write_concern(intern->client));
}

/* Unregisters an event subscriber for this Manager */
static PHP_METHOD(MongoDB_Driver_Manager, removeSubscriber)
{
	php_phongo_manager_t* intern;
	zval*                 subscriber;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(subscriber, php_phongo_subscriber_ce)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_MANAGER_OBJ_P(getThis());

	/* NOP if subscribers HashTable was never initialized by addSubscriber */
	if (!intern->subscribers) {
		return;
	}

	phongo_apm_remove_subscriber(intern->subscribers, subscriber);
}

/* Selects a Server for the given ReadPreference (default: primary). */
static PHP_METHOD(MongoDB_Driver_Manager, selectServer)
{
	php_phongo_manager_t* intern;
	zval*                 zreadPreference = NULL;
	uint32_t              server_id       = 0;

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_OBJECT_OF_CLASS_OR_NULL(zreadPreference, php_phongo_readpreference_ce)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_MANAGER_OBJ_P(getThis());

	if (!php_phongo_manager_select_server(false, false, zreadPreference, NULL, intern->client, &server_id)) {
		/* Exception should already have been thrown */
		return;
	}

	phongo_server_init(return_value, getThis(), server_id);
}

/* Returns a new client session */
static PHP_METHOD(MongoDB_Driver_Manager, startSession)
{
	php_phongo_manager_t*     intern;
	zval*                     options = NULL;
	mongoc_session_opt_t*     cs_opts = NULL;
	mongoc_client_session_t*  cs;
	bson_error_t              error    = { 0 };
	mongoc_transaction_opt_t* txn_opts = NULL;

	intern = Z_MANAGER_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	if (options && php_array_existsc(options, "causalConsistency")) {
		cs_opts = mongoc_session_opts_new();
		mongoc_session_opts_set_causal_consistency(cs_opts, php_array_fetchc_bool(options, "causalConsistency"));
	}

	if (options && php_array_existsc(options, "defaultTransactionOptions")) {
		zval* txn_options = php_array_fetchc(options, "defaultTransactionOptions");

		/* Thrown exception and return if the defaultTransactionOptions is not an array */
		if (Z_TYPE_P(txn_options) != IS_ARRAY) {
			phongo_throw_exception(
				PHONGO_ERROR_INVALID_ARGUMENT,
				"Expected \"defaultTransactionOptions\" option to be an array, %s given",
				PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(txn_options));
			goto cleanup;
		}

		/* Parse transaction options */
		txn_opts = php_mongodb_session_parse_transaction_options(txn_options);

		/* If an exception is thrown while parsing, the txn_opts struct is also
		 * NULL, so no need to free it here */
		if (EG(exception)) {
			goto cleanup;
		}

		/* If the options are non-empty, add them to the client session opts struct */
		if (txn_opts) {
			if (!cs_opts) {
				cs_opts = mongoc_session_opts_new();
			}

			mongoc_session_opts_set_default_transaction_opts(cs_opts, txn_opts);
			mongoc_transaction_opts_destroy(txn_opts);
		}
	}

	if (options && php_array_existsc(options, "snapshot")) {
		if (!cs_opts) {
			cs_opts = mongoc_session_opts_new();
		}
		mongoc_session_opts_set_snapshot(cs_opts, php_array_fetchc_bool(options, "snapshot"));
	}

	if (cs_opts && mongoc_session_opts_get_causal_consistency(cs_opts) && mongoc_session_opts_get_snapshot(cs_opts)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Only one of \"causalConsistency\" and \"snapshot\" can be enabled");
		goto cleanup;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that its session pool is cleared. This will ensure that we do not re-use
	 * a server session (i.e. LSID) created by a parent process. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, intern);

	cs = mongoc_client_start_session(intern->client, cs_opts, &error);

	if (cs) {
		phongo_session_init(return_value, getThis(), cs);
	} else {
		phongo_throw_exception_from_bson_error_t(&error);
	}

cleanup:
	if (cs_opts) {
		mongoc_session_opts_destroy(cs_opts);
	}
}

/* MongoDB\Driver\Manager object handlers */
static zend_object_handlers php_phongo_handler_manager;

static void php_phongo_manager_free_object(zend_object* object)
{
	php_phongo_manager_t* intern = Z_OBJ_MANAGER(object);

	zend_object_std_dtor(&intern->std);

	if (intern->client) {
		/* Request-scoped clients will be removed from the registry and
		 * destroyed. This is a NOP for persistent clients. The return value is
		 * ignored because we can't reasonably report an error here. On the off
		 * chance any request-scoped clients are missed, they will ultimately be
		 * destroyed in RSHUTDOWN along with the registry HashTable. */
		php_phongo_client_unregister(intern);
	}

	/* Update the request-scoped Manager registry. The return value is ignored
	 * because it's possible that the Manager was never registered due to a
	 * constructor exception.
	 *
	 * Note: this is done after unregistering a request-scoped client to ensure
	 * APM events can be observed by per-client subscribers, which are collected
	 * in phongo_apm_get_subscribers_to_notify. */
	php_phongo_manager_unregister(intern);

	if (intern->client_hash) {
		efree(intern->client_hash);
	}

	if (!Z_ISUNDEF(intern->enc_fields_map)) {
		zval_ptr_dtor(&intern->enc_fields_map);
	}

	/* Free the keyVaultClient last to ensure that potential non-persistent
	 * clients are destroyed in the correct order */
	if (!Z_ISUNDEF(intern->key_vault_client_manager)) {
		zval_ptr_dtor(&intern->key_vault_client_manager);
	}

	if (intern->subscribers) {
		zend_hash_destroy(intern->subscribers);
		FREE_HASHTABLE(intern->subscribers);
	}
}

static zend_object* php_phongo_manager_create_object(zend_class_entry* class_type)
{
	php_phongo_manager_t* intern = zend_object_alloc(sizeof(php_phongo_manager_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	PHONGO_SET_CREATED_BY_PID(intern);

	intern->std.handlers = &php_phongo_handler_manager;

	return &intern->std;
}

static HashTable* php_phongo_manager_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_manager_t*         intern;
	mongoc_server_description_t** sds;
	size_t                        i, n = 0;
	zval                          retval = ZVAL_STATIC_INIT;
	zval                          cluster;

	*is_temp = 1;
	intern   = Z_OBJ_MANAGER(object);

	array_init_size(&retval, 2);

	ADD_ASSOC_STRING(&retval, "uri", mongoc_uri_get_string(mongoc_client_get_uri(intern->client)));

	sds = mongoc_client_get_server_descriptions(intern->client, &n);

	array_init_size(&cluster, n);

	for (i = 0; i < n; i++) {
		zval obj;

		if (!php_phongo_server_to_zval(&obj, intern->client, sds[i])) {
			/* Exception already thrown */
			zval_ptr_dtor(&obj);
			zval_ptr_dtor(&cluster);
			goto done;
		}

		add_next_index_zval(&cluster, &obj);
	}

	ADD_ASSOC_ZVAL_EX(&retval, "cluster", &cluster);

done:
	mongoc_server_descriptions_destroy_all(sds, n);

	return Z_ARRVAL(retval);
}

void php_phongo_manager_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_manager_ce                = register_class_MongoDB_Driver_Manager();
	php_phongo_manager_ce->create_object = php_phongo_manager_create_object;

	memcpy(&php_phongo_handler_manager, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_manager.get_debug_info = php_phongo_manager_get_debug_info;
	php_phongo_handler_manager.free_obj       = php_phongo_manager_free_object;
	php_phongo_handler_manager.offset         = XtOffsetOf(php_phongo_manager_t, std);
}
