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

#include "bson/bson.h"

#include <php.h>
#include <zend_smart_str.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "DBPointer_arginfo.h"

zend_class_entry* php_phongo_dbpointer_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool php_phongo_dbpointer_init(php_phongo_dbpointer_t* intern, const char* ref, size_t ref_len, const char* id, size_t id_len)
{
	if (strlen(ref) != (size_t) ref_len) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Ref cannot contain null bytes");
		return false;
	}

	if (!bson_oid_is_valid(id, id_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing ObjectId string: %s", id);
		return false;
	}

	intern->ref     = estrndup(ref, ref_len);
	intern->ref_len = ref_len;

	memset(intern->id, 0, sizeof(intern->id));
	strncpy(intern->id, id, sizeof(intern->id) - 1);

	return true;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_dbpointer_init_from_hash(php_phongo_dbpointer_t* intern, HashTable* props)
{
	zval *ref, *id;

	if ((ref = zend_hash_str_find(props, "ref", sizeof("ref") - 1)) && Z_TYPE_P(ref) == IS_STRING &&
		(id = zend_hash_str_find(props, "id", sizeof("id") - 1)) && Z_TYPE_P(id) == IS_STRING) {

		return php_phongo_dbpointer_init(intern, Z_STRVAL_P(ref), Z_STRLEN_P(ref), Z_STRVAL_P(id), Z_STRLEN_P(id));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"ref\" and \"id\" string fields", ZSTR_VAL(php_phongo_dbpointer_ce->name));
	return false;
}

HashTable* php_phongo_dbpointer_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_dbpointer_t* intern;
	HashTable*              props;

	intern = Z_OBJ_DBPOINTER(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->ref) {
		return props;
	}

	{
		zval ref, id;

		ZVAL_STRING(&ref, intern->ref);
		ZVAL_STRING(&id, intern->id);
		zend_hash_str_update(props, "ref", sizeof("ref") - 1, &ref);
		zend_hash_str_update(props, "id", sizeof("id") - 1, &id);
	}

	return props;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_BSON_DBPointer)

/* Return the DBPointer's namespace string and ObjectId. */
static PHP_METHOD(MongoDB_BSON_DBPointer, __toString)
{
	php_phongo_dbpointer_t* intern;
	char*                   retval;
	int                     retval_len;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_DBPOINTER_OBJ_P(getThis());

	retval_len = spprintf(&retval, 0, "[%s/%s]", intern->ref, intern->id);
	RETVAL_STRINGL(retval, retval_len);
	efree(retval);
}

static PHP_METHOD(MongoDB_BSON_DBPointer, __set_state)
{
	php_phongo_dbpointer_t* intern;
	HashTable*              props;
	zval*                   array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_dbpointer_ce);

	intern = Z_DBPOINTER_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_dbpointer_init_from_hash(intern, props);
}

static PHP_METHOD(MongoDB_BSON_DBPointer, jsonSerialize)
{
	php_phongo_dbpointer_t* intern;
	zval                    zdb_pointer;
	zval                    zoid;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_DBPOINTER_OBJ_P(getThis());

	array_init_size(&zdb_pointer, 2);
	array_init_size(&zoid, 1);
	ADD_ASSOC_STRINGL(&zdb_pointer, "$ref", intern->ref, intern->ref_len);
	ADD_ASSOC_STRING(&zoid, "$oid", intern->id);
	ADD_ASSOC_ZVAL(&zdb_pointer, "$id", &zoid);

	array_init_size(return_value, 1);
	ADD_ASSOC_ZVAL(return_value, "$dbPointer", &zdb_pointer);
}

static PHP_METHOD(MongoDB_BSON_DBPointer, serialize)
{
	php_phongo_dbpointer_t* intern;
	zval                    retval;
	php_serialize_data_t    var_hash;
	smart_str               buf = { 0 };

	intern = Z_DBPOINTER_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(&retval, 2);
	ADD_ASSOC_STRINGL(&retval, "ref", intern->ref, intern->ref_len);
	ADD_ASSOC_STRING(&retval, "id", intern->id);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_BSON_DBPointer, unserialize)
{
	php_phongo_dbpointer_t* intern;
	char*                   serialized;
	size_t                  serialized_len;
	zval                    props;
	php_unserialize_data_t  var_hash;

	intern = Z_DBPOINTER_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_dbpointer_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_dbpointer_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_BSON_DBPointer, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_dbpointer_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_DBPointer, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_dbpointer_init_from_hash(Z_DBPOINTER_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\DBPointer object handlers */
static zend_object_handlers php_phongo_handler_dbpointer;

static void php_phongo_dbpointer_free_object(zend_object* object)
{
	php_phongo_dbpointer_t* intern = Z_OBJ_DBPOINTER(object);

	zend_object_std_dtor(&intern->std);

	if (intern->ref) {
		efree(intern->ref);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

zend_object* php_phongo_dbpointer_create_object(zend_class_entry* class_type)
{
	php_phongo_dbpointer_t* intern = zend_object_alloc(sizeof(php_phongo_dbpointer_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_dbpointer;

	return &intern->std;
}

static zend_object* php_phongo_dbpointer_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_dbpointer_t* intern;
	php_phongo_dbpointer_t* new_intern;
	zend_object*            new_object;

	intern     = Z_OBJ_DBPOINTER(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_dbpointer_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_DBPOINTER(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_dbpointer_init(new_intern, intern->ref, intern->ref_len, intern->id, 24);

	return new_object;
}

static int php_phongo_dbpointer_compare_objects(zval* o1, zval* o2)
{
	php_phongo_dbpointer_t *intern1, *intern2;
	int                     retval;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_DBPOINTER_OBJ_P(o1);
	intern2 = Z_DBPOINTER_OBJ_P(o2);

	retval = strcmp(intern1->ref, intern2->ref);

	if (retval != 0) {
		return retval;
	}

	return strcmp(intern1->id, intern2->id);
}

static HashTable* php_phongo_dbpointer_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_dbpointer_get_properties_hash(object, true);
}

static HashTable* php_phongo_dbpointer_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_dbpointer_get_properties_hash(object, false);
}

void php_phongo_dbpointer_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_dbpointer_ce                = register_class_MongoDB_BSON_DBPointer(php_phongo_json_serializable_ce, php_phongo_type_ce, zend_ce_serializable);
	php_phongo_dbpointer_ce->create_object = php_phongo_dbpointer_create_object;

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_dbpointer_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_dbpointer, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(dbpointer);
	php_phongo_handler_dbpointer.clone_obj      = php_phongo_dbpointer_clone_object;
	php_phongo_handler_dbpointer.get_debug_info = php_phongo_dbpointer_get_debug_info;
	php_phongo_handler_dbpointer.get_properties = php_phongo_dbpointer_get_properties;
	php_phongo_handler_dbpointer.free_obj       = php_phongo_dbpointer_free_object;
	php_phongo_handler_dbpointer.offset         = XtOffsetOf(php_phongo_dbpointer_t, std);
}

bool phongo_dbpointer_new(zval* object, const char* ref, size_t ref_len, const bson_oid_t* oid)
{
	php_phongo_dbpointer_t* intern;

	object_init_ex(object, php_phongo_dbpointer_ce);

	intern          = Z_DBPOINTER_OBJ_P(object);
	intern->ref     = estrndup(ref, ref_len);
	intern->ref_len = ref_len;
	bson_oid_to_string(oid, intern->id);

	return true;
}
