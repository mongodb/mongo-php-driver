/*
 * Copyright 2016-2017 MongoDB, Inc.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

ZEND_EXTERN_MODULE_GLOBALS(mongodb)

static char* php_phongo_make_subscriber_hash(zval* subscriber TSRMLS_DC)
{
	char* hash;
	int   hash_len;

	hash_len = spprintf(&hash, 0, "SUBS-%09d", Z_OBJ_HANDLE_P(subscriber));

	return hash;
}

/* {{{ proto void MongoDB\Driver\Monitoring\addSubscriber(MongoDB\Driver\Monitoring\Subscriber $subscriber)
   Adds a monitoring subscriber to the set of subscribers */
PHP_FUNCTION(MongoDB_Driver_Monitoring_addSubscriber)
{
	zval* zSubscriber = NULL;
	char* hash;
#if PHP_VERSION_ID >= 70000
	zval* subscriber;
#else
	zval** subscriber;
#endif

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &zSubscriber, php_phongo_subscriber_ce) == FAILURE) {
		return;
	}

	/* The HashTable should never be NULL, as it's initialized during RINIT and
	 * destroyed during RSHUTDOWN. This is simply a defensive guard. */
	if (!MONGODB_G(subscribers)) {
		return;
	}

	hash = php_phongo_make_subscriber_hash(zSubscriber TSRMLS_CC);

	/* If we have already stored the subscriber, bail out. Otherwise, add
	 * subscriber to list */
#if PHP_VERSION_ID >= 70000
	if ((subscriber = zend_hash_str_find(MONGODB_G(subscribers), hash, strlen(hash)))) {
		efree(hash);
		return;
	}

	zend_hash_str_update(MONGODB_G(subscribers), hash, strlen(hash), zSubscriber);
#else
	if (zend_hash_find(MONGODB_G(subscribers), hash, strlen(hash) + 1, (void**) &subscriber) == SUCCESS) {
		efree(hash);
		return;
	}

	zend_hash_update(MONGODB_G(subscribers), hash, strlen(hash) + 1, (void*) &zSubscriber, sizeof(zval*), NULL);
#endif
	Z_ADDREF_P(zSubscriber);
	efree(hash);
} /* }}} */

/* {{{ proto void MongoDB\Driver\Monitoring\removeSubscriber(MongoDB\Driver\Monitoring\Subscriber $subscriber)
   Removes a monitoring subscriber from the set of subscribers */
PHP_FUNCTION(MongoDB_Driver_Monitoring_removeSubscriber)
{
	zval* zSubscriber = NULL;
	char* hash;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &zSubscriber, php_phongo_subscriber_ce) == FAILURE) {
		return;
	}

	/* The HashTable should never be NULL, as it's initialized during RINIT and
	 * destroyed during RSHUTDOWN. This is simply a defensive guard. */
	if (!MONGODB_G(subscribers)) {
		return;
	}

	hash = php_phongo_make_subscriber_hash(zSubscriber TSRMLS_CC);

#if PHP_VERSION_ID >= 70000
	zend_hash_str_del(MONGODB_G(subscribers), hash, strlen(hash));
#else
	zend_hash_del(MONGODB_G(subscribers), hash, strlen(hash) + 1);
#endif
	efree(hash);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
