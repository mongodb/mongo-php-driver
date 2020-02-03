/*
 * Copyright 2014-2017 MongoDB, Inc.
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
#include <Zend/zend_hash.h>
#include <Zend/zend_interfaces.h>
#include <ext/standard/file.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_array_api.h"
#include "phongo_compat.h"
#include "php_phongo.h"
#include "Session.h"

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
static bool php_phongo_manager_merge_context_options(zval* zdriverOptions TSRMLS_DC) /* {{{ */
{
	php_stream_context* context;
	zval *              zcontext, *zcontextOptions;

	if (!php_array_existsc(zdriverOptions, "context")) {
		return true;
	}

	zcontext = php_array_fetchc(zdriverOptions, "context");
	context  = php_stream_context_from_zval(zcontext, 1);

	if (!context) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "\"context\" driver option is not a valid Stream-Context resource");
		return false;
	}

#if PHP_VERSION_ID >= 70000
	zcontextOptions = php_array_fetchc_array(&context->options, "ssl");
#else
	zcontextOptions = php_array_fetchc_array(context->options, "ssl");
#endif

	if (!zcontextOptions) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Stream-Context resource does not contain \"ssl\" options array");
		return false;
	}

	/* When running PHP in debug mode, php_error_docref duplicates the current
	 * scope, leading to a COW violation in zend_hash_merge and
	 * zend_symtable_str_del (called by php_array_unsetc). This macro allows
	 * that violation in debug mode and is a NOOP when in non-debug. */
#if PHP_VERSION_ID >= 70200
	HT_ALLOW_COW_VIOLATION(Z_ARRVAL_P(zdriverOptions));
#endif

	php_error_docref(NULL TSRMLS_CC, E_DEPRECATED, "The \"context\" driver option is deprecated.");

	/* Perform array union (see: add_function() in zend_operators.c) */
#if PHP_VERSION_ID >= 70000
	zend_hash_merge(Z_ARRVAL_P(zdriverOptions), Z_ARRVAL_P(zcontextOptions), zval_add_ref, 0);
#else
	{
		zval* tmp;
		zend_hash_merge(Z_ARRVAL_P(zdriverOptions), Z_ARRVAL_P(zcontextOptions), (void (*)(void* pData)) zval_add_ref, (void*) &tmp, sizeof(zval*), 0);
	}
#endif

	php_array_unsetc(zdriverOptions, "context");

	return true;
} /* }}} */

/* Prepare authMechanismProperties for BSON encoding by converting a boolean
 * value for the "CANONICALIZE_HOST_NAME" option to a string.
 *
 * Note: URI options are case-insensitive, so we must iterate through the
 * HashTable in order to detect options. */
static void php_phongo_manager_prep_authmechanismproperties(zval* properties TSRMLS_DC) /* {{{ */
{
	HashTable* ht_data;

	if (Z_TYPE_P(properties) != IS_ARRAY && Z_TYPE_P(properties) != IS_OBJECT) {
		return;
	}

	ht_data = HASH_OF(properties);

#if PHP_VERSION_ID >= 70000
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
#else
	{
		HashPosition pos;
		zval**       property;

		for (
			zend_hash_internal_pointer_reset_ex(ht_data, &pos);
			zend_hash_get_current_data_ex(ht_data, (void**) &property, &pos) == SUCCESS;
			zend_hash_move_forward_ex(ht_data, &pos)) {

			char* string_key     = NULL;
			uint  string_key_len = 0;
			ulong num_key        = 0;

			if (HASH_KEY_IS_STRING != zend_hash_get_current_key_ex(ht_data, &string_key, &string_key_len, &num_key, 0, &pos)) {
				continue;
			}

			/* URI options are case-insensitive */
			if (!strcasecmp(string_key, "CANONICALIZE_HOST_NAME")) {
				if (Z_TYPE_PP(property) != IS_STRING && zend_is_true(*property)) {
					SEPARATE_ZVAL_IF_NOT_REF(property);
					Z_TYPE_PP(property)   = IS_STRING;
					Z_STRVAL_PP(property) = estrndup("true", sizeof("true") - 1);
					Z_STRLEN_PP(property) = sizeof("true") - 1;
				}
			}
		}
	}
#endif /* PHP_VERSION_ID >= 70000 */

	return;
} /* }}} */

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
static void php_phongo_manager_prep_uri_options(zval* options TSRMLS_DC) /* {{{ */
{
	HashTable* ht_data;

	if (Z_TYPE_P(options) != IS_ARRAY) {
		return;
	}

	ht_data = HASH_OF(options);

#if PHP_VERSION_ID >= 70000
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
				php_phongo_read_preference_prep_tagsets(option TSRMLS_CC);
				continue;
			}

			if (!strcasecmp(ZSTR_VAL(string_key), MONGOC_URI_AUTHMECHANISMPROPERTIES)) {
				ZVAL_DEREF(option);
				SEPARATE_ZVAL_NOREF(option);
				php_phongo_manager_prep_authmechanismproperties(option TSRMLS_CC);
				continue;
			}
		}
		ZEND_HASH_FOREACH_END();
	}
#else
	{
		HashPosition pos;
		zval**       option;

		for (
			zend_hash_internal_pointer_reset_ex(ht_data, &pos);
			zend_hash_get_current_data_ex(ht_data, (void**) &option, &pos) == SUCCESS;
			zend_hash_move_forward_ex(ht_data, &pos)) {

			char* string_key     = NULL;
			uint  string_key_len = 0;
			ulong num_key        = 0;

			if (HASH_KEY_IS_STRING != zend_hash_get_current_key_ex(ht_data, &string_key, &string_key_len, &num_key, 0, &pos)) {
				continue;
			}

			if (!strcasecmp(string_key, MONGOC_URI_READPREFERENCETAGS)) {
				SEPARATE_ZVAL_IF_NOT_REF(option);
				php_phongo_read_preference_prep_tagsets(*option TSRMLS_CC);
				continue;
			}

			if (!strcasecmp(string_key, MONGOC_URI_AUTHMECHANISMPROPERTIES)) {
				SEPARATE_ZVAL_IF_NOT_REF(option);
				php_phongo_manager_prep_authmechanismproperties(*option TSRMLS_CC);
				continue;
			}
		}
	}
#endif

	return;
} /* }}} */

/* Selects a server for an execute method. If "for_writes" is true, a primary
 * will be selected. Otherwise, a read preference will be used to select the
 * server. If zreadPreference is NULL, the client's read preference will be
 * used. If zsession is a session object in a sharded transaction, the session
 * will be checked whether it is pinned to a server. If so, that server will be
 * selected. Otherwise, server selection
 *
 * On success, server_id will be set and the function will return true;
 * otherwise, false is returned and an exception is thrown. */
static bool php_phongo_manager_select_server(bool for_writes, zval* zreadPreference, zval* zsession, mongoc_client_t* client, uint32_t* server_id TSRMLS_DC) /* {{{ */
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
		read_preference = zreadPreference ? phongo_read_preference_from_zval(zreadPreference TSRMLS_CC) : mongoc_client_get_read_prefs(client);
	}

	selected_server = mongoc_client_select_server(client, for_writes, read_preference, &error);

	if (selected_server) {
		*server_id = mongoc_server_description_id(selected_server);
		mongoc_server_description_destroy(selected_server);

		return true;
	}

	/* Check for connection related exceptions */
	if (!EG(exception)) {
		phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
	}

	return false;
} /* }}} */

/* {{{ proto void MongoDB\Driver\Manager::__construct([string $uri = "mongodb://127.0.0.1/"[, array $options = array()[, array $driverOptions = array()]]])
   Constructs a new Manager */
static PHP_METHOD(Manager, __construct)
{
	php_phongo_manager_t* intern;
	zend_error_handling   error_handling;
	char*                 uri_string     = NULL;
	phongo_zpp_char_len   uri_string_len = 0;
	zval*                 options        = NULL;
	zval*                 driverOptions  = NULL;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);

	intern = Z_MANAGER_OBJ_P(getThis());

	/* Separate the options and driverOptions zvals, since we may end up
	 * modifying them in php_phongo_manager_prep_uri_options() and
	 * php_phongo_manager_merge_context_options() below, respectively. */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s!a/!a/!", &uri_string, &uri_string_len, &options, &driverOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (options) {
		php_phongo_manager_prep_uri_options(options TSRMLS_CC);
	}

	if (driverOptions && !php_phongo_manager_merge_context_options(driverOptions TSRMLS_CC)) {
		/* Exception should already have been thrown */
		return;
	}

	phongo_manager_init(intern, uri_string ? uri_string : PHONGO_MANAGER_URI_DEFAULT, options, driverOptions TSRMLS_CC);

	if (intern->client) {
		php_phongo_set_monitoring_callbacks(intern->client);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\ClientEncryption MongoDB\Driver\Manager::createClientEncryption(array $options)
   Return a ClientEncryption instance */
static PHP_METHOD(Manager, createClientEncryption)
{
	php_phongo_manager_t*          intern;
	php_phongo_clientencryption_t* clientencryption;
	zval*                          options;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &options) == FAILURE) {
		return;
	}

	intern = Z_MANAGER_OBJ_P(getThis());

	object_init_ex(return_value, php_phongo_clientencryption_ce);
	clientencryption = Z_CLIENTENCRYPTION_OBJ_P(return_value);

	phongo_clientencryption_init(clientencryption, intern->client, options TSRMLS_CC);
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Manager::executeCommand(string $db, MongoDB\Driver\Command $command[, array $options = null])
   Execute a Command */
static PHP_METHOD(Manager, executeCommand)
{
	php_phongo_manager_t* intern;
	char*                 db;
	phongo_zpp_char_len   db_len;
	zval*                 command;
	zval*                 options         = NULL;
	bool                  free_options    = false;
	zval*                 zreadPreference = NULL;
	zval*                 zsession        = NULL;
	uint32_t              server_id       = 0;
	DECLARE_RETURN_VALUE_USED

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|z!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		return;
	}

	intern = Z_MANAGER_OBJ_P(getThis());

	options = php_phongo_prep_legacy_option(options, "readPreference", &free_options TSRMLS_CC);

	if (!phongo_parse_session(options, intern->client, NULL, &zsession TSRMLS_CC)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!phongo_parse_read_preference(options, &zreadPreference TSRMLS_CC)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!php_phongo_manager_select_server(false, zreadPreference, zsession, intern->client, &server_id TSRMLS_CC)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_command(intern->client, PHONGO_COMMAND_RAW, db, command, options, server_id, return_value, return_value_used TSRMLS_CC);

cleanup:
	if (free_options) {
		php_phongo_prep_legacy_option_free(options TSRMLS_CC);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Manager::executeReadCommand(string $db, MongoDB\Driver\Command $command[, array $options = null])
   Execute a ReadCommand */
static PHP_METHOD(Manager, executeReadCommand)
{
	php_phongo_manager_t* intern;
	char*                 db;
	phongo_zpp_char_len   db_len;
	zval*                 command;
	zval*                 options         = NULL;
	zval*                 zreadPreference = NULL;
	uint32_t              server_id       = 0;
	zval*                 zsession        = NULL;
	DECLARE_RETURN_VALUE_USED

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|a!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		return;
	}

	intern = Z_MANAGER_OBJ_P(getThis());

	if (!phongo_parse_session(options, intern->client, NULL, &zsession TSRMLS_CC)) {
		/* Exception should already have been thrown */
		return;
	}

	if (!phongo_parse_read_preference(options, &zreadPreference TSRMLS_CC)) {
		/* Exception should already have been thrown */
		return;
	}

	if (!php_phongo_manager_select_server(false, zreadPreference, zsession, intern->client, &server_id TSRMLS_CC)) {
		/* Exception should already have been thrown */
		return;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_command(intern->client, PHONGO_COMMAND_READ, db, command, options, server_id, return_value, return_value_used TSRMLS_CC);
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Manager::executeWriteCommand(string $db, MongoDB\Driver\Command $command[, array $options = null])
   Execute a WriteCommand */
static PHP_METHOD(Manager, executeWriteCommand)
{
	php_phongo_manager_t* intern;
	char*                 db;
	phongo_zpp_char_len   db_len;
	zval*                 command;
	zval*                 options   = NULL;
	uint32_t              server_id = 0;
	zval*                 zsession  = NULL;
	DECLARE_RETURN_VALUE_USED

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|a!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		return;
	}

	intern = Z_MANAGER_OBJ_P(getThis());

	if (!phongo_parse_session(options, intern->client, NULL, &zsession TSRMLS_CC)) {
		/* Exception should already have been thrown */
		return;
	}

	if (!php_phongo_manager_select_server(true, NULL, zsession, intern->client, &server_id TSRMLS_CC)) {
		/* Exception should already have been thrown */
		return;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_command(intern->client, PHONGO_COMMAND_WRITE, db, command, options, server_id, return_value, return_value_used TSRMLS_CC);
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Manager::executeReadWriteCommand(string $db, MongoDB\Driver\Command $command[, array $options = null])
   Execute a ReadWriteCommand */
static PHP_METHOD(Manager, executeReadWriteCommand)
{
	php_phongo_manager_t* intern;
	char*                 db;
	phongo_zpp_char_len   db_len;
	zval*                 command;
	zval*                 options   = NULL;
	uint32_t              server_id = 0;
	zval*                 zsession  = NULL;
	DECLARE_RETURN_VALUE_USED

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|a!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		return;
	}

	intern = Z_MANAGER_OBJ_P(getThis());

	if (!phongo_parse_session(options, intern->client, NULL, &zsession TSRMLS_CC)) {
		/* Exception should already have been thrown */
		return;
	}

	if (!php_phongo_manager_select_server(true, NULL, zsession, intern->client, &server_id TSRMLS_CC)) {
		/* Exception should already have been thrown */
		return;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_command(intern->client, PHONGO_COMMAND_READ_WRITE, db, command, options, server_id, return_value, return_value_used TSRMLS_CC);
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Manager::executeQuery(string $namespace, MongoDB\Driver\Query $query[, array $options = null])
   Execute a Query */
static PHP_METHOD(Manager, executeQuery)
{
	php_phongo_manager_t* intern;
	char* namespace;
	phongo_zpp_char_len namespace_len;
	zval*               query;
	zval*               options         = NULL;
	bool                free_options    = false;
	zval*               zreadPreference = NULL;
	uint32_t            server_id       = 0;
	zval*               zsession        = NULL;
	DECLARE_RETURN_VALUE_USED

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|z!", &namespace, &namespace_len, &query, php_phongo_query_ce, &options) == FAILURE) {
		return;
	}

	intern = Z_MANAGER_OBJ_P(getThis());

	options = php_phongo_prep_legacy_option(options, "readPreference", &free_options TSRMLS_CC);

	if (!phongo_parse_session(options, intern->client, NULL, &zsession TSRMLS_CC)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!phongo_parse_read_preference(options, &zreadPreference TSRMLS_CC)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!php_phongo_manager_select_server(false, zreadPreference, zsession, intern->client, &server_id TSRMLS_CC)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	/* If the Manager was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_query(intern->client, namespace, query, options, server_id, return_value, return_value_used TSRMLS_CC);

cleanup:
	if (free_options) {
		php_phongo_prep_legacy_option_free(options TSRMLS_CC);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\WriteResult MongoDB\Driver\Manager::executeBulkWrite(string $namespace, MongoDB\Driver\BulkWrite $zbulk[, array $options = null])
   Executes a BulkWrite (i.e. any number of insert, update, and delete ops) */
static PHP_METHOD(Manager, executeBulkWrite)
{
	php_phongo_manager_t* intern;
	char* namespace;
	phongo_zpp_char_len     namespace_len;
	zval*                   zbulk;
	php_phongo_bulkwrite_t* bulk;
	zval*                   options      = NULL;
	bool                    free_options = false;
	uint32_t                server_id    = 0;
	zval*                   zsession     = NULL;
	DECLARE_RETURN_VALUE_USED

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|z!", &namespace, &namespace_len, &zbulk, php_phongo_bulkwrite_ce, &options) == FAILURE) {
		return;
	}

	intern = Z_MANAGER_OBJ_P(getThis());
	bulk   = Z_BULKWRITE_OBJ_P(zbulk);

	options = php_phongo_prep_legacy_option(options, "writeConcern", &free_options TSRMLS_CC);

	if (!phongo_parse_session(options, intern->client, NULL, &zsession TSRMLS_CC)) {
		/* Exception should already have been thrown */
		return;
	}

	if (!php_phongo_manager_select_server(true, NULL, zsession, intern->client, &server_id TSRMLS_CC)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	/* If the Server was created in a different process, reset the client so
	 * that its session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_bulk_write(intern->client, namespace, bulk, options, server_id, return_value, return_value_used TSRMLS_CC);

cleanup:
	if (free_options) {
		php_phongo_prep_legacy_option_free(options TSRMLS_CC);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\ReadConcern MongoDB\Driver\Manager::getReadConcern()
   Returns the ReadConcern associated with this Manager */
static PHP_METHOD(Manager, getReadConcern)
{
	php_phongo_manager_t* intern;
	DECLARE_RETURN_VALUE_USED

	intern = Z_MANAGER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (return_value_used) {
		phongo_readconcern_init(return_value, mongoc_client_get_read_concern(intern->client) TSRMLS_CC);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\ReadPreference MongoDB\Driver\Manager::getReadPreference()
   Returns the ReadPreference associated with this Manager */
static PHP_METHOD(Manager, getReadPreference)
{
	php_phongo_manager_t* intern;
	DECLARE_RETURN_VALUE_USED

	intern = Z_MANAGER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (return_value_used) {
		phongo_readpreference_init(return_value, mongoc_client_get_read_prefs(intern->client) TSRMLS_CC);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\Server[] MongoDB\Driver\Manager::getServers()
   Returns the Servers associated with this Manager */
static PHP_METHOD(Manager, getServers)
{
	php_phongo_manager_t*         intern;
	mongoc_server_description_t** sds;
	size_t                        i, n = 0;

	intern = Z_MANAGER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	sds = mongoc_client_get_server_descriptions(intern->client, &n);
	array_init_size(return_value, n);

	for (i = 0; i < n; i++) {
#if PHP_VERSION_ID >= 70000
		zval obj;

		phongo_server_init(&obj, intern->client, mongoc_server_description_id(sds[i]) TSRMLS_CC);
		add_next_index_zval(return_value, &obj);
#else
		zval* obj = NULL;

		MAKE_STD_ZVAL(obj);
		phongo_server_init(obj, intern->client, mongoc_server_description_id(sds[i]) TSRMLS_CC);
		add_next_index_zval(return_value, obj);
#endif
	}

	mongoc_server_descriptions_destroy_all(sds, n);
} /* }}} */

/* {{{ proto MongoDB\Driver\WriteConcern MongoDB\Driver\Manager::getWriteConcern()
   Returns the WriteConcern associated with this Manager */
static PHP_METHOD(Manager, getWriteConcern)
{
	php_phongo_manager_t* intern;
	DECLARE_RETURN_VALUE_USED

	intern = Z_MANAGER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (return_value_used) {
		phongo_writeconcern_init(return_value, mongoc_client_get_write_concern(intern->client) TSRMLS_CC);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\Server MongoDB\Driver\Manager::selectServers(MongoDB\Driver\ReadPreference $readPreference)
   Returns a suitable Server for the given ReadPreference */
static PHP_METHOD(Manager, selectServer)
{
	php_phongo_manager_t* intern;
	zval*                 zreadPreference = NULL;
	uint32_t              server_id       = 0;

	intern = Z_MANAGER_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &zreadPreference, php_phongo_readpreference_ce) == FAILURE) {
		return;
	}

	if (!php_phongo_manager_select_server(false, zreadPreference, NULL, intern->client, &server_id TSRMLS_CC)) {
		/* Exception should already have been thrown */
		return;
	}

	phongo_server_init(return_value, intern->client, server_id TSRMLS_CC);
} /* }}} */

/* {{{ proto MongoDB\Driver\Session MongoDB\Driver\Manager::startSession([array $options = null])
   Returns a new client session */
static PHP_METHOD(Manager, startSession)
{
	php_phongo_manager_t*     intern;
	zval*                     options = NULL;
	mongoc_session_opt_t*     cs_opts = NULL;
	mongoc_client_session_t*  cs;
	bson_error_t              error    = { 0 };
	mongoc_transaction_opt_t* txn_opts = NULL;

	intern = Z_MANAGER_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a!", &options) == FAILURE) {
		return;
	}

	if (options && php_array_existsc(options, "causalConsistency")) {
		cs_opts = mongoc_session_opts_new();
		mongoc_session_opts_set_causal_consistency(cs_opts, php_array_fetchc_bool(options, "causalConsistency"));
	}

	if (options && php_array_existsc(options, "defaultTransactionOptions")) {
		zval* txn_options = php_array_fetchc(options, "defaultTransactionOptions");

		/* Thrown exception and return if the defaultTransactionOptions is not an array */
		if (Z_TYPE_P(txn_options) != IS_ARRAY) {
			phongo_throw_exception(
				PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC,
				"Expected \"defaultTransactionOptions\" option to be an array, %s given",
				PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(txn_options));
			goto cleanup;
		}

		/* Parse transaction options */
		txn_opts = php_mongodb_session_parse_transaction_options(txn_options TSRMLS_CC);

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

	/* If the Manager was created in a different process, reset the client so
	 * that its session pool is cleared. This will ensure that we do not re-use
	 * a server session (i.e. LSID) created by a parent process. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	cs = mongoc_client_start_session(intern->client, cs_opts, &error);

	if (cs) {
		phongo_session_init(return_value, cs TSRMLS_CC);
	} else {
		phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
	}

cleanup:
	if (cs_opts) {
		mongoc_session_opts_destroy(cs_opts);
	}
} /* }}} */

/* {{{ MongoDB\Driver\Manager function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Manager___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, uri)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
	ZEND_ARG_ARRAY_INFO(0, driverOptions, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_createClientEncryption, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeRWCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeQuery, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zquery, MongoDB\\Driver\\Query, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeBulkWrite, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zbulk, MongoDB\\Driver\\BulkWrite, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_selectServer, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\Driver\\ReadPreference, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_startSession, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_manager_me[] = {
	/* clang-format off */
	PHP_ME(Manager, __construct, ai_Manager___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, createClientEncryption, ai_Manager_createClientEncryption, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, executeCommand, ai_Manager_executeCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, executeReadCommand, ai_Manager_executeRWCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, executeWriteCommand, ai_Manager_executeRWCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, executeReadWriteCommand, ai_Manager_executeCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, executeQuery, ai_Manager_executeQuery, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, executeBulkWrite, ai_Manager_executeBulkWrite, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, getReadConcern, ai_Manager_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, getReadPreference, ai_Manager_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, getServers, ai_Manager_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, getWriteConcern, ai_Manager_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, selectServer, ai_Manager_selectServer, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Manager, startSession, ai_Manager_startSession, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_Manager_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Manager object handlers */
static zend_object_handlers php_phongo_handler_manager;

static void php_phongo_manager_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_manager_t* intern = Z_OBJ_MANAGER(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->client) {
		MONGOC_DEBUG("Not destroying persistent client for Manager");
		intern->client = NULL;
	}

	if (intern->client_hash) {
		efree(intern->client_hash);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_manager_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_manager_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_manager_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	PHONGO_SET_CREATED_BY_PID(intern);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_manager;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_manager_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_manager;

		return retval;
	}
#endif
} /* }}} */

static HashTable* php_phongo_manager_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_manager_t*         intern;
	mongoc_server_description_t** sds;
	size_t                        i, n = 0;
	zval                          retval = ZVAL_STATIC_INIT;
	ZVAL_RETVAL_TYPE              cluster;

	*is_temp = 1;
	intern   = Z_MANAGER_OBJ_P(object);

	array_init_size(&retval, 2);

	ADD_ASSOC_STRING(&retval, "uri", mongoc_uri_get_string(mongoc_client_get_uri(intern->client)));

	sds = mongoc_client_get_server_descriptions(intern->client, &n);

#if PHP_VERSION_ID >= 70000
	array_init_size(&cluster, n);

	for (i = 0; i < n; i++) {
		zval obj;

		if (!php_phongo_server_to_zval(&obj, sds[i])) {
			/* Exception already thrown */
			zval_ptr_dtor(&obj);
			zval_ptr_dtor(&cluster);
			goto done;
		}

		add_next_index_zval(&cluster, &obj);
	}

	ADD_ASSOC_ZVAL_EX(&retval, "cluster", &cluster);
#else
	MAKE_STD_ZVAL(cluster);
	array_init_size(cluster, n);

	for (i = 0; i < n; i++) {
		zval* obj = NULL;

		MAKE_STD_ZVAL(obj);
		if (!php_phongo_server_to_zval(obj, sds[i])) {
			/* Exception already thrown */
			zval_ptr_dtor(&obj);
			zval_ptr_dtor(&cluster);
			goto done;
		}

		add_next_index_zval(cluster, obj);
	}

	ADD_ASSOC_ZVAL_EX(&retval, "cluster", cluster);
#endif

done:
	mongoc_server_descriptions_destroy_all(sds, n);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_manager_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Manager", php_phongo_manager_me);
	php_phongo_manager_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_manager_ce->create_object = php_phongo_manager_create_object;
	PHONGO_CE_FINAL(php_phongo_manager_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_manager_ce);

	memcpy(&php_phongo_handler_manager, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_manager.get_debug_info = php_phongo_manager_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_manager.free_obj = php_phongo_manager_free_object;
	php_phongo_handler_manager.offset   = XtOffsetOf(php_phongo_manager_t, std);
#endif
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
