/*
 * Copyright 2015-2017 MongoDB, Inc.
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

/* Our Compatability header */
#include "phongo_compat.h"

void phongo_add_exception_prop(const char* prop, int prop_len, zval* value TSRMLS_DC)
{
	if (EG(exception)) {
#if PHP_VERSION_ID >= 70000
		zval ex;
		EXCEPTION_P(EG(exception), ex);
		zend_update_property(Z_OBJCE(ex), &ex, prop, prop_len, value);
#else
		zval* ex = NULL;
		EXCEPTION_P(EG(exception), ex);
		zend_update_property(Z_OBJCE_P(ex), ex, prop, prop_len, value TSRMLS_CC);
#endif
	}
}

#ifdef ZEND_HASH_GET_APPLY_COUNT /* PHP 7.2 or earlier recursion protection */
zend_bool php_phongo_zend_hash_apply_protection_begin(HashTable* ht)
{
	if (!ht) {
		return 1;
	}
	if (ZEND_HASH_GET_APPLY_COUNT(ht) > 0) {
		return 0;
	}
	if (ZEND_HASH_APPLY_PROTECTION(ht)) {
		ZEND_HASH_INC_APPLY_COUNT(ht);
	}
	return 1;
}

zend_bool php_phongo_zend_hash_apply_protection_end(HashTable* ht)
{
	if (!ht) {
		return 1;
	}
	if (ZEND_HASH_GET_APPLY_COUNT(ht) == 0) {
		return 0;
	}
	if (ZEND_HASH_APPLY_PROTECTION(ht)) {
		ZEND_HASH_DEC_APPLY_COUNT(ht);
	}
	return 1;
}
#else /* PHP 7.3 or later */
zend_bool php_phongo_zend_hash_apply_protection_begin(zend_array* ht)
{
	if (GC_IS_RECURSIVE(ht)) {
		return 0;
	}
	if (!(GC_FLAGS(ht) & GC_IMMUTABLE)) {
		GC_PROTECT_RECURSION(ht);
	}
	return 1;
}

zend_bool php_phongo_zend_hash_apply_protection_end(zend_array* ht)
{
	if (!GC_IS_RECURSIVE(ht)) {
		return 0;
	}
	if (!(GC_FLAGS(ht) & GC_IMMUTABLE)) {
		GC_UNPROTECT_RECURSION(ht);
	}
	return 1;
}
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
