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

#include "mongoc/mongoc.h"

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "phongo.h"
#include "phongo_error.h"
#include "phongo_util.h"

#include "MongoDB/WriteConcern.h"
#include "WriteConcern_arginfo.h"

zend_class_entry* phongo_writeconcern_ce;

static void phongo_writeconcern_update_properties(phongo_writeconcern_t* intern)
{
	const char* wtag     = mongoc_write_concern_get_wtag(intern->write_concern);
	int32_t     w        = mongoc_write_concern_get_w(intern->write_concern);
	int64_t     wtimeout = mongoc_write_concern_get_wtimeout_int64(intern->write_concern);

	if (wtag) {
		zend_update_property_string(phongo_writeconcern_ce, &intern->std, ZEND_STRL("w"), wtag);
	} else if (mongoc_write_concern_get_wmajority(intern->write_concern)) {
		zend_update_property_string(phongo_writeconcern_ce, &intern->std, ZEND_STRL("w"), PHONGO_WRITE_CONCERN_W_MAJORITY);
	} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
		zend_update_property_long(phongo_writeconcern_ce, &intern->std, ZEND_STRL("w"), w);
	} else {
		zend_update_property_null(phongo_writeconcern_ce, &intern->std, ZEND_STRL("w"));
	}

	if (mongoc_write_concern_journal_is_set(intern->write_concern)) {
		zend_update_property_bool(phongo_writeconcern_ce, &intern->std, ZEND_STRL("j"), mongoc_write_concern_get_journal(intern->write_concern));
	} else {
		zend_update_property_null(phongo_writeconcern_ce, &intern->std, ZEND_STRL("j"));
	}

#if SIZEOF_ZEND_LONG == 4
	if (wtimeout > INT32_MAX || wtimeout < INT32_MIN) {
		zend_update_property_long(phongo_writeconcern_ce, &intern->std, ZEND_STRL("wtimeout"), (zend_long) INT32_MAX);
	} else {
		zend_update_property_long(phongo_writeconcern_ce, &intern->std, ZEND_STRL("wtimeout"), (zend_long) wtimeout);
	}
#else
	zend_update_property_long(phongo_writeconcern_ce, &intern->std, ZEND_STRL("wtimeout"), (zend_long) wtimeout);
#endif
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool phongo_writeconcern_init_from_hash(phongo_writeconcern_t* intern, HashTable* props)
{
	zval *w, *wtimeout, *j;

	intern->write_concern = mongoc_write_concern_new();

	if ((w = zend_hash_str_find(props, "w", sizeof("w") - 1))) {
		if (Z_TYPE_P(w) == IS_LONG) {
			if (Z_LVAL_P(w) < -3) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"w\" integer field to be >= -3", ZSTR_VAL(phongo_writeconcern_ce->name));
				goto failure;
			}
			mongoc_write_concern_set_w(intern->write_concern, Z_LVAL_P(w));
		} else if (Z_TYPE_P(w) == IS_STRING) {
			if (strcmp(Z_STRVAL_P(w), PHONGO_WRITE_CONCERN_W_MAJORITY) == 0) {
				mongoc_write_concern_set_w(intern->write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);
			} else {
				mongoc_write_concern_set_wtag(intern->write_concern, Z_STRVAL_P(w));
			}
		} else if (Z_TYPE_P(w) != IS_NULL) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"w\" field to be integer or string", ZSTR_VAL(phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if ((wtimeout = zend_hash_str_find(props, "wtimeout", sizeof("wtimeout") - 1))) {
		if (Z_TYPE_P(wtimeout) == IS_LONG) {
			if (Z_LVAL_P(wtimeout) < 0) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"wtimeout\" integer field to be >= 0", ZSTR_VAL(phongo_writeconcern_ce->name));
				goto failure;
			}

			mongoc_write_concern_set_wtimeout_int64(intern->write_concern, (int64_t) Z_LVAL_P(wtimeout));
		} else if (Z_TYPE_P(wtimeout) == IS_STRING) {
			int64_t timeout;

			if (!phongo_parse_int64(&timeout, Z_STRVAL_P(wtimeout), Z_STRLEN_P(wtimeout))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit value for %s initialization", Z_STRVAL_P(wtimeout), ZSTR_VAL(phongo_writeconcern_ce->name));
				return false;
			}

			mongoc_write_concern_set_wtimeout_int64(intern->write_concern, timeout);
		} else if (Z_TYPE_P(wtimeout) != IS_NULL) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"wtimeout\" field to be integer or string", ZSTR_VAL(phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if ((j = zend_hash_str_find(props, "j", sizeof("j") - 1))) {
		if (Z_TYPE_P(j) == IS_TRUE || Z_TYPE_P(j) == IS_FALSE) {
			if (zend_is_true(j) && (mongoc_write_concern_get_w(intern->write_concern) == MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Cannot enable journaling when using w = 0");
				goto failure;
			}

			mongoc_write_concern_set_journal(intern->write_concern, zend_is_true(j));
		} else if (Z_TYPE_P(j) != IS_NULL) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"j\" field to be boolean", ZSTR_VAL(phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if (!mongoc_write_concern_is_valid(intern->write_concern)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Write concern is not valid");
		goto failure;
	}

	phongo_writeconcern_update_properties(intern);

	return true;

failure:
	mongoc_write_concern_destroy(intern->write_concern);
	intern->write_concern = NULL;
	return false;
}

/* Constructs a new WriteConcern */
static PHP_METHOD(MongoDB_Driver_WriteConcern, __construct)
{
	PHONGO_INTERN_FROM_THIS(writeconcern);

	zval *    w, *journal = NULL;
	zend_long wtimeout = 0;

	PHONGO_PARSE_PARAMETERS_START(1, 3)
	Z_PARAM_ZVAL(w)
	Z_PARAM_OPTIONAL
	Z_PARAM_LONG(wtimeout)
	Z_PARAM_ZVAL(journal)
	PHONGO_PARSE_PARAMETERS_END();

	intern->write_concern = mongoc_write_concern_new();

	if (Z_TYPE_P(w) == IS_LONG) {
		if (Z_LVAL_P(w) < -3) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected w to be >= -3, %ld given", Z_LVAL_P(w));
			return;
		}
		mongoc_write_concern_set_w(intern->write_concern, Z_LVAL_P(w));
	} else if (Z_TYPE_P(w) == IS_STRING) {
		if (strcmp(Z_STRVAL_P(w), PHONGO_WRITE_CONCERN_W_MAJORITY) == 0) {
			mongoc_write_concern_set_w(intern->write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);
		} else {
			mongoc_write_concern_set_wtag(intern->write_concern, Z_STRVAL_P(w));
		}
	} else {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected w to be integer or string, %s given", zend_zval_type_name(w));
		return;
	}

	switch (ZEND_NUM_ARGS()) {
		case 3:
			if (journal && Z_TYPE_P(journal) != IS_NULL) {
				if (zend_is_true(journal) && (mongoc_write_concern_get_w(intern->write_concern) == MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED)) {
					phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Cannot enable journaling when using w = 0");
					return;
				}

				mongoc_write_concern_set_journal(intern->write_concern, zend_is_true(journal));
			}
			PHONGO_BREAK_INTENTIONALLY_MISSING

		case 2:
			if (wtimeout < 0) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected wtimeout to be >= 0, %" PHONGO_LONG_FORMAT " given", wtimeout);
				return;
			}

			mongoc_write_concern_set_wtimeout_int64(intern->write_concern, (int64_t) wtimeout);
	}

	if (!mongoc_write_concern_is_valid(intern->write_concern)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Write concern is not valid");
		return;
	}

	phongo_writeconcern_update_properties(intern);
}

static PHP_METHOD(MongoDB_Driver_WriteConcern, __set_state)
{
	HashTable* props;
	zval*      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	PHONGO_INTERN_INIT_EX(writeconcern, return_value);
	props = Z_ARRVAL_P(array);

	phongo_writeconcern_init_from_hash(intern, props);
}

/* Returns the WriteConcern "w" option */
static PHP_METHOD(MongoDB_Driver_WriteConcern, getW)
{
	PHONGO_INTERN_FROM_THIS(writeconcern);

	const char* wtag;

	PHONGO_PARSE_PARAMETERS_NONE();

	wtag = mongoc_write_concern_get_wtag(intern->write_concern);

	if (wtag) {
		RETURN_STRING(wtag);
	}

	if (mongoc_write_concern_get_wmajority(intern->write_concern)) {
		RETURN_STRING(PHONGO_WRITE_CONCERN_W_MAJORITY);
	}

	if (mongoc_write_concern_get_w(intern->write_concern) != MONGOC_WRITE_CONCERN_W_DEFAULT) {
		RETURN_LONG(mongoc_write_concern_get_w(intern->write_concern));
	}

	RETURN_NULL();
}

/* Returns the WriteConcern "wtimeout" option */
static PHP_METHOD(MongoDB_Driver_WriteConcern, getWtimeout)
{
	PHONGO_INTERN_FROM_THIS(writeconcern);

	int64_t wtimeout;

	PHONGO_PARSE_PARAMETERS_NONE();

	wtimeout = mongoc_write_concern_get_wtimeout_int64(intern->write_concern);

#if SIZEOF_ZEND_LONG == 4
	if (wtimeout > INT32_MAX || wtimeout < INT32_MIN) {
		zend_error(E_WARNING, "Truncating 64-bit value for wTimeoutMS");
	}
#endif

	RETURN_LONG(wtimeout);
}

/* Returns the WriteConcern "journal" option */
static PHP_METHOD(MongoDB_Driver_WriteConcern, getJournal)
{
	PHONGO_INTERN_FROM_THIS(writeconcern);

	PHONGO_PARSE_PARAMETERS_NONE();

	if (mongoc_write_concern_journal_is_set(intern->write_concern)) {
		RETURN_BOOL(mongoc_write_concern_get_journal(intern->write_concern));
	}

	RETURN_NULL();
}

/* Returns whether the write concern has not been modified (i.e. from a Manager
   with no write concern URI options). */
static PHP_METHOD(MongoDB_Driver_WriteConcern, isDefault)
{
	PHONGO_INTERN_FROM_THIS(writeconcern);

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(mongoc_write_concern_is_default(intern->write_concern));
}

static PHP_METHOD(MongoDB_Driver_WriteConcern, bsonSerialize)
{
	PHONGO_INTERN_FROM_THIS(writeconcern);

	const char* wtag;
	int32_t     w;
	int64_t     wtimeout;

	PHONGO_PARSE_PARAMETERS_NONE();

	wtag     = mongoc_write_concern_get_wtag(intern->write_concern);
	w        = mongoc_write_concern_get_w(intern->write_concern);
	wtimeout = mongoc_write_concern_get_wtimeout_int64(intern->write_concern);

	array_init_size(return_value, 3);

	if (wtag) {
		ADD_ASSOC_STRING(return_value, "w", wtag);
	} else if (mongoc_write_concern_get_wmajority(intern->write_concern)) {
		ADD_ASSOC_STRING(return_value, "w", PHONGO_WRITE_CONCERN_W_MAJORITY);
	} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
		ADD_ASSOC_LONG_EX(return_value, "w", w);
	}

	if (mongoc_write_concern_journal_is_set(intern->write_concern)) {
		ADD_ASSOC_BOOL_EX(return_value, "j", mongoc_write_concern_get_journal(intern->write_concern));
	}

	if (wtimeout != 0) {
		ADD_ASSOC_INT64(return_value, "wtimeout", wtimeout);
	}

	convert_to_object(return_value);
}

static PHP_METHOD(MongoDB_Driver_WriteConcern, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_writeconcern_init_from_hash(Z_WRITECONCERN_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\Driver\WriteConcern object handlers */
static zend_object_handlers phongo_handler_writeconcern;

static void phongo_writeconcern_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(writeconcern, object);

	zend_object_std_dtor(&intern->std);

	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}
}

static zend_object* phongo_writeconcern_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(writeconcern, class_type);

	intern->std.handlers = &phongo_handler_writeconcern;

	return &intern->std;
}

void phongo_writeconcern_init_ce(INIT_FUNC_ARGS)
{
	phongo_writeconcern_ce                = register_class_MongoDB_Driver_WriteConcern(phongo_serializable_ce);
	phongo_writeconcern_ce->create_object = phongo_writeconcern_create_object;

	memcpy(&phongo_handler_writeconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_writeconcern.free_obj = phongo_writeconcern_free_object;
	phongo_handler_writeconcern.offset   = XtOffsetOf(phongo_writeconcern_t, std);
}

void phongo_writeconcern_init(zval* return_value, const mongoc_write_concern_t* write_concern)
{
	PHONGO_INTERN_INIT_EX(writeconcern, return_value);
	intern->write_concern = mongoc_write_concern_copy(write_concern);

	phongo_writeconcern_update_properties(intern);
}

const mongoc_write_concern_t* phongo_write_concern_from_zval(zval* zwrite_concern)
{
	if (zwrite_concern) {
		PHONGO_INTERN_FROM_ZVAL(writeconcern, zwrite_concern);

		if (intern) {
			return intern->write_concern;
		}
	}

	return NULL;
}
