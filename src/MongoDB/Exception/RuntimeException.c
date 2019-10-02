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

#include <php.h>
#include <ext/spl/spl_exceptions.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"
#include "php_array_api.h"

zend_class_entry* php_phongo_runtimeexception_ce;

static bool php_phongo_has_string_array_element(zval* labels, char* label TSRMLS_DC)
{
	HashTable* ht_data;

	if (Z_TYPE_P(labels) != IS_ARRAY) {
		return false;
	}

	ht_data = HASH_OF(labels);

#if PHP_VERSION_ID >= 70000
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
#else
	{
		HashPosition pos;
		zval**       z_label;

		for (
			zend_hash_internal_pointer_reset_ex(ht_data, &pos);
			zend_hash_get_current_data_ex(ht_data, (void**) &z_label, &pos) == SUCCESS;
			zend_hash_move_forward_ex(ht_data, &pos)) {

			if (Z_TYPE_PP(z_label) == IS_STRING) {
				if (strcmp(Z_STRVAL_PP(z_label), label) == 0) {
					return true;
				}
			}
		}
	}
#endif

	return false;
}

/* {{{ proto bool MongoDB\Driver\Exception\RuntimeException::hasErrorLabel(string $label)
   Returns whether a specific error label has been set */
static PHP_METHOD(RuntimeException, hasErrorLabel)
{
	char*               label;
	phongo_zpp_char_len label_len;
	zval*               error_labels;
#if PHP_VERSION_ID >= 70000
	zval rv;
#endif

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &label, &label_len) == FAILURE) {
		return;
	}

#if PHP_VERSION_ID >= 70000
	error_labels = zend_read_property(php_phongo_runtimeexception_ce, getThis(), ZEND_STRL("errorLabels"), 0, &rv TSRMLS_CC);
#else
	error_labels = zend_read_property(php_phongo_runtimeexception_ce, getThis(), ZEND_STRL("errorLabels"), 0 TSRMLS_CC);
#endif

	RETURN_BOOL(php_phongo_has_string_array_element(error_labels, label TSRMLS_CC));
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(ai_RuntimeException_hasErrorLabel, 0, 0, 1)
	ZEND_ARG_INFO(0, label)
ZEND_END_ARG_INFO()

/* {{{ MongoDB\Driver\Exception\RuntimeException function entries */
static zend_function_entry php_phongo_runtimeexception_me[] = {
	/* clang-format off */
	PHP_ME(RuntimeException, hasErrorLabel, ai_RuntimeException_hasErrorLabel, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

void php_phongo_runtimeexception_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "RuntimeException", php_phongo_runtimeexception_me);
#if PHP_VERSION_ID >= 70000
	php_phongo_runtimeexception_ce = zend_register_internal_class_ex(&ce, spl_ce_RuntimeException);
#else
	php_phongo_runtimeexception_ce = zend_register_internal_class_ex(&ce, spl_ce_RuntimeException, NULL TSRMLS_CC);
#endif
	zend_class_implements(php_phongo_runtimeexception_ce TSRMLS_CC, 1, php_phongo_exception_ce);

	zend_declare_property_null(php_phongo_runtimeexception_ce, ZEND_STRL("errorLabels"), ZEND_ACC_PROTECTED TSRMLS_CC);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
