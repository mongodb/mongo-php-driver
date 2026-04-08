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
#include "ObjectId_arginfo.h"

#define PHONGO_OID_SIZE sizeof(((phongo_objectid_t*) 0)->oid)
#define PHONGO_OID_LEN (PHONGO_OID_SIZE - 1)

zend_class_entry* phongo_objectid_ce;

/* Initialize the object with a generated value and return whether it was
 * successful. */
static bool phongo_objectid_init(phongo_objectid_t* intern)
{
	bson_oid_t oid;

	intern->initialized = true;

	bson_oid_init(&oid, NULL);
	bson_oid_to_string(&oid, intern->oid);

	return true;
}

/* Initialize the object from a hex string and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_objectid_init_from_hex_string(phongo_objectid_t* intern, const char* hex, size_t hex_len)
{
	if (bson_oid_is_valid(hex, hex_len)) {
		bson_oid_t oid;

		bson_oid_init_from_string(&oid, hex);
		bson_oid_to_string(&oid, intern->oid);
		intern->initialized = true;

		return true;
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing ObjectId string: %s", hex);

	return false;
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_objectid_init_from_hash(phongo_objectid_t* intern, HashTable* props)
{
	zval* z_oid;

	z_oid = zend_hash_str_find(props, "oid", sizeof("oid") - 1);

	if (z_oid && Z_TYPE_P(z_oid) == IS_STRING) {
		return phongo_objectid_init_from_hex_string(intern, Z_STRVAL_P(z_oid), Z_STRLEN_P(z_oid));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"oid\" string field", ZSTR_VAL(phongo_objectid_ce->name));
	return false;
}

static HashTable* phongo_objectid_get_properties_hash(zend_object* object, bool is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(objectid, object);

	HashTable* props;

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 1);

	if (!intern->initialized) {
		return props;
	}

	{
		zval zv;

		ZVAL_STRING(&zv, intern->oid);
		zend_hash_str_update(props, "oid", sizeof("oid") - 1, &zv);
	}

	return props;
}

/* Constructs a new BSON ObjectId type, optionally from a hex string. */
static PHP_METHOD(MongoDB_BSON_ObjectId, __construct)
{
	PHONGO_INTERN_FROM_THIS(objectid);

	char*  id = NULL;
	size_t id_len;

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_STRING_OR_NULL(id, id_len)
	PHONGO_PARSE_PARAMETERS_END();

	if (id) {
		phongo_objectid_init_from_hex_string(intern, id, id_len);
	} else {
		phongo_objectid_init(intern);
	}
}

static PHP_METHOD(MongoDB_BSON_ObjectId, getTimestamp)
{
	PHONGO_INTERN_FROM_THIS(objectid);

	bson_oid_t tmp_oid;

	PHONGO_PARSE_PARAMETERS_NONE();

	bson_oid_init_from_string(&tmp_oid, intern->oid);
	RETVAL_LONG(bson_oid_get_time_t(&tmp_oid));
}

static PHP_METHOD(MongoDB_BSON_ObjectId, __set_state)
{
	HashTable* props;
	zval*      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(objectid, return_value);
	props = Z_ARRVAL_P(array);

	phongo_objectid_init_from_hash(intern, props);
}

static PHP_METHOD(MongoDB_BSON_ObjectId, __toString)
{
	PHONGO_INTERN_FROM_THIS(objectid);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRINGL(intern->oid, PHONGO_OID_LEN);
}

static PHP_METHOD(MongoDB_BSON_ObjectId, jsonSerialize)
{
	PHONGO_INTERN_FROM_THIS(objectid);

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(return_value, 1);
	ADD_ASSOC_STRINGL(return_value, "$oid", intern->oid, PHONGO_OID_LEN);
}

static PHP_METHOD(MongoDB_BSON_ObjectId, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(phongo_objectid_get_properties_hash(Z_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_ObjectId, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_objectid_init_from_hash(Z_OBJECTID_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\ObjectId object handlers */
static zend_object_handlers phongo_handler_objectid;

static void phongo_objectid_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(objectid, object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* phongo_objectid_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(objectid, class_type);

	intern->std.handlers = &phongo_handler_objectid;

	return &intern->std;
}

static zend_object* phongo_objectid_clone_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(objectid, object);

	phongo_objectid_t* new_intern;
	zend_object*       new_object;

	new_object = phongo_objectid_create_object(object->ce);

	new_intern = Z_OBJ_OBJECTID(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	// Use memcpy to copy bson value to avoid converting to string and back
	memcpy(&new_intern->oid, &intern->oid, PHONGO_OID_SIZE);
	new_intern->initialized = true;

	return new_object;
}

static int phongo_objectid_compare_objects(zval* o1, zval* o2)
{
	phongo_objectid_t* intern1;
	phongo_objectid_t* intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_OBJECTID_OBJ_P(o1);
	intern2 = Z_OBJECTID_OBJ_P(o2);

	return strcmp(intern1->oid, intern2->oid);
}

static HashTable* phongo_objectid_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return phongo_objectid_get_properties_hash(object, true);
}

static HashTable* phongo_objectid_get_properties(zend_object* object)
{
	return phongo_objectid_get_properties_hash(object, false);
}

void phongo_objectid_init_ce(INIT_FUNC_ARGS)
{
	phongo_objectid_ce                = register_class_MongoDB_BSON_ObjectId(phongo_objectid_interface_ce, phongo_json_serializable_ce, phongo_type_ce, zend_ce_stringable);
	phongo_objectid_ce->create_object = phongo_objectid_create_object;

	memcpy(&phongo_handler_objectid, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_objectid.compare        = phongo_objectid_compare_objects;
	phongo_handler_objectid.clone_obj      = phongo_objectid_clone_object;
	phongo_handler_objectid.get_debug_info = phongo_objectid_get_debug_info;
	phongo_handler_objectid.get_properties = phongo_objectid_get_properties;
	phongo_handler_objectid.free_obj       = phongo_objectid_free_object;
	phongo_handler_objectid.offset         = XtOffsetOf(phongo_objectid_t, std);
}

bool phongo_objectid_new(zval* return_value, const bson_oid_t* oid)
{
	PHONGO_INTERN_INIT_EX(objectid, return_value);
	bson_oid_to_string(oid, intern->oid);
	intern->initialized = true;

	return true;
}
