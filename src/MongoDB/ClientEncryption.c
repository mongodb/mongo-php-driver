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

zend_class_entry* php_phongo_clientencryption_ce;

/* Forward declarations */
static void phongo_clientencryption_create_datakey(php_phongo_clientencryption_t* clientencryption, zval* return_value, char* kms_provider, zval* options);
static void phongo_clientencryption_encrypt(php_phongo_clientencryption_t* clientencryption, zval* zvalue, zval* zciphertext, zval* options);
static void phongo_clientencryption_decrypt(php_phongo_clientencryption_t* clientencryption, zval* zciphertext, zval* zvalue);

/* {{{ proto MongoDB\BSON\Binary MongoDB\Driver\ClientEncryption::createDataKey(string $kmsProvider[, array $options])
   Creates a new key document and inserts into the key vault collection. */
static PHP_METHOD(ClientEncryption, createDataKey)
{
	char*                          kms_provider     = NULL;
	size_t                         kms_provider_len = 0;
	zval*                          options          = NULL;
	zend_error_handling            error_handling;
	php_phongo_clientencryption_t* intern;

	intern = Z_CLIENTENCRYPTION_OBJ_P(getThis());
	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|a!", &kms_provider, &kms_provider_len, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}

	zend_restore_error_handling(&error_handling);

	phongo_clientencryption_create_datakey(intern, return_value, kms_provider, options);
} /* }}} */

/* {{{ proto MongoDB\BSON\Binary MongoDB\Driver\ClientEncryption::encrypt(mixed $value[, array $options])
   Encrypts a value with a given key and algorithm */
static PHP_METHOD(ClientEncryption, encrypt)
{
	zval*                          value   = NULL;
	zval*                          options = NULL;
	zend_error_handling            error_handling;
	php_phongo_clientencryption_t* intern;

	intern = Z_CLIENTENCRYPTION_OBJ_P(getThis());
	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z|a!", &value, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}

	zend_restore_error_handling(&error_handling);

	phongo_clientencryption_encrypt(intern, value, return_value, options);
} /* }}} */

/* {{{ proto mixed MongoDB\Driver\ClientEncryption::decrypt(MongoDB\BSON\BinaryInterface $value)
   Decrypts an encrypted value (BSON binary of subtype 6). Returns the original BSON value */
static PHP_METHOD(ClientEncryption, decrypt)
{
	zval*                          ciphertext;
	zend_error_handling            error_handling;
	php_phongo_clientencryption_t* intern;

	intern = Z_CLIENTENCRYPTION_OBJ_P(getThis());
	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "O", &ciphertext, php_phongo_binary_interface_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}

	zend_restore_error_handling(&error_handling);

	phongo_clientencryption_decrypt(intern, ciphertext, return_value);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(ai_ClientEncryption_createDataKey, 0, 0, 1)
	ZEND_ARG_INFO(0, kmsProvider)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ClientEncryption_encrypt, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ClientEncryption_decrypt, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, keyVaultClient, MongoDB\\BSON\\BinaryInterface, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_ClientEncryption_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_clientencryption_me[] = {
	/* clang-format off */
	PHP_ME(ClientEncryption, createDataKey, ai_ClientEncryption_createDataKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ClientEncryption, encrypt, ai_ClientEncryption_encrypt, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(ClientEncryption, decrypt, ai_ClientEncryption_decrypt, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_ClientEncryption_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_ClientEncryption_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\ClientEncryption object handlers */
static zend_object_handlers php_phongo_handler_clientencryption;

static void php_phongo_clientencryption_free_object(zend_object* object) /* {{{ */
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
} /* }}} */

static zend_object* php_phongo_clientencryption_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_clientencryption_t* intern = zend_object_alloc(sizeof(php_phongo_clientencryption_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_clientencryption;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_clientencryption_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_clientencryption_t* intern = NULL;
	zval                           retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_OBJ_CLIENTENCRYPTION(PHONGO_COMPAT_GET_OBJ(object));

	array_init(&retval);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_clientencryption_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ClientEncryption", php_phongo_clientencryption_me);
	php_phongo_clientencryption_ce                = zend_register_internal_class(&ce);
	php_phongo_clientencryption_ce->create_object = php_phongo_clientencryption_create_object;
	PHONGO_CE_FINAL(php_phongo_clientencryption_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_clientencryption_ce);

	memcpy(&php_phongo_handler_clientencryption, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_clientencryption.get_debug_info = php_phongo_clientencryption_get_debug_info;
	php_phongo_handler_clientencryption.free_obj       = php_phongo_clientencryption_free_object;
	php_phongo_handler_clientencryption.offset         = XtOffsetOf(php_phongo_clientencryption_t, std);

	zend_declare_class_constant_string(php_phongo_clientencryption_ce, ZEND_STRL("AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC"), MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_DETERMINISTIC);
	zend_declare_class_constant_string(php_phongo_clientencryption_ce, ZEND_STRL("AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM"), MONGOC_AEAD_AES_256_CBC_HMAC_SHA_512_RANDOM);
} /* }}} */

#ifdef MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION
/* keyVaultClientManager is an output parameter and will be assigned the
 * keyVaultNamespace Manager (if any). */
static mongoc_client_encryption_opts_t* phongo_clientencryption_opts_from_zval(zval* defaultKeyVaultClient, zval* options, zval** keyVaultClientManager) /* {{{ */
{
	mongoc_client_encryption_opts_t* opts;

	opts                   = mongoc_client_encryption_opts_new();
	*keyVaultClientManager = NULL;

	if (!options || Z_TYPE_P(options) != IS_ARRAY) {
		/* Returning opts as-is will defer to mongoc_client_encryption_new to
		 * raise an error for missing required options */
		return opts;
	}

	if (php_array_existsc(options, "keyVaultClient")) {
		zval* key_vault_client = php_array_fetch(options, "keyVaultClient");

		if (Z_TYPE_P(key_vault_client) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(key_vault_client), php_phongo_manager_ce)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"keyVaultClient\" encryption option to be %s, %s given", ZSTR_VAL(php_phongo_manager_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(key_vault_client));
			goto cleanup;
		}

		mongoc_client_encryption_opts_set_keyvault_client(opts, Z_MANAGER_OBJ_P(key_vault_client)->client);
		*keyVaultClientManager = key_vault_client;
	} else {
		mongoc_client_encryption_opts_set_keyvault_client(opts, Z_MANAGER_OBJ_P(defaultKeyVaultClient)->client);
		*keyVaultClientManager = defaultKeyVaultClient;
	}

	if (php_array_existsc(options, "keyVaultNamespace")) {
		char*     keyvault_namespace;
		char*     db_name;
		char*     coll_name;
		int       plen;
		zend_bool pfree;

		keyvault_namespace = php_array_fetchc_string(options, "keyVaultNamespace", &plen, &pfree);

		if (!phongo_split_namespace(keyvault_namespace, &db_name, &coll_name)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"keyVaultNamespace\" encryption option to contain a full collection name");

			if (pfree) {
				efree(keyvault_namespace);
			}

			goto cleanup;
		}

		mongoc_client_encryption_opts_set_keyvault_namespace(opts, db_name, coll_name);
		efree(db_name);
		efree(coll_name);

		if (pfree) {
			efree(keyvault_namespace);
		}
	}

	if (php_array_existsc(options, "kmsProviders")) {
		zval*  kms_providers  = php_array_fetchc(options, "kmsProviders");
		bson_t bson_providers = BSON_INITIALIZER;

		if (Z_TYPE_P(kms_providers) != IS_ARRAY && Z_TYPE_P(kms_providers) != IS_OBJECT) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"kmsProviders\" encryption option to be an array or object");
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
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"tlsOptions\" encryption option to be an array or object");
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
} /* }}} */

void phongo_clientencryption_init(zval* return_value, zval* manager, zval* options) /* {{{ */
{
	php_phongo_clientencryption_t*   intern;
	mongoc_client_encryption_t*      client_encryption;
	mongoc_client_encryption_opts_t* opts;
	zval*                            key_vault_client_manager = manager;
	bson_error_t                     error                    = { 0 };

	opts = phongo_clientencryption_opts_from_zval(manager, options, &key_vault_client_manager);
	if (!opts) {
		/* Exception already thrown */
		goto cleanup;
	}

	client_encryption = mongoc_client_encryption_new(opts, &error);
	if (!client_encryption) {
		phongo_throw_exception_from_bson_error_t(&error);

		goto cleanup;
	}

	object_init_ex(return_value, php_phongo_clientencryption_ce);

	intern                    = Z_CLIENTENCRYPTION_OBJ_P(return_value);
	intern->client_encryption = client_encryption;
	ZVAL_ZVAL(&intern->key_vault_client_manager, key_vault_client_manager, 1, 0);

cleanup:
	if (opts) {
		mongoc_client_encryption_opts_destroy(opts);
	}
} /* }}} */

static mongoc_client_encryption_datakey_opts_t* phongo_clientencryption_datakey_opts_from_zval(zval* options) /* {{{ */
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

	if (php_array_existsc(options, "masterKey")) {
		bson_t masterkey = BSON_INITIALIZER;

		php_phongo_zval_to_bson(php_array_fetchc(options, "masterKey"), PHONGO_BSON_NONE, &masterkey, NULL);
		if (EG(exception)) {
			goto cleanup;
		}

		mongoc_client_encryption_datakey_opts_set_masterkey(opts, &masterkey);
	}

	return opts;

cleanup:
	if (opts) {
		mongoc_client_encryption_datakey_opts_destroy(opts);
	}

	return NULL;
} /* }}} */

static void phongo_clientencryption_create_datakey(php_phongo_clientencryption_t* clientencryption, zval* return_value, char* kms_provider, zval* options) /* {{{ */
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

	if (!php_phongo_bson_value_to_zval(&keyid, return_value)) {
		/* Exception already thrown */
		goto cleanup;
	}

cleanup:
	if (opts) {
		mongoc_client_encryption_datakey_opts_destroy(opts);
	}

	bson_value_destroy(&keyid);
} /* }}} */

static mongoc_client_encryption_encrypt_opts_t* phongo_clientencryption_encrypt_opts_from_zval(zval* options) /* {{{ */
{
	mongoc_client_encryption_encrypt_opts_t* opts;

	opts = mongoc_client_encryption_encrypt_opts_new();

	if (!options || Z_TYPE_P(options) != IS_ARRAY) {
		return opts;
	}

	if (php_array_existsc(options, "keyId")) {
		bson_value_t keyid = { 0 };

		php_phongo_zval_to_bson_value(php_array_fetchc(options, "keyId"), PHONGO_BSON_NONE, &keyid);

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

	return opts;

cleanup:
	if (opts) {
		mongoc_client_encryption_encrypt_opts_destroy(opts);
	}

	return NULL;
} /* }}} */

static void phongo_clientencryption_encrypt(php_phongo_clientencryption_t* clientencryption, zval* zvalue, zval* zciphertext, zval* options) /* {{{ */
{
	mongoc_client_encryption_encrypt_opts_t* opts;
	bson_value_t                             ciphertext = { 0 };
	bson_value_t                             value      = { 0 };
	bson_error_t                             error      = { 0 };

	php_phongo_zval_to_bson_value(zvalue, PHONGO_BSON_NONE, &value);

	opts = phongo_clientencryption_encrypt_opts_from_zval(options);

	if (!opts) {
		/* Exception already thrown */
		goto cleanup;
	}

	if (!mongoc_client_encryption_encrypt(clientencryption->client_encryption, &value, opts, &ciphertext, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	if (!php_phongo_bson_value_to_zval(&ciphertext, zciphertext)) {
		/* Exception already thrown */
		goto cleanup;
	}

cleanup:
	if (opts) {
		mongoc_client_encryption_encrypt_opts_destroy(opts);
	}

	bson_value_destroy(&ciphertext);
	bson_value_destroy(&value);
} /* }}} */

static void phongo_clientencryption_decrypt(php_phongo_clientencryption_t* clientencryption, zval* zciphertext, zval* zvalue) /* {{{ */
{
	bson_value_t ciphertext = { 0 };
	bson_value_t value      = { 0 };
	bson_error_t error      = { 0 };

	php_phongo_zval_to_bson_value(zciphertext, PHONGO_BSON_NONE, &ciphertext);

	if (!mongoc_client_encryption_decrypt(clientencryption->client_encryption, &ciphertext, &value, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	if (!php_phongo_bson_value_to_zval(&value, zvalue)) {
		/* Exception already thrown */
		goto cleanup;
	}

cleanup:
	bson_value_destroy(&ciphertext);
	bson_value_destroy(&value);
} /* }}} */
#else  /* MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION */
void phongo_clientencryption_init(php_phongo_clientencryption_t* clientencryption, zval* manager, zval* options) /* {{{ */
{
	phongo_throw_exception_no_cse(PHONGO_ERROR_RUNTIME, "Cannot configure clientEncryption object.");
}
/* }}} */

static void phongo_clientencryption_create_datakey(php_phongo_clientencryption_t* clientencryption, zval* return_value, char* kms_provider, zval* options) /* {{{ */
{
	phongo_throw_exception_no_cse(PHONGO_ERROR_RUNTIME, "Cannot create encryption key.");
}
/* }}} */

static void phongo_clientencryption_encrypt(php_phongo_clientencryption_t* clientencryption, zval* zvalue, zval* zciphertext, zval* options) /* {{{ */
{
	phongo_throw_exception_no_cse(PHONGO_ERROR_RUNTIME, "Cannot encrypt value.");
}
/* }}} */

static void phongo_clientencryption_decrypt(php_phongo_clientencryption_t* clientencryption, zval* zciphertext, zval* zvalue) /* {{{ */
{
	phongo_throw_exception_no_cse(PHONGO_ERROR_RUNTIME, "Cannot decrypt value.");
}
/* }}} */
#endif /* MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION */
