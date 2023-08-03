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
#include "BSON/Iterator.h"
#include "BSON/Iterator_arginfo.h"

zend_class_entry* php_phongo_iterator_ce;

static const bson_t* php_phongo_iterator_get_bson_from_zval(zval* zbson)
{
	if (Z_TYPE_P(zbson) != IS_OBJECT) {
		return NULL;
	}

	if (instanceof_function(Z_OBJCE_P(zbson), php_phongo_document_ce)) {
		return Z_DOCUMENT_OBJ_P(zbson)->bson;
	} else if (instanceof_function(Z_OBJCE_P(zbson), php_phongo_packedarray_ce)) {
		return Z_PACKEDARRAY_OBJ_P(zbson)->bson;
	} else {
		return NULL;
	}
}

static bool php_phongo_iterator_init_with_zval(php_phongo_iterator_t* iterator, zval* zbson)
{
	const bson_t* bson;

	bson = php_phongo_iterator_get_bson_from_zval(zbson);
	if (!bson) {
		/* Should never happen, but if it does: exception */
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Could not create iterator for %s instance", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(zbson));

		return false;
	}

	if (!bson_iter_init(&iterator->iter, bson)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not create iterator for %s instance", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(zbson));

		return false;
	}

	ZVAL_COPY(&iterator->bson, zbson);
	if (instanceof_function(Z_OBJCE_P(zbson), php_phongo_packedarray_ce)) {
		iterator->is_array = true;
	}

	iterator->valid = bson_iter_next(&iterator->iter);

	return true;
}

static void php_phongo_iterator_free_current(php_phongo_iterator_t* intern)
{
	if (Z_ISUNDEF(intern->current)) {
		return;
	}

	zval_ptr_dtor(&intern->current);
	ZVAL_UNDEF(&intern->current);
}

static void php_phongo_iterator_build_current(php_phongo_iterator_t* intern)
{
	php_phongo_iterator_free_current(intern);

	phongo_bson_value_to_zval(bson_iter_value(&intern->iter), &intern->current);
}

static HashTable* php_phongo_iterator_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_iterator_t* intern;
	HashTable*             props;

	intern = Z_OBJ_ITERATOR(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 1);

	zend_hash_str_update(props, "bson", sizeof("bson") - 1, &intern->bson);
	Z_TRY_ADDREF(intern->bson);

	return props;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_BSON_Iterator)
PHONGO_DISABLED_WAKEUP(MongoDB_BSON_Iterator)

static PHP_METHOD(MongoDB_BSON_Iterator, current)
{
	php_phongo_iterator_t* intern = Z_ITERATOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->valid) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cannot call current() on an exhausted iterator");
		return;
	}

	if (Z_ISUNDEF(intern->current)) {
		php_phongo_iterator_build_current(intern);
	}

	if (Z_ISUNDEF(intern->current)) {
		RETURN_NULL();
	} else {
		ZVAL_COPY_DEREF(return_value, &intern->current);
	}
}

static PHP_METHOD(MongoDB_BSON_Iterator, key)
{
	const char*            key;
	php_phongo_iterator_t* intern = Z_ITERATOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->valid) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cannot call key() on an exhausted iterator");
		return;
	}

	if (intern->is_array) {
		RETURN_LONG(intern->key);
	}

	key = bson_iter_key(&intern->iter);
	if (!bson_utf8_validate(key, strlen(key), false)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected corrupt BSON data at offset %d", intern->iter.off);
		return;
	}

	RETURN_STRING(key);
}

static PHP_METHOD(MongoDB_BSON_Iterator, next)
{
	php_phongo_iterator_t* intern = Z_ITERATOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	intern->valid = bson_iter_next(&intern->iter);
	intern->key++;
	php_phongo_iterator_free_current(intern);
}

static PHP_METHOD(MongoDB_BSON_Iterator, valid)
{
	php_phongo_iterator_t* intern = Z_ITERATOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(intern->valid);
}

static PHP_METHOD(MongoDB_BSON_Iterator, rewind)
{
	php_phongo_iterator_t* intern = Z_ITERATOR_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	/* Don't re-initialise iterator if we're still on the first item */
	if (intern->key == 0) {
		return;
	}

	php_phongo_iterator_free_current(intern);

	bson_iter_init(&intern->iter, php_phongo_iterator_get_bson_from_zval(&intern->bson));
	intern->key   = 0;
	intern->valid = bson_iter_next(&intern->iter);
}

void phongo_iterator_init(zval* return_value, zval* document_or_packedarray)
{
	php_phongo_iterator_t* intern;

	object_init_ex(return_value, php_phongo_iterator_ce);
	intern = Z_ITERATOR_OBJ_P(return_value);

	php_phongo_iterator_init_with_zval(intern, document_or_packedarray);
}

/* MongoDB\BSON\Iterator object handlers */
static zend_object_handlers php_phongo_handler_iterator;

static void php_phongo_iterator_free_object(zend_object* object)
{
	php_phongo_iterator_t* intern = Z_OBJ_ITERATOR(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	php_phongo_iterator_free_current(intern);

	zval_ptr_dtor(&intern->bson);
}

static zend_object* php_phongo_iterator_create_object(zend_class_entry* class_type)
{
	php_phongo_iterator_t* intern = zend_object_alloc(sizeof(php_phongo_iterator_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_iterator;

	return &intern->std;
}

static zend_object* php_phongo_iterator_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_iterator_t* intern;
	php_phongo_iterator_t* new_intern;
	zend_object*           new_object;

	intern     = Z_OBJ_ITERATOR(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_iterator_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);
	new_intern = Z_OBJ_ITERATOR(new_object);

	php_phongo_iterator_init_with_zval(new_intern, &intern->bson);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	return new_object;
}

static HashTable* php_phongo_iterator_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_iterator_get_properties_hash(object, true);
}

static HashTable* php_phongo_iterator_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_iterator_get_properties_hash(object, false);
}

void php_phongo_iterator_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_iterator_ce                = register_class_MongoDB_BSON_Iterator(zend_ce_iterator);
	php_phongo_iterator_ce->create_object = php_phongo_iterator_create_object;
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_iterator_ce);

	memcpy(&php_phongo_handler_iterator, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_iterator.clone_obj      = php_phongo_iterator_clone_object;
	php_phongo_handler_iterator.get_debug_info = php_phongo_iterator_get_debug_info;
	php_phongo_handler_iterator.get_properties = php_phongo_iterator_get_properties;
	php_phongo_handler_iterator.free_obj       = php_phongo_iterator_free_object;
	php_phongo_handler_iterator.offset         = XtOffsetOf(php_phongo_iterator_t, std);
}
