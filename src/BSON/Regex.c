/*
 * Copyright 2014-present MongoDB, Inc.
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
#include <zend_smart_str.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "Regex_arginfo.h"

zend_class_entry* php_phongo_regex_ce;

/* qsort() compare callback for alphabetizing regex flags upon initialization */
static int php_phongo_regex_compare_flags(const void* f1, const void* f2)
{
	if (*(const char*) f1 == *(const char*) f2) {
		return 0;
	}

	return (*(const char*) f1 > *(const char*) f2) ? 1 : -1;
}

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_regex_init(php_phongo_regex_t* intern, const char* pattern, size_t pattern_len, const char* flags, size_t flags_len)
{
	if (strlen(pattern) != (size_t) pattern_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Pattern cannot contain null bytes");
		return false;
	}
	intern->pattern     = estrndup(pattern, pattern_len);
	intern->pattern_len = pattern_len;

	if (flags) {
		if (strlen(flags) != (size_t) flags_len) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Flags cannot contain null bytes");
			return false;
		}
		intern->flags     = estrndup(flags, flags_len);
		intern->flags_len = flags_len;
		/* Ensure flags are alphabetized upon initialization */
		qsort((void*) intern->flags, flags_len, 1, php_phongo_regex_compare_flags);
	} else {
		intern->flags     = estrdup("");
		intern->flags_len = 0;
	}

	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_regex_init_from_hash(php_phongo_regex_t* intern, HashTable* props)
{
	zval *pattern, *flags;

	if ((pattern = zend_hash_str_find(props, "pattern", sizeof("pattern") - 1)) && Z_TYPE_P(pattern) == IS_STRING &&
		(flags = zend_hash_str_find(props, "flags", sizeof("flags") - 1)) && Z_TYPE_P(flags) == IS_STRING) {

		return php_phongo_regex_init(intern, Z_STRVAL_P(pattern), Z_STRLEN_P(pattern), Z_STRVAL_P(flags), Z_STRLEN_P(flags));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"pattern\" and \"flags\" string fields", ZSTR_VAL(php_phongo_regex_ce->name));
	return false;
}

static HashTable* php_phongo_regex_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_regex_t* intern;
	HashTable*          props;

	intern = Z_OBJ_REGEX(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->pattern) {
		return props;
	}

	{
		zval pattern, flags;

		ZVAL_STRINGL(&pattern, intern->pattern, intern->pattern_len);
		zend_hash_str_update(props, "pattern", sizeof("pattern") - 1, &pattern);

		ZVAL_STRINGL(&flags, intern->flags, intern->flags_len);
		zend_hash_str_update(props, "flags", sizeof("flags") - 1, &flags);
	}

	return props;
}

/* Constructs a new BSON regular expression type. */
static PHP_METHOD(MongoDB_BSON_Regex, __construct)
{
	php_phongo_regex_t* intern;
	char*               pattern;
	size_t              pattern_len;
	char*               flags     = NULL;
	size_t              flags_len = 0;

	intern = Z_REGEX_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_STRING(pattern, pattern_len)
	Z_PARAM_OPTIONAL
	Z_PARAM_STRING(flags, flags_len)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_regex_init(intern, pattern, pattern_len, flags, flags_len);
}

static PHP_METHOD(MongoDB_BSON_Regex, getPattern)
{
	php_phongo_regex_t* intern;

	intern = Z_REGEX_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRINGL(intern->pattern, intern->pattern_len);
}

static PHP_METHOD(MongoDB_BSON_Regex, getFlags)
{
	php_phongo_regex_t* intern;

	intern = Z_REGEX_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRINGL(intern->flags, intern->flags_len);
}

static PHP_METHOD(MongoDB_BSON_Regex, __set_state)
{
	php_phongo_regex_t* intern;
	HashTable*          props;
	zval*               array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_regex_ce);

	intern = Z_REGEX_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_regex_init_from_hash(intern, props);
}

/* Returns a string in the form: /pattern/flags */
static PHP_METHOD(MongoDB_BSON_Regex, __toString)
{
	php_phongo_regex_t* intern;
	char*               regex;
	int                 regex_len;

	intern = Z_REGEX_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	regex_len = spprintf(&regex, 0, "/%s/%s", intern->pattern, intern->flags);
	RETVAL_STRINGL(regex, regex_len);
	efree(regex);
}

static PHP_METHOD(MongoDB_BSON_Regex, jsonSerialize)
{
	php_phongo_regex_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_REGEX_OBJ_P(getThis());

	array_init_size(return_value, 2);
	ADD_ASSOC_STRINGL(return_value, "$regex", intern->pattern, intern->pattern_len);
	ADD_ASSOC_STRINGL(return_value, "$options", intern->flags, intern->flags_len);
}

static PHP_METHOD(MongoDB_BSON_Regex, serialize)
{
	php_phongo_regex_t*  intern;
	zval                 retval;
	php_serialize_data_t var_hash;
	smart_str            buf = { 0 };

	intern = Z_REGEX_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(&retval, 2);
	ADD_ASSOC_STRINGL(&retval, "pattern", intern->pattern, intern->pattern_len);
	ADD_ASSOC_STRINGL(&retval, "flags", intern->flags, intern->flags_len);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_BSON_Regex, unserialize)
{
	php_phongo_regex_t*    intern;
	char*                  serialized;
	size_t                 serialized_len;
	zval                   props;
	php_unserialize_data_t var_hash;

	intern = Z_REGEX_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_regex_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_regex_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_BSON_Regex, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_regex_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_Regex, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_regex_init_from_hash(Z_REGEX_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\Regex object handlers */
static zend_object_handlers php_phongo_handler_regex;

static void php_phongo_regex_free_object(zend_object* object)
{
	php_phongo_regex_t* intern = Z_OBJ_REGEX(object);

	zend_object_std_dtor(&intern->std);

	if (intern->pattern) {
		efree(intern->pattern);
	}

	if (intern->flags) {
		efree(intern->flags);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* php_phongo_regex_create_object(zend_class_entry* class_type)
{
	php_phongo_regex_t* intern = zend_object_alloc(sizeof(php_phongo_regex_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_regex;

	return &intern->std;
}

static zend_object* php_phongo_regex_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_regex_t* intern;
	php_phongo_regex_t* new_intern;
	zend_object*        new_object;

	intern     = Z_OBJ_REGEX(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_regex_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_REGEX(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_regex_init(new_intern, intern->pattern, intern->pattern_len, intern->flags, intern->flags_len);

	return new_object;
}

static int php_phongo_regex_compare_objects(zval* o1, zval* o2)
{
	php_phongo_regex_t *intern1, *intern2;
	int                 retval;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_REGEX_OBJ_P(o1);
	intern2 = Z_REGEX_OBJ_P(o2);

	/* MongoDB compares the pattern string before the flags. */
	retval = strcmp(intern1->pattern, intern2->pattern);

	if (retval != 0) {
		return retval;
	}

	return strcmp(intern1->flags, intern2->flags);
}

static HashTable* php_phongo_regex_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_regex_get_properties_hash(object, true);
}

static HashTable* php_phongo_regex_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_regex_get_properties_hash(object, false);
}

void php_phongo_regex_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_regex_ce                = register_class_MongoDB_BSON_Regex(php_phongo_regex_interface_ce, php_phongo_json_serializable_ce, php_phongo_type_ce, zend_ce_serializable);
	php_phongo_regex_ce->create_object = php_phongo_regex_create_object;

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_regex_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_regex, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(regex);
	php_phongo_handler_regex.clone_obj      = php_phongo_regex_clone_object;
	php_phongo_handler_regex.get_debug_info = php_phongo_regex_get_debug_info;
	php_phongo_handler_regex.get_properties = php_phongo_regex_get_properties;
	php_phongo_handler_regex.free_obj       = php_phongo_regex_free_object;
	php_phongo_handler_regex.offset         = XtOffsetOf(php_phongo_regex_t, std);
}

bool phongo_regex_new(zval* object, const char* pattern, const char* flags)
{
	php_phongo_regex_t* intern;

	object_init_ex(object, php_phongo_regex_ce);

	intern              = Z_REGEX_OBJ_P(object);
	intern->pattern_len = strlen(pattern);
	intern->pattern     = estrndup(pattern, intern->pattern_len);
	intern->flags_len   = strlen(flags);
	intern->flags       = estrndup(flags, intern->flags_len);

	return true;
}
