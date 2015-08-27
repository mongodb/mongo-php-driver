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


PHONGO_API zend_class_entry *php_phongo_maxkey_ce;

zend_object_handlers php_phongo_handler_maxkey;

/* {{{ BSON\MaxKey */


static zend_function_entry php_phongo_maxkey_me[] = {
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_maxkey_t object handlers */
static void php_phongo_maxkey_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_maxkey_t *intern = (php_phongo_maxkey_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
} /* }}} */

#if PHP_VERSION_ID >= 70000
zend_object* php_phongo_maxkey_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
        php_phongo_regex_t *intern;

        intern = (php_phongo_maxkey_t *)ecalloc(1, sizeof(php_phongo_maxkey_t)+zend_object_properties_size(class_type));

        zend_object_std_init(&intern->std, class_type TSRMLS_CC);
        object_properties_init(&intern->std, class_type);

        intern->std.handlers = &php_phongo_handler_maxkey;

        return &intern->std;
} /* }}} */
#else
zend_object_value php_phongo_maxkey_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_maxkey_t *intern;

	intern = (php_phongo_maxkey_t *)emalloc(sizeof(php_phongo_maxkey_t));
	memset(intern, 0, sizeof(php_phongo_maxkey_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_maxkey_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_maxkey;

	return retval;
} /* }}} */
#endif
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(MaxKey)
{
	zend_class_entry ce;
	(void)type;(void)module_number;


	INIT_NS_CLASS_ENTRY(ce, BSON_NAMESPACE, "MaxKey", php_phongo_maxkey_me);
	ce.create_object = php_phongo_maxkey_create_object;
	php_phongo_maxkey_ce = zend_register_internal_class(&ce TSRMLS_CC);

	zend_class_implements(php_phongo_maxkey_ce TSRMLS_CC, 1, php_phongo_type_ce);
        memcpy(&php_phongo_handler_maxkey, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
#if PHP_VERSION_ID >= 70000
        php_phongo_handler_maxkey.free_obj = php_phongo_maxkey_free_object;
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
