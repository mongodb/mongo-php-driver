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
#include "ObjectId_arginfo.h"

#define PHONGO_OID_SIZE sizeof(((php_phongo_objectid_t*) 0)->oid)
#define PHONGO_OID_LEN (PHONGO_OID_SIZE - 1)

zend_class_entry* php_phongo_objectid_ce;

/* Initialize the object with a generated value and return whether it was
 * successful. */
static bool php_phongo_objectid_init(php_phongo_objectid_t* intern)
{
	bson_oid_t oid;

	intern->initialized = true;

	bson_oid_init(&oid, NULL);
	bson_oid_to_string(&oid, intern->oid);

	return true;
}

/* Initialize the object from a hex string and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_objectid_init_from_hex_string(php_phongo_objectid_t* intern, const char* hex, size_t hex_len)
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
static bool php_phongo_objectid_init_from_hash(php_phongo_objectid_t* intern, HashTable* props)
{
	zval* z_oid;

	z_oid = zend_hash_str_find(props, "oid", sizeof("oid") - 1);

	if (z_oid && Z_TYPE_P(z_oid) == IS_STRING) {
		return php_phongo_objectid_init_from_hex_string(intern, Z_STRVAL_P(z_oid), Z_STRLEN_P(z_oid));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"oid\" string field", ZSTR_VAL(php_phongo_objectid_ce->name));
	return false;
}

static HashTable* php_phongo_objectid_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_objectid_t* intern;
	HashTable*             props;

	intern = Z_OBJ_OBJECTID(PHONGO_COMPAT_GET_OBJ(object));

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
	php_phongo_objectid_t* intern;
	char*                  id = NULL;
	size_t                 id_len;

	intern = Z_OBJECTID_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(0, 1)
	Z_PARAM_OPTIONAL
	Z_PARAM_STRING_OR_NULL(id, id_len)
	PHONGO_PARSE_PARAMETERS_END();

	if (id) {
		php_phongo_objectid_init_from_hex_string(intern, id, id_len);
	} else {
		php_phongo_objectid_init(intern);
	}
}

static PHP_METHOD(MongoDB_BSON_ObjectId, getTimestamp)
{
	php_phongo_objectid_t* intern;
	bson_oid_t             tmp_oid;

	intern = Z_OBJECTID_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	bson_oid_init_from_string(&tmp_oid, intern->oid);
	RETVAL_LONG(bson_oid_get_time_t(&tmp_oid));
}

static PHP_METHOD(MongoDB_BSON_ObjectId, __set_state)
{
	php_phongo_objectid_t* intern;
	HashTable*             props;
	zval*                  array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_objectid_ce);

	intern = Z_OBJECTID_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_objectid_init_from_hash(intern, props);
}

static PHP_METHOD(MongoDB_BSON_ObjectId, __toString)
{
	php_phongo_objectid_t* intern;

	intern = Z_OBJECTID_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_STRINGL(intern->oid, PHONGO_OID_LEN);
}

static PHP_METHOD(MongoDB_BSON_ObjectId, jsonSerialize)
{
	php_phongo_objectid_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_OBJECTID_OBJ_P(getThis());

	array_init_size(return_value, 1);
	ADD_ASSOC_STRINGL(return_value, "$oid", intern->oid, PHONGO_OID_LEN);
}

static PHP_METHOD(MongoDB_BSON_ObjectId, serialize)
{
	php_phongo_objectid_t* intern;
	zval                   retval;
	php_serialize_data_t   var_hash;
	smart_str              buf = { 0 };

	intern = Z_OBJECTID_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	array_init_size(&retval, 1);
	ADD_ASSOC_STRINGL(&retval, "oid", intern->oid, PHONGO_OID_LEN);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_BSON_ObjectId, unserialize)
{
	php_phongo_objectid_t* intern;
	char*                  serialized;
	size_t                 serialized_len;
	zval                   props;
	php_unserialize_data_t var_hash;

	intern = Z_OBJECTID_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_objectid_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_objectid_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_BSON_ObjectId, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_objectid_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_ObjectId, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_objectid_init_from_hash(Z_OBJECTID_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\ObjectId object handlers */
static zend_object_handlers php_phongo_handler_objectid;

static void php_phongo_objectid_free_object(zend_object* object)
{
	php_phongo_objectid_t* intern = Z_OBJ_OBJECTID(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

static zend_object* php_phongo_objectid_create_object(zend_class_entry* class_type)
{
	php_phongo_objectid_t* intern = zend_object_alloc(sizeof(php_phongo_objectid_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_objectid;

	return &intern->std;
}

static zend_object* php_phongo_objectid_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_objectid_t* intern;
	php_phongo_objectid_t* new_intern;
	zend_object*           new_object;

	intern     = Z_OBJ_OBJECTID(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_objectid_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_OBJECTID(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	// Use memcpy to copy bson value to avoid converting to string and back
	memcpy(&new_intern->oid, &intern->oid, PHONGO_OID_SIZE);
	new_intern->initialized = true;

	return new_object;
}

static int php_phongo_objectid_compare_objects(zval* o1, zval* o2)
{
	php_phongo_objectid_t* intern1;
	php_phongo_objectid_t* intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_OBJECTID_OBJ_P(o1);
	intern2 = Z_OBJECTID_OBJ_P(o2);

	return strcmp(intern1->oid, intern2->oid);
}

static HashTable* php_phongo_objectid_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_objectid_get_properties_hash(object, true);
}

static HashTable* php_phongo_objectid_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_objectid_get_properties_hash(object, false);
}

void php_phongo_objectid_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_objectid_ce                = register_class_MongoDB_BSON_ObjectId(php_phongo_objectid_interface_ce, php_phongo_json_serializable_ce, php_phongo_type_ce, zend_ce_serializable);
	php_phongo_objectid_ce->create_object = php_phongo_objectid_create_object;

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_objectid_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_objectid, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(objectid);
	php_phongo_handler_objectid.clone_obj      = php_phongo_objectid_clone_object;
	php_phongo_handler_objectid.get_debug_info = php_phongo_objectid_get_debug_info;
	php_phongo_handler_objectid.get_properties = php_phongo_objectid_get_properties;
	php_phongo_handler_objectid.free_obj       = php_phongo_objectid_free_object;
	php_phongo_handler_objectid.offset         = XtOffsetOf(php_phongo_objectid_t, std);
}

bool phongo_objectid_new(zval* return_value, const bson_oid_t* oid)
{
	php_phongo_objectid_t* intern;

	object_init_ex(return_value, php_phongo_objectid_ce);

	intern = Z_OBJECTID_OBJ_P(return_value);
	bson_oid_to_string(oid, intern->oid);
	intern->initialized = true;

	return true;
}
