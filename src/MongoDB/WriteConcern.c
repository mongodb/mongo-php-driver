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
#include <zend_smart_str.h>
#include <ext/standard/php_var.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_util.h"

#include "MongoDB/WriteConcern.h"
#include "WriteConcern_arginfo.h"

zend_class_entry* php_phongo_writeconcern_ce;

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_writeconcern_init_from_hash(php_phongo_writeconcern_t* intern, HashTable* props)
{
	zval *w, *wtimeout, *j;

	intern->write_concern = mongoc_write_concern_new();

	if ((w = zend_hash_str_find(props, "w", sizeof("w") - 1))) {
		if (Z_TYPE_P(w) == IS_LONG) {
			if (Z_LVAL_P(w) < -3) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"w\" integer field to be >= -3", ZSTR_VAL(php_phongo_writeconcern_ce->name));
				goto failure;
			}
			mongoc_write_concern_set_w(intern->write_concern, Z_LVAL_P(w));
		} else if (Z_TYPE_P(w) == IS_STRING) {
			if (strcmp(Z_STRVAL_P(w), PHONGO_WRITE_CONCERN_W_MAJORITY) == 0) {
				mongoc_write_concern_set_w(intern->write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);
			} else {
				mongoc_write_concern_set_wtag(intern->write_concern, Z_STRVAL_P(w));
			}
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"w\" field to be integer or string", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if ((wtimeout = zend_hash_str_find(props, "wtimeout", sizeof("wtimeout") - 1))) {
		if (Z_TYPE_P(wtimeout) == IS_LONG) {
			if (Z_LVAL_P(wtimeout) < 0) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"wtimeout\" integer field to be >= 0", ZSTR_VAL(php_phongo_writeconcern_ce->name));
				goto failure;
			}

			mongoc_write_concern_set_wtimeout_int64(intern->write_concern, (int64_t) Z_LVAL_P(wtimeout));
		} else if (Z_TYPE_P(wtimeout) == IS_STRING) {
			int64_t timeout;

			if (!php_phongo_parse_int64(&timeout, Z_STRVAL_P(wtimeout), Z_STRLEN_P(wtimeout))) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit value for %s initialization", Z_STRVAL_P(wtimeout), ZSTR_VAL(php_phongo_writeconcern_ce->name));
				return false;
			}

			mongoc_write_concern_set_wtimeout_int64(intern->write_concern, timeout);
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"wtimeout\" field to be integer or string", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if ((j = zend_hash_str_find(props, "j", sizeof("j") - 1))) {
		if (Z_TYPE_P(j) == IS_TRUE || Z_TYPE_P(j) == IS_FALSE) {
			if (zend_is_true(j) && (mongoc_write_concern_get_w(intern->write_concern) == MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED || mongoc_write_concern_get_w(intern->write_concern) == MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED)) {
				phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Cannot enable journaling when using w = 0");
				goto failure;
			}

			mongoc_write_concern_set_journal(intern->write_concern, zend_is_true(j));
		} else {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"j\" field to be boolean", ZSTR_VAL(php_phongo_writeconcern_ce->name));
			goto failure;
		}
	}

	if (!mongoc_write_concern_is_valid(intern->write_concern)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Write concern is not valid");
		goto failure;
	}

	return true;

failure:
	mongoc_write_concern_destroy(intern->write_concern);
	intern->write_concern = NULL;
	return false;
}

/* Constructs a new WriteConcern */
static PHP_METHOD(MongoDB_Driver_WriteConcern, __construct)
{
	php_phongo_writeconcern_t* intern;
	zval *                     w, *journal = NULL;
	zend_long                  wtimeout = 0;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

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
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected w to be integer or string, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(w));
		return;
	}

	switch (ZEND_NUM_ARGS()) {
		case 3:
			if (journal && Z_TYPE_P(journal) != IS_NULL) {
				if (zend_is_true(journal) && (mongoc_write_concern_get_w(intern->write_concern) == MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED || mongoc_write_concern_get_w(intern->write_concern) == MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED)) {
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
}

static PHP_METHOD(MongoDB_Driver_WriteConcern, __set_state)
{
	php_phongo_writeconcern_t* intern;
	HashTable*                 props;
	zval*                      array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_writeconcern_ce);

	intern = Z_WRITECONCERN_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_writeconcern_init_from_hash(intern, props);
}

/* Returns the WriteConcern "w" option */
static PHP_METHOD(MongoDB_Driver_WriteConcern, getW)
{
	php_phongo_writeconcern_t* intern;
	const char*                wtag;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

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
	php_phongo_writeconcern_t* intern;
	int64_t                    wtimeout;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

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
	php_phongo_writeconcern_t* intern;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

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
	php_phongo_writeconcern_t* intern;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_BOOL(mongoc_write_concern_is_default(intern->write_concern));
}

static HashTable* php_phongo_writeconcern_get_properties_hash(zend_object* object, bool is_temp, bool is_bson, bool is_serialize)
{
	php_phongo_writeconcern_t* intern;
	HashTable*                 props;
	const char*                wtag;
	int32_t                    w;
	int64_t                    wtimeout;

	intern = Z_OBJ_WRITECONCERN(object);

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 4);

	if (!intern->write_concern) {
		return props;
	}

	wtag     = mongoc_write_concern_get_wtag(intern->write_concern);
	w        = mongoc_write_concern_get_w(intern->write_concern);
	wtimeout = mongoc_write_concern_get_wtimeout_int64(intern->write_concern);

	{
		zval z_w;

		if (wtag) {
			ZVAL_STRING(&z_w, wtag);
			zend_hash_str_update(props, "w", sizeof("w") - 1, &z_w);
		} else if (mongoc_write_concern_get_wmajority(intern->write_concern)) {
			ZVAL_STRING(&z_w, PHONGO_WRITE_CONCERN_W_MAJORITY);
			zend_hash_str_update(props, "w", sizeof("w") - 1, &z_w);
		} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
			ZVAL_LONG(&z_w, w);
			zend_hash_str_update(props, "w", sizeof("w") - 1, &z_w);
		}

		if (mongoc_write_concern_journal_is_set(intern->write_concern)) {
			zval z_j;

			ZVAL_BOOL(&z_j, mongoc_write_concern_get_journal(intern->write_concern));
			zend_hash_str_update(props, "j", sizeof("j") - 1, &z_j);
		}

		if (wtimeout != 0) {
			zval z_wtimeout;

			if (is_bson) {
				ZVAL_INT64(&z_wtimeout, wtimeout);
			} else if (is_serialize) {
				if (wtimeout > INT32_MAX || wtimeout < INT32_MIN) {
					ZVAL_INT64_STRING(&z_wtimeout, wtimeout);
				} else {
					ZVAL_LONG(&z_wtimeout, wtimeout);
				}
			} else {
#if SIZEOF_ZEND_LONG == 4
				if (wtimeout > INT32_MAX || wtimeout < INT32_MIN) {
					ZVAL_INT64_STRING(&z_wtimeout, wtimeout);
				} else {
					ZVAL_LONG(&z_wtimeout, wtimeout);
				}
#else
				ZVAL_LONG(&z_wtimeout, wtimeout);
#endif
			}

			zend_hash_str_update(props, "wtimeout", sizeof("wtimeout") - 1, &z_wtimeout);
		}
	}

	return props;
}

static PHP_METHOD(MongoDB_Driver_WriteConcern, bsonSerialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	ZVAL_ARR(return_value, php_phongo_writeconcern_get_properties_hash(Z_OBJ_P(getThis()), true, true, false));
	convert_to_object(return_value);
}

static PHP_METHOD(MongoDB_Driver_WriteConcern, serialize)
{
	php_phongo_writeconcern_t* intern;
	zval                       retval;
	php_serialize_data_t       var_hash;
	smart_str                  buf = { 0 };
	const char*                wtag;
	int32_t                    w;
	int64_t                    wtimeout;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	if (!intern->write_concern) {
		return;
	}

	wtag     = mongoc_write_concern_get_wtag(intern->write_concern);
	w        = mongoc_write_concern_get_w(intern->write_concern);
	wtimeout = mongoc_write_concern_get_wtimeout_int64(intern->write_concern);

	array_init_size(&retval, 3);

	if (wtag) {
		ADD_ASSOC_STRING(&retval, "w", wtag);
	} else if (mongoc_write_concern_get_wmajority(intern->write_concern)) {
		ADD_ASSOC_STRING(&retval, "w", PHONGO_WRITE_CONCERN_W_MAJORITY);
	} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
		ADD_ASSOC_LONG_EX(&retval, "w", w);
	}

	if (mongoc_write_concern_journal_is_set(intern->write_concern)) {
		ADD_ASSOC_BOOL_EX(&retval, "j", mongoc_write_concern_get_journal(intern->write_concern));
	}

	if (wtimeout != 0) {
		if (wtimeout > INT32_MAX || wtimeout < INT32_MIN) {
			ADD_ASSOC_INT64_AS_STRING(&retval, "wtimeout", wtimeout);
		} else {
			ADD_ASSOC_LONG_EX(&retval, "wtimeout", wtimeout);
		}
	}

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_Driver_WriteConcern, unserialize)
{
	php_phongo_writeconcern_t* intern;
	char*                      serialized;
	size_t                     serialized_len;
	zval                       props;
	php_unserialize_data_t     var_hash;

	intern = Z_WRITECONCERN_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	if (!serialized_len) {
		return;
	}

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_writeconcern_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_writeconcern_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_Driver_WriteConcern, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_writeconcern_get_properties_hash(Z_OBJ_P(getThis()), true, false, true));
}

static PHP_METHOD(MongoDB_Driver_WriteConcern, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_writeconcern_init_from_hash(Z_WRITECONCERN_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\Driver\WriteConcern object handlers */
static zend_object_handlers php_phongo_handler_writeconcern;

static void php_phongo_writeconcern_free_object(zend_object* object)
{
	php_phongo_writeconcern_t* intern = Z_OBJ_WRITECONCERN(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}

	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}
}

static zend_object* php_phongo_writeconcern_create_object(zend_class_entry* class_type)
{
	php_phongo_writeconcern_t* intern = zend_object_alloc(sizeof(php_phongo_writeconcern_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_writeconcern;

	return &intern->std;
}

static HashTable* php_phongo_writeconcern_get_debug_info(zend_object* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_writeconcern_get_properties_hash(object, true, false, false);
}

static HashTable* php_phongo_writeconcern_get_properties(zend_object* object)
{
	return php_phongo_writeconcern_get_properties_hash(object, false, false, false);
}

void php_phongo_writeconcern_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_writeconcern_ce                = register_class_MongoDB_Driver_WriteConcern(php_phongo_serializable_ce, zend_ce_serializable);
	php_phongo_writeconcern_ce->create_object = php_phongo_writeconcern_create_object;

	memcpy(&php_phongo_handler_writeconcern, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_writeconcern.get_debug_info = php_phongo_writeconcern_get_debug_info;
	php_phongo_handler_writeconcern.get_properties = php_phongo_writeconcern_get_properties;
	php_phongo_handler_writeconcern.free_obj       = php_phongo_writeconcern_free_object;
	php_phongo_handler_writeconcern.offset         = XtOffsetOf(php_phongo_writeconcern_t, std);
}

void phongo_writeconcern_init(zval* return_value, const mongoc_write_concern_t* write_concern)
{
	php_phongo_writeconcern_t* intern;

	object_init_ex(return_value, php_phongo_writeconcern_ce);

	intern                = Z_WRITECONCERN_OBJ_P(return_value);
	intern->write_concern = mongoc_write_concern_copy(write_concern);
}

const mongoc_write_concern_t* phongo_write_concern_from_zval(zval* zwrite_concern)
{
	if (zwrite_concern) {
		php_phongo_writeconcern_t* intern = Z_WRITECONCERN_OBJ_P(zwrite_concern);

		if (intern) {
			return intern->write_concern;
		}
	}

	return NULL;
}

void php_phongo_write_concern_to_zval(zval* retval, const mongoc_write_concern_t* write_concern)
{
	const char*   wtag     = mongoc_write_concern_get_wtag(write_concern);
	const int32_t w        = mongoc_write_concern_get_w(write_concern);
	const int64_t wtimeout = mongoc_write_concern_get_wtimeout_int64(write_concern);

	array_init_size(retval, 4);

	if (wtag) {
		ADD_ASSOC_STRING(retval, "w", wtag);
	} else if (mongoc_write_concern_get_wmajority(write_concern)) {
		ADD_ASSOC_STRING(retval, "w", PHONGO_WRITE_CONCERN_W_MAJORITY);
	} else if (w != MONGOC_WRITE_CONCERN_W_DEFAULT) {
		ADD_ASSOC_LONG_EX(retval, "w", w);
	}

	if (mongoc_write_concern_journal_is_set(write_concern)) {
		ADD_ASSOC_BOOL_EX(retval, "j", mongoc_write_concern_get_journal(write_concern));
	}

	if (wtimeout != 0) {
#if SIZEOF_ZEND_LONG == 4
		if (wtimeout > INT32_MAX || wtimeout < INT32_MIN) {
			ADD_ASSOC_INT64_AS_STRING(retval, "wtimeout", wtimeout);
		} else {
			ADD_ASSOC_LONG_EX(retval, "wtimeout", wtimeout);
		}
#else
		ADD_ASSOC_LONG_EX(retval, "wtimeout", wtimeout);
#endif
	}
}
