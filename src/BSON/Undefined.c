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

#include <php.h>
#include <Zend/zend_interfaces.h>
#include <ext/standard/php_var.h>
#if PHP_VERSION_ID >= 70000
#include <zend_smart_str.h>
#else
#include <ext/standard/php_smart_str.h>
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_undefined_ce;

/* {{{ proto string MongoDB\BSON\Undefined::__toString()
   Return the empty string. */
static PHP_METHOD(Undefined, __toString)
{
	PHONGO_RETURN_STRINGL("", 0);
} /* }}} */

/* {{{ proto array MongoDB\BSON\Undefined::jsonSerialize()
*/
static PHP_METHOD(Undefined, jsonSerialize)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	array_init_size(return_value, 1);
	ADD_ASSOC_BOOL_EX(return_value, "$undefined", 1);
} /* }}} */

/* {{{ proto string MongoDB\BSON\Undefined::serialize()
*/
static PHP_METHOD(Undefined, serialize)
{
	PHONGO_RETURN_STRING("");
} /* }}} */

/* {{{ proto void MongoDB\BSON\Undefined::unserialize(string $serialized)
*/
static PHP_METHOD(Undefined, unserialize)
{
	zend_error_handling error_handling;
	char*               serialized;
	phongo_zpp_char_len serialized_len;

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
} /* }}} */

/* {{{ MongoDB\BSON\Undefined function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Undefined_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Undefined_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_undefined_me[] = {
	/* clang-format off */
	/* __set_state intentionally missing */
	PHP_ME(Undefined, __toString, ai_Undefined_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Undefined, jsonSerialize, ai_Undefined_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Undefined, serialize, ai_Undefined_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Undefined, unserialize, ai_Undefined_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_Undefined_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\BSON\Undefined object handlers */
static zend_object_handlers php_phongo_handler_undefined;

static void php_phongo_undefined_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_undefined_t* intern = Z_OBJ_UNDEFINED(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_undefined_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_undefined_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_undefined_t, class_type);
	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_undefined;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_undefined_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_undefined;

		return retval;
	}
#endif
} /* }}} */
/* }}} */

void php_phongo_undefined_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Undefined", php_phongo_undefined_me);
	php_phongo_undefined_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_undefined_ce->create_object = php_phongo_undefined_create_object;
	PHONGO_CE_FINAL(php_phongo_undefined_ce);

	zend_class_implements(php_phongo_undefined_ce TSRMLS_CC, 1, php_phongo_json_serializable_ce);
	zend_class_implements(php_phongo_undefined_ce TSRMLS_CC, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_undefined_ce TSRMLS_CC, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_undefined, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	/* Re-assign default handler previously removed in php_phongo.c */
	php_phongo_handler_undefined.clone_obj = zend_objects_clone_obj;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_undefined.free_obj = php_phongo_undefined_free_object;
	php_phongo_handler_undefined.offset   = XtOffsetOf(php_phongo_undefined_t, std);
#endif
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
