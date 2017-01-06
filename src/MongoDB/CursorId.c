/*
 * Copyright 2014-2017 MongoDB, Inc.
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


PHONGO_API zend_class_entry *php_phongo_cursorid_ce;

zend_object_handlers php_phongo_handler_cursorid;

/* {{{ proto string CursorId::__toString()
   Returns the string representation of the CursorId */
PHP_METHOD(CursorId, __toString)
{
	php_phongo_cursorid_t    *intern;
	char *tmp;
	int tmp_len;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

	intern = Z_CURSORID_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	tmp_len = spprintf(&tmp, 0, "%" PRIu64, intern->id);
	PHONGO_RETVAL_STRINGL(tmp, tmp_len);
	efree(tmp);
}
/* }}} */

/* {{{ MongoDB\Driver\CursorId */

ZEND_BEGIN_ARG_INFO_EX(ai_CursorId_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_cursorid_me[] = {
	PHP_ME(Server, __construct, ai_CursorId_void, ZEND_ACC_FINAL|ZEND_ACC_PRIVATE)
	PHP_ME(CursorId, __toString, ai_CursorId_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeup, ai_CursorId_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_cursorid_t object handlers */
static void php_phongo_cursorid_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_cursorid_t *intern = Z_OBJ_CURSORID(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_cursorid_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_cursorid_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_cursorid_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_cursorid;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_cursorid_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_cursorid;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_cursorid_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_cursorid_t  *intern;
#if PHP_VERSION_ID >= 70000
	zval                    retval;
#else
	zval                    retval = zval_used_for_init;
#endif


	*is_temp = 1;
	intern = Z_CURSORID_OBJ_P(object);

	array_init(&retval);

#if SIZEOF_LONG == 4
	{
		char tmp[24];
		int tmp_len;

		tmp_len = snprintf(tmp, sizeof(tmp), "%" PRIu64, intern->id);
		ADD_ASSOC_STRINGL(&retval, "id", tmp, tmp_len);
	}
#else
	ADD_ASSOC_LONG_EX(&retval, "id", intern->id);
#endif

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(CursorId)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "CursorId", php_phongo_cursorid_me);
	php_phongo_cursorid_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_cursorid_ce->create_object = php_phongo_cursorid_create_object;
	PHONGO_CE_FINAL(php_phongo_cursorid_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_cursorid_ce);

	memcpy(&php_phongo_handler_cursorid, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_cursorid.get_debug_info = php_phongo_cursorid_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_cursorid.free_obj = php_phongo_cursorid_free_object;
	php_phongo_handler_cursorid.offset = XtOffsetOf(php_phongo_cursorid_t, std);
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
