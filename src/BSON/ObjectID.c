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


PHONGO_API zend_class_entry *php_phongo_objectid_ce;

zend_object_handlers php_phongo_handler_objectid;


/* {{{ proto BSON\ObjectID ObjectID::__construct(string $id)
   Constructs a new Object ID, optionally from a string */
PHP_METHOD(ObjectID, __construct)
{
	php_phongo_objectid_t    *intern;
	zend_error_handling       error_handling;
	char                     *id = NULL;
	int                       id_len;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_objectid_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s!", &id, &id_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	if (ZEND_NUM_ARGS()) {
		if (bson_oid_is_valid(id, id_len)) {
			bson_oid_t oid;

			bson_oid_init_from_string(&oid, id);
			bson_oid_to_string(&oid, intern->oid);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s", "Invalid BSON ID provided");
		}
	} else {
		bson_oid_t oid;

		bson_oid_init(&oid, NULL);
		bson_oid_to_string(&oid, intern->oid);
	}
}
/* }}} */
/* {{{ proto void ObjectID::__toString()
    */
PHP_METHOD(ObjectID, __toString)
{
	php_phongo_objectid_t    *intern;


	intern = (php_phongo_objectid_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	RETURN_STRINGL(intern->oid, 24, 1);
}
/* }}} */


/* {{{ BSON\ObjectID */

ZEND_BEGIN_ARG_INFO_EX(ai_ObjectID___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_ObjectID___toString, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_objectid_me[] = {
	PHP_ME(ObjectID, __construct, ai_ObjectID___construct, ZEND_ACC_PUBLIC)
	PHP_ME(ObjectID, __toString, ai_ObjectID___toString, ZEND_ACC_PUBLIC)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_objectid_t object handlers */
static void php_phongo_objectid_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_objectid_t *intern = (php_phongo_objectid_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_objectid_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_objectid_t *intern = NULL;

	intern = (php_phongo_objectid_t *)ecalloc(1, sizeof *intern);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_objectid_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_objectid;

	return retval;
} /* }}} */

static int php_phongo_objectid_compare_objects(zval *o1, zval *o2 TSRMLS_DC) /* {{{ */
{
	php_phongo_objectid_t *intern1;
	php_phongo_objectid_t *intern2;

	intern1 = (php_phongo_objectid_t *)zend_object_store_get_object(o1 TSRMLS_CC);
	intern2 = (php_phongo_objectid_t *)zend_object_store_get_object(o2 TSRMLS_CC);

	return strcmp(intern1->oid, intern2->oid);
} /* }}} */

HashTable *php_phongo_objectid_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_objectid_t    *intern;
	zval                      retval = zval_used_for_init;


	*is_temp = 1;
	intern = (php_phongo_objectid_t *)zend_object_store_get_object(object TSRMLS_CC);

	array_init(&retval);

	add_assoc_stringl_ex(&retval, ZEND_STRS("oid"), intern->oid, 24, 1);

	return Z_ARRVAL(retval);

} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(ObjectID)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, BSON_NAMESPACE, "ObjectID", php_phongo_objectid_me);
	php_phongo_objectid_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_objectid_ce->create_object = php_phongo_objectid_create_object;
	PHONGO_CE_INIT(php_phongo_objectid_ce);

	zend_class_implements(php_phongo_objectid_ce TSRMLS_CC, 1, php_phongo_type_ce);

	memcpy(&php_phongo_handler_objectid, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_objectid.compare_objects = php_phongo_objectid_compare_objects;
	php_phongo_handler_objectid.get_debug_info = php_phongo_objectid_get_debug_info;


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
