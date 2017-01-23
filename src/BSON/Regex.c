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
#include <ext/standard/php_var.h>
#if PHP_VERSION_ID >= 70000
# include <zend_smart_str.h>
#else
# include <ext/standard/php_smart_str.h>
#endif
/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_regex_ce;

zend_object_handlers php_phongo_handler_regex;

/* qsort() compare callback for alphabetizing regex flags upon initialization */
static int php_phongo_regex_compare_flags(const void *f1, const void *f2) {
	if (* (const char *) f1 == * (const char *) f2) {
		return 0;
	}

	return (* (const char *) f1 > * (const char *) f2) ? 1 : -1;
}

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_regex_init(php_phongo_regex_t *intern, const char *pattern, phongo_zpp_char_len pattern_len, const char *flags, phongo_zpp_char_len flags_len TSRMLS_DC)
{
	if (strlen(pattern) != pattern_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Pattern cannot contain null bytes");
		return false;
	}
	intern->pattern = estrndup(pattern, pattern_len);
	intern->pattern_len = pattern_len;

	if (flags) {
		if (strlen(flags) != flags_len) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Flags cannot contain null bytes");
			return false;
		}
		intern->flags = estrndup(flags, flags_len);
		intern->flags_len = flags_len;
		/* Ensure flags are alphabetized upon initialization */
		qsort((void *) intern->flags, flags_len, 1, php_phongo_regex_compare_flags);
	} else {
		intern->flags = estrdup("");
		intern->flags_len = 0;
	}

	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_regex_init_from_hash(php_phongo_regex_t *intern, HashTable *props TSRMLS_DC)
{
#if PHP_VERSION_ID >= 70000
	zval *pattern, *flags;

	if ((pattern = zend_hash_str_find(props, "pattern", sizeof("pattern")-1)) && Z_TYPE_P(pattern) == IS_STRING &&
	    (flags = zend_hash_str_find(props, "flags", sizeof("flags")-1)) && Z_TYPE_P(flags) == IS_STRING) {
		return php_phongo_regex_init(intern, Z_STRVAL_P(pattern), Z_STRLEN_P(pattern), Z_STRVAL_P(flags), Z_STRLEN_P(flags) TSRMLS_CC);
	}
#else
	zval **pattern, **flags;

	if (zend_hash_find(props, "pattern", sizeof("pattern"), (void**) &pattern) == SUCCESS && Z_TYPE_PP(pattern) == IS_STRING &&
	    zend_hash_find(props, "flags", sizeof("flags"), (void**) &flags) == SUCCESS && Z_TYPE_PP(flags) == IS_STRING) {
		return php_phongo_regex_init(intern, Z_STRVAL_PP(pattern), Z_STRLEN_PP(pattern), Z_STRVAL_PP(flags), Z_STRLEN_PP(flags) TSRMLS_CC);
	}
#endif

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "%s initialization requires \"pattern\" and \"flags\" string fields", ZSTR_VAL(php_phongo_regex_ce->name));
	return false;
}

/* {{{ proto void Regex::__construct(string $pattern [, string $flags])
   Constructs a new BSON regular expression type. */
PHP_METHOD(Regex, __construct)
{
	php_phongo_regex_t       *intern;
	zend_error_handling       error_handling;
	char                     *pattern;
	phongo_zpp_char_len       pattern_len;
	char                     *flags = NULL;
	phongo_zpp_char_len       flags_len = 0;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = Z_REGEX_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &pattern, &pattern_len, &flags, &flags_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	php_phongo_regex_init(intern, pattern, pattern_len, flags, flags_len TSRMLS_CC);
}
/* }}} */

/* {{{ proto string Regex::getPattern()
*/
PHP_METHOD(Regex, getPattern)
{
	php_phongo_regex_t       *intern;


	intern = Z_REGEX_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	PHONGO_RETURN_STRINGL(intern->pattern, intern->pattern_len);
}
/* }}} */

/* {{{ proto string Regex::getFlags()
*/
PHP_METHOD(Regex, getFlags)
{
	php_phongo_regex_t       *intern;


	intern = Z_REGEX_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	PHONGO_RETURN_STRINGL(intern->flags, intern->flags_len);
}
/* }}} */

/* {{{ proto void Regex::__set_state(array $properties)
*/
PHP_METHOD(Regex, __set_state)
{
	php_phongo_regex_t *intern;
	HashTable          *props;
	zval               *array;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array) == FAILURE) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, php_phongo_regex_ce);

	intern = Z_REGEX_OBJ_P(return_value);
	props = Z_ARRVAL_P(array);

	php_phongo_regex_init_from_hash(intern, props TSRMLS_CC);
}
/* }}} */

/* {{{ proto string Regex::__toString()
   Returns a string in the form: /pattern/flags */
PHP_METHOD(Regex, __toString)
{
	php_phongo_regex_t       *intern;
	char                     *regex;
	int                       regex_len;


	intern = Z_REGEX_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	regex_len = spprintf(&regex, 0, "/%s/%s", intern->pattern, intern->flags);
	PHONGO_RETVAL_STRINGL(regex, regex_len);
	efree(regex);
}
/* }}} */

/* {{{ proto array Regex::jsonSerialize()
*/
PHP_METHOD(Regex, jsonSerialize)
{
	php_phongo_regex_t *intern;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	intern = Z_REGEX_OBJ_P(getThis());

	array_init_size(return_value, 2);
	ADD_ASSOC_STRINGL(return_value, "$regex", intern->pattern, intern->pattern_len);
	ADD_ASSOC_STRINGL(return_value, "$options", intern->flags, intern->flags_len);
}
/* }}} */

/* {{{ proto string Regex::serialize()
*/
PHP_METHOD(Regex, serialize)
{
	php_phongo_regex_t       *intern;
#if PHP_VERSION_ID >= 70000
	zval                      retval;
#else
	zval                     *retval;
#endif
	php_serialize_data_t      var_hash;
	smart_str                 buf = { 0 };

	intern = Z_REGEX_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

#if PHP_VERSION_ID >= 70000
	array_init_size(&retval, 2);
	ADD_ASSOC_STRINGL(&retval, "pattern", intern->pattern, intern->pattern_len);
	ADD_ASSOC_STRINGL(&retval, "flags", intern->flags, intern->flags_len);
#else
	ALLOC_INIT_ZVAL(retval);
	array_init_size(retval, 2);
	ADD_ASSOC_STRINGL(retval, "pattern", intern->pattern, intern->pattern_len);
	ADD_ASSOC_STRINGL(retval, "flags", intern->flags, intern->flags_len);
#endif

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash TSRMLS_CC);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}
/* }}} */

/* {{{ proto string Regex::unserialize(string $serialized)
*/
PHP_METHOD(Regex, unserialize)
{
	php_phongo_regex_t     *intern;
	zend_error_handling     error_handling;
	char                   *serialized;
	phongo_zpp_char_len     serialized_len;
#if PHP_VERSION_ID >= 70000
	zval                    props;
#else
	zval                   *props;
#endif
	php_unserialize_data_t  var_hash;

	intern = Z_REGEX_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &serialized, &serialized_len) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

#if PHP_VERSION_ID < 70000
	ALLOC_INIT_ZVAL(props);
#endif
	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char *) serialized + serialized_len, &var_hash TSRMLS_CC)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE TSRMLS_CC, "%s unserialization failed", ZSTR_VAL(php_phongo_regex_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

#if PHP_VERSION_ID >= 70000
	php_phongo_regex_init_from_hash(intern, HASH_OF(&props) TSRMLS_CC);
#else
	php_phongo_regex_init_from_hash(intern, HASH_OF(props) TSRMLS_CC);
#endif
	zval_ptr_dtor(&props);
}
/* }}} */

/* {{{ BSON\Regex */

ZEND_BEGIN_ARG_INFO_EX(ai_Regex___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Regex___set_state, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Regex_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Regex_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_regex_me[] = {
	PHP_ME(Regex, __construct, ai_Regex___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Regex, __set_state, ai_Regex___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Regex, __toString, ai_Regex_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Regex, jsonSerialize, ai_Regex_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Regex, serialize, ai_Regex_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Regex, unserialize, ai_Regex_unserialize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Regex, getPattern, ai_Regex_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Regex, getFlags, ai_Regex_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_regex_t object handlers */
static void php_phongo_regex_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_regex_t *intern = Z_OBJ_REGEX(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->pattern) {
		efree(intern->pattern);
	}

	if (intern->flags) {
		efree(intern->flags);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_regex_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_regex_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_regex_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_regex;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_regex_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_regex;

		return retval;
	}
#endif
} /* }}} */

static int php_phongo_regex_compare_objects(zval *o1, zval *o2 TSRMLS_DC) /* {{{ */
{
	php_phongo_regex_t *intern1, *intern2;
	int retval;

	intern1 = Z_REGEX_OBJ_P(o1);
	intern2 = Z_REGEX_OBJ_P(o2);

	/* MongoDB compares the pattern string before the flags. */
	retval = strcmp(intern1->pattern, intern2->pattern);

	if (retval != 0) {
		return retval;
	}

	return strcmp(intern1->flags, intern2->flags);
} /* }}} */

static HashTable *php_phongo_regex_get_gc(zval *object, phongo_get_gc_table table, int *n TSRMLS_DC) /* {{{ */
{
	*table = NULL;
	*n = 0;

	return zend_std_get_properties(object TSRMLS_CC);
} /* }}} */

HashTable *php_phongo_regex_get_properties(zval *object TSRMLS_DC) /* {{{ */
{
	php_phongo_regex_t *intern;
	HashTable          *props;

	intern = Z_REGEX_OBJ_P(object);
	props = zend_std_get_properties(object TSRMLS_CC);

	if (!intern->pattern) {
		return props;
	}

#if PHP_VERSION_ID >= 70000
	{
		zval pattern, flags;

		ZVAL_STRINGL(&pattern, intern->pattern, intern->pattern_len);
		zend_hash_str_update(props, "pattern", sizeof("pattern")-1, &pattern);

		ZVAL_STRINGL(&flags, intern->flags, intern->flags_len);
		zend_hash_str_update(props, "flags", sizeof("flags")-1, &flags);
	}
#else
	{
		zval *pattern, *flags;

		MAKE_STD_ZVAL(pattern);
		ZVAL_STRINGL(pattern, intern->pattern, intern->pattern_len, 1);
		zend_hash_update(props, "pattern", sizeof("pattern"), &pattern, sizeof(pattern), NULL);

		MAKE_STD_ZVAL(flags);
		ZVAL_STRINGL(flags, intern->flags, intern->flags_len, 1);
		zend_hash_update(props, "flags", sizeof("flags"), &flags, sizeof(flags), NULL);
	}
#endif

	return props;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Regex)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\BSON", "Regex", php_phongo_regex_me);
	php_phongo_regex_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_regex_ce->create_object = php_phongo_regex_create_object;
	PHONGO_CE_FINAL(php_phongo_regex_ce);

	zend_class_implements(php_phongo_regex_ce TSRMLS_CC, 1, php_phongo_type_ce);
	zend_class_implements(php_phongo_regex_ce TSRMLS_CC, 1, zend_ce_serializable);
	zend_class_implements(php_phongo_regex_ce TSRMLS_CC, 1, php_phongo_json_serializable_ce);

	memcpy(&php_phongo_handler_regex, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_regex.compare_objects = php_phongo_regex_compare_objects;
	php_phongo_handler_regex.get_gc = php_phongo_regex_get_gc;
	php_phongo_handler_regex.get_properties = php_phongo_regex_get_properties;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_regex.free_obj = php_phongo_regex_free_object;
	php_phongo_handler_regex.offset = XtOffsetOf(php_phongo_regex_t, std);
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
