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

#include "phongo.h"
#include "phongo_error.h"
#include "BSON/Iterator.h"
#include "BSON/Iterator_arginfo.h"

zend_class_entry* phongo_iterator_ce;

static const bson_t* phongo_iterator_get_bson_from_zval(zval* zbson)
{
	if (Z_TYPE_P(zbson) != IS_OBJECT) {
		return NULL;
	}

	if (instanceof_function(Z_OBJCE_P(zbson), phongo_document_ce)) {
		return Z_DOCUMENT_OBJ_P(zbson)->bson;
	} else if (instanceof_function(Z_OBJCE_P(zbson), phongo_packedarray_ce)) {
		return Z_PACKEDARRAY_OBJ_P(zbson)->bson;
	} else {
		return NULL;
	}
}

static bool phongo_iterator_init_with_zval(phongo_iterator_t* iterator, zval* zbson)
{
	const bson_t* bson;

	bson = phongo_iterator_get_bson_from_zval(zbson);
	if (!bson) {
		/* Should never happen, but if it does: exception */
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Could not create iterator for %s instance", zend_zval_type_name(zbson));

		return false;
	}

	if (!bson_iter_init(&iterator->iter, bson)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Could not create iterator for %s instance", zend_zval_type_name(zbson));

		return false;
	}

	ZVAL_COPY(&iterator->bson, zbson);
	if (instanceof_function(Z_OBJCE_P(zbson), phongo_packedarray_ce)) {
		iterator->is_array = true;
	}

	iterator->valid = bson_iter_next(&iterator->iter);

	return true;
}

static void phongo_iterator_free_current(phongo_iterator_t* intern)
{
	if (Z_ISUNDEF(intern->current)) {
		return;
	}

	zval_ptr_dtor(&intern->current);
	ZVAL_UNDEF(&intern->current);
}

static void phongo_iterator_build_current(phongo_iterator_t* intern)
{
	phongo_iterator_free_current(intern);

	phongo_bson_value_to_zval(bson_iter_value(&intern->iter), &intern->current);
}

static zval* phongo_iterator_get_current(phongo_iterator_t* intern)
{
	if (!intern->valid) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cannot call current() on an exhausted iterator");
		return NULL;
	}

	if (Z_ISUNDEF(intern->current)) {
		phongo_iterator_build_current(intern);
	}

	return &intern->current;
}

static void phongo_iterator_next(phongo_iterator_t* intern)
{
	intern->valid = bson_iter_next(&intern->iter);
	intern->key++;
	phongo_iterator_free_current(intern);
}

static bool phongo_iterator_key(phongo_iterator_t* intern, zval* key)
{
	const char* ckey;

	if (!intern->valid) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "Cannot call key() on an exhausted iterator");
		return false;
	}

	if (intern->is_array) {
		ZVAL_LONG(key, intern->key);

		return true;
	}

	ckey = bson_iter_key(&intern->iter);
	if (!bson_utf8_validate(ckey, strlen(ckey), false)) {
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "Detected corrupt BSON data at offset %d", intern->iter.off);
		return false;
	}

	ZVAL_STRING(key, ckey);

	return true;
}

static void phongo_iterator_rewind(phongo_iterator_t* intern)
{
	/* Don't re-initialise iterator if we're still on the first item */
	if (intern->key == 0) {
		return;
	}

	phongo_iterator_free_current(intern);

	bson_iter_init(&intern->iter, phongo_iterator_get_bson_from_zval(&intern->bson));
	intern->key   = 0;
	intern->valid = bson_iter_next(&intern->iter);
}

static HashTable* phongo_iterator_get_properties_hash(zend_object* object, bool is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(iterator, object);

	HashTable* props;

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 1);

	zend_hash_str_update(props, "bson", sizeof("bson") - 1, &intern->bson);
	Z_TRY_ADDREF(intern->bson);

	return props;
}

PHONGO_DISABLED_CONSTRUCTOR(MongoDB_BSON_Iterator)

static PHP_METHOD(MongoDB_BSON_Iterator, current)
{
	PHONGO_INTERN_FROM_THIS(iterator);

	zval* data;

	PHONGO_PARSE_PARAMETERS_NONE();

	data = phongo_iterator_get_current(intern);
	if (!data) {
		// Exception already thrown
		RETURN_NULL();
	}

	if (Z_ISUNDEF_P(data)) {
		RETURN_NULL();
	} else {
		ZVAL_COPY_DEREF(return_value, data);
	}
}

static PHP_METHOD(MongoDB_BSON_Iterator, key)
{
	PHONGO_INTERN_FROM_THIS(iterator);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!phongo_iterator_key(intern, return_value)) {
		// Exception already thrown
		return;
	}
}

static PHP_METHOD(MongoDB_BSON_Iterator, next)
{
	PHONGO_INTERN_FROM_THIS(iterator);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_iterator_next(intern);
}

static PHP_METHOD(MongoDB_BSON_Iterator, valid)
{
	PHONGO_INTERN_FROM_THIS(iterator);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(intern->valid);
}

static PHP_METHOD(MongoDB_BSON_Iterator, rewind)
{
	PHONGO_INTERN_FROM_THIS(iterator);

	PHONGO_PARSE_PARAMETERS_NONE();

	phongo_iterator_rewind(intern);
}

void phongo_iterator_init(zval* return_value, zval* document_or_packedarray)
{
	PHONGO_INTERN_INIT_EX(iterator, return_value);

	phongo_iterator_init_with_zval(intern, document_or_packedarray);
}

/* MongoDB\BSON\Iterator object handlers */
static zend_object_handlers phongo_handler_iterator;

static void phongo_iterator_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(iterator, object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	phongo_iterator_free_current(intern);

	zval_ptr_dtor(&intern->bson);
}

static zend_object* phongo_iterator_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(iterator, class_type);

	intern->std.handlers = &phongo_handler_iterator;

	return &intern->std;
}

static zend_object* phongo_iterator_clone_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(iterator, object);

	phongo_iterator_t* new_intern;
	zend_object*       new_object;

	new_object = phongo_iterator_create_object(object->ce);
	new_intern = Z_OBJ_ITERATOR(new_object);

	phongo_iterator_init_with_zval(new_intern, &intern->bson);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	return new_object;
}

static HashTable* phongo_iterator_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return phongo_iterator_get_properties_hash(object, true);
}

static HashTable* phongo_iterator_get_properties(zend_object* object)
{
	return phongo_iterator_get_properties_hash(object, false);
}

/* Iterator handlers */
static void phongo_iterator_it_dtor(zend_object_iterator* iter)
{
	zval_ptr_dtor(&iter->data);
}

static PHONGO_ITERATOR_VALID_RESULT phongo_iterator_it_valid(zend_object_iterator* iter)
{
	PHONGO_INTERN_FROM_ZVAL(iterator, &iter->data);

	return intern->valid ? SUCCESS : FAILURE;
}

static zval* phongo_iterator_it_get_current_data(zend_object_iterator* iter)
{
	PHONGO_INTERN_FROM_ZVAL(iterator, &iter->data);

	return phongo_iterator_get_current(intern);
}

static void phongo_iterator_it_get_current_key(zend_object_iterator* iter, zval* key)
{
	PHONGO_INTERN_FROM_ZVAL(iterator, &iter->data);

	if (!phongo_iterator_key(intern, key)) {
		// Exception already thrown
		return;
	}
}

static void phongo_iterator_it_move_forward(zend_object_iterator* iter)
{
	PHONGO_INTERN_FROM_ZVAL(iterator, &iter->data);

	phongo_iterator_next(intern);
}

static void phongo_iterator_it_rewind(zend_object_iterator* iter)
{
	PHONGO_INTERN_FROM_ZVAL(iterator, &iter->data);

	phongo_iterator_rewind(intern);
}

static HashTable* phongo_iterator_it_get_gc(zend_object_iterator* iter, zval** table, int* n)
{
	*n     = 1;
	*table = &iter->data;
	return NULL;
}

static const zend_object_iterator_funcs phongo_iterator_it_funcs = {
	phongo_iterator_it_dtor,
	phongo_iterator_it_valid,
	phongo_iterator_it_get_current_data,
	phongo_iterator_it_get_current_key,
	phongo_iterator_it_move_forward,
	phongo_iterator_it_rewind,
	NULL, /* invalidate_current */
	phongo_iterator_it_get_gc
};

static zend_object_iterator* phongo_iterator_get_iterator(zend_class_entry* ce, zval* object, int by_ref)
{
	zend_object_iterator* iterator;

	if (by_ref) {
		phongo_throw_exception(PHONGO_ERROR_LOGIC, "An iterator cannot be used with foreach by reference");
		return NULL;
	}

	iterator = emalloc(sizeof(zend_object_iterator));
	zend_iterator_init(iterator);

	ZVAL_OBJ_COPY(&iterator->data, Z_OBJ_P(object));

	iterator->funcs = &phongo_iterator_it_funcs;

	return iterator;
}

void phongo_iterator_init_ce(INIT_FUNC_ARGS)
{
	phongo_iterator_ce                = register_class_MongoDB_BSON_Iterator(zend_ce_iterator);
	phongo_iterator_ce->create_object = phongo_iterator_create_object;
	phongo_iterator_ce->get_iterator  = phongo_iterator_get_iterator;

	memcpy(&phongo_handler_iterator, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_iterator.clone_obj      = phongo_iterator_clone_object;
	phongo_handler_iterator.get_debug_info = phongo_iterator_get_debug_info;
	phongo_handler_iterator.get_properties = phongo_iterator_get_properties;
	phongo_handler_iterator.free_obj       = phongo_iterator_free_object;
	phongo_handler_iterator.offset         = XtOffsetOf(phongo_iterator_t, std);
}
