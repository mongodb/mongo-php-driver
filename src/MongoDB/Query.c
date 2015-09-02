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


PHONGO_API zend_class_entry *php_phongo_query_ce;

zend_object_handlers php_phongo_handler_query;

/* {{{ proto MongoDB\Driver\Query Query::__construct(array|object $filter[, array $options = array()])
   Constructs a new Query */
PHP_METHOD(Query, __construct)
{
	php_phongo_query_t       *intern;
	zend_error_handling       error_handling;
	zval                     *zfilter;
	zval                     *zoptions = NULL;
	bson_t                    bfilter;
	bson_t                    boptions = BSON_INITIALIZER;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_used)


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_QUERY_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A|a!", &zfilter, &zoptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	intern->query = bson_new();

	bson_init(&bfilter);
	zval_to_bson(zfilter, PHONGO_BSON_NONE, &bfilter, NULL TSRMLS_CC);

	if (zoptions) {
		bson_init(&boptions);
		zval_to_bson(zoptions, PHONGO_BSON_NONE, &boptions, NULL TSRMLS_CC);
	}
	if (!phongo_query_init(intern, &bfilter, &boptions TSRMLS_CC)) {
		bson_clear(&intern->query);
	}
	bson_destroy(&bfilter);
	bson_destroy(&boptions);
}
/* }}} */

/* {{{ MongoDB\Driver\Query */

ZEND_BEGIN_ARG_INFO_EX(ai_Query___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, filter)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_query_me[] = {
	PHP_ME(Query, __construct, ai_Query___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_query_t object handlers */
static void php_phongo_query_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_query_t *intern = Z_OBJ_QUERY(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->selector) {
		bson_clear(&intern->selector);
	}

	if (intern->query) {
		bson_clear(&intern->query);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_query_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_query_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_query_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_query;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_query_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_query;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_query_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_query_t    *intern;
#if PHP_VERSION_ID >= 70000
	zval                   retval;
#else
	zval                   retval = zval_used_for_init;
#endif


	*is_temp = 1;
	intern = Z_QUERY_OBJ_P(object);

	array_init_size(&retval, 6);

	/* Avoid using PHONGO_TYPEMAP_NATIVE_ARRAY for decoding query and selector
	 * documents so that users can differentiate BSON arrays and documents. */
	if (intern->query) {
#if PHP_VERSION_ID >= 70000
		zval zv;
#else
		zval *zv;
#endif

		bson_to_zval(bson_get_data(intern->query), intern->query->len, &zv);
#if PHP_VERSION_ID >= 70000
		ADD_ASSOC_ZVAL_EX(&retval, "query", &zv);
#else
		ADD_ASSOC_ZVAL_EX(&retval, "query", zv);
#endif
	} else {
		ADD_ASSOC_NULL_EX(&retval, "query");
	}

	if (intern->selector) {
#if PHP_VERSION_ID >= 70000
		zval zv;
#else
		zval *zv;
#endif

		bson_to_zval(bson_get_data(intern->selector), intern->selector->len, &zv);
#if PHP_VERSION_ID >= 70000
		ADD_ASSOC_ZVAL_EX(&retval, "selector", &zv);
#else
		ADD_ASSOC_ZVAL_EX(&retval, "selector", zv);
#endif
	} else {
		ADD_ASSOC_NULL_EX(&retval, "selector");
	}

	ADD_ASSOC_LONG_EX(&retval, "flags", intern->flags);
	ADD_ASSOC_LONG_EX(&retval, "skip", intern->skip);
	ADD_ASSOC_LONG_EX(&retval, "limit", intern->limit);
	ADD_ASSOC_LONG_EX(&retval, "batch_size", intern->batch_size);

	return Z_ARRVAL(retval);

} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Query)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Query", php_phongo_query_me);
	php_phongo_query_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_query_ce->create_object = php_phongo_query_create_object;
	PHONGO_CE_INIT(php_phongo_query_ce);

	memcpy(&php_phongo_handler_query, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_query.get_debug_info = php_phongo_query_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_query.free_obj = php_phongo_query_free_object;
	php_phongo_handler_query.offset = XtOffsetOf(php_phongo_query_t, std);
#endif

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
