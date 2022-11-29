/*
 * Copyright 2015-present MongoDB, Inc.
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

#include "phongo_compat.h"

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
#else  /* PHP 7.3 or later */
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
#endif /* ZEND_HASH_GET_APPLY_COUNT */

#if PHP_VERSION_ID < 80200
const char* zend_get_object_type_case(const zend_class_entry* ce, zend_bool upper_case)
{
	if (ce->ce_flags & ZEND_ACC_TRAIT) {
		return upper_case ? "Trait" : "trait";
	}
	if (ce->ce_flags & ZEND_ACC_INTERFACE) {
		return upper_case ? "Interface" : "interface";
	}
#if PHP_VERSION_ID >= 80100
	if (ce->ce_flags & ZEND_ACC_ENUM) {
		return upper_case ? "Enum" : "enum";
	}
#endif /* PHP_VERSION_ID > 80100 */
	return upper_case ? "Class" : "class";
}
#endif /* PHP_VERSION_ID < 80200 */

#if PHP_VERSION_ID < 80100
zend_bool zend_array_is_list(zend_array* array)
{
	zend_ulong   expected_idx = 0;
	zend_ulong   num_idx;
	zend_string* str_idx;
	/* Empty arrays are lists */
	if (zend_hash_num_elements(array) == 0) {
		return 1;
	}

	ZEND_HASH_FOREACH_KEY(array, num_idx, str_idx)
	{
		if (str_idx != NULL || num_idx != expected_idx++) {
			return 0;
		}
	}
	ZEND_HASH_FOREACH_END();

	return 1;
}
#endif /* PHP_VERSION_ID < 80100 */
