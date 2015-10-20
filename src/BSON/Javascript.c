/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2015 MongoDB, Inc.                                         |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2014-2015 MongoDB, Inc.                                     |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_javascript_ce;

zend_object_handlers php_phongo_handler_javascript;

/* {{{ proto BSON\Javascript Javascript::__construct(string $javascript[, array|object $document])
 * The string is JavaScript code. The document is a mapping from identifiers to values, representing the scope in which the string should be evaluated
 * NOTE: eJSON does not support this type :( */
PHP_METHOD(Javascript, __construct)
{
	php_phongo_javascript_t   *intern;
	zend_error_handling        error_handling;
	char                      *javascript;
	int                        javascript_len;
	zval                      *document = NULL;
	bson_t                     scope = BSON_INITIALIZER;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_javascript_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|A!", &javascript, &javascript_len, &document) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	if (document) {
		zval_to_bson(document, PHONGO_BSON_NONE, &scope, NULL TSRMLS_CC);
	}

	php_phongo_new_javascript_from_javascript_and_scope(0, getThis(), javascript, javascript_len, &scope TSRMLS_CC);
}
/* }}} */

/* {{{ BSON\Javascript */

ZEND_BEGIN_ARG_INFO_EX(ai_Javascript___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, javascript)
	ZEND_ARG_INFO(0, scope)
ZEND_END_ARG_INFO();

static zend_function_entry php_phongo_javascript_me[] = {
	PHP_ME(Javascript, __construct, ai_Javascript___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_javascript_t object handlers */
static void php_phongo_javascript_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_javascript_t *intern = (php_phongo_javascript_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->javascript) {
		efree(intern->javascript);
	}
	if (intern->document) {
		bson_destroy(intern->document);
		intern->document = NULL;
	}
	efree(intern);
} /* }}} */

zend_object_value php_phongo_javascript_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_javascript_t *intern;

	intern = (php_phongo_javascript_t *)emalloc(sizeof(php_phongo_javascript_t));
	memset(intern, 0, sizeof(php_phongo_javascript_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_javascript_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_javascript;

	intern->document = NULL;

	return retval;
} /* }}} */

HashTable *php_phongo_javascript_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_javascript_t *intern;
	zval                     retval = zval_used_for_init;


	*is_temp = 1;
	intern = (php_phongo_javascript_t *)zend_object_store_get_object(object TSRMLS_CC);

	array_init(&retval);

	add_assoc_stringl_ex(&retval, ZEND_STRS("javascript"), intern->javascript, intern->javascript_len, 1);

	if (intern->document) {
		php_phongo_bson_state state = PHONGO_BSON_STATE_INITIALIZER;

		MAKE_STD_ZVAL(state.zchild);

		if (bson_to_zval(bson_get_data(intern->document), intern->document->len, &state)) {
			Z_ADDREF_P(state.zchild);
			add_assoc_zval_ex(&retval, ZEND_STRS("scope"), state.zchild);
		} else {
			add_assoc_null_ex(&retval, ZEND_STRS("scope"));
		}

		zval_ptr_dtor(&state.zchild);
	}

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Javascript)
{
	zend_class_entry ce;
	(void)type;(void)module_number;


	INIT_NS_CLASS_ENTRY(ce, BSON_NAMESPACE, "Javascript", php_phongo_javascript_me);
	php_phongo_javascript_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_javascript_ce->create_object = php_phongo_javascript_create_object;
	PHONGO_CE_INIT(php_phongo_javascript_ce);

	zend_class_implements(php_phongo_javascript_ce TSRMLS_CC, 1, php_phongo_type_ce);

	memcpy(&php_phongo_handler_javascript, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_javascript.get_debug_info = php_phongo_javascript_get_debug_info;

	return SUCCESS;
}
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
