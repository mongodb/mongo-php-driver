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

#ifdef MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION
#include <mongocrypt/mongocrypt.h>
#endif

#include <php.h>
#include <ext/standard/info.h>

#include "php_phongo.h"
#include "src/phongo_client.h"
#include "src/phongo_error.h"
#include "src/phongo_ini.h"
#include "src/functions_arginfo.h"

ZEND_DECLARE_MODULE_GLOBALS(mongodb)
#if defined(ZTS) && defined(COMPILE_DL_MONGODB)
ZEND_TSRMLS_CACHE_DEFINE();
#endif

/* Initialize a thread counter, which will be atomically incremented in GINIT.
 * In turn, GSHUTDOWN will decrement the counter and call mongoc_cleanup() when
 * it reaches zero (i.e. last thread is shutdown). This is necessary because
 * mongoc_cleanup() must be called after all persistent clients have been
 * destroyed. */
static int32_t phongo_num_threads = 0;

/* Declare zend_class_entry dependencies, which are initialized in MINIT */
zend_class_entry* php_phongo_json_serializable_ce;

/* {{{ phongo_std_object_handlers */
zend_object_handlers phongo_std_object_handlers;

zend_object_handlers* phongo_get_std_object_handlers(void)
{
	return &phongo_std_object_handlers;
}
/* }}} */

/* {{{ Memory allocation wrappers */
static void* php_phongo_malloc(size_t num_bytes) /* {{{ */
{
	return pemalloc(num_bytes, 1);
} /* }}} */

static void* php_phongo_calloc(size_t num_members, size_t num_bytes) /* {{{ */
{
	return pecalloc(num_members, num_bytes, 1);
} /* }}} */

static void* php_phongo_realloc(void* mem, size_t num_bytes) /* {{{ */
{
	return perealloc(mem, num_bytes, 1);
} /* }}} */

static void php_phongo_free(void* mem) /* {{{ */
{
	if (mem) {
		pefree(mem, 1);
	}
} /* }}} */

PHP_RINIT_FUNCTION(mongodb) /* {{{ */
{
	/* Initialize HashTable for non-persistent clients, which is initialized to
	 * NULL in GINIT and destroyed and reset to NULL in RSHUTDOWN. Although we
	 * specify an element destructor here, all request clients should be freed
	 * naturally via garbage collection (i.e. the HashTable should be empty at
	 * the time it is destroyed in RSHUTDOWN). */
	if (MONGODB_G(request_clients) == NULL) {
		ALLOC_HASHTABLE(MONGODB_G(request_clients));
		zend_hash_init(MONGODB_G(request_clients), 0, NULL, php_phongo_pclient_destroy_ptr, 0);
	}

	/* Initialize HashTable for APM subscribers, which is initialized to NULL in
	 * GINIT and destroyed and reset to NULL in RSHUTDOWN. Since this HashTable
	 * will store subscriber object zvals, we specify ZVAL_PTR_DTOR as its
	 * element destructor so that any still-registered subscribers can be freed
	 * in RSHUTDOWN. */
	if (MONGODB_G(subscribers) == NULL) {
		ALLOC_HASHTABLE(MONGODB_G(subscribers));
		zend_hash_init(MONGODB_G(subscribers), 0, NULL, ZVAL_PTR_DTOR, 0);
	}

	/* Initialize HashTable for registering Manager objects. This is initialized
	 * to NULL in GINIT and destroyed and reset to NULL in RSHUTDOWN. Since this
	 * HashTable stores pointers to existing php_phongo_manager_t objects (not
	 * counted references), the element destructor is intentionally NULL. */
	if (MONGODB_G(managers) == NULL) {
		ALLOC_HASHTABLE(MONGODB_G(managers));
		zend_hash_init(MONGODB_G(managers), 0, NULL, NULL, 0);
	}

	return SUCCESS;
} /* }}} */

PHP_GINIT_FUNCTION(mongodb) /* {{{ */
{
#if defined(COMPILE_DL_MONGODB) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	/* Increment the thread counter. */
	bson_atomic_int32_fetch_add(&phongo_num_threads, 1, bson_memory_order_seq_cst);

	/* Clear extension globals */
	memset(mongodb_globals, 0, sizeof(zend_mongodb_globals));

	/* Initialize HashTable for persistent clients, which will be destroyed in
	 * GSHUTDOWN. We specify an element destructor so that persistent clients
	 * can be destroyed along with the HashTable. The HashTable's struct is
	 * nested within globals, so no allocation is needed (unlike the HashTables
	 * allocated in RINIT). */
	zend_hash_init(&mongodb_globals->persistent_clients, 0, NULL, php_phongo_pclient_destroy_ptr, 1);
} /* }}} */

static zend_class_entry* php_phongo_fetch_internal_class(const char* class_name, size_t class_name_len)
{
	zend_class_entry* pce;

	if ((pce = zend_hash_str_find_ptr(CG(class_table), class_name, class_name_len))) {
		return pce;
	}

	return NULL;
}

static HashTable* php_phongo_std_get_gc(phongo_compat_object_handler_type* object, zval** table, int* n) /* {{{ */
{
	*table = NULL;
	*n     = 0;
	return zend_std_get_properties(object);
} /* }}} */

PHP_MINIT_FUNCTION(mongodb) /* {{{ */
{
	bson_mem_vtable_t bson_mem_vtable = {
		php_phongo_malloc,
		php_phongo_calloc,
		php_phongo_realloc,
		php_phongo_free,
	};

	(void) type; /* We don't care if we are loaded via dl() or extension= */

	phongo_register_ini_entries(INIT_FUNC_ARGS_PASSTHRU);

	/* Assign our custom vtable to libbson, so all memory allocation in libbson
	 * (and libmongoc) will use PHP's persistent memory API. After doing so,
	 * initialize libmongoc. Later, we will shutdown libmongoc and restore
	 * libbson's vtable in the final GSHUTDOWN. */
	bson_mem_set_vtable(&bson_mem_vtable);
	mongoc_init();

	/* Prep default object handlers to be used when we register the classes */
	memcpy(&phongo_std_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	/* Disable cloning by default. Individual classes can opt in if they need to
	 * support this (e.g. BSON objects). */
	phongo_std_object_handlers.clone_obj = NULL;
	/* Ensure that get_gc delegates to zend_std_get_properties directly in case
	 * our class defines a get_properties handler for debugging purposes. */
	phongo_std_object_handlers.get_gc = php_phongo_std_get_gc;

	/* Initialize zend_class_entry dependencies.
	 *
	 * Although JsonSerializable was introduced in PHP 5.4.0,
	 * php_json_serializable_ce is not exported in PHP versions before 5.4.26
	 * and 5.5.10. For later PHP versions, looking up the class manually also
	 * helps with distros that disable LTDL_LAZY for dlopen() (e.g. Fedora).
	 */
	php_phongo_json_serializable_ce = php_phongo_fetch_internal_class(ZEND_STRL("jsonserializable"));

	if (php_phongo_json_serializable_ce == NULL) {
		zend_error(E_ERROR, "JsonSerializable class is not defined. Please ensure that the 'json' module is loaded before the 'mongodb' module.");
		return FAILURE;
	}

	/* Register base BSON classes first */
	php_phongo_type_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_serializable_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_unserializable_init_ce(INIT_FUNC_ARGS_PASSTHRU);

	php_phongo_binary_interface_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_decimal128_interface_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_javascript_interface_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_maxkey_interface_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_minkey_interface_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_objectid_interface_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_regex_interface_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_timestamp_interface_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_utcdatetime_interface_init_ce(INIT_FUNC_ARGS_PASSTHRU);

	php_phongo_iterator_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_packedarray_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_document_init_ce(INIT_FUNC_ARGS_PASSTHRU);

	php_phongo_binary_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_dbpointer_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_decimal128_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_int64_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_javascript_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_maxkey_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_minkey_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_objectid_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_persistable_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_regex_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_symbol_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_timestamp_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_undefined_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_utcdatetime_init_ce(INIT_FUNC_ARGS_PASSTHRU);

	php_phongo_cursor_interface_init_ce(INIT_FUNC_ARGS_PASSTHRU);

	php_phongo_bulkwrite_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_clientencryption_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_command_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_cursor_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_cursorid_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_manager_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_query_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_readconcern_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_readpreference_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_server_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_serverapi_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_serverdescription_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_topologydescription_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_session_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_writeconcern_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_writeconcernerror_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_writeerror_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_writeresult_init_ce(INIT_FUNC_ARGS_PASSTHRU);

	/* Register base exception classes first */
	php_phongo_exception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_runtimeexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_serverexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_connectionexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_writeexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);

	php_phongo_authenticationexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_bulkwriteexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_commandexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_connectiontimeoutexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_encryptionexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_executiontimeoutexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_invalidargumentexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_logicexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_sslconnectionexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_unexpectedvalueexception_init_ce(INIT_FUNC_ARGS_PASSTHRU);

	/* Register base APM classes first */
	php_phongo_subscriber_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_commandsubscriber_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_commandfailedevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_commandstartedevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_commandsucceededevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_sdamsubscriber_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_serverchangedevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_serverclosedevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_serverheartbeatfailedevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_serverheartbeatstartedevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_serverheartbeatsucceededevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_serveropeningevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_topologychangedevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_topologyclosedevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);
	php_phongo_topologyopeningevent_init_ce(INIT_FUNC_ARGS_PASSTHRU);

	REGISTER_STRING_CONSTANT("MONGODB_VERSION", (char*) PHP_MONGODB_VERSION, CONST_CS | CONST_PERSISTENT);
	REGISTER_STRING_CONSTANT("MONGODB_STABILITY", (char*) PHP_MONGODB_STABILITY, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
} /* }}} */

PHP_MSHUTDOWN_FUNCTION(mongodb) /* {{{ */
{
	phongo_unregister_ini_entries(SHUTDOWN_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
} /* }}} */

PHP_RSHUTDOWN_FUNCTION(mongodb) /* {{{ */
{
	/* Destroy HashTable for APM subscribers, which was initialized in RINIT. */
	if (MONGODB_G(subscribers)) {
		zend_hash_destroy(MONGODB_G(subscribers));
		FREE_HASHTABLE(MONGODB_G(subscribers));
		MONGODB_G(subscribers) = NULL;
	}

	/* Destroy HashTable for non-persistent clients, which was initialized in
	 * RINIT. This is intentionally done after the APM subscribers to allow any
	 * non-persistent clients still referenced by a subscriber (not freed prior
	 * to RSHUTDOWN) to be naturally garbage collected and freed by the Manager
	 * free_object handler rather than the HashTable's element destructor. There
	 * is no need to use zend_hash_graceful_reverse_destroy here like we do for
	 * persistent clients; moreover, the HashTable should already be empty. */
	if (MONGODB_G(request_clients)) {
		zend_hash_destroy(MONGODB_G(request_clients));
		FREE_HASHTABLE(MONGODB_G(request_clients));
		MONGODB_G(request_clients) = NULL;
	}

	/* Destroy HashTable for Managers, which was initialized in RINIT. */
	if (MONGODB_G(managers)) {
		zend_hash_destroy(MONGODB_G(managers));
		FREE_HASHTABLE(MONGODB_G(managers));
		MONGODB_G(managers) = NULL;
	}

	return SUCCESS;
} /* }}} */

PHP_GSHUTDOWN_FUNCTION(mongodb) /* {{{ */
{
	/* Destroy persistent client HashTable in reverse order. This is necessary
	 * to prevent segmentation faults as clients may reference other clients in
	 * encryption settings. */
	zend_hash_graceful_reverse_destroy(&mongodb_globals->persistent_clients);

	phongo_log_disable(mongodb_globals->debug_fd);
	mongodb_globals->debug_fd = NULL;

	/* Decrement the thread counter. If it reaches zero, we can infer that this
	 * is the last thread, MSHUTDOWN has been called, persistent clients from
	 * all threads have been destroyed, and it is now safe to shutdown libmongoc
	 * and restore libbson's original vtable. */
	if (bson_atomic_int32_fetch_sub(&phongo_num_threads, 1, bson_memory_order_seq_cst) - 1 == 0) {
		mongoc_cleanup();
		bson_mem_restore_vtable();
	}
} /* }}} */

PHP_MINFO_FUNCTION(mongodb) /* {{{ */
{
	php_info_print_table_start();
	php_info_print_table_header(2, "MongoDB support", "enabled");
	php_info_print_table_row(2, "MongoDB extension version", PHP_MONGODB_VERSION);
	php_info_print_table_row(2, "MongoDB extension stability", PHP_MONGODB_STABILITY);

#ifdef HAVE_SYSTEM_LIBBSON
	php_info_print_table_row(2, "libbson headers version", BSON_VERSION_S);
	php_info_print_table_row(2, "libbson library version", bson_get_version());
#else
	php_info_print_table_row(2, "libbson bundled version", BSON_VERSION_S);
#endif

#ifdef HAVE_SYSTEM_LIBMONGOC
	php_info_print_table_row(2, "libmongoc headers version", MONGOC_VERSION_S);
	php_info_print_table_row(2, "libmongoc library version", mongoc_get_version());
#else
	/* Bundled libraries, buildtime = runtime */
	php_info_print_table_row(2, "libmongoc bundled version", MONGOC_VERSION_S);
#endif

#ifdef MONGOC_ENABLE_SSL
	php_info_print_table_row(2, "libmongoc SSL", "enabled");
#if defined(MONGOC_ENABLE_SSL_OPENSSL)
	php_info_print_table_row(2, "libmongoc SSL library", "OpenSSL");
#elif defined(MONGOC_ENABLE_SSL_LIBRESSL)
	php_info_print_table_row(2, "libmongoc SSL library", "LibreSSL");
#elif defined(MONGOC_ENABLE_SSL_SECURE_TRANSPORT)
	php_info_print_table_row(2, "libmongoc SSL library", "Secure Transport");
#elif defined(MONGOC_ENABLE_SSL_SECURE_CHANNEL)
	php_info_print_table_row(2, "libmongoc SSL library", "Secure Channel");
#else
	php_info_print_table_row(2, "libmongoc SSL library", "unknown");
#endif
#else /* MONGOC_ENABLE_SSL */
	php_info_print_table_row(2, "libmongoc SSL", "disabled");
#endif

#ifdef MONGOC_ENABLE_CRYPTO
	php_info_print_table_row(2, "libmongoc crypto", "enabled");
#if defined(MONGOC_ENABLE_CRYPTO_LIBCRYPTO)
	php_info_print_table_row(2, "libmongoc crypto library", "libcrypto");
#elif defined(MONGOC_ENABLE_CRYPTO_COMMON_CRYPTO)
	php_info_print_table_row(2, "libmongoc crypto library", "Common Crypto");
#elif defined(MONGOC_ENABLE_CRYPTO_CNG)
	php_info_print_table_row(2, "libmongoc crypto library", "CNG");
#else
	php_info_print_table_row(2, "libmongoc crypto library", "unknown");
#endif
#ifdef MONGOC_ENABLE_CRYPTO_SYSTEM_PROFILE
	php_info_print_table_row(2, "libmongoc crypto system profile", "enabled");
#else
	php_info_print_table_row(2, "libmongoc crypto system profile", "disabled");
#endif
#else /* MONGOC_ENABLE_CRYPTO */
	php_info_print_table_row(2, "libmongoc crypto", "disabled");
#endif

#ifdef MONGOC_ENABLE_SASL
	php_info_print_table_row(2, "libmongoc SASL", "enabled");
#else
	php_info_print_table_row(2, "libmongoc SASL", "disabled");
#endif

#ifdef MONGOC_ENABLE_ICU
	php_info_print_table_row(2, "libmongoc ICU", "enabled");
#else
	php_info_print_table_row(2, "libmongoc ICU", "disabled");
#endif

#ifdef MONGOC_ENABLE_COMPRESSION
	php_info_print_table_row(2, "libmongoc compression", "enabled");
#ifdef MONGOC_ENABLE_COMPRESSION_SNAPPY
	php_info_print_table_row(2, "libmongoc compression snappy", "enabled");
#else
	php_info_print_table_row(2, "libmongoc compression snappy", "disabled");
#endif
#ifdef MONGOC_ENABLE_COMPRESSION_ZLIB
	php_info_print_table_row(2, "libmongoc compression zlib", "enabled");
#else
	php_info_print_table_row(2, "libmongoc compression zlib", "disabled");
#endif
#ifdef MONGOC_ENABLE_COMPRESSION_ZSTD
	php_info_print_table_row(2, "libmongoc compression zstd", "enabled");
#else
	php_info_print_table_row(2, "libmongoc compression zstd", "disabled");
#endif
#else /* MONGOC_ENABLE_COMPRESSION */
	php_info_print_table_row(2, "libmongoc compression", "disabled");
#endif

#ifdef MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION
#ifdef HAVE_SYSTEM_LIBMONGOCRYPT
	php_info_print_table_row(2, "libmongocrypt headers version", MONGOCRYPT_VERSION);
	php_info_print_table_row(2, "libmongocrypt library version", mongocrypt_version(NULL));
#else
	php_info_print_table_row(2, "libmongocrypt bundled version", MONGOCRYPT_VERSION);
#endif

#ifdef MONGOCRYPT_ENABLE_CRYPTO
	php_info_print_table_row(2, "libmongocrypt crypto", "enabled");

#if defined(MONGOCRYPT_ENABLE_CRYPTO_LIBCRYPTO)
	php_info_print_table_row(2, "libmongocrypt crypto library", "libcrypto");
#elif defined(MONGOCRYPT_ENABLE_CRYPTO_COMMON_CRYPTO)
	php_info_print_table_row(2, "libmongocrypt crypto library", "Common Crypto");
#elif defined(MONGOCRYPT_ENABLE_CRYPTO_CNG)
	php_info_print_table_row(2, "libmongocrypt crypto library", "CNG");
#else
	php_info_print_table_row(2, "libmongocrypt crypto library", "unknown");
#endif
#else /* MONGOCRYPT_ENABLE_CRYPTO */
	php_info_print_table_row(2, "libmongocrypt crypto", "disabled");
#endif

	{
		const char* crypt_shared_version = php_phongo_crypt_shared_version();

		if (crypt_shared_version) {
			php_info_print_table_row(2, "crypt_shared library version", crypt_shared_version);
		} else {
			php_info_print_table_row(2, "crypt_shared library version", "unknown");
		}
	}

#else /* MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION */
	php_info_print_table_row(2, "libmongocrypt", "disabled");
#endif

	php_info_print_table_end();

	phongo_display_ini_entries(ZEND_MODULE_INFO_FUNC_ARGS_PASSTHRU);
} /* }}} */

/* {{{ Shared function entries for disabling constructors and unserialize() */
PHP_FUNCTION(MongoDB_disabled___construct) /* {{{ */
{
	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Accessing private constructor");
} /* }}} */

PHP_FUNCTION(MongoDB_disabled___wakeup) /* {{{ */
{
	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "%s", "MongoDB\\Driver objects cannot be serialized");
} /* }}} */
/* }}} */

/* {{{ Module dependencies and module entry */
static const zend_module_dep mongodb_deps[] = {
	/* clang-format off */
	ZEND_MOD_REQUIRED("date")
	ZEND_MOD_REQUIRED("json")
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_REQUIRED("standard")
	ZEND_MOD_END
	/* clang-format on */
};

zend_module_entry mongodb_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	mongodb_deps,
	"mongodb",
	ext_functions,
	PHP_MINIT(mongodb),
	PHP_MSHUTDOWN(mongodb),
	PHP_RINIT(mongodb),
	PHP_RSHUTDOWN(mongodb),
	PHP_MINFO(mongodb),
	PHP_MONGODB_VERSION,
	PHP_MODULE_GLOBALS(mongodb),
	PHP_GINIT(mongodb),
	PHP_GSHUTDOWN(mongodb),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_MONGODB
ZEND_GET_MODULE(mongodb)
#endif
