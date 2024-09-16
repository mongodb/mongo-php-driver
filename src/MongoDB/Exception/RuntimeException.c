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
#include <ext/spl/spl_exceptions.h>

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_error.h"
#include "RuntimeException_arginfo.h"

zend_class_entry* php_phongo_runtimeexception_ce;

static bool php_phongo_has_string_array_element(zval* labels, char* label)
{
	HashTable* ht_data;

	if (Z_TYPE_P(labels) != IS_ARRAY) {
		return false;
	}

	ht_data = HASH_OF(labels);

	{
		zval* z_label;

		ZEND_HASH_FOREACH_VAL_IND(ht_data, z_label)
		{
			if ((Z_TYPE_P(z_label) == IS_STRING) && (strcmp(Z_STRVAL_P(z_label), label) == 0)) {
				return true;
			}
		}
		ZEND_HASH_FOREACH_END();
	}

	return false;
}

/* Returns whether a specific error label has been set */
static PHP_METHOD(MongoDB_Driver_Exception_RuntimeException, hasErrorLabel)
{
	char*  label;
	size_t label_len;
	zval*  error_labels;
	zval   rv;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(label, label_len)
	PHONGO_PARSE_PARAMETERS_END();

	error_labels = zend_read_property(php_phongo_runtimeexception_ce, Z_OBJ_P(getThis()), ZEND_STRL("errorLabels"), 0, &rv);

	RETURN_BOOL(php_phongo_has_string_array_element(error_labels, label));
}

void php_phongo_runtimeexception_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_runtimeexception_ce = register_class_MongoDB_Driver_Exception_RuntimeException(spl_ce_RuntimeException, php_phongo_exception_ce);
}
