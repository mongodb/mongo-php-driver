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
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_error.h"
#include "DBPointer_arginfo.h"

zend_class_entry* phongo_dbpointer_ce;

/* Initialize the object and return whether it was successful. An exception will
 * be thrown on error. */
static bool phongo_dbpointer_init(phongo_dbpointer_t* intern, const char* ref, size_t ref_len, const char* id, size_t id_len)
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
static bool phongo_dbpointer_init_from_hash(phongo_dbpointer_t* intern, HashTable* props)
{
	zval *ref, *id;

	if ((ref = zend_hash_str_find(props, "ref", sizeof("ref") - 1)) && Z_TYPE_P(ref) == IS_STRING &&
		(id = zend_hash_str_find(props, "id", sizeof("id") - 1)) && Z_TYPE_P(id) == IS_STRING) {

		return phongo_dbpointer_init(intern, Z_STRVAL_P(ref), Z_STRLEN_P(ref), Z_STRVAL_P(id), Z_STRLEN_P(id));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"ref\" and \"id\" string fields", ZSTR_VAL(phongo_dbpointer_ce->name));
	return false;
}

HashTable* phongo_dbpointer_get_properties_hash(zend_object* object, bool is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(dbpointer, object);

	HashTable* props;

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
	PHONGO_INTERN_FROM_THIS(dbpointer);

	char* retval;
	int   retval_len;

	PHONGO_PARSE_PARAMETERS_NONE();

	retval_len = spprintf(&retval, 0, "[%s/%s]", intern->ref, intern->id);
	RETVAL_STRINGL(retval, retval_len);
	efree(retval);
}

static PHP_METHOD(MongoDB_BSON_DBPointer, __set_state)
{
	HashTable* props;
	zval*      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(dbpointer, return_value);
	props = Z_ARRVAL_P(array);

	phongo_dbpointer_init_from_hash(intern, props);
}

static PHP_METHOD(MongoDB_BSON_DBPointer, jsonSerialize)
{
	PHONGO_INTERN_FROM_THIS(dbpointer);

	zval zdb_pointer;
	zval zoid;

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(&zdb_pointer, 2);
	array_init_size(&zoid, 1);
	ADD_ASSOC_STRINGL(&zdb_pointer, "$ref", intern->ref, intern->ref_len);
	ADD_ASSOC_STRING(&zoid, "$oid", intern->id);
	ADD_ASSOC_ZVAL(&zdb_pointer, "$id", &zoid);

	array_init_size(return_value, 1);
	ADD_ASSOC_ZVAL(return_value, "$dbPointer", &zdb_pointer);
}

static PHP_METHOD(MongoDB_BSON_DBPointer, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(phongo_dbpointer_get_properties_hash(Z_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_DBPointer, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_dbpointer_init_from_hash(Z_DBPOINTER_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\DBPointer object handlers */
static zend_object_handlers phongo_handler_dbpointer;

static void phongo_dbpointer_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(dbpointer, object);

	zend_object_std_dtor(&intern->std);

	if (intern->ref) {
		efree(intern->ref);
	}

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

zend_object* phongo_dbpointer_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(dbpointer, class_type);

	intern->std.handlers = &phongo_handler_dbpointer;

	return &intern->std;
}

static zend_object* phongo_dbpointer_clone_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(dbpointer, object);

	phongo_dbpointer_t* new_intern;
	zend_object*        new_object;

	new_object = phongo_dbpointer_create_object(object->ce);

	new_intern = Z_OBJ_DBPOINTER(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	phongo_dbpointer_init(new_intern, intern->ref, intern->ref_len, intern->id, 24);

	return new_object;
}

static int phongo_dbpointer_compare_objects(zval* o1, zval* o2)
{
	phongo_dbpointer_t *intern1, *intern2;
	int                 retval;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_DBPOINTER_OBJ_P(o1);
	intern2 = Z_DBPOINTER_OBJ_P(o2);

	retval = strcmp(intern1->ref, intern2->ref);

	if (retval != 0) {
		return retval;
	}

	return strcmp(intern1->id, intern2->id);
}

static HashTable* phongo_dbpointer_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return phongo_dbpointer_get_properties_hash(object, true);
}

static HashTable* phongo_dbpointer_get_properties(zend_object* object)
{
	return phongo_dbpointer_get_properties_hash(object, false);
}

void phongo_dbpointer_init_ce(INIT_FUNC_ARGS)
{
	phongo_dbpointer_ce                = register_class_MongoDB_BSON_DBPointer(phongo_json_serializable_ce, phongo_type_ce, zend_ce_stringable);
	phongo_dbpointer_ce->create_object = phongo_dbpointer_create_object;

	memcpy(&phongo_handler_dbpointer, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_dbpointer.compare        = phongo_dbpointer_compare_objects;
	phongo_handler_dbpointer.clone_obj      = phongo_dbpointer_clone_object;
	phongo_handler_dbpointer.get_debug_info = phongo_dbpointer_get_debug_info;
	phongo_handler_dbpointer.get_properties = phongo_dbpointer_get_properties;
	phongo_handler_dbpointer.free_obj       = phongo_dbpointer_free_object;
	phongo_handler_dbpointer.offset         = XtOffsetOf(phongo_dbpointer_t, std);
}

bool phongo_dbpointer_new(zval* object, const char* ref, size_t ref_len, const bson_oid_t* oid)
{
	PHONGO_INTERN_INIT_EX(dbpointer, object);

	intern->ref     = estrndup(ref, ref_len);
	intern->ref_len = ref_len;
	bson_oid_to_string(oid, intern->id);

	return true;
}
