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
#include <ext/standard/base64.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_operators.h>
#include <ext/standard/php_var.h>
#include <zend_smart_str.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_bson_encode.h"
#include "BSON/ArrayList_arginfo.h"
#include "BSON/Iterator.h"

zend_class_entry* php_phongo_arraylist_ce;

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_arraylist_init_from_hash(php_phongo_arraylist_t* intern, HashTable* props)
{
	zval* data;

	if ((data = zend_hash_str_find(props, "data", sizeof("data") - 1)) && Z_TYPE_P(data) == IS_STRING) {
		zend_string* decoded = php_base64_decode_str(Z_STR_P(data));

		intern->bson = bson_new_from_data((const uint8_t*) ZSTR_VAL(decoded), ZSTR_LEN(decoded));
		zend_string_free(decoded);

		return intern->bson != NULL;
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"data\" string field", ZSTR_VAL(php_phongo_arraylist_ce->name));
	return false;
}

static HashTable* php_phongo_arraylist_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_arraylist_t* intern;
	HashTable*              props;

	intern = Z_OBJ_ARRAYLIST(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->bson) {
		return props;
	}

	{
		zval data, length;

		ZVAL_STR(&data, php_base64_encode((const unsigned char*) bson_get_data(intern->bson), intern->bson->len));
		zend_hash_str_update(props, "data", sizeof("data") - 1, &data);

		ZVAL_LONG(&length, intern->bson->len);
		zend_hash_str_update(props, "length", sizeof("length") - 1, &length);
	}

	return props;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_BSON_ArrayList)

static PHP_METHOD(MongoDB_BSON_ArrayList, fromPHP)
{
	zval                    zv;
	php_phongo_arraylist_t* intern;
	zval*                   data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	if (!zend_array_is_list(Z_ARRVAL_P(data))) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected value to be a list, but given array is not.");
		return;
	}

	object_init_ex(&zv, php_phongo_arraylist_ce);
	intern = Z_ARRAYLIST_OBJ_P(&zv);

	intern->bson = bson_new();
	php_phongo_zval_to_bson(data, PHONGO_BSON_NONE, intern->bson, NULL);

	RETURN_ZVAL(&zv, 1, 1);
}

static bool seek_iter_to_index(bson_iter_t* iter, zend_long index)
{
	for (zend_long i = 0; i <= index; i++) {
		if (!bson_iter_next(iter)) {
			return false;
		}
	}

	return true;
}

static PHP_METHOD(MongoDB_BSON_ArrayList, get)
{
	php_phongo_arraylist_t* intern;
	zend_long               index;
	bson_iter_t             iter;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_LONG(index)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_ARRAYLIST_OBJ_P(getThis());
	if (!bson_iter_init(&iter, intern->bson)) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Could not initialize BSON iterator.");
	}

	if (!seek_iter_to_index(&iter, index)) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Could not find index \"%d\" in BSON data", index);
		RETURN_NULL();
	}

	php_phongo_bson_iter_to_zval(return_value, &iter);
}

static PHP_METHOD(MongoDB_BSON_ArrayList, getIterator)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_iterator_init(return_value, getThis());
}

static PHP_METHOD(MongoDB_BSON_ArrayList, has)
{
	php_phongo_arraylist_t* intern;
	zend_long               index;
	bson_iter_t             iter;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_LONG(index)
	PHONGO_PARSE_PARAMETERS_END();

	intern = Z_ARRAYLIST_OBJ_P(getThis());
	if (!bson_iter_init(&iter, intern->bson)) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Could not initialize BSON iterator.");
	}

	RETURN_BOOL(seek_iter_to_index(&iter, index));
}

static PHP_METHOD(MongoDB_BSON_ArrayList, toPHP)
{
	php_phongo_arraylist_t* intern;
	zval*                   typemap = NULL;
	php_phongo_bson_state   state;

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY(typemap)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_BSON_INIT_STATE(state);

	if (!php_phongo_bson_typemap_to_state(typemap, &state.map)) {
		return;
	}

	intern = Z_ARRAYLIST_OBJ_P(getThis());

	state.is_visiting_array = true;

	if (!php_phongo_bson_to_zval_ex(intern->bson, &state)) {
		zval_ptr_dtor(&state.zchild);
		php_phongo_bson_typemap_dtor(&state.map);
		RETURN_NULL();
	}

	php_phongo_bson_typemap_dtor(&state.map);

	RETURN_ZVAL(&state.zchild, 0, 1);
}

static PHP_METHOD(MongoDB_BSON_ArrayList, __toString)
{
	php_phongo_arraylist_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_ARRAYLIST_OBJ_P(getThis());

	RETVAL_STRINGL((const char*) bson_get_data(intern->bson), intern->bson->len);
}

static PHP_METHOD(MongoDB_BSON_ArrayList, __set_state)
{
	php_phongo_arraylist_t* intern;
	HashTable*              props;
	zval*                   array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_arraylist_ce);

	intern = Z_ARRAYLIST_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_arraylist_init_from_hash(intern, props);
}

static PHP_METHOD(MongoDB_BSON_ArrayList, serialize)
{
	php_phongo_arraylist_t* intern;
	zval                    retval;
	php_serialize_data_t    var_hash;
	smart_str               buf = { 0 };
	zend_string*            str;

	intern = Z_ARRAYLIST_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(&retval, 2);
	str = php_base64_encode(bson_get_data(intern->bson), intern->bson->len);
	ADD_ASSOC_STR(&retval, "data", str);
	ADD_ASSOC_LONG_EX(&retval, "length", intern->bson->len);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	zend_string_free(str);
	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_BSON_ArrayList, unserialize)
{
	php_phongo_arraylist_t* intern;
	char*                   serialized;
	size_t                  serialized_len;
	zval                    props;
	php_unserialize_data_t  var_hash;

	intern = Z_ARRAYLIST_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_arraylist_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_arraylist_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_BSON_ArrayList, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_arraylist_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_ArrayList, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_arraylist_init_from_hash(Z_ARRAYLIST_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\ArrayList object handlers */
static zend_object_handlers php_phongo_handler_arraylist;

static void php_phongo_arraylist_free_object(zend_object* object)
{
	php_phongo_arraylist_t* intern = Z_OBJ_ARRAYLIST(object);

	zend_object_std_dtor(&intern->std);

	if (intern->bson) {
		bson_destroy(intern->bson);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* php_phongo_arraylist_create_object(zend_class_entry* class_type)
{
	php_phongo_arraylist_t* intern = zend_object_alloc(sizeof(php_phongo_arraylist_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_arraylist;

	return &intern->std;
}

static zend_object* php_phongo_arraylist_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_arraylist_t* intern;
	php_phongo_arraylist_t* new_intern;
	zend_object*            new_object;

	intern     = Z_OBJ_ARRAYLIST(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_arraylist_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_ARRAYLIST(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	new_intern->bson = bson_copy(intern->bson);

	return new_object;
}

static int php_phongo_arraylist_compare_objects(zval* o1, zval* o2)
{
	php_phongo_arraylist_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_ARRAYLIST_OBJ_P(o1);
	intern2 = Z_ARRAYLIST_OBJ_P(o2);

	return bson_compare(intern1->bson, intern2->bson);
}

static HashTable* php_phongo_arraylist_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_arraylist_get_properties_hash(object, true);
}

static HashTable* php_phongo_arraylist_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_arraylist_get_properties_hash(object, false);
}

void php_phongo_arraylist_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_arraylist_ce                = register_class_MongoDB_BSON_ArrayList(zend_ce_aggregate, zend_ce_serializable);
	php_phongo_arraylist_ce->create_object = php_phongo_arraylist_create_object;

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_arraylist_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_arraylist, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(arraylist);
	php_phongo_handler_arraylist.clone_obj      = php_phongo_arraylist_clone_object;
	php_phongo_handler_arraylist.get_debug_info = php_phongo_arraylist_get_debug_info;
	php_phongo_handler_arraylist.get_properties = php_phongo_arraylist_get_properties;
	php_phongo_handler_arraylist.free_obj       = php_phongo_arraylist_free_object;
	php_phongo_handler_arraylist.offset         = XtOffsetOf(php_phongo_arraylist_t, std);
}
