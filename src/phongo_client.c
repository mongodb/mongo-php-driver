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
#include <ext/standard/php_var.h>
#include <Zend/zend_smart_str.h>

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_apm.h"
#include "phongo_bson_encode.h"
#include "phongo_client.h"
#include "phongo_error.h"
#include "phongo_util.h"

#include "MongoDB/ReadPreference.h"
#include "MongoDB/WriteConcern.h"

ZEND_EXTERN_MODULE_GLOBALS(mongodb)

#define PHONGO_METADATA_SEPARATOR " / "
#define PHONGO_METADATA_SEPARATOR_LEN (sizeof(PHONGO_METADATA_SEPARATOR) - 1)
#define PHONGO_METADATA_PHP_VERSION_PREFIX "PHP "
#define PHONGO_METADATA_PHP_VERSION_PREFIX_LEN (sizeof(PHONGO_METADATA_PHP_VERSION_PREFIX) - 1)

/* Structure for tracking libmongoc clients (both persisted and non-persisted).
 * The PID is included to ensure that processes do not destroy clients created
 * by other processes (relevant for forking). We avoid using pid_t for Windows
 * compatibility. */
typedef struct {
	mongoc_client_t* client;
	int              created_by_pid;
	int              last_reset_by_pid;
	bool             is_persistent;
} php_phongo_pclient_t;

static const mongoc_client_t* get_first_pclient_client(HashTable* ht)
{
	if (ht) {
		php_phongo_pclient_t* pclient = NULL;

		zend_hash_internal_pointer_reset(ht);
		pclient = zend_hash_get_current_data_ptr(ht);

		if (pclient) {
			return pclient->client;
		}
	}

	return NULL;
}

/* Returns the version of the crypt_shared library, or NULL if it's unavailable.
 * Querying the version requires a mongoc_client_t pointer. Since the shared
 * library can only be loaded once, any client will return the same result so we
 * consult the first persistent or request-scoped client we can find.
 *
 * Note: this may incorrectly return NULL if crypt_shared was loaded through a
 * mongoc_client_t since destroyed (e.g. single requested-scoped client);
 * however, that's the best can do with libmongoc's API. */
const char* php_phongo_crypt_shared_version(void)
{
	const mongoc_client_t* client = NULL;

	client = get_first_pclient_client(&MONGODB_G(persistent_clients));

	if (!client) {
		client = get_first_pclient_client(MONGODB_G(request_clients));
	}

	if (client) {
		return mongoc_client_get_crypt_shared_version(client);
	}

	return NULL;
}

static mongoc_uri_t* php_phongo_make_uri(const char* uri_string)
{
	mongoc_uri_t* uri;
	bson_error_t  error = { 0 };

	uri = mongoc_uri_new_with_error(uri_string, &error);
	MONGOC_DEBUG("Connection string: '%s'", uri_string);

	if (!uri) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse MongoDB URI: '%s'. %s.", uri_string, error.message);
		return NULL;
	}

	return uri;
}

#define PHONGO_URI_INVALID_TYPE(iter, expected)        \
	phongo_throw_exception(                            \
		PHONGO_ERROR_INVALID_ARGUMENT,                 \
		"Expected %s for \"%s\" URI option, %s given", \
		(expected),                                    \
		bson_iter_key(&(iter)),                        \
		php_phongo_bson_type_to_string(bson_iter_type(&(iter))))

static bool php_phongo_apply_options_to_uri(mongoc_uri_t* uri, bson_t* options)
{
	bson_iter_t iter;

	/* Return early if there are no options to apply */
	if (bson_empty0(options) || !bson_iter_init(&iter, options)) {
		return true;
	}

	while (bson_iter_next(&iter)) {
		const char* key = bson_iter_key(&iter);

		/* Skip read preference, read concern, and write concern options, as
		 * those will be processed by other functions. */
		if (!strcasecmp(key, MONGOC_URI_JOURNAL) ||
			!strcasecmp(key, MONGOC_URI_MAXSTALENESSSECONDS) ||
			!strcasecmp(key, MONGOC_URI_READCONCERNLEVEL) ||
			!strcasecmp(key, MONGOC_URI_READPREFERENCE) ||
			!strcasecmp(key, MONGOC_URI_READPREFERENCETAGS) ||
			!strcasecmp(key, MONGOC_URI_SAFE) ||
			!strcasecmp(key, MONGOC_URI_W) ||
			!strcasecmp(key, MONGOC_URI_WTIMEOUTMS)) {

			continue;
		}

		if (mongoc_uri_option_is_bool(key)) {
			/* The option's type is not validated because bson_iter_as_bool() is
			 * used to cast the value to a boolean. Validation may be introduced
			 * in PHPC-990. */
			if (!mongoc_uri_set_option_as_bool(uri, key, bson_iter_as_bool(&iter))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" URI option", key);
				return false;
			}

			continue;
		}

		if (mongoc_uri_option_is_int32(key)) {
			if (!BSON_ITER_HOLDS_INT32(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "32-bit integer");
				return false;
			}

			if (!mongoc_uri_set_option_as_int32(uri, key, bson_iter_int32(&iter))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" URI option", key);
				return false;
			}

			continue;
		}

		if (mongoc_uri_option_is_utf8(key)) {
			if (!BSON_ITER_HOLDS_UTF8(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "string");
				return false;
			}

			if (!strcasecmp(key, MONGOC_URI_REPLICASET) && !strcmp("", bson_iter_utf8(&iter, NULL))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Value for URI option \"%s\" cannot be empty string.", key);
				return false;
			}

			if (!mongoc_uri_set_option_as_utf8(uri, key, bson_iter_utf8(&iter, NULL))) {
				/* Assignment uses mongoc_uri_set_appname() for the "appname"
				 * option, which validates length in addition to UTF-8 encoding.
				 * For BC, we report the invalid string to the user. */
				if (!strcasecmp(key, MONGOC_URI_APPNAME)) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid appname value: '%s'", bson_iter_utf8(&iter, NULL));
				} else {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" URI option", key);
				}
				return false;
			}

			continue;
		}

		if (!strcasecmp(key, "username")) {
			if (!BSON_ITER_HOLDS_UTF8(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "string");
				return false;
			}

			if (!mongoc_uri_set_username(uri, bson_iter_utf8(&iter, NULL))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" URI option", key);
				return false;
			}

			continue;
		}

		if (!strcasecmp(key, "password")) {
			if (!BSON_ITER_HOLDS_UTF8(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "string");
				return false;
			}

			if (!mongoc_uri_set_password(uri, bson_iter_utf8(&iter, NULL))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" URI option", key);
				return false;
			}

			continue;
		}

		if (!strcasecmp(key, MONGOC_URI_AUTHMECHANISM)) {
			if (!BSON_ITER_HOLDS_UTF8(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "string");
				return false;
			}

			if (!mongoc_uri_set_auth_mechanism(uri, bson_iter_utf8(&iter, NULL))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" URI option", key);
				return false;
			}

			continue;
		}

		if (!strcasecmp(key, MONGOC_URI_AUTHSOURCE)) {
			if (!BSON_ITER_HOLDS_UTF8(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "string");
				return false;
			}

			if (!mongoc_uri_set_auth_source(uri, bson_iter_utf8(&iter, NULL))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" URI option", key);
				return false;
			}

			continue;
		}

		if (!strcasecmp(key, MONGOC_URI_AUTHMECHANISMPROPERTIES)) {
			bson_t         properties;
			uint32_t       len;
			const uint8_t* data;

			if (!BSON_ITER_HOLDS_DOCUMENT(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "array or object");
				return false;
			}

			bson_iter_document(&iter, &len, &data);

			if (!bson_init_static(&properties, data, len)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Could not initialize BSON structure for auth mechanism properties");
				return false;
			}

			if (!mongoc_uri_set_mechanism_properties(uri, &properties)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" URI option", key);
				return false;
			}

			continue;
		}

		if (!strcasecmp(key, MONGOC_URI_GSSAPISERVICENAME)) {
			bson_t unused, properties = BSON_INITIALIZER;

			if (mongoc_uri_get_mechanism_properties(uri, &unused)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "authMechanismProperties SERVICE_NAME already set, ignoring \"%s\"", key);
				return false;
			}

			if (!BSON_ITER_HOLDS_UTF8(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "string");
				return false;
			}

			bson_append_utf8(&properties, "SERVICE_NAME", -1, bson_iter_utf8(&iter, NULL), -1);

			if (!mongoc_uri_set_mechanism_properties(uri, &properties)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" URI option", key);
				bson_destroy(&properties);
				return false;
			}

			bson_destroy(&properties);

			continue;
		}

		if (!strcasecmp(key, MONGOC_URI_COMPRESSORS)) {
			if (!BSON_ITER_HOLDS_UTF8(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "string");
				return false;
			}

			if (!mongoc_uri_set_compressors(uri, bson_iter_utf8(&iter, NULL))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" URI option", key);
				return false;
			}

			continue;
		}
	}

	return true;
}

static bool php_phongo_apply_rc_options_to_uri(mongoc_uri_t* uri, bson_t* options)
{
	bson_iter_t                  iter;
	mongoc_read_concern_t*       new_rc;
	const mongoc_read_concern_t* old_rc;

	if (!(old_rc = mongoc_uri_get_read_concern(uri))) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED, "mongoc_uri_t does not have a read concern");

		return false;
	}

	/* Return early if there are no options to apply */
	if (bson_empty0(options) || !bson_iter_init(&iter, options)) {
		return true;
	}

	new_rc = mongoc_read_concern_copy(old_rc);

	while (bson_iter_next(&iter)) {
		const char* key = bson_iter_key(&iter);

		if (!strcasecmp(key, MONGOC_URI_READCONCERNLEVEL)) {
			if (!BSON_ITER_HOLDS_UTF8(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "string");
				mongoc_read_concern_destroy(new_rc);

				return false;
			}

			mongoc_read_concern_set_level(new_rc, bson_iter_utf8(&iter, NULL));
		}
	}

	mongoc_uri_set_read_concern(uri, new_rc);
	mongoc_read_concern_destroy(new_rc);

	return true;
}

static bool php_phongo_apply_rp_options_to_uri(mongoc_uri_t* uri, bson_t* options)
{
	bson_iter_t                iter;
	mongoc_read_prefs_t*       new_rp;
	const mongoc_read_prefs_t* old_rp;

	if (!(old_rp = mongoc_uri_get_read_prefs_t(uri))) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED, "mongoc_uri_t does not have a read preference");

		return false;
	}

	/* Return early if there are no options to apply */
	if (bson_empty0(options) || !bson_iter_init(&iter, options)) {
		return true;
	}

	new_rp = mongoc_read_prefs_copy(old_rp);

	while (bson_iter_next(&iter)) {
		const char* key = bson_iter_key(&iter);

		if (!strcasecmp(key, MONGOC_URI_READPREFERENCE)) {
			const char* str;

			if (!BSON_ITER_HOLDS_UTF8(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "string");
				mongoc_read_prefs_destroy(new_rp);

				return false;
			}

			str = bson_iter_utf8(&iter, NULL);

			if (0 == strcasecmp("primary", str)) {
				mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_PRIMARY);
			} else if (0 == strcasecmp("primarypreferred", str)) {
				mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_PRIMARY_PREFERRED);
			} else if (0 == strcasecmp("secondary", str)) {
				mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_SECONDARY);
			} else if (0 == strcasecmp("secondarypreferred", str)) {
				mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_SECONDARY_PREFERRED);
			} else if (0 == strcasecmp("nearest", str)) {
				mongoc_read_prefs_set_mode(new_rp, MONGOC_READ_NEAREST);
			} else {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Unsupported %s value: '%s'", bson_iter_key(&iter), str);
				mongoc_read_prefs_destroy(new_rp);

				return false;
			}
		}

		if (!strcasecmp(key, MONGOC_URI_READPREFERENCETAGS)) {
			bson_t         tags;
			uint32_t       len;
			const uint8_t* data;

			if (!BSON_ITER_HOLDS_ARRAY(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "array");
				mongoc_read_prefs_destroy(new_rp);

				return false;
			}

			bson_iter_array(&iter, &len, &data);

			if (!bson_init_static(&tags, data, len)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Could not initialize BSON structure for read preference tags");
				mongoc_read_prefs_destroy(new_rp);

				return false;
			}

			if (!php_phongo_read_preference_tags_are_valid(&tags)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Read preference tags must be an array of zero or more documents");
				mongoc_read_prefs_destroy(new_rp);

				return false;
			}

			mongoc_read_prefs_set_tags(new_rp, &tags);
		}

		if (!strcasecmp(key, MONGOC_URI_MAXSTALENESSSECONDS)) {
			int64_t max_staleness_seconds;

			if (!BSON_ITER_HOLDS_INT(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "integer");
				mongoc_read_prefs_destroy(new_rp);

				return false;
			}

			max_staleness_seconds = bson_iter_as_int64(&iter);

			if (max_staleness_seconds != MONGOC_NO_MAX_STALENESS) {

				if (max_staleness_seconds < MONGOC_SMALLEST_MAX_STALENESS_SECONDS) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected maxStalenessSeconds to be >= %d, %" PRId64 " given", MONGOC_SMALLEST_MAX_STALENESS_SECONDS, max_staleness_seconds);
					mongoc_read_prefs_destroy(new_rp);

					return false;
				}

				if (max_staleness_seconds > INT32_MAX) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected maxStalenessSeconds to be <= %d, %" PRId64 " given", INT32_MAX, max_staleness_seconds);
					mongoc_read_prefs_destroy(new_rp);

					return false;
				}

				if (mongoc_read_prefs_get_mode(new_rp) == MONGOC_READ_PRIMARY) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Primary read preference mode conflicts with maxStalenessSeconds");
					mongoc_read_prefs_destroy(new_rp);

					return false;
				}
			}

			mongoc_read_prefs_set_max_staleness_seconds(new_rp, max_staleness_seconds);
		}
	}

	if (mongoc_read_prefs_get_mode(new_rp) == MONGOC_READ_PRIMARY &&
		!bson_empty(mongoc_read_prefs_get_tags(new_rp))) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Primary read preference mode conflicts with tags");
		mongoc_read_prefs_destroy(new_rp);

		return false;
	}

	/* Make sure maxStalenessSeconds is not combined with primary readPreference */
	if (mongoc_read_prefs_get_mode(new_rp) == MONGOC_READ_PRIMARY &&
		mongoc_read_prefs_get_max_staleness_seconds(new_rp) != MONGOC_NO_MAX_STALENESS) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Primary read preference mode conflicts with maxStalenessSeconds");
		mongoc_read_prefs_destroy(new_rp);

		return false;
	}

	/* This may be redundant in light of the previous checks (primary with tags
	 * or maxStalenessSeconds), but we'll check anyway in case additional
	 * validation is implemented. */
	if (!mongoc_read_prefs_is_valid(new_rp)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Read preference is not valid");
		mongoc_read_prefs_destroy(new_rp);

		return false;
	}

	mongoc_uri_set_read_prefs_t(uri, new_rp);
	mongoc_read_prefs_destroy(new_rp);

	return true;
}

static bool php_phongo_apply_wc_options_to_uri(mongoc_uri_t* uri, bson_t* options)
{
	bson_iter_t                   iter;
	mongoc_write_concern_t*       new_wc;
	const mongoc_write_concern_t* old_wc;
	bool                          ignore_safe = false;

	if (!(old_wc = mongoc_uri_get_write_concern(uri))) {
		phongo_throw_exception(PHONGO_ERROR_MONGOC_FAILED, "mongoc_uri_t does not have a write concern");

		return false;
	}

	/* Return early if there are no options to apply */
	if (bson_empty0(options) || !bson_iter_init(&iter, options)) {
		return true;
	}

	new_wc = mongoc_write_concern_copy(old_wc);

	while (bson_iter_next(&iter)) {
		const char* key = bson_iter_key(&iter);

		if (!ignore_safe && !strcasecmp(key, MONGOC_URI_SAFE)) {
			if (!BSON_ITER_HOLDS_BOOL(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "boolean");
				mongoc_write_concern_destroy(new_wc);

				return false;
			}

			mongoc_write_concern_set_w(new_wc, bson_iter_bool(&iter) ? 1 : MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED);
		}

		if (!strcasecmp(key, MONGOC_URI_WTIMEOUTMS)) {
			int64_t wtimeout;

			if (!BSON_ITER_HOLDS_INT(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "integer");
				mongoc_write_concern_destroy(new_wc);

				return false;
			}

			wtimeout = bson_iter_as_int64(&iter);

			if (wtimeout < 0) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected wtimeoutMS to be >= 0, %" PRId64 " given", wtimeout);
				mongoc_write_concern_destroy(new_wc);

				return false;
			}

			mongoc_write_concern_set_wtimeout_int64(new_wc, wtimeout);
		}

		if (!strcasecmp(key, MONGOC_URI_JOURNAL)) {
			if (!BSON_ITER_HOLDS_BOOL(&iter)) {
				PHONGO_URI_INVALID_TYPE(iter, "boolean");
				mongoc_write_concern_destroy(new_wc);

				return false;
			}

			mongoc_write_concern_set_journal(new_wc, bson_iter_bool(&iter));
		}

		if (!strcasecmp(key, MONGOC_URI_W)) {
			if (BSON_ITER_HOLDS_INT32(&iter)) {
				int32_t value = bson_iter_int32(&iter);

				switch (value) {
					case MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED:
					case MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED:
						mongoc_write_concern_set_w(new_wc, value);
						break;

					default:
						if (value > 0) {
							mongoc_write_concern_set_w(new_wc, value);
							break;
						}
						phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Unsupported w value: %d", value);
						mongoc_write_concern_destroy(new_wc);

						return false;
				}
			} else if (BSON_ITER_HOLDS_UTF8(&iter)) {
				const char* str = bson_iter_utf8(&iter, NULL);

				if (0 == strcasecmp(PHONGO_WRITE_CONCERN_W_MAJORITY, str)) {
					mongoc_write_concern_set_w(new_wc, MONGOC_WRITE_CONCERN_W_MAJORITY);
				} else {
					mongoc_write_concern_set_wtag(new_wc, str);
				}
			} else {
				PHONGO_URI_INVALID_TYPE(iter, "32-bit integer or string");
				mongoc_write_concern_destroy(new_wc);

				return false;
			}

			ignore_safe = true;
		}
	}

	if (mongoc_write_concern_get_journal(new_wc)) {
		int32_t w = mongoc_write_concern_get_w(new_wc);

		if (w == MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED || w == MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Journal conflicts with w value: %d", w);
			mongoc_write_concern_destroy(new_wc);

			return false;
		}
	}

	/* This may be redundant in light of the last check (unacknowledged w with
	   journal), but we'll check anyway in case additional validation is
	   implemented. */
	if (!mongoc_write_concern_is_valid(new_wc)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Write concern is not valid");
		mongoc_write_concern_destroy(new_wc);

		return false;
	}

	mongoc_uri_set_write_concern(uri, new_wc);
	mongoc_write_concern_destroy(new_wc);

	return true;
}

#ifdef MONGOC_ENABLE_SSL
static void php_phongo_mongoc_ssl_opts_from_uri(mongoc_ssl_opt_t* ssl_opt, mongoc_uri_t* uri, bool* any_ssl_option_set)
{
	bool        insecure = mongoc_uri_get_option_as_bool(uri, MONGOC_URI_TLSINSECURE, false);
	const char* pem_file = mongoc_uri_get_option_as_utf8(uri, MONGOC_URI_TLSCERTIFICATEKEYFILE, NULL);
	const char* pem_pwd  = mongoc_uri_get_option_as_utf8(uri, MONGOC_URI_TLSCERTIFICATEKEYFILEPASSWORD, NULL);
	const char* ca_file  = mongoc_uri_get_option_as_utf8(uri, MONGOC_URI_TLSCAFILE, NULL);

	ssl_opt->pem_file               = pem_file ? estrdup(pem_file) : NULL;
	ssl_opt->pem_pwd                = pem_pwd ? estrdup(pem_pwd) : NULL;
	ssl_opt->ca_file                = ca_file ? estrdup(ca_file) : NULL;
	ssl_opt->weak_cert_validation   = mongoc_uri_get_option_as_bool(uri, MONGOC_URI_TLSALLOWINVALIDCERTIFICATES, insecure);
	ssl_opt->allow_invalid_hostname = mongoc_uri_get_option_as_bool(uri, MONGOC_URI_TLSALLOWINVALIDHOSTNAMES, insecure);

	/* Boolean options default to false, so we cannot consider them for
	 * any_ssl_option_set. This isn't actually a problem as libmongoc will
	 * already have assigned them when creating the client, enabling SSL, and
	 * assigning SSL options. Therefore, we only need to check for non-defaults
	 * (i.e. non-NULL strings, true booleans). */
	if (pem_file || pem_pwd || ca_file || ssl_opt->weak_cert_validation || ssl_opt->allow_invalid_hostname) {
		*any_ssl_option_set = true;
	}
}

static inline char* php_phongo_fetch_ssl_opt_string(zval* zoptions, const char* key)
{
	int       plen;
	zend_bool pfree;
	char*     pval;
	char*     value;

	pval  = php_array_fetch_string(zoptions, key, &plen, &pfree);
	value = pfree ? pval : estrndup(pval, plen);

	return value;
}

static mongoc_ssl_opt_t* php_phongo_make_ssl_opt(mongoc_uri_t* uri, zval* zoptions)
{
	mongoc_ssl_opt_t* ssl_opt;
	bool              any_ssl_option_set = false;

	if (!zoptions) {
		return NULL;
	}

#if defined(MONGOC_ENABLE_SSL_SECURE_CHANNEL) || defined(MONGOC_ENABLE_SSL_SECURE_TRANSPORT)
	if (php_array_existsc(zoptions, "ca_dir")) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "\"ca_dir\" option is not supported by Secure Channel and Secure Transport");
		return NULL;
	}

	if (php_array_existsc(zoptions, "capath")) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "\"capath\" option is not supported by Secure Channel and Secure Transport");
		return NULL;
	}
#endif

#if defined(MONGOC_ENABLE_SSL_LIBRESSL) || defined(MONGOC_ENABLE_SSL_SECURE_TRANSPORT)
	if (php_array_existsc(zoptions, "crl_file")) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "\"crl_file\" option is not supported by LibreSSL and Secure Transport");
		return NULL;
	}
#endif

	ssl_opt = ecalloc(1, sizeof(mongoc_ssl_opt_t));

	/* If SSL options are set in the URL, we need to read them and set them on
	 * the options struct so we can merge potential options from passed in
	 * driverOptions (zoptions) */
	if (mongoc_uri_get_tls(uri)) {
		php_phongo_mongoc_ssl_opts_from_uri(ssl_opt, uri, &any_ssl_option_set);
	}

#define PHONGO_SSL_OPTION_SWAP_STRING(o, n) \
	if ((o)) {                              \
		efree((char*) (o));                 \
	}                                       \
	(o) = php_phongo_fetch_ssl_opt_string(zoptions, n);

	/* Apply driver options that don't have a corresponding URI option. These
	 * are set directly on the SSL options struct. */
	if (php_array_existsc(zoptions, "ca_dir")) {
		PHONGO_SSL_OPTION_SWAP_STRING(ssl_opt->ca_dir, "ca_dir");
		any_ssl_option_set = true;
	} else if (php_array_existsc(zoptions, "capath")) {
		PHONGO_SSL_OPTION_SWAP_STRING(ssl_opt->ca_dir, "capath");
		any_ssl_option_set = true;

		php_error_docref(NULL, E_DEPRECATED, "The \"capath\" context driver option is deprecated. Please use the \"ca_dir\" driver option instead.");
	}

	if (php_array_existsc(zoptions, "crl_file")) {
		PHONGO_SSL_OPTION_SWAP_STRING(ssl_opt->crl_file, "crl_file");
		any_ssl_option_set = true;
	}

#undef PHONGO_SSL_OPTION_SWAP_STRING

	if (!any_ssl_option_set) {
		efree(ssl_opt);
		return NULL;
	}

	return ssl_opt;
}

static void php_phongo_free_ssl_opt(mongoc_ssl_opt_t* ssl_opt)
{
	if (ssl_opt->pem_file) {
		efree((char*) ssl_opt->pem_file);
	}

	if (ssl_opt->pem_pwd) {
		efree((char*) ssl_opt->pem_pwd);
	}

	if (ssl_opt->ca_file) {
		efree((char*) ssl_opt->ca_file);
	}

	if (ssl_opt->ca_dir) {
		efree((char*) ssl_opt->ca_dir);
	}

	if (ssl_opt->crl_file) {
		efree((char*) ssl_opt->crl_file);
	}

	efree(ssl_opt);
}

static inline bool php_phongo_apply_driver_option_to_uri(mongoc_uri_t* uri, zval* zoptions, const char* driverOptionKey, const char* optionKey)
{
	bool  ret;
	char* value;

	value = php_phongo_fetch_ssl_opt_string(zoptions, driverOptionKey);
	ret   = mongoc_uri_set_option_as_utf8(uri, optionKey, value);
	efree(value);

	return ret;
}

static bool php_phongo_apply_driver_options_to_uri(mongoc_uri_t* uri, zval* zoptions)
{
	if (!zoptions) {
		return true;
	}

	/* Map TLS driver options to the canonical tls options in the URI. */
	if (php_array_existsc(zoptions, "allow_invalid_hostname")) {
		if (!mongoc_uri_set_option_as_bool(uri, MONGOC_URI_TLSALLOWINVALIDHOSTNAMES, php_array_fetchc_bool(zoptions, "allow_invalid_hostname"))) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" driver option", "allow_invalid_hostname");

			return false;
		}

		php_error_docref(NULL, E_DEPRECATED, "The \"allow_invalid_hostname\" driver option is deprecated. Please use the \"tlsAllowInvalidHostnames\" URI option instead.");
	}

	if (php_array_existsc(zoptions, "weak_cert_validation")) {
		if (!mongoc_uri_set_option_as_bool(uri, MONGOC_URI_TLSALLOWINVALIDCERTIFICATES, php_array_fetchc_bool(zoptions, "weak_cert_validation"))) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" driver option", "weak_cert_validation");

			return false;
		}

		php_error_docref(NULL, E_DEPRECATED, "The \"weak_cert_validation\" driver option is deprecated. Please use the \"tlsAllowInvalidCertificates\" URI option instead.");
	} else if (php_array_existsc(zoptions, "allow_self_signed")) {
		if (!mongoc_uri_set_option_as_bool(uri, MONGOC_URI_TLSALLOWINVALIDCERTIFICATES, php_array_fetchc_bool(zoptions, "allow_self_signed"))) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" driver option", "allow_self_signed");

			return false;
		}

		php_error_docref(NULL, E_DEPRECATED, "The \"allow_self_signed\" context driver option is deprecated. Please use the \"tlsAllowInvalidCertificates\" URI option instead.");
	}

	if (php_array_existsc(zoptions, "pem_file")) {
		if (!php_phongo_apply_driver_option_to_uri(uri, zoptions, "pem_file", MONGOC_URI_TLSCERTIFICATEKEYFILE)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" driver option", "pem_file");

			return false;
		}

		php_error_docref(NULL, E_DEPRECATED, "The \"pem_file\" driver option is deprecated. Please use the \"tlsCertificateKeyFile\" URI option instead.");
	} else if (php_array_existsc(zoptions, "local_cert")) {
		if (!php_phongo_apply_driver_option_to_uri(uri, zoptions, "local_cert", MONGOC_URI_TLSCERTIFICATEKEYFILE)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" driver option", "local_cert");

			return false;
		}

		php_error_docref(NULL, E_DEPRECATED, "The \"local_cert\" context driver option is deprecated. Please use the \"tlsCertificateKeyFile\" URI option instead.");
	}

	if (php_array_existsc(zoptions, "pem_pwd")) {
		if (!php_phongo_apply_driver_option_to_uri(uri, zoptions, "pem_pwd", MONGOC_URI_TLSCERTIFICATEKEYFILEPASSWORD)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" driver option", "pem_pwd");

			return false;
		}

		php_error_docref(NULL, E_DEPRECATED, "The \"pem_pwd\" driver option is deprecated. Please use the \"tlsCertificateKeyFilePassword\" URI option instead.");
	} else if (php_array_existsc(zoptions, "passphrase")) {
		if (!php_phongo_apply_driver_option_to_uri(uri, zoptions, "passphrase", MONGOC_URI_TLSCERTIFICATEKEYFILEPASSWORD)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" driver option", "passphrase");

			return false;
		}

		php_error_docref(NULL, E_DEPRECATED, "The \"passphrase\" context driver option is deprecated. Please use the \"tlsCertificateKeyFilePassword\" URI option instead.");
	}

	if (php_array_existsc(zoptions, "ca_file")) {
		if (!php_phongo_apply_driver_option_to_uri(uri, zoptions, "ca_file", MONGOC_URI_TLSCAFILE)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" driver option", "ca_file");

			return false;
		}

		php_error_docref(NULL, E_DEPRECATED, "The \"ca_file\" driver option is deprecated. Please use the \"tlsCAFile\" URI option instead.");
	} else if (php_array_existsc(zoptions, "cafile")) {
		if (!php_phongo_apply_driver_option_to_uri(uri, zoptions, "cafile", MONGOC_URI_TLSCAFILE)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse \"%s\" driver option", "cafile");

			return false;
		}

		php_error_docref(NULL, E_DEPRECATED, "The \"cafile\" context driver option is deprecated. Please use the \"tlsCAFile\" URI option instead.");
	}

	return true;
}
#endif /* MONGOC_ENABLE_SSL */

static zval* php_phongo_manager_prepare_manager_for_hash(zval* driverOptions, bool* free)
{
	php_phongo_manager_t* manager;
	zval*                 autoEncryptionOpts      = NULL;
	zval*                 keyVaultClient          = NULL;
	zval*                 driverOptionsClone      = NULL;
	zval*                 autoEncryptionOptsClone = NULL;
	zval                  stackAutoEncryptionOptsClone;

	*free = false;

	if (!driverOptions) {
		return NULL;
	}

	if (!php_array_existsc(driverOptions, "autoEncryption")) {
		goto ref;
	}

	autoEncryptionOpts = php_array_fetchc(driverOptions, "autoEncryption");
	if (Z_TYPE_P(autoEncryptionOpts) != IS_ARRAY) {
		goto ref;
	}

	if (!php_array_existsc(autoEncryptionOpts, "keyVaultClient")) {
		goto ref;
	}

	keyVaultClient = php_array_fetchc(autoEncryptionOpts, "keyVaultClient");
	if (Z_TYPE_P(keyVaultClient) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(keyVaultClient), php_phongo_manager_ce)) {
		goto ref;
	}

	*free = true;

	manager = Z_MANAGER_OBJ_P(keyVaultClient);

	driverOptionsClone      = ecalloc(1, sizeof(zval));
	autoEncryptionOptsClone = &stackAutoEncryptionOptsClone;

	ZVAL_DUP(autoEncryptionOptsClone, autoEncryptionOpts);
	ADD_ASSOC_STRINGL(autoEncryptionOptsClone, "keyVaultClient", manager->client_hash, manager->client_hash_len);

	ZVAL_DUP(driverOptionsClone, driverOptions);
	ADD_ASSOC_ZVAL_EX(driverOptionsClone, "autoEncryption", autoEncryptionOptsClone);

	return driverOptionsClone;

ref:
	Z_ADDREF_P(driverOptions);
	return driverOptions;
}

/* Creates a hash for a client by concatenating the URI string with serialized
 * options arrays. On success, a persistent string is returned (i.e. pefree()
 * should be used to free it) and hash_len will be set to the string's length.
 * On error, an exception will have been thrown and NULL will be returned. */
static char* php_phongo_manager_make_client_hash(const char* uri_string, zval* options, zval* driverOptions, size_t* hash_len)
{
	char*                hash    = NULL;
	smart_str            var_buf = { 0 };
	php_serialize_data_t var_hash;
	zval*                serializable_driver_options = NULL;
	bool                 free_driver_options         = false;

	zval args;

	array_init_size(&args, 4);
	ADD_ASSOC_LONG_EX(&args, "pid", getpid());
	ADD_ASSOC_STRING(&args, "uri", uri_string);

	if (options) {
		ADD_ASSOC_ZVAL_EX(&args, "options", options);
		Z_ADDREF_P(options);
	} else {
		ADD_ASSOC_NULL_EX(&args, "options");
	}

	if (driverOptions) {
		serializable_driver_options = php_phongo_manager_prepare_manager_for_hash(driverOptions, &free_driver_options);
		ADD_ASSOC_ZVAL_EX(&args, "driverOptions", serializable_driver_options);
	} else {
		ADD_ASSOC_NULL_EX(&args, "driverOptions");
	}

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&var_buf, &args, &var_hash);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	if (!EG(exception)) {
		*hash_len = ZSTR_LEN(var_buf.s);
		hash      = estrndup(ZSTR_VAL(var_buf.s), *hash_len);
	}

	zval_ptr_dtor(&args);

	if (free_driver_options) {
		efree(serializable_driver_options);
	}

	smart_str_free(&var_buf);

	return hash;
}

static bool php_phongo_extract_handshake_data(zval* driver, const char* key, char** value, size_t* value_len)
{
	zval* zvalue;

	if (!php_array_exists(driver, key)) {
		*value     = NULL;
		*value_len = 0;

		return true;
	}

	zvalue = php_array_fetch(driver, key);

	if (Z_TYPE_P(zvalue) != IS_STRING) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"%s\" handshake option to be a string, %s given", key, PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(zvalue));
		return false;
	}

	*value     = estrdup(Z_STRVAL_P(zvalue));
	*value_len = Z_STRLEN_P(zvalue);

	return true;
}

static char* php_phongo_concat_handshake_data(const char* default_value, const char* custom_value, size_t custom_value_len)
{
	char* ret;
	/* Length of the returned value needs to include a trailing space and null byte */
	size_t ret_len = strlen(default_value) + 2;

	if (custom_value) {
		/* Increase the length by that of the custom value as well as the separator length */
		ret_len += custom_value_len + PHONGO_METADATA_SEPARATOR_LEN;
	}

	ret = ecalloc(ret_len, sizeof(char));

	if (custom_value) {
		snprintf(ret, ret_len, "%s%s%s ", default_value, PHONGO_METADATA_SEPARATOR, custom_value);
	} else {
		snprintf(ret, ret_len, "%s ", default_value);
	}

	return ret;
}

static void php_phongo_handshake_data_append(const char* name, size_t name_len, const char* version, size_t version_len, const char* platform, size_t platform_len)
{
	char*  php_version_string;
	size_t php_version_string_len;
	char*  driver_name;
	char*  driver_version;
	char*  full_platform;

	php_version_string_len = strlen(PHP_VERSION) + PHONGO_METADATA_PHP_VERSION_PREFIX_LEN + 1;
	php_version_string     = ecalloc(php_version_string_len, sizeof(char));
	snprintf(php_version_string, php_version_string_len, "%s%s", PHONGO_METADATA_PHP_VERSION_PREFIX, PHP_VERSION);

	driver_name    = php_phongo_concat_handshake_data("ext-mongodb:PHP", name, name_len);
	driver_version = php_phongo_concat_handshake_data(PHP_MONGODB_VERSION, version, version_len);
	full_platform  = php_phongo_concat_handshake_data(php_version_string, platform, platform_len);

	MONGOC_DEBUG(
		"Setting driver handshake data: { name: '%s', version: '%s', platform: '%s' }",
		driver_name,
		driver_version,
		full_platform);

	mongoc_handshake_data_append(driver_name, driver_version, full_platform);

	efree(php_version_string);
	efree(driver_name);
	efree(driver_version);
	efree(full_platform);
}

static void php_phongo_set_handshake_data(zval* driverOptions)
{
	char*  name         = NULL;
	size_t name_len     = 0;
	char*  version      = NULL;
	size_t version_len  = 0;
	char*  platform     = NULL;
	size_t platform_len = 0;

	if (driverOptions && php_array_existsc(driverOptions, "driver")) {
		zval* driver = php_array_fetchc(driverOptions, "driver");

		if (Z_TYPE_P(driver) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"driver\" driver option to be an array, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(driver));
			return;
		}

		if (!php_phongo_extract_handshake_data(driver, "name", &name, &name_len)) {
			/* Exception already thrown */
			goto cleanup;
		}

		if (!php_phongo_extract_handshake_data(driver, "version", &version, &version_len)) {
			/* Exception already thrown */
			goto cleanup;
		}

		if (!php_phongo_extract_handshake_data(driver, "platform", &platform, &platform_len)) {
			/* Exception already thrown */
			goto cleanup;
		}
	}

	php_phongo_handshake_data_append(name, name_len, version, version_len, platform, platform_len);

cleanup:
	if (name) {
		efree(name);
	}
	if (version) {
		efree(version);
	}
	if (platform) {
		efree(platform);
	}
}

static mongoc_client_t* php_phongo_make_mongo_client(const mongoc_uri_t* uri, zval* driverOptions)
{
	const char *     mongoc_version, *bson_version;
	mongoc_client_t* client;
	bson_error_t     error = { 0 };

#ifdef HAVE_SYSTEM_LIBMONGOC
	mongoc_version = mongoc_get_version();
#else
	mongoc_version = "bundled";
#endif

#ifdef HAVE_SYSTEM_LIBBSON
	bson_version = bson_get_version();
#else
	bson_version   = "bundled";
#endif

	MONGOC_DEBUG(
		"Creating Manager, phongo-%s[%s] - mongoc-%s(%s), libbson-%s(%s), php-%s",
		PHP_MONGODB_VERSION,
		PHP_MONGODB_STABILITY,
		MONGOC_VERSION_S,
		mongoc_version,
		BSON_VERSION_S,
		bson_version,
		PHP_VERSION);

	php_phongo_set_handshake_data(driverOptions);

	if (!(client = mongoc_client_new_from_uri_with_error(uri, &error))) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Failed to parse URI options: %s", error.message);
	}

	return client;
}

/* Adds a client to the appropriate registry. Persistent and request-scoped
 * clients each have their own registries (i.e. HashTables), which use different
 * forms of memory allocation. Both registries are used for PID tracking.
 * Returns true if the client was successfully added; otherwise, false. */
bool php_phongo_client_register(php_phongo_manager_t* manager)
{
	bool                  is_persistent = manager->use_persistent_client;
	php_phongo_pclient_t* pclient       = pecalloc(1, sizeof(php_phongo_pclient_t), is_persistent);

	pclient->client         = manager->client;
	pclient->created_by_pid = (int) getpid();
	pclient->is_persistent  = is_persistent;

	if (is_persistent) {
		MONGOC_DEBUG("Stored persistent client with hash: %s", manager->client_hash);
		return zend_hash_str_update_ptr(&MONGODB_G(persistent_clients), manager->client_hash, manager->client_hash_len, pclient) != NULL;
	} else {
		MONGOC_DEBUG("Stored non-persistent client");
		return zend_hash_next_index_insert_ptr(MONGODB_G(request_clients), pclient) != NULL;
	}
}

/* Removes a client from the request-scoped registry. This function is a NOP for
 * persistent clients, since they are destroyed along with their registry (i.e.
 * HashTable) in GSHUTDOWN. Returns true if the client was successfully removed;
 * otherwise, false. */
bool php_phongo_client_unregister(php_phongo_manager_t* manager)
{
	zend_ulong            index;
	php_phongo_pclient_t* pclient;

	/* Persistent clients do not get unregistered. */
	if (manager->use_persistent_client) {
		MONGOC_DEBUG("Not destroying persistent client for Manager");

		return false;
	}

	/* Ensure the request-scoped registry is initialized. This is needed because
	 * RSHUTDOWN may occur before a Manager's free_object handler is
	 * executed. */
	if (MONGODB_G(request_clients) == NULL) {
		return false;
	}

	ZEND_HASH_FOREACH_NUM_KEY_PTR(MONGODB_G(request_clients), index, pclient)
	{
		if (pclient->client == manager->client) {
			MONGOC_DEBUG("Destroying non-persistent client for Manager");

			return zend_hash_index_del(MONGODB_G(request_clients), index) == SUCCESS;
		}
	}
	ZEND_HASH_FOREACH_END();

	return false;
}

static mongoc_client_t* php_phongo_find_persistent_client(const char* hash, size_t hash_len)
{
	php_phongo_pclient_t* pclient = zend_hash_str_find_ptr(&MONGODB_G(persistent_clients), hash, hash_len);

	if (pclient) {
		return pclient->client;
	}

	return NULL;
}

static bool phongo_manager_set_serverapi_opts(php_phongo_manager_t* manager, zval* driverOptions)
{
	zval*                   zServerApi;
	php_phongo_serverapi_t* server_api;
	bson_error_t            error = { 0 };

	if (!driverOptions || !php_array_existsc(driverOptions, "serverApi")) {
		return true;
	}

	zServerApi = php_array_fetch(driverOptions, "serverApi");

	if (Z_TYPE_P(zServerApi) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(zServerApi), php_phongo_serverapi_ce)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"serverApi\" driver option to be %s, %s given", ZSTR_VAL(php_phongo_serverapi_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(zServerApi));
		return false;
	}

	server_api = Z_SERVERAPI_OBJ_P(zServerApi);

	if (!mongoc_client_set_server_api(manager->client, server_api->server_api, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		return false;
	}

	return true;
}

#ifdef MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION
static bool phongo_manager_set_auto_encryption_opts(php_phongo_manager_t* manager, zval* driverOptions)
{
	zval*                          zAutoEncryptionOpts;
	bson_error_t                   error                = { 0 };
	mongoc_auto_encryption_opts_t* auto_encryption_opts = NULL;
	bool                           retval               = false;

	if (!driverOptions || !php_array_existsc(driverOptions, "autoEncryption")) {
		return true;
	}

	zAutoEncryptionOpts = php_array_fetch(driverOptions, "autoEncryption");

	if (Z_TYPE_P(zAutoEncryptionOpts) != IS_ARRAY) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"autoEncryption\" driver option to be array, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(zAutoEncryptionOpts));
		return false;
	}

	auto_encryption_opts = mongoc_auto_encryption_opts_new();

	if (php_array_existsc(zAutoEncryptionOpts, "bypassAutoEncryption")) {
		mongoc_auto_encryption_opts_set_bypass_auto_encryption(auto_encryption_opts, php_array_fetch_bool(zAutoEncryptionOpts, "bypassAutoEncryption"));
	}

	if (php_array_existsc(zAutoEncryptionOpts, "bypassQueryAnalysis")) {
		mongoc_auto_encryption_opts_set_bypass_query_analysis(auto_encryption_opts, php_array_fetch_bool(zAutoEncryptionOpts, "bypassQueryAnalysis"));
	}

	if (php_array_existsc(zAutoEncryptionOpts, "encryptedFieldsMap")) {
		zval*  enc_fields_map = php_array_fetch(zAutoEncryptionOpts, "encryptedFieldsMap");
		bson_t bson_map       = BSON_INITIALIZER;

		if (Z_TYPE_P(enc_fields_map) != IS_OBJECT && Z_TYPE_P(enc_fields_map) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"encryptedFieldsMap\" autoEncryption option to be an array or object, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(enc_fields_map));
			goto cleanup;
		}

		php_phongo_zval_to_bson(enc_fields_map, PHONGO_BSON_NONE, &bson_map, NULL);
		if (EG(exception)) {
			goto cleanup;
		}

		mongoc_auto_encryption_opts_set_encrypted_fields_map(auto_encryption_opts, &bson_map);

		bson_destroy(&bson_map);

		/* Copy the encryptedFieldsMap to the Manager since PHPLIB may need to
		 * access it for createCollection and dropCollection helpers. */
		ZVAL_ZVAL(&manager->enc_fields_map, enc_fields_map, 1, 0);
	}

	if (php_array_existsc(zAutoEncryptionOpts, "keyVaultClient")) {
		zval* key_vault_client = php_array_fetch(zAutoEncryptionOpts, "keyVaultClient");

		if (Z_TYPE_P(key_vault_client) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(key_vault_client), php_phongo_manager_ce)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"keyVaultClient\" autoEncryption option to be %s, %s given", ZSTR_VAL(php_phongo_manager_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(key_vault_client));
			goto cleanup;
		}

		/* Ensure the Manager and keyVaultClient are consistent in their use of
		 * persistent clients. A non-persistent Manager could theoretically use
		 * a persistent keyVaultClient, but this prohibition may help prevent
		 * users from inadvertently creating a persistent keyVaultClient. */
		if (manager->use_persistent_client != Z_MANAGER_OBJ_P(key_vault_client)->use_persistent_client) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "The \"disableClientPersistence\" option for a Manager and its \"keyVaultClient\" must be the same");
			goto cleanup;
		}

		mongoc_auto_encryption_opts_set_keyvault_client(auto_encryption_opts, Z_MANAGER_OBJ_P(key_vault_client)->client);

		/* Copy the keyVaultClient to the Manager to allow for ref-counting (for
		 * non-persistent clients) and reset-on-fork behavior. */
		ZVAL_ZVAL(&manager->key_vault_client_manager, key_vault_client, 1, 0);
	}

	if (php_array_existsc(zAutoEncryptionOpts, "keyVaultNamespace")) {
		char*     key_vault_ns;
		char*     db_name;
		char*     coll_name;
		int       plen;
		zend_bool pfree;

		key_vault_ns = php_array_fetch_string(zAutoEncryptionOpts, "keyVaultNamespace", &plen, &pfree);

		if (!phongo_split_namespace(key_vault_ns, &db_name, &coll_name)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"keyVaultNamespace\" autoEncryption option to contain a full collection namespace");

			if (pfree) {
				efree(key_vault_ns);
			}

			goto cleanup;
		}

		mongoc_auto_encryption_opts_set_keyvault_namespace(auto_encryption_opts, db_name, coll_name);

		efree(db_name);
		efree(coll_name);

		if (pfree) {
			efree(key_vault_ns);
		}
	}

	if (php_array_existsc(zAutoEncryptionOpts, "kmsProviders")) {
		zval*  kms_providers  = php_array_fetch(zAutoEncryptionOpts, "kmsProviders");
		bson_t bson_providers = BSON_INITIALIZER;

		if (Z_TYPE_P(kms_providers) != IS_OBJECT && Z_TYPE_P(kms_providers) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"kmsProviders\" autoEncryption option to be an array or object, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(kms_providers));
			goto cleanup;
		}

		php_phongo_zval_to_bson(kms_providers, PHONGO_BSON_NONE, &bson_providers, NULL);
		if (EG(exception)) {
			goto cleanup;
		}

		mongoc_auto_encryption_opts_set_kms_providers(auto_encryption_opts, &bson_providers);

		bson_destroy(&bson_providers);
	}

	if (php_array_existsc(zAutoEncryptionOpts, "schemaMap")) {
		zval*  schema_map = php_array_fetch(zAutoEncryptionOpts, "schemaMap");
		bson_t bson_map   = BSON_INITIALIZER;

		if (Z_TYPE_P(schema_map) != IS_OBJECT && Z_TYPE_P(schema_map) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"schemaMap\" autoEncryption option to be an array or object, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(schema_map));
			goto cleanup;
		}

		php_phongo_zval_to_bson(schema_map, PHONGO_BSON_NONE, &bson_map, NULL);
		if (EG(exception)) {
			goto cleanup;
		}

		mongoc_auto_encryption_opts_set_schema_map(auto_encryption_opts, &bson_map);

		bson_destroy(&bson_map);
	}

	if (php_array_existsc(zAutoEncryptionOpts, "tlsOptions")) {
		zval*  tls_options  = php_array_fetch(zAutoEncryptionOpts, "tlsOptions");
		bson_t bson_options = BSON_INITIALIZER;

		if (Z_TYPE_P(tls_options) != IS_OBJECT && Z_TYPE_P(tls_options) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"tlsOptions\" autoEncryption option to be an array or object, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(tls_options));
			goto cleanup;
		}

		php_phongo_zval_to_bson(tls_options, PHONGO_BSON_NONE, &bson_options, NULL);
		if (EG(exception)) {
			goto cleanup;
		}

		mongoc_auto_encryption_opts_set_tls_opts(auto_encryption_opts, &bson_options);

		bson_destroy(&bson_options);
	}

	if (php_array_existsc(zAutoEncryptionOpts, "extraOptions")) {
		zval*  extra_options = php_array_fetch(zAutoEncryptionOpts, "extraOptions");
		bson_t bson_options  = BSON_INITIALIZER;

		if (Z_TYPE_P(extra_options) != IS_OBJECT && Z_TYPE_P(extra_options) != IS_ARRAY) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"extraOptions\" autoEncryption option to be an array or object, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(extra_options));
			goto cleanup;
		}

		php_phongo_zval_to_bson(extra_options, PHONGO_BSON_NONE, &bson_options, NULL);
		if (EG(exception)) {
			goto cleanup;
		}

		mongoc_auto_encryption_opts_set_extra(auto_encryption_opts, &bson_options);

		bson_destroy(&bson_options);
	}

	if (!mongoc_client_enable_auto_encryption(manager->client, auto_encryption_opts, &error)) {
		phongo_throw_exception_from_bson_error_t(&error);
		goto cleanup;
	}

	retval = true;

cleanup:
	mongoc_auto_encryption_opts_destroy(auto_encryption_opts);
	return retval;
}
#else  /* MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION */
static bool phongo_manager_set_auto_encryption_opts(php_phongo_manager_t* manager, zval* driverOptions)
{
	if (!driverOptions || !php_array_existsc(driverOptions, "autoEncryption")) {
		return true;
	}

	phongo_throw_exception_no_cse(PHONGO_ERROR_INVALID_ARGUMENT, "Cannot enable automatic field-level encryption.");

	return false;
}
#endif /* MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION */

void phongo_manager_init(php_phongo_manager_t* manager, const char* uri_string, zval* options, zval* driverOptions)
{
	bson_t        bson_options = BSON_INITIALIZER;
	mongoc_uri_t* uri          = NULL;
#ifdef MONGOC_ENABLE_SSL
	mongoc_ssl_opt_t* ssl_opt = NULL;
#endif

	if (!(manager->client_hash = php_phongo_manager_make_client_hash(uri_string, options, driverOptions, &manager->client_hash_len))) {
		/* Exception should already have been thrown and there is nothing to free */
		return;
	}

	if (driverOptions && php_array_existsc(driverOptions, "disableClientPersistence")) {
		manager->use_persistent_client = !php_array_fetchc_bool(driverOptions, "disableClientPersistence");
	} else {
		manager->use_persistent_client = true;
	}

	if (manager->use_persistent_client && (manager->client = php_phongo_find_persistent_client(manager->client_hash, manager->client_hash_len))) {
		MONGOC_DEBUG("Found client for hash: %s", manager->client_hash);
		goto cleanup;
	}

	if (options) {
		php_phongo_zval_to_bson(options, PHONGO_BSON_NONE, &bson_options, NULL);
	}

	/* An exception may be thrown during BSON conversion */
	if (EG(exception)) {
		goto cleanup;
	}

	if (!(uri = php_phongo_make_uri(uri_string))) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!php_phongo_apply_options_to_uri(uri, &bson_options) ||
		!php_phongo_apply_rc_options_to_uri(uri, &bson_options) ||
		!php_phongo_apply_rp_options_to_uri(uri, &bson_options) ||
		!php_phongo_apply_wc_options_to_uri(uri, &bson_options)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

#ifdef MONGOC_ENABLE_SSL
	if (!php_phongo_apply_driver_options_to_uri(uri, driverOptions)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	ssl_opt = php_phongo_make_ssl_opt(uri, driverOptions);

	/* An exception may be thrown during SSL option creation */
	if (EG(exception)) {
		goto cleanup;
	}
#endif

	manager->client = php_phongo_make_mongo_client(uri, driverOptions);

	if (!manager->client) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	mongoc_client_set_error_api(manager->client, MONGOC_ERROR_API_VERSION_2);

#ifdef MONGOC_ENABLE_SSL
	if (ssl_opt) {
		mongoc_client_set_ssl_opts(manager->client, ssl_opt);
	}
#endif

	if (!phongo_manager_set_auto_encryption_opts(manager, driverOptions)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!phongo_manager_set_serverapi_opts(manager, driverOptions)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	if (!phongo_apm_set_callbacks(manager->client)) {
		/* Exception should already have been thrown */
		goto cleanup;
	}

	MONGOC_DEBUG("Created client with hash: %s", manager->client_hash);

	/* Register the newly created client in the appropriate registry (for either
	 * persistent or request-scoped clients). */
	if (!php_phongo_client_register(manager)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Failed to add Manager client to internal registry");
		goto cleanup;
	}

cleanup:
	bson_destroy(&bson_options);

	if (uri) {
		mongoc_uri_destroy(uri);
	}

#ifdef MONGOC_ENABLE_SSL
	if (ssl_opt) {
		php_phongo_free_ssl_opt(ssl_opt);
	}
#endif
}

static void phongo_pclient_reset_once(php_phongo_pclient_t* pclient, int pid)
{
	if (pclient->last_reset_by_pid != pid) {
		mongoc_client_reset(pclient->client);
		pclient->last_reset_by_pid = pid;
	}
}

/* Resets the libmongoc client if it has not already been reset for the current
 * PID (based on information in the hash table of persisted libmongoc clients).
 * This ensures that we do not reset a client multiple times from the same child
 * process. */
void php_phongo_client_reset_once(php_phongo_manager_t* manager, int pid)
{
	php_phongo_pclient_t* pclient;

	/* Reset associated key vault client */
	if (!Z_ISUNDEF(manager->key_vault_client_manager)) {
		php_phongo_client_reset_once(Z_MANAGER_OBJ_P(&manager->key_vault_client_manager), pid);
	}

	if (manager->use_persistent_client) {
		pclient = zend_hash_str_find_ptr(&MONGODB_G(persistent_clients), manager->client_hash, manager->client_hash_len);

		if (pclient) {
			phongo_pclient_reset_once(pclient, pid);
		}

		return;
	}

	ZEND_HASH_FOREACH_PTR(MONGODB_G(request_clients), pclient)
	{
		if (pclient->client == manager->client) {
			phongo_pclient_reset_once(pclient, pid);

			/* Request-scoped clients are only used by a single Manager, so we
			 * can return early after finding a match. */
			return;
		}
	}
	ZEND_HASH_FOREACH_END();
}

/* Returns whether a Manager exists in the request-scoped registry. If found and
 * the output parameter is non-NULL, the Manager's index will be assigned. */
static bool php_phongo_manager_exists(php_phongo_manager_t* manager, zend_ulong* index_out)
{
	zend_ulong            index;
	php_phongo_manager_t* value;

	if (!MONGODB_G(managers) || zend_hash_num_elements(MONGODB_G(managers)) == 0) {
		return false;
	}

	ZEND_HASH_FOREACH_NUM_KEY_PTR(MONGODB_G(managers), index, value)
	{
		if (value != manager) {
			continue;
		}

		if (index_out) {
			*index_out = index;
		}

		return true;
	}
	ZEND_HASH_FOREACH_END();

	return false;
}

/* Adds a Manager to the request-scoped registry. Returns true if the Manager
 * did not exist and was successfully added; otherwise, returns false. */
bool php_phongo_manager_register(php_phongo_manager_t* manager)
{
	if (!MONGODB_G(managers)) {
		return false;
	}

	if (php_phongo_manager_exists(manager, NULL)) {
		return false;
	}

	return zend_hash_next_index_insert_ptr(MONGODB_G(managers), manager) != NULL;
}

/* Removes a Manager from the request-scoped registry. Returns true if the
 * Manager was found and successfully removed; otherwise, false is returned. */
bool php_phongo_manager_unregister(php_phongo_manager_t* manager)
{
	zend_ulong index;

	/* Ensure the registry is initialized. This is needed because RSHUTDOWN may
	 * occur before a Manager's free_object handler is executed. */
	if (!MONGODB_G(managers)) {
		return false;
	}

	if (php_phongo_manager_exists(manager, &index)) {
		return zend_hash_index_del(MONGODB_G(managers), index) == SUCCESS;
	}

	return false;
}

static void php_phongo_pclient_destroy(php_phongo_pclient_t* pclient)
{
	/* Do not destroy mongoc_client_t objects created by other processes. This
	 * ensures that we do not shutdown sockets that may still be in use by our
	 * parent process (see: PHPC-1522).
	 *
	 * This is a leak; however, we are already in GSHUTDOWN for a persistent
	 * clients. For a request-scoped client, we are either in the Manager's
	 * free_object handler or RSHUTDOWN, but there the application is capable of
	 * freeing its Manager and its client before forking. */
	if (pclient->created_by_pid == getpid()) {
		/* If we are in request shutdown, disable APM to avoid dispatching more
		 * events. This means that certain events (e.g. TopologyClosedEvent,
		 * command monitoring for endSessions) may not be observed. */
		if (EG(flags) & EG_FLAGS_IN_SHUTDOWN) {
			mongoc_client_set_apm_callbacks(pclient->client, NULL, NULL);
		}
		mongoc_client_destroy(pclient->client);
	}

	/* Persistent and request-scoped clients use different memory allocation */
	pefree(pclient, pclient->is_persistent);
}

void php_phongo_pclient_destroy_ptr(zval* ptr)
{
	php_phongo_pclient_destroy(Z_PTR_P(ptr));
}
