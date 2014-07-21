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


PHONGO_API zend_class_entry *php_phongo_readpreference_ce;

/* {{{ proto MongoDB\ReadPreference ReadPreference::__construct(integer $readPreference[, array $tagSets = array()])
   Constructs a new ReadPreference */
PHP_METHOD(ReadPreference, __construct)
{
	php_phongo_readpreference_t *intern;
	zend_error_handling       error_handling;
	long                      readPreference;
	zval                     *tagSets = NULL;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_readpreference_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|a!", &readPreference, &tagSets) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	switch(readPreference) {
		case MONGOC_READ_PRIMARY:
		case MONGOC_READ_SECONDARY:
		case MONGOC_READ_PRIMARY_PREFERRED:
		case MONGOC_READ_SECONDARY_PREFERRED:
		case MONGOC_READ_NEAREST:
			intern->read_preference = mongoc_read_prefs_new(readPreference);

			if (tagSets) {
				const bson_t tags;

				zval_to_bson(tagSets, PHONGO_BSON_NONE, (bson_t *)&tags, NULL TSRMLS_CC);
				mongoc_read_prefs_set_tags(intern->read_preference, &tags);
				if (!mongoc_read_prefs_is_valid(intern->read_preference)) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid tagSet");
					return;
				}
			}
			break;
		default:
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Invalid ReadPreference");
			return;
	}
}
/* }}} */

/**
 * Value object for read preferences used in issuing commands and queries.
 */
/* {{{ MongoDB\ReadPreference */

ZEND_BEGIN_ARG_INFO_EX(ai_ReadPreference___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, readPreference)
	ZEND_ARG_ARRAY_INFO(0, tagSets, 1)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_readpreference_me[] = {
	PHP_ME(ReadPreference, __construct, ai_ReadPreference___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_readpreference_t object handlers */
static void php_phongo_readpreference_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_readpreference_t *intern = (php_phongo_readpreference_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_readpreference_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_readpreference_t *intern;

	intern = (php_phongo_readpreference_t *)emalloc(sizeof(php_phongo_readpreference_t));
	memset(intern, 0, sizeof(php_phongo_readpreference_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_readpreference_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(ReadPreference)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB", "ReadPreference", php_phongo_readpreference_me);
	ce.create_object = php_phongo_readpreference_create_object;
	php_phongo_readpreference_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_readpreference_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	zend_declare_class_constant_string(php_phongo_readpreference_ce, ZEND_STRL("RP_PRIMARY"), "primary" TSRMLS_CC);
	zend_declare_class_constant_string(php_phongo_readpreference_ce, ZEND_STRL("RP_PRIMARY_PREFERRED"), "primaryPreferred" TSRMLS_CC);
	zend_declare_class_constant_string(php_phongo_readpreference_ce, ZEND_STRL("RP_SECONDARY"), "secondary" TSRMLS_CC);
	zend_declare_class_constant_string(php_phongo_readpreference_ce, ZEND_STRL("RP_SECONDARY_PREFERRED"), "secondaryPreferred" TSRMLS_CC);
	zend_declare_class_constant_string(php_phongo_readpreference_ce, ZEND_STRL("RP_NEAREST"), "nearest" TSRMLS_CC);

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
