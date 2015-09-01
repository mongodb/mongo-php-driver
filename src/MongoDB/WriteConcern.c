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


PHONGO_API zend_class_entry *php_phongo_writeconcern_ce;

zend_object_handlers php_phongo_handler_writeconcern;

#define PHONGO_WRITE_CONCERN_W_MAJORITY "majority"

/* {{{ proto MongoDB\Driver\WriteConcern WriteConcern::__construct(integer|string $w[, integer $wtimeout[, boolean $journal[, boolean $fsync]]])
   Constructs a new WriteConcern */
PHP_METHOD(WriteConcern, __construct)
{
	php_phongo_writeconcern_t *intern;
	zend_error_handling       error_handling;
	char                     *wstring;
	phongo_zpp_char_len       wstring_len;
	long                      wtimeout = 0;
	zend_bool                 journal = 0;
	zend_bool                 fsync = 0;
	long                      w;
	SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_WRITECONCERN_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|lbb", &wstring, &wstring_len, &wtimeout, &journal, &fsync) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	intern->write_concern = mongoc_write_concern_new();

	if (IS_LONG == is_numeric_string(wstring, wstring_len, &w, NULL, 0)) {
		mongoc_write_concern_set_w(intern->write_concern, w);
	} else {
		if (strcmp(wstring, PHONGO_WRITE_CONCERN_W_MAJORITY) == 0) {
			mongoc_write_concern_set_w(intern->write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);
		} else {
			mongoc_write_concern_set_wtag(intern->write_concern, wstring);
		}
	}

	switch(ZEND_NUM_ARGS()) {
		case 4:
			mongoc_write_concern_set_fsync(intern->write_concern, fsync);
			/* fallthrough */
		case 3:
			mongoc_write_concern_set_journal(intern->write_concern, journal);
			/* fallthrough */
		case 2:
			if (wtimeout > 0) {
				mongoc_write_concern_set_wtimeout(intern->write_concern, wtimeout);
			}
	}
}
/* }}} */

/**
 * Value object for write concern used in issuing write operations.
 */
/* {{{ MongoDB\Driver\WriteConcern */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcern___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, wstring)
	ZEND_ARG_INFO(0, wtimeout)
	ZEND_ARG_INFO(0, journal)
	ZEND_ARG_INFO(0, fsync)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_writeconcern_me[] = {
	PHP_ME(WriteConcern, __construct, ai_WriteConcern___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_writeconcern_t object handlers */
static void php_phongo_writeconcern_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcern_t *intern = (php_phongo_writeconcern_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_writeconcern_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcern_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_writeconcern_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_writeconcern;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeconcern_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_writeconcern;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_writeconcern_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	zval                    retval = zval_used_for_init;
	const mongoc_write_concern_t *write_concern = phongo_write_concern_from_zval(object TSRMLS_CC);


	*is_temp = 1;
	php_phongo_write_concern_to_zval(&retval, write_concern);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteConcern)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "WriteConcern", php_phongo_writeconcern_me);
	php_phongo_writeconcern_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_writeconcern_ce->create_object = php_phongo_writeconcern_create_object;
	PHONGO_CE_INIT(php_phongo_writeconcern_ce);

	memcpy(&php_phongo_handler_writeconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeconcern.get_debug_info = php_phongo_writeconcern_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_writeconcern.free_obj = php_phongo_writeconcern_free_object;
	php_phongo_handler_writeconcern.offset = XtOffsetOf(php_phongo_writeconcern_t, std);
#endif

	zend_declare_class_constant_stringl(php_phongo_writeconcern_ce, ZEND_STRL("MAJORITY"), ZEND_STRL(PHONGO_WRITE_CONCERN_W_MAJORITY) TSRMLS_CC);

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
