/*
 * Copyright 2019-present MongoDB, Inc.
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

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"
#include "phongo_util.h"
#include "ClientEncryption_arginfo.h"

#include "BSON/Binary.h"
#include "MongoDB/ClientEncryption.h"
#include "MongoDB/Cursor.h"
#include "MongoDB/Query.h"

zend_class_entry* php_phongo_clientencryption_ce;

/* Forward declarations */
static void phongo_clientencryption_create_datakey(php_phongo_clientencryption_t* clientencryption, zval* return_value, char* kms_provider, zval* options);
static void phongo_clientencryption_encrypt(php_phongo_clientencryption_t* clientencryption, zval* zvalue, zval* zciphertext, zval* options);
static void phongo_clientencryption_encrypt_expression(php_phongo_clientencryption_t* clientencryption, zval* zexpr, zval* return_value, zval* options);
static void phongo_clientencryption_decrypt(php_phongo_clientencryption_t* clientencryption, zval* zciphertext, zval* zvalue);

#define RETVAL_BSON_T(reply)                                 \
	do {                                                     \
		php_phongo_bson_state state;                         \
		PHONGO_BSON_INIT_STATE(state);                       \
		if (!php_phongo_bson_to_zval_ex(&(reply), &state)) { \
			zval_ptr_dtor(&state.zchild);                    \
			goto cleanup;                                    \
		}                                                    \
		RETVAL_ZVAL(&state.zchild, 0, 1);                    \
	} while (0)

#define RETVAL_OPTIONAL_BSON_T(reply) \
	do {                              \
		RETVAL_NULL();                \
		if (!bson_empty(&(reply))) {  \
			RETVAL_BSON_T(reply);     \
		}                             \
	} while (0)

/* Returns true if keyid is a UUID Binary value with an appropriate data length;
 * otherwise, throws an exception and returns false. */
static bool validate_keyid(bson_value_t* keyid)
{
	if (keyid->value_type != BSON_TYPE_BINARY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected keyid to have Binary BSON type, %s given", php_phongo_bson_type_to_string(keyid->value_type));
		return false;
	}

	if (keyid->value.v_binary.subtype != BSON_SUBTYPE_UUID) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected keyid to have UUID Binary subtype (%d), %d given", BSON_SUBTYPE_UUID, keyid->value.v_binary.subtype);
		return false;
	}

	/* php_phongo_binary_init already enforces the data length for Binary objects
	 * with UUID subtypes so we throw a different exception here. */
	if (keyid->value.v_binary.data_len != PHONGO_BINARY_UUID_SIZE) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Expected keyid to have data length of %d bytes, %d given", PHONGO_BINARY_UUID_SIZE, keyid->value.v_binary.data_len);
		return false;
	}

	return true;
}

/* Constructs a new ClientEncryption */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, __construct)
{
	zval* options;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(options)
	PHONGO_PARSE_PARAMETERS_END();

	/* An exception will be thrown on error. */
	phongo_clientencryption_init(Z_CLIENTENCRYPTION_OBJ_P(getThis()), options, NULL);
}

/* Adds a keyAltName to the keyAltNames array of the key document in the key
   vault collection with the given UUID (BSON binary subtype 0x04). Returns the
   previous version of the key document, or null if no document matched. */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, addKeyAltName)
{
	zval*        zkeyid         = NULL;
	char*        keyaltname     = NULL;
	size_t       keyaltname_len = 0;
	bson_value_t keyid          = { 0 };
	bson_t       key_doc        = BSON_INITIALIZER;
	bson_error_t error          = { 0 };

	PHONGO_PARSE_PARAMETERS_START(2, 2)
	Z_PARAM_OBJECT_OF_CLASS(zkeyid, php_phongo_binary_ce)
	Z_PARAM_STRING(keyaltname, keyaltname_len);
	PHONGO_PARSE_PARAMETERS_END();

	phongo_zval_to_bson_value(zkeyid, &keyid);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!validate_keyid(&keyid)) {
		/* Exception already thrown */
		goto cleanup;
	}

	if (!mongoc_client_encryption_add_key_alt_name(Z_CLIENTENCRYPTION_OBJ_P(getThis())->client_encryption, &keyid, keyaltname, &key_doc, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	RETVAL_OPTIONAL_BSON_T(key_doc);

cleanup:
	bson_value_destroy(&keyid);
	bson_destroy(&key_doc);
}

/* Creates a new key document and inserts into the key vault collection and
   returns its identifier (UUID as a BSON binary with subtype 0x04). */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, createDataKey)
{
	char*                          kms_provider     = NULL;
	size_t                         kms_provider_len = 0;
	zval*                          options          = NULL;
	php_phongo_clientencryption_t* intern;

	intern = Z_CLIENTENCRYPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_STRING(kms_provider, kms_provider_len)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_clientencryption_create_datakey(intern, return_value, kms_provider, options);
}

/* Removes the key document with the given UUID (BSON binary subtype 0x04) from
   the key vault collection. Returns the result of the internal deleteOne()
   operation on the key vault collection. */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, deleteKey)
{
	zval*        zkeyid = NULL;
	bson_value_t keyid  = { 0 };
	bson_t       reply  = BSON_INITIALIZER;
	bson_error_t error  = { 0 };

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(zkeyid, php_phongo_binary_ce)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_zval_to_bson_value(zkeyid, &keyid);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!validate_keyid(&keyid)) {
		/* Exception already thrown */
		goto cleanup;
	}

	if (!mongoc_client_encryption_delete_key(Z_CLIENTENCRYPTION_OBJ_P(getThis())->client_encryption, &keyid, &reply, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	if (bson_empty(&reply)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "mongoc_client_encryption_delete_key returned an empty document");
		goto cleanup;
	}

	RETVAL_BSON_T(reply);

cleanup:
	bson_value_destroy(&keyid);
	bson_destroy(&reply);
}

/* Encrypts a value with a given key and algorithm */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, encrypt)
{
	zval*                          value   = NULL;
	zval*                          options = NULL;
	php_phongo_clientencryption_t* intern;

	intern = Z_CLIENTENCRYPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_ZVAL(value)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_clientencryption_encrypt(intern, value, return_value, options);
}

/* Encrypts a value with a given key and algorithm */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, encryptExpression)
{
	zval*                          expr    = NULL;
	zval*                          options = NULL;
	php_phongo_clientencryption_t* intern;

	intern = Z_CLIENTENCRYPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_ZVAL(expr)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_clientencryption_encrypt_expression(intern, expr, return_value, options);
}

/* Decrypts an encrypted value (BSON binary of subtype 6). Returns the original BSON value */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, decrypt)
{
	zval*                          ciphertext;
	php_phongo_clientencryption_t* intern;

	intern = Z_CLIENTENCRYPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(ciphertext, php_phongo_binary_interface_ce)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_clientencryption_decrypt(intern, ciphertext, return_value);
}

/* Finds a single key document with the given UUID (BSON binary subtype 0x04).
   Returns the result of the internal find() operation on the key vault
   collection, or null if no document matched. */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, getKey)
{
	zval*        zkeyid  = NULL;
	bson_value_t keyid   = { 0 };
	bson_t       key_doc = BSON_INITIALIZER;
	bson_error_t error   = { 0 };

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_OBJECT_OF_CLASS(zkeyid, php_phongo_binary_ce)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_zval_to_bson_value(zkeyid, &keyid);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!validate_keyid(&keyid)) {
		/* Exception already thrown */
		goto cleanup;
	}

	if (!mongoc_client_encryption_get_key(Z_CLIENTENCRYPTION_OBJ_P(getThis())->client_encryption, &keyid, &key_doc, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	RETVAL_OPTIONAL_BSON_T(key_doc);

cleanup:
	bson_value_destroy(&keyid);
	bson_destroy(&key_doc);
}

/* Returns a key document in the key vault collection with the given keyAltName,
   or null if no document matched. */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, getKeyByAltName)
{
	char*        keyaltname     = NULL;
	size_t       keyaltname_len = 0;
	bson_t       key_doc        = BSON_INITIALIZER;
	bson_error_t error          = { 0 };

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(keyaltname, keyaltname_len);
	PHONGO_PARSE_PARAMETERS_END();

	if (!mongoc_client_encryption_get_key_by_alt_name(Z_CLIENTENCRYPTION_OBJ_P(getThis())->client_encryption, keyaltname, &key_doc, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	RETVAL_OPTIONAL_BSON_T(key_doc);

cleanup:
	bson_destroy(&key_doc);
}

/* Finds all documents in the key vault collection. Returns the result of the
   internal find() operation on the key vault collection as a cursor. */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, getKeys)
{
	mongoc_cursor_t*               cursor;
	bson_error_t                   error = { 0 };
	php_phongo_clientencryption_t* intern;
	zval                           query = ZVAL_STATIC_INIT;

	intern = Z_CLIENTENCRYPTION_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	/* mongoc_client_encryption_get_keys executes a query against its internal
	 * key vault collection. The collection has a majority read concern, but the
	 * query itself specifies no filter or options. Create an empty query object
	 * and attach it to the cursor for the benefit of debugging. */
	if (!phongo_query_init(&query, NULL, NULL)) {
		/* Exception already thrown */
		goto cleanup;
	}

	cursor = mongoc_client_encryption_get_keys(intern->client_encryption, &error);

	if (!cursor) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	if (!phongo_cursor_init_for_query(return_value, &intern->key_vault_client_manager, cursor, intern->key_vault_namespace, &query, NULL, NULL)) {
		/* Exception already thrown */
		mongoc_cursor_destroy(cursor);
		goto cleanup;
	}

cleanup:
	zval_ptr_dtor(&query);
}

/* Removes a keyAltName from the keyAltNames array of the key document in the
   key vault collection with the given UUID (BSON binary subtype 0x04). Returns
   the previous version of the key document, or null if no document matched. */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, removeKeyAltName)
{
	zval*        zkeyid         = NULL;
	char*        keyaltname     = NULL;
	size_t       keyaltname_len = 0;
	bson_value_t keyid          = { 0 };
	bson_t       key_doc        = BSON_INITIALIZER;
	bson_error_t error          = { 0 };

	PHONGO_PARSE_PARAMETERS_START(2, 2)
	Z_PARAM_OBJECT_OF_CLASS(zkeyid, php_phongo_binary_ce)
	Z_PARAM_STRING(keyaltname, keyaltname_len);
	PHONGO_PARSE_PARAMETERS_END();

	phongo_zval_to_bson_value(zkeyid, &keyid);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!validate_keyid(&keyid)) {
		/* Exception already thrown */
		goto cleanup;
	}

	if (!mongoc_client_encryption_remove_key_alt_name(Z_CLIENTENCRYPTION_OBJ_P(getThis())->client_encryption, &keyid, keyaltname, &key_doc, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	RETVAL_OPTIONAL_BSON_T(key_doc);

cleanup:
	bson_value_destroy(&keyid);
	bson_destroy(&key_doc);
}

/* Decrypts multiple data keys and (re-)encrypts them with a new provider (and
   masterKey if applicable), or with their current provider if a new one is not
   given. Returns an object corresponding to the internal libmongoc result. */
static PHP_METHOD(MongoDB_Driver_ClientEncryption, rewrapManyDataKey)
{
	zval*        zfilter       = NULL;
	zval*        options       = NULL;
	bson_t       filter        = BSON_INITIALIZER;
	char*        provider      = NULL;
	zend_bool    free_provider = false;
	bson_t*      masterkey     = NULL;
	bson_error_t error         = { 0 };
	bson_t       reply         = BSON_INITIALIZER;

	mongoc_client_encryption_rewrap_many_datakey_result_t* result = NULL;
	const bson_t*                                          bulk_write_result;

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_ARRAY_OR_OBJECT(zfilter)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_zval_to_bson(zfilter, PHONGO_BSON_NONE, &filter, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	if (options && php_array_existsc(options, "provider")) {
		int provider_len;

		provider = php_array_fetchc_string(options, "provider", &provider_len, &free_provider);
	}

	if (options && php_array_existsc(options, "masterKey")) {
		zval* zmasterkey = php_array_fetchc(options, "masterKey");

		if (Z_TYPE_P(zmasterkey) != IS_OBJECT && Z_TYPE_P(zmasterkey) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"masterKey\" option to be array or object, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(zmasterkey));
			goto cleanup;
		}

		if (!provider) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "The \"masterKey\" option should not be specified without \"provider\"");
			goto cleanup;
		}

		masterkey = bson_new();
		php_phongo_zval_to_bson(zmasterkey, PHONGO_BSON_NONE, masterkey, NULL);

		if (EG(exception)) {
			goto cleanup;
		}
	}

	result = mongoc_client_encryption_rewrap_many_datakey_result_new();

	if (!mongoc_client_encryption_rewrap_many_datakey(Z_CLIENTENCRYPTION_OBJ_P(getThis())->client_encryption, &filter, provider, masterkey, result, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	bulk_write_result = mongoc_client_encryption_rewrap_many_datakey_result_get_bulk_write_result(result);

	if (bson_empty0(bulk_write_result)) {
		BSON_APPEND_NULL(&reply, "bulkWriteResult");
	} else {
		BSON_APPEND_DOCUMENT(&reply, "bulkWriteResult", bulk_write_result);
	}

	RETVAL_BSON_T(reply);

cleanup:
	if (free_provider) {
		efree(provider);
	}

	bson_destroy(&filter);
	bson_destroy(masterkey);
	mongoc_client_encryption_rewrap_many_datakey_result_destroy(result);
}

/* MongoDB\Driver\ClientEncryption object handlers */
static zend_object_handlers php_phongo_handler_clientencryption;

static void php_phongo_clientencryption_free_object(zend_object* object)
{
	php_phongo_clientencryption_t* intern = Z_OBJ_CLIENTENCRYPTION(object);

	zend_object_std_dtor(&intern->std);

	if (intern->client_encryption) {
		mongoc_client_encryption_destroy(intern->client_encryption);
	}

	/* Free the keyVaultClient last to ensure that a potential non-persistent
	 * client outlives the mongoc_client_encryption_t as needed */
	if (!Z_ISUNDEF(intern->key_vault_client_manager)) {
		zval_ptr_dtor(&intern->key_vault_client_manager);
	}

	if (intern->key_vault_namespace) {
		efree(intern->key_vault_namespace);
	}
}

static zend_object* php_phongo_clientencryption_create_object(zend_class_entry* class_type)
{
	php_phongo_clientencryption_t* intern = zend_object_alloc(sizeof(php_phongo_clientencryption_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_clientencryption;

	return &intern->std;
}

static HashTable* php_phongo_clientencryption_get_debug_info(zend_object* object, int* is_temp)
{
	php_phongo_clientencryption_t* intern = NULL;
	zval                           retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_OBJ_CLIENTENCRYPTION(object);

	array_init(&retval);

	return Z_ARRVAL(retval);
}

void php_phongo_clientencryption_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_clientencryption_ce                = register_class_MongoDB_Driver_ClientEncryption();
	php_phongo_clientencryption_ce->create_object = php_phongo_clientencryption_create_object;

	memcpy(&php_phongo_handler_clientencryption, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_clientencryption.get_debug_info = php_phongo_clientencryption_get_debug_info;
	php_phongo_handler_clientencryption.free_obj       = php_phongo_clientencryption_free_object;
	php_phongo_handler_clientencryption.offset         = XtOffsetOf(php_phongo_clientencryption_t, std);
}

#ifdef MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION
/* key_vault_client_manager is an output parameter and will be assigned to the
 * effective keyVaultClient. */
static mongoc_client_encryption_opts_t* phongo_clientencryption_opts_from_zval(zval* options, zval* default_key_vault_client_manager, zval** key_vault_client_manager)
{
	mongoc_client_encryption_opts_t* opts = mongoc_client_encryption_opts_new();

	*key_vault_client_manager = NULL;

	if (!options || Z_TYPE_P(options) != IS_ARRAY) {
		/* Returning opts as-is will defer to mongoc_client_encryption_new to
		 * raise an error for missing required options */
		return opts;
	}

	if (php_array_existsc(options, "keyVaultClient")) {
		zval* key_vault_client = php_array_fetch(options, "keyVaultClient");

		if (Z_TYPE_P(key_vault_client) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(key_vault_client), php_phongo_manager_ce)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"keyVaultClient\" option to be %s, %s given", ZSTR_VAL(php_phongo_manager_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(key_vault_client));
			goto cleanup;
		}

		mongoc_client_encryption_opts_set_keyvault_client(opts, Z_MANAGER_OBJ_P(key_vault_client)->client);
		*key_vault_client_manager = key_vault_client;
	} else if (default_key_vault_client_manager) {
		mongoc_client_encryption_opts_set_keyvault_client(opts, Z_MANAGER_OBJ_P(default_key_vault_client_manager)->client);
		*key_vault_client_manager = default_key_vault_client_manager;
	} else {
		/* If the ClientEncryption object is being constructed directly, the
		 * "keyVaultClient" option must be explicitly provided. */
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "The \"keyVaultClient\" option is required when constructing a ClientEncryption object directly");
		goto cleanup;
	}

	if (php_array_existsc(options, "keyVaultNamespace")) {
		char*     key_vault_namespace;
		char*     db_name;
		char*     coll_name;
		int       plen;
		zend_bool pfree;

		key_vault_namespace = php_array_fetchc_string(options, "keyVaultNamespace", &plen, &pfree);

		if (!phongo_split_namespace(key_vault_namespace, &db_name, &coll_name)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"keyVaultNamespace\" option to contain a full collection namespace");

			if (pfree) {
				efree(key_vault_namespace);
			}

			goto cleanup;
		}

		mongoc_client_encryption_opts_set_keyvault_namespace(opts, db_name, coll_name);
		efree(db_name);
		efree(coll_name);

		if (pfree) {
			efree(key_vault_namespace);
		}
	}

	if (php_array_existsc(options, "kmsProviders")) {
		zval*  kms_providers  = php_array_fetchc(options, "kmsProviders");
		bson_t bson_providers = BSON_INITIALIZER;

		if (Z_TYPE_P(kms_providers) != IS_ARRAY && Z_TYPE_P(kms_providers) != IS_OBJECT) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"kmsProviders\" option to be an array or object, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(kms_providers));
			goto cleanup;
		}

		php_phongo_zval_to_bson(kms_providers, PHONGO_BSON_NONE, &bson_providers, NULL);
		if (EG(exception)) {
			goto cleanup;
		}

		mongoc_client_encryption_opts_set_kms_providers(opts, &bson_providers);
		bson_destroy(&bson_providers);
	}

	if (php_array_existsc(options, "tlsOptions")) {
		zval*  tls_options  = php_array_fetchc(options, "tlsOptions");
		bson_t bson_options = BSON_INITIALIZER;

		if (Z_TYPE_P(tls_options) != IS_ARRAY && Z_TYPE_P(tls_options) != IS_OBJECT) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"tlsOptions\" option to be an array or object, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(tls_options));
			goto cleanup;
		}

		php_phongo_zval_to_bson(tls_options, PHONGO_BSON_NONE, &bson_options, NULL);
		if (EG(exception)) {
			goto cleanup;
		}

		mongoc_client_encryption_opts_set_tls_opts(opts, &bson_options);
		bson_destroy(&bson_options);
	}

	return opts;

cleanup:
	if (opts) {
		mongoc_client_encryption_opts_destroy(opts);
	}

	return NULL;
}

void phongo_clientencryption_init(php_phongo_clientencryption_t* intern, zval* options, zval* default_key_vault_client_manager)
{
	mongoc_client_encryption_t*      client_encryption;
	mongoc_client_encryption_opts_t* opts;
	zval*                            key_vault_client_manager = NULL;
	bson_error_t                     error                    = { 0 };

	opts = phongo_clientencryption_opts_from_zval(options, default_key_vault_client_manager, &key_vault_client_manager);

	if (!opts) {
		/* Exception already thrown */
		goto cleanup;
	}

	client_encryption = mongoc_client_encryption_new(opts, &error);

	if (!client_encryption) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	intern->client_encryption = client_encryption;

	/* Note: key_vault_client_manager should always be assigned if options were
	 * successfully parsed by phongo_clientencryption_opts_from_zval, but let's
	 * be defensive. */
	if (key_vault_client_manager) {
		ZVAL_ZVAL(&intern->key_vault_client_manager, key_vault_client_manager, 1, 0);
	}

	/* Copy the key vault namespace, since it may be referenced later by
	 * ClientEncryption::getKeys(). The namespace will already have been
	 * validated by phongo_clientencryption_opts_from_zval. */
	if (php_array_existsc(options, "keyVaultNamespace")) {
		char*     key_vault_namespace;
		int       plen;
		zend_bool pfree;

		key_vault_namespace = php_array_fetchc_string(options, "keyVaultNamespace", &plen, &pfree);

		intern->key_vault_namespace = estrdup(key_vault_namespace);

		if (pfree) {
			efree(key_vault_namespace);
		}
	}

cleanup:
	if (opts) {
		mongoc_client_encryption_opts_destroy(opts);
	}
}

static mongoc_client_encryption_datakey_opts_t* phongo_clientencryption_datakey_opts_from_zval(zval* options)
{
	mongoc_client_encryption_datakey_opts_t* opts;

	opts = mongoc_client_encryption_datakey_opts_new();

	if (!options || Z_TYPE_P(options) != IS_ARRAY) {
		return opts;
	}

	if (php_array_existsc(options, "keyAltNames")) {
		zval*      zkeyaltnames = php_array_fetchc(options, "keyAltNames");
		HashTable* ht_data;
		uint32_t   keyaltnames_count;
		char**     keyaltnames;
		uint32_t   i      = 0;
		uint32_t   j      = 0;
		bool       failed = false;

		if (!zkeyaltnames || Z_TYPE_P(zkeyaltnames) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected keyAltNames to be array, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(zkeyaltnames));
			goto cleanup;
		}

		ht_data           = HASH_OF(zkeyaltnames);
		keyaltnames_count = ht_data ? zend_hash_num_elements(ht_data) : 0;
		keyaltnames       = ecalloc(keyaltnames_count, sizeof(char*));

		{
			zend_string* string_key = NULL;
			zend_ulong   num_key    = 0;
			zval*        keyaltname;

			ZEND_HASH_FOREACH_KEY_VAL(ht_data, num_key, string_key, keyaltname)
			{
				if (i >= keyaltnames_count) {
					phongo_throw_exception(PHONGO_ERROR_LOGIC, "Iterating over too many keyAltNames. Please file a bug report");
					failed = true;
					break;
				}

				if (Z_TYPE_P(keyaltname) != IS_STRING) {
					if (string_key) {
						phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected keyAltName with index \"%s\" to be string, %s given", ZSTR_VAL(string_key), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(keyaltname));
					} else {
						phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected keyAltName with index \"%lu\" to be string, %s given", num_key, PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(keyaltname));
					}

					failed = true;
					break;
				}

				keyaltnames[i] = estrdup(Z_STRVAL_P(keyaltname));
				i++;
			}
			ZEND_HASH_FOREACH_END();
		}

		if (!failed) {
			mongoc_client_encryption_datakey_opts_set_keyaltnames(opts, keyaltnames, keyaltnames_count);
		}

		for (j = 0; j < i; j++) {
			efree(keyaltnames[j]);
		}
		efree(keyaltnames);

		if (failed) {
			goto cleanup;
		}
	}

	if (php_array_existsc(options, "keyMaterial")) {
		zval* keyMaterial = php_array_fetchc(options, "keyMaterial");

		if (Z_TYPE_P(keyMaterial) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(keyMaterial), php_phongo_binary_ce)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"keyMaterial\" option to be %s, %s given", ZSTR_VAL(php_phongo_binary_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(keyMaterial));
			goto cleanup;
		}

		mongoc_client_encryption_datakey_opts_set_keymaterial(opts, (uint8_t*) Z_BINARY_OBJ_P(keyMaterial)->data, Z_BINARY_OBJ_P(keyMaterial)->data_len);
	}

	if (php_array_existsc(options, "masterKey")) {
		zval*  zmasterkey = php_array_fetchc(options, "masterKey");
		bson_t masterkey  = BSON_INITIALIZER;

		if (Z_TYPE_P(zmasterkey) != IS_OBJECT && Z_TYPE_P(zmasterkey) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"masterKey\" option to be array or object, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(zmasterkey));
			goto cleanup;
		}

		php_phongo_zval_to_bson(zmasterkey, PHONGO_BSON_NONE, &masterkey, NULL);

		if (EG(exception)) {
			bson_destroy(&masterkey);
			goto cleanup;
		}

		mongoc_client_encryption_datakey_opts_set_masterkey(opts, &masterkey);
		bson_destroy(&masterkey);
	}

	return opts;

cleanup:
	if (opts) {
		mongoc_client_encryption_datakey_opts_destroy(opts);
	}

	return NULL;
}

static void phongo_clientencryption_create_datakey(php_phongo_clientencryption_t* clientencryption, zval* return_value, char* kms_provider, zval* options)
{
	mongoc_client_encryption_datakey_opts_t* opts;
	bson_value_t                             keyid = { 0 };
	bson_error_t                             error = { 0 };

	opts = phongo_clientencryption_datakey_opts_from_zval(options);

	if (!opts) {
		/* Exception already thrown */
		goto cleanup;
	}

	if (!mongoc_client_encryption_create_datakey(clientencryption->client_encryption, kms_provider, opts, &keyid, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	if (!phongo_bson_value_to_zval(&keyid, return_value)) {
		/* Exception already thrown */
		goto cleanup;
	}

cleanup:
	if (opts) {
		mongoc_client_encryption_datakey_opts_destroy(opts);
	}

	bson_value_destroy(&keyid);
}

static mongoc_client_encryption_encrypt_range_opts_t* phongo_clientencryption_encrypt_range_opts_from_zval(zval* options)
{
	mongoc_client_encryption_encrypt_range_opts_t* opts;

	opts = mongoc_client_encryption_encrypt_range_opts_new();

	if (!options || Z_TYPE_P(options) != IS_ARRAY) {
		return opts;
	}

	if (php_array_existsc(options, "trimFactor")) {
		int64_t trimfactor = php_array_fetchc_long(options, "trimFactor");

		if (trimfactor < 0 || trimfactor > INT32_MAX) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"trimFactor\" range option to be a positive 32-bit integer, %" PRId64 " given", trimfactor);
			goto cleanup;
		}

		mongoc_client_encryption_encrypt_range_opts_set_trim_factor(opts, (int32_t) trimfactor);
	}

	if (php_array_existsc(options, "sparsity")) {
		int64_t sparsity = php_array_fetchc_long(options, "sparsity");

		if (sparsity < 0 || sparsity > INT64_MAX) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"sparsity\" range option to be a positive 64-bit integer, %" PRId64 " given", sparsity);
			goto cleanup;
		}

		mongoc_client_encryption_encrypt_range_opts_set_sparsity(opts, sparsity);
	}

	if (php_array_existsc(options, "precision")) {
		int64_t precision = php_array_fetchc_long(options, "precision");

		if (precision < 0 || precision > INT32_MAX) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"precision\" range option to be a positive 32-bit integer, %" PRId64 " given", precision);
			goto cleanup;
		}

		mongoc_client_encryption_encrypt_range_opts_set_precision(opts, (int32_t) precision);
	}

	if (php_array_existsc(options, "min")) {
		bson_value_t min = { 0 };

		phongo_zval_to_bson_value(php_array_fetchc(options, "min"), &min);

		if (EG(exception)) {
			bson_value_destroy(&min);
			goto cleanup;
		}

		mongoc_client_encryption_encrypt_range_opts_set_min(opts, &min);
		bson_value_destroy(&min);
	}

	if (php_array_existsc(options, "max")) {
		bson_value_t max = { 0 };

		phongo_zval_to_bson_value(php_array_fetchc(options, "max"), &max);

		if (EG(exception)) {
			bson_value_destroy(&max);
			goto cleanup;
		}

		mongoc_client_encryption_encrypt_range_opts_set_max(opts, &max);
		bson_value_destroy(&max);
	}

	return opts;

cleanup:
	if (opts) {
		mongoc_client_encryption_encrypt_range_opts_destroy(opts);
	}

	return NULL;
}

static mongoc_client_encryption_encrypt_opts_t* phongo_clientencryption_encrypt_opts_from_zval(zval* options)
{
	mongoc_client_encryption_encrypt_opts_t* opts;

	opts = mongoc_client_encryption_encrypt_opts_new();

	if (!options || Z_TYPE_P(options) != IS_ARRAY) {
		return opts;
	}

	if (php_array_existsc(options, "contentionFactor")) {
		mongoc_client_encryption_encrypt_opts_set_contention_factor(opts, php_array_fetch_long(options, "contentionFactor"));
	}

	if (php_array_existsc(options, "keyId")) {
		bson_value_t keyid = { 0 };

		phongo_zval_to_bson_value(php_array_fetchc(options, "keyId"), &keyid);

		if (EG(exception)) {
			bson_value_destroy(&keyid);
			goto cleanup;
		}

		mongoc_client_encryption_encrypt_opts_set_keyid(opts, &keyid);
		bson_value_destroy(&keyid);
	}

	if (php_array_existsc(options, "keyAltName")) {
		char*     keyaltname;
		int       plen;
		zend_bool pfree;

		keyaltname = php_array_fetch_string(options, "keyAltName", &plen, &pfree);
		mongoc_client_encryption_encrypt_opts_set_keyaltname(opts, keyaltname);

		if (pfree) {
			efree(keyaltname);
		}
	}

	if (php_array_existsc(options, "algorithm")) {
		char*     algorithm;
		int       plen;
		zend_bool pfree;

		algorithm = php_array_fetch_string(options, "algorithm", &plen, &pfree);
		mongoc_client_encryption_encrypt_opts_set_algorithm(opts, algorithm);

		if (pfree) {
			efree(algorithm);
		}
	}

	if (php_array_existsc(options, "queryType")) {
		char*     querytype;
		int       plen;
		zend_bool pfree;

		querytype = php_array_fetch_string(options, "queryType", &plen, &pfree);
		mongoc_client_encryption_encrypt_opts_set_query_type(opts, querytype);

		if (pfree) {
			efree(querytype);
		}
	}

	if (php_array_existsc(options, "rangeOpts")) {
		mongoc_client_encryption_encrypt_range_opts_t* range_opts;

		range_opts = phongo_clientencryption_encrypt_range_opts_from_zval(php_array_fetchc(options, "rangeOpts"));

		if (!range_opts) {
			/* Exception already thrown */
			goto cleanup;
		}

		mongoc_client_encryption_encrypt_opts_set_range_opts(opts, range_opts);
		mongoc_client_encryption_encrypt_range_opts_destroy(range_opts);
	}

	return opts;

cleanup:
	if (opts) {
		mongoc_client_encryption_encrypt_opts_destroy(opts);
	}

	return NULL;
}

static void phongo_clientencryption_encrypt(php_phongo_clientencryption_t* clientencryption, zval* zvalue, zval* zciphertext, zval* options)
{
	mongoc_client_encryption_encrypt_opts_t* opts       = NULL;
	bson_value_t                             ciphertext = { 0 };
	bson_value_t                             value      = { 0 };
	bson_error_t                             error      = { 0 };

	phongo_zval_to_bson_value(zvalue, &value);

	if (EG(exception)) {
		goto cleanup;
	}

	opts = phongo_clientencryption_encrypt_opts_from_zval(options);

	if (!opts) {
		/* Exception already thrown */
		goto cleanup;
	}

	if (!mongoc_client_encryption_encrypt(clientencryption->client_encryption, &value, opts, &ciphertext, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	if (!phongo_bson_value_to_zval(&ciphertext, zciphertext)) {
		/* Exception already thrown */
		goto cleanup;
	}

cleanup:
	if (opts) {
		mongoc_client_encryption_encrypt_opts_destroy(opts);
	}

	bson_value_destroy(&ciphertext);
	bson_value_destroy(&value);
}

static void phongo_clientencryption_encrypt_expression(php_phongo_clientencryption_t* clientencryption, zval* zexpr, zval* return_value, zval* options)
{
	mongoc_client_encryption_encrypt_opts_t* opts           = NULL;
	bson_t                                   expr           = BSON_INITIALIZER;
	bson_t                                   expr_encrypted = BSON_INITIALIZER;
	bson_error_t                             error          = { 0 };

	php_phongo_zval_to_bson(zexpr, PHONGO_BSON_NONE, &expr, NULL);

	if (EG(exception)) {
		goto cleanup;
	}

	opts = phongo_clientencryption_encrypt_opts_from_zval(options);

	if (!opts) {
		/* Exception already thrown */
		goto cleanup;
	}

	if (!mongoc_client_encryption_encrypt_expression(clientencryption->client_encryption, &expr, opts, &expr_encrypted, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	if (!php_phongo_bson_to_zval(&expr_encrypted, return_value)) {
		/* Exception already thrown */
		goto cleanup;
	}

cleanup:
	if (opts) {
		mongoc_client_encryption_encrypt_opts_destroy(opts);
	}

	bson_destroy(&expr);
	bson_destroy(&expr_encrypted);
}

static void phongo_clientencryption_decrypt(php_phongo_clientencryption_t* clientencryption, zval* zciphertext, zval* zvalue)
{
	bson_value_t ciphertext = { 0 };
	bson_value_t value      = { 0 };
	bson_error_t error      = { 0 };

	phongo_zval_to_bson_value(zciphertext, &ciphertext);

	if (EG(exception)) {
		goto cleanup;
	}

	if (!mongoc_client_encryption_decrypt(clientencryption->client_encryption, &ciphertext, &value, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	/** Use the legacy decoder to return PHP types instead of BSON types */
	if (!phongo_bson_value_to_zval_legacy(&value, zvalue)) {
		/* Exception already thrown */
		goto cleanup;
	}

cleanup:
	bson_value_destroy(&ciphertext);
	bson_value_destroy(&value);
}
#else  /* MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION */
void phongo_clientencryption_init(php_phongo_clientencryption_t* intern, zval* options, zval* default_key_vault_client_manager)
{
	phongo_throw_exception_no_cse(PHONGO_ERROR_RUNTIME, "Cannot configure clientEncryption object.");
}

static void phongo_clientencryption_create_datakey(php_phongo_clientencryption_t* clientencryption, zval* return_value, char* kms_provider, zval* options)
{
	phongo_throw_exception_no_cse(PHONGO_ERROR_RUNTIME, "Cannot create encryption key.");
}

static void phongo_clientencryption_encrypt(php_phongo_clientencryption_t* clientencryption, zval* zvalue, zval* zciphertext, zval* options)
{
	phongo_throw_exception_no_cse(PHONGO_ERROR_RUNTIME, "Cannot encrypt value.");
}

static void phongo_clientencryption_encrypt_expression(php_phongo_clientencryption_t* clientencryption, zval* zexpr, zval* return_value, zval* options)
{
	phongo_throw_exception_no_cse(PHONGO_ERROR_RUNTIME, "Cannot encrypt expression.");
}

static void phongo_clientencryption_decrypt(php_phongo_clientencryption_t* clientencryption, zval* zciphertext, zval* zvalue)
{
	phongo_throw_exception_no_cse(PHONGO_ERROR_RUNTIME, "Cannot decrypt value.");
}
#endif /* MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION */
