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
#include <zend_smart_str.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"

zend_class_entry* php_phongo_cursorid_ce;

/* Initialize the object from a numeric string and return whether it was
 * successful. An exception will be thrown on error. */
static bool php_phongo_cursorid_init_from_string(php_phongo_cursorid_t* intern, const char* s_id, size_t s_id_len) /* {{{ */
{
	int64_t id;

	if (!php_phongo_parse_int64(&id, s_id, s_id_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit id for %s initialization", s_id, ZSTR_VAL(php_phongo_cursorid_ce->name));
		return false;
	}

	intern->id = id;
	return true;
} /* }}} */

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_cursorid_init_from_hash(php_phongo_cursorid_t* intern, HashTable* props) /* {{{ */
{
	zval* value;

	if ((value = zend_hash_str_find(props, "id", sizeof("id") - 1)) && Z_TYPE_P(value) == IS_STRING) {
		return php_phongo_cursorid_init_from_string(intern, Z_STRVAL_P(value), Z_STRLEN_P(value));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"id\" string field", ZSTR_VAL(php_phongo_cursorid_ce->name));
	return false;
} /* }}} */

/* {{{ proto string MongoDB\Driver\CursorId::__toString()
   Returns the string representation of the CursorId */
static PHP_METHOD(CursorId, __toString)
{
	php_phongo_cursorid_t* intern;
	char*                  tmp;
	int                    tmp_len;

	intern = Z_CURSORID_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	tmp_len = spprintf(&tmp, 0, "%" PRId64, intern->id);
	RETVAL_STRINGL(tmp, tmp_len);
	efree(tmp);
} /* }}} */

/* {{{ proto string MongoDB\Driver\CursorId::serialize()
*/
static PHP_METHOD(CursorId, serialize)
{
	php_phongo_cursorid_t* intern;
	zval                   retval;
	php_serialize_data_t   var_hash;
	smart_str              buf = { 0 };

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_CURSORID_OBJ_P(getThis());

	array_init_size(&retval, 1);
	ADD_ASSOC_INT64_AS_STRING(&retval, "id", intern->id);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
} /* }}} */

/* {{{ proto void MongoDB\Driver\CursorId::unserialize(string $serialized)
*/
static PHP_METHOD(CursorId, unserialize)
{
	php_phongo_cursorid_t* intern;
	zend_error_handling    error_handling;
	char*                  serialized;
	size_t                 serialized_len;
	zval                   props;
	php_unserialize_data_t var_hash;

	intern = Z_CURSORID_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_cursorid_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_cursorid_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
} /* }}} */

/* {{{ MongoDB\Driver\CursorId function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_CursorId_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_CursorId_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_cursorid_me[] = {
	/* clang-format off */
	PHP_ME(CursorId, __toString, ai_CursorId_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CursorId, serialize, ai_CursorId_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(CursorId, unserialize, ai_CursorId_unserialize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_CursorId_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_CursorId_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\CursorId object handlers */
static zend_object_handlers php_phongo_handler_cursorid;

static void php_phongo_cursorid_free_object(zend_object* object) /* {{{ */
{
	php_phongo_cursorid_t* intern = Z_OBJ_CURSORID(object);

	zend_object_std_dtor(&intern->std);
} /* }}} */

static zend_object* php_phongo_cursorid_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_cursorid_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_cursorid_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_cursorid;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_cursorid_get_debug_info(zval* object, int* is_temp) /* {{{ */
{
	php_phongo_cursorid_t* intern;
	zval                   retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_CURSORID_OBJ_P(object);

	array_init(&retval);

#if SIZEOF_LONG == 4
	ADD_ASSOC_INT64_AS_STRING(&retval, "id", intern->id);
#else
	ADD_ASSOC_LONG_EX(&retval, "id", intern->id);
#endif

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_cursorid_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "CursorId", php_phongo_cursorid_me);
	php_phongo_cursorid_ce                = zend_register_internal_class(&ce);
	php_phongo_cursorid_ce->create_object = php_phongo_cursorid_create_object;
	PHONGO_CE_FINAL(php_phongo_cursorid_ce);

	zend_class_implements(php_phongo_cursorid_ce, 1, zend_ce_serializable);

	memcpy(&php_phongo_handler_cursorid, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_cursorid.get_debug_info = php_phongo_cursorid_get_debug_info;
	php_phongo_handler_cursorid.free_obj       = php_phongo_cursorid_free_object;
	php_phongo_handler_cursorid.offset         = XtOffsetOf(php_phongo_cursorid_t, std);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
