/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2014 MongoDB, Inc.                                         |
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
  | Copyright (c) 2014, MongoDB, Inc.                                         |
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
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_writeconcern_ce;

/* {{{ proto MongoDB\WriteConcern WriteConcern::__construct(integer|string $w[, integer $wtimeout[, array $options = array()]])
   Constructs a new ReadPreference */
PHP_METHOD(WriteConcern, __construct)
{
	php_phongo_writeconcern_t *intern;
	zend_error_handling       error_handling;
	zval                     *w;
	long                      wtimeout = 0;
	zval                     *options = NULL;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeconcern_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O|la!", &w, php_phongo_integer|string_ce, &wtimeout, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */

/**
 * Value object for write concern used in issuing write operations.
 */
/* {{{ MongoDB\WriteConcern */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcern___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, w)
	ZEND_ARG_INFO(0, wtimeout)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_writeconcern_me[] = {
	PHP_ME(WriteConcern, __construct, ai_WriteConcern___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_writeconcern_t object handlers */
static void php_phongo_writeconcern_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcern_t *intern = (php_phongo_writeconcern_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_writeconcern_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_writeconcern_t *intern;

	intern = (php_phongo_writeconcern_t *)emalloc(sizeof(php_phongo_writeconcern_t));
	memset(intern, 0, sizeof(php_phongo_writeconcern_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeconcern_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteConcern)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB", "WriteConcern", php_phongo_writeconcern_me);
	ce.create_object = php_phongo_writeconcern_create_object;
	php_phongo_writeconcern_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_writeconcern_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;


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
