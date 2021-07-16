/*
 * Copyright 2021-present MongoDB, Inc.
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
#include <zend_smart_str.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_serverdescription_ce;

/* {{{ proto string|null MongoDB\Driver\ServerDescription::getHelloResponse()
   Returns the the most recent “hello” response */
static PHP_METHOD(ServerDescription, getHelloResponse)
{
	zend_error_handling             error_handling;
	php_phongo_serverdescription_t* intern;
	const bson_t*                   getHelloResponse;

	intern = Z_SERVERDESCRIPTION_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	getHelloResponse = mongoc_server_description_hello_response(intern->server_description);

	if (getHelloResponse->len) {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!php_phongo_bson_to_zval_ex(bson_get_data(getHelloResponse), getHelloResponse->len, &state)) {
			zval_ptr_dtor(&state.zchild);
			return;
		}

		RETURN_ZVAL(&state.zchild, 0, 1);
	} else {
		RETURN_NULL();
	}
} /* }}} */

/* {{{ MongoDB\Driver\ServerDescription function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_ServerDescription_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_serverdescription_me[] = {
	/* clang-format off */
	PHP_ME(ServerDescription, getHelloResponse, ai_ServerDescription_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)

	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_ServerDescription_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\ServerDescription object handlers */
static zend_object_handlers php_phongo_handler_serverdescription;

static void php_phongo_serverdescription_free_object(zend_object* object) /* {{{ */
{
	php_phongo_serverdescription_t* intern = Z_OBJ_SERVERDESCRIPTION(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->server_description) {
		mongoc_server_description_destroy(intern->server_description);
	}
}

static zend_object* php_phongo_serverdescription_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_serverdescription_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_serverdescription_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_serverdescription;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_serverdescription_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	*is_temp = 1;
	// return php_phongo_server_description_get_properties_hash(object, true);
} /* }}} */

static HashTable* php_phongo_serverdescription_get_properties(phongo_compat_object_handler_type* object) /* {{{ */
{
	// return php_phongo_serverdescription_get_properties_hash(object, false);
} /* }}} */
/* }}} */

void php_phongo_serverdescription_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "ServerDescription", php_phongo_serverdescription_me);
	php_phongo_serverdescription_ce                = zend_register_internal_class(&ce);
	php_phongo_serverdescription_ce->create_object = php_phongo_serverdescription_create_object;
	PHONGO_CE_FINAL(php_phongo_serverdescription_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_serverdescription_ce);

	zend_class_implements(php_phongo_serverdescription_ce, 1, zend_ce_iterator);

	memcpy(&php_phongo_handler_serverdescription, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_serverdescription.get_debug_info = php_phongo_serverdescription_get_debug_info;
	php_phongo_handler_serverdescription.get_properties = php_phongo_serverdescription_get_properties;
	php_phongo_handler_serverdescription.free_obj       = php_phongo_serverdescription_free_object;
	php_phongo_handler_serverdescription.offset         = XtOffsetOf(php_phongo_serverdescription_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
