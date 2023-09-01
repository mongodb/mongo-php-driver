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

#if PHP_VERSION_ID < 80000
zend_string* zend_string_concat3(const char* str1, size_t str1_len, const char* str2, size_t str2_len, const char* str3, size_t str3_len)
{
	size_t       len = str1_len + str2_len + str3_len;
	zend_string* res = zend_string_alloc(len, 0);

	memcpy(ZSTR_VAL(res), str1, str1_len);
	memcpy(ZSTR_VAL(res) + str1_len, str2, str2_len);
	memcpy(ZSTR_VAL(res) + str1_len + str2_len, str3, str3_len);
	/* clang-format off */
	ZSTR_VAL(res)[len] = '\0';
	/* clang-format on */

	return res;
}
#endif /* PHP_VERSION_ID < 80000 */
