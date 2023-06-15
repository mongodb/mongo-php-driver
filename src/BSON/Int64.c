/*
 * Copyright 2018-present MongoDB, Inc.
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
#include <Zend/zend_exceptions.h>

#include "php_phongo.h"
#include "phongo_error.h"
#include "phongo_util.h"
#include "Int64_arginfo.h"

zend_class_entry* php_phongo_int64_ce;

/* Initialize the object and return whether it was successful. */
static bool php_phongo_int64_init(php_phongo_int64_t* intern, int64_t integer)
{
	intern->integer     = integer;
	intern->initialized = true;

	return true;
}

/* Initialize the object from a numeric string and return whether it was
 * successful. An exception will be thrown on error. */
static bool php_phongo_int64_init_from_string(php_phongo_int64_t* intern, const char* s_integer, size_t s_integer_len)
{
	int64_t integer;

	if (!php_phongo_parse_int64(&integer, s_integer, s_integer_len)) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Error parsing \"%s\" as 64-bit integer for %s initialization", s_integer, ZSTR_VAL(php_phongo_int64_ce->name));
		return false;
	}

	return php_phongo_int64_init(intern, integer);
}

/* Initialize the object from a HashTable and return whether it was successful.
 * An exception will be thrown on error. */
static bool php_phongo_int64_init_from_hash(php_phongo_int64_t* intern, HashTable* props)
{
	zval* value;

	if ((value = zend_hash_str_find(props, "integer", sizeof("integer") - 1)) && Z_TYPE_P(value) == IS_STRING) {
		return php_phongo_int64_init_from_string(intern, Z_STRVAL_P(value), Z_STRLEN_P(value));
	}

	phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "%s initialization requires \"integer\" string field", ZSTR_VAL(php_phongo_int64_ce->name));
	return false;
}

HashTable* php_phongo_int64_get_properties_hash(phongo_compat_object_handler_type* object, bool is_temp)
{
	php_phongo_int64_t* intern;
	HashTable*          props;

	intern = Z_OBJ_INT64(PHONGO_COMPAT_GET_OBJ(object));

	PHONGO_GET_PROPERTY_HASH_INIT_PROPS(is_temp, intern, props, 2);

	if (!intern->initialized) {
		return props;
	}

	{
		zval value;

		ZVAL_INT64_STRING(&value, intern->integer);
		zend_hash_str_update(props, "integer", sizeof("integer") - 1, &value);
	}

	return props;
}

static PHP_METHOD(MongoDB_BSON_Int64, __construct)
{
	php_phongo_int64_t* intern;
	zval*               value;

	intern = Z_INT64_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ZVAL(value);
	PHONGO_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(value) == IS_STRING) {
		php_phongo_int64_init_from_string(intern, Z_STRVAL_P(value), Z_STRLEN_P(value));
	} else if (Z_TYPE_P(value) == IS_LONG) {
		php_phongo_int64_init(intern, Z_LVAL_P(value));
	} else {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected value to be integer or string, %s given", PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(value));
	}
}

/* Return the Int64's value as a string. */
static PHP_METHOD(MongoDB_BSON_Int64, __toString)
{
	php_phongo_int64_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_INT64_OBJ_P(getThis());

	ZVAL_INT64_STRING(return_value, intern->integer);
}

static PHP_METHOD(MongoDB_BSON_Int64, __set_state)
{
	php_phongo_int64_t* intern;
	HashTable*          props;
	zval*               array;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(array)
	PHONGO_PARSE_PARAMETERS_END();

	object_init_ex(return_value, php_phongo_int64_ce);

	intern = Z_INT64_OBJ_P(return_value);
	props  = Z_ARRVAL_P(array);

	php_phongo_int64_init_from_hash(intern, props);
}

static PHP_METHOD(MongoDB_BSON_Int64, jsonSerialize)
{
	php_phongo_int64_t* intern;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_INT64_OBJ_P(getThis());

	array_init_size(return_value, 1);

	ADD_ASSOC_INT64_AS_STRING(return_value, "$numberLong", intern->integer);
}

static PHP_METHOD(MongoDB_BSON_Int64, serialize)
{
	php_phongo_int64_t*  intern;
	zval                 retval;
	php_serialize_data_t var_hash;
	smart_str            buf = { 0 };

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_INT64_OBJ_P(getThis());

	array_init_size(&retval, 1);
	ADD_ASSOC_INT64_AS_STRING(&retval, "integer", intern->integer);

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, &retval, &var_hash);
	smart_str_0(&buf);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	PHONGO_RETVAL_SMART_STR(buf);

	smart_str_free(&buf);
	zval_ptr_dtor(&retval);
}

static PHP_METHOD(MongoDB_BSON_Int64, unserialize)
{
	php_phongo_int64_t*    intern;
	char*                  serialized;
	size_t                 serialized_len;
	zval                   props;
	php_unserialize_data_t var_hash;

	intern = Z_INT64_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(serialized, serialized_len)
	PHONGO_PARSE_PARAMETERS_END();

	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&props, (const unsigned char**) &serialized, (unsigned char*) serialized + serialized_len, &var_hash)) {
		zval_ptr_dtor(&props);
		phongo_throw_exception(PHONGO_ERROR_UNEXPECTED_VALUE, "%s unserialization failed", ZSTR_VAL(php_phongo_int64_ce->name));

		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		return;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

	php_phongo_int64_init_from_hash(intern, HASH_OF(&props));
	zval_ptr_dtor(&props);
}

static PHP_METHOD(MongoDB_BSON_Int64, __serialize)
{
	PHONGO_PARSE_PARAMETERS_NONE();

	RETURN_ARR(php_phongo_int64_get_properties_hash(PHONGO_COMPAT_OBJ_P(getThis()), true));
}

static PHP_METHOD(MongoDB_BSON_Int64, __unserialize)
{
	zval* data;

	PHONGO_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_ARRAY(data)
	PHONGO_PARSE_PARAMETERS_END();

	php_phongo_int64_init_from_hash(Z_INT64_OBJ_P(getThis()), Z_ARRVAL_P(data));
}

/* MongoDB\BSON\Int64 object handlers */
static zend_object_handlers php_phongo_handler_int64;

static void php_phongo_int64_free_object(zend_object* object)
{
	php_phongo_int64_t* intern = Z_OBJ_INT64(object);

	zend_object_std_dtor(&intern->std);

	if (intern->properties) {
		zend_hash_destroy(intern->properties);
		FREE_HASHTABLE(intern->properties);
	}
}

zend_object* php_phongo_int64_create_object(zend_class_entry* class_type)
{
	php_phongo_int64_t* intern = zend_object_alloc(sizeof(php_phongo_int64_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_int64;

	return &intern->std;
}

static zend_object* php_phongo_int64_clone_object(phongo_compat_object_handler_type* object)
{
	php_phongo_int64_t* intern;
	php_phongo_int64_t* new_intern;
	zend_object*        new_object;

	intern     = Z_OBJ_INT64(PHONGO_COMPAT_GET_OBJ(object));
	new_object = php_phongo_int64_create_object(PHONGO_COMPAT_GET_OBJ(object)->ce);

	new_intern = Z_OBJ_INT64(new_object);
	zend_objects_clone_members(&new_intern->std, &intern->std);

	php_phongo_int64_init(new_intern, intern->integer);

	return new_object;
}

static int php_phongo_int64_compare_objects(zval* o1, zval* o2)
{
	php_phongo_int64_t *intern1, *intern2;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_INT64_OBJ_P(o1);
	intern2 = Z_INT64_OBJ_P(o2);

	if (intern1->integer != intern2->integer) {
		return intern1->integer < intern2->integer ? -1 : 1;
	}

	return 0;
}

static zend_result php_phongo_int64_cast_object(phongo_compat_object_handler_type* readobj, zval* retval, int type)
{
	php_phongo_int64_t* intern;

	intern = Z_OBJ_INT64(PHONGO_COMPAT_GET_OBJ(readobj));

	switch (type) {
		case IS_DOUBLE:
			ZVAL_DOUBLE(retval, (double) intern->integer);

			return SUCCESS;

		case IS_LONG:
#if PHP_VERSION_ID >= 70300
		case _IS_NUMBER:
#endif
#if SIZEOF_ZEND_LONG == 4
			if (intern->integer > INT32_MAX || intern->integer < INT32_MIN) {
				zend_error(E_WARNING, "Truncating 64-bit integer value %" PRId64 " to 32 bits", intern->integer);
			}
#endif

			ZVAL_LONG(retval, intern->integer);

			return SUCCESS;

		case _IS_BOOL:
			ZVAL_BOOL(retval, intern->integer != 0);

			return SUCCESS;

		default:
			return zend_std_cast_object_tostring(readobj, retval, type);
	}
}

/* Computes the power of two int64_t values by using the exponentiation by
 * squaring algorithm. This is necessary because in case the result exceeds
 * the range of a int64_t, we want PHP to return a float as it would when
 * using 64-bit values directly. We can't use anything involving zend_long
 * here as this would limit us to 32 bits on a 32-bit platform. This also
 * prohibits us from falling back to PHP's default functions after unwrapping
 * the int64_t from the php_phongo_int64_t instance. */
static int64_t phongo_pow_int64(int64_t base, int64_t exp)
{
	if (exp == 0) {
		return 1;
	}

	if (exp % 2) {
		return base * phongo_pow_int64(base * base, (exp - 1) / 2);
	}

	return phongo_pow_int64(base * base, exp / 2);
}

#define OPERATION_RESULT_INT64(value) phongo_int64_new(result, (value));

#define PHONGO_GET_INT64(int64, zval)                                                       \
	if (Z_TYPE_P((zval)) == IS_LONG) {                                                      \
		(int64) = Z_LVAL_P((zval));                                                         \
	} else if (Z_TYPE_P((zval)) == IS_OBJECT && Z_OBJCE_P((zval)) == php_phongo_int64_ce) { \
		(int64) = Z_INT64_OBJ_P((zval))->integer;                                           \
	} else {                                                                                \
		return FAILURE;                                                                     \
	}

#define INT64_SIGN_MASK INT64_MIN

/* Overload arithmetic operators for computation on int64_t values.
 * This ensures that any computation involving at least one php_phongo_int64_t
 * results in a php_phongo_int64_t value, regardless of whether the result
 * would fit in an int32_t or not. Results that exceed the 64-bit integer
 * range are returned as float as PHP would do when using 64-bit integers.
 * Note that ZEND_(PRE|POST)_(INC|DEC) are not handled here: when checking for
 * a do_operation handler for inc/dec, PHP calls the handler with a ZEND_ADD
 * or ZEND_SUB opcode and the same pointer for result and op1, and a ZVAL_LONG
 * of 1 for op2. */
static zend_result php_phongo_int64_do_operation_ex(zend_uchar opcode, zval* result, zval* op1, zval* op2)
{
	int64_t value1, value2, lresult;

	PHONGO_GET_INT64(value1, op1);

	switch (opcode) {
		case ZEND_ADD:
			PHONGO_GET_INT64(value2, op2);

			lresult = value1 + value2;

			/* The following is based on the logic in fast_long_add_function() in PHP.
			 * If the result sign differs from the first operand sign, we have an overflow if:
			 * - adding a positive to a positive yields a negative, or
			 * - adding a negative to a negative (i.e. subtraction) yields a positive */
			if ((value1 & INT64_SIGN_MASK) != (lresult & INT64_SIGN_MASK) && (value1 & INT64_SIGN_MASK) == (value2 & INT64_SIGN_MASK)) {
				ZVAL_DOUBLE(result, (double) value1 + (double) value2);
			} else {
				OPERATION_RESULT_INT64(lresult);
			}

			return SUCCESS;

		case ZEND_SUB:
			PHONGO_GET_INT64(value2, op2);

			lresult = value1 - value2;

			/* The following is based on the logic in fast_long_sub_function() in PHP.
			 * If the result sign differs from the first operand sign, we have an overflow if:
			 * - subtracting a positive from a negative yields a positive, or
			 * - subtracting a negative from a positive (i.e. addition) yields a negative */
			if ((value1 & INT64_SIGN_MASK) != (lresult & INT64_SIGN_MASK) && (value1 & INT64_SIGN_MASK) != (value2 & INT64_SIGN_MASK)) {
				ZVAL_DOUBLE(result, (double) value1 - (double) value2);
			} else {
				OPERATION_RESULT_INT64(lresult);
			}

			return SUCCESS;

		case ZEND_MUL:
			PHONGO_GET_INT64(value2, op2);

			/* The following is based on the C-native implementation of
			 * ZEND_SIGNED_MULTIPLY_LONG() in PHP if no other methods (e.g. ASM
			 * or _builtin_smull_overflow) can be used. */
			{
				int64_t     lres  = value1 * value2;
				long double dres  = (long double) value1 * (long double) value2;
				long double delta = (long double) lres - dres;

				if ((dres + delta) != dres) {
					ZVAL_DOUBLE(result, dres);
				} else {
					OPERATION_RESULT_INT64(lres);
				}
			}

			return SUCCESS;

		case ZEND_DIV:
			PHONGO_GET_INT64(value2, op2);
			if (value2 == 0) {
				zend_throw_exception(zend_ce_division_by_zero_error, "Division by zero", 0);
				return FAILURE;
			}

			/* The following is based on div_function_base() in PHP.
			 * - INT64_MIN / 1 exceeds the int64 range -> return double
			 * - if division has a remainder, return double as result can't be
			 *   an int */
			if ((value1 == INT64_MIN && value2 == -1) || (value1 % value2 != 0)) {
				ZVAL_DOUBLE(result, (double) value1 / value2);
			} else {
				OPERATION_RESULT_INT64(value1 / value2);
			}

			return SUCCESS;

		case ZEND_MOD:
			PHONGO_GET_INT64(value2, op2);
			if (value2 == 0) {
				zend_throw_exception(zend_ce_division_by_zero_error, "Division by zero", 0);
				return FAILURE;
			}

			OPERATION_RESULT_INT64(value1 % value2);
			return SUCCESS;

		case ZEND_SL:
			PHONGO_GET_INT64(value2, op2);
			OPERATION_RESULT_INT64(value1 << value2);
			return SUCCESS;

		case ZEND_SR:
			PHONGO_GET_INT64(value2, op2);
			OPERATION_RESULT_INT64(value1 >> value2);
			return SUCCESS;

		case ZEND_POW:
			PHONGO_GET_INT64(value2, op2);

			// Negative exponents always yield floats, leave them for PHP to handle
			if (value2 < 0) {
				return FAILURE;
			}

			// Handle 0 separately to distinguish between base 0 and
			// phongo_pow_int64 overflowing
			if (value1 == 0) {
				OPERATION_RESULT_INT64(0);
				return SUCCESS;
			}

			{
				int64_t pow_result = phongo_pow_int64(value1, value2);

				// If the result would overflow an int64_t, we let PHP fall back
				// to its default pow() implementation which returns a float.
				if (pow_result == 0) {
					return FAILURE;
				}

				OPERATION_RESULT_INT64(pow_result);
			}

			return SUCCESS;

		case ZEND_BW_AND:
			PHONGO_GET_INT64(value2, op2);
			OPERATION_RESULT_INT64(value1 & value2);
			return SUCCESS;

		case ZEND_BW_OR:
			PHONGO_GET_INT64(value2, op2);
			OPERATION_RESULT_INT64(value1 | value2);
			return SUCCESS;

		case ZEND_BW_XOR:
			PHONGO_GET_INT64(value2, op2);
			OPERATION_RESULT_INT64(value1 ^ value2);
			return SUCCESS;

		case ZEND_BW_NOT:
			OPERATION_RESULT_INT64(~value1);
			return SUCCESS;

		default:
			return FAILURE;
	}
}

static zend_result php_phongo_int64_do_operation(zend_uchar opcode, zval* result, zval* op1, zval* op2)
{
	zval op1_copy;
	int  retval;

	// Copy op1 for unary operations (e.g. $int64++) to ensure correct return values
	if (result == op1) {
		ZVAL_COPY_VALUE(&op1_copy, op1);
		op1 = &op1_copy;
	}

	retval = php_phongo_int64_do_operation_ex(opcode, result, op1, op2);

	if (retval == SUCCESS && op1 == &op1_copy) {
		zval_ptr_dtor(op1);
	}

	return retval;
}

#undef OPERATION_RESULT_INT64
#undef PHONGO_GET_INT64
#undef INT64_SIGN_MASK

static HashTable* php_phongo_int64_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp)
{
	*is_temp = 1;
	return php_phongo_int64_get_properties_hash(object, true);
}

static HashTable* php_phongo_int64_get_properties(phongo_compat_object_handler_type* object)
{
	return php_phongo_int64_get_properties_hash(object, false);
}

void php_phongo_int64_init_ce(INIT_FUNC_ARGS)
{
	php_phongo_int64_ce                = register_class_MongoDB_BSON_Int64(php_phongo_json_serializable_ce, php_phongo_type_ce, zend_ce_serializable);
	php_phongo_int64_ce->create_object = php_phongo_int64_create_object;

#if PHP_VERSION_ID >= 80000
	zend_class_implements(php_phongo_int64_ce, 1, zend_ce_stringable);
#endif

	memcpy(&php_phongo_handler_int64, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(int64);
	php_phongo_handler_int64.clone_obj      = php_phongo_int64_clone_object;
	php_phongo_handler_int64.get_debug_info = php_phongo_int64_get_debug_info;
	php_phongo_handler_int64.get_properties = php_phongo_int64_get_properties;
	php_phongo_handler_int64.free_obj       = php_phongo_int64_free_object;
	php_phongo_handler_int64.offset         = XtOffsetOf(php_phongo_int64_t, std);
	php_phongo_handler_int64.cast_object    = php_phongo_int64_cast_object;
	php_phongo_handler_int64.do_operation   = php_phongo_int64_do_operation;
}

bool phongo_int64_new(zval* object, int64_t integer)
{
	php_phongo_int64_t* intern;

	object_init_ex(object, php_phongo_int64_ce);

	intern              = Z_INT64_OBJ_P(object);
	intern->integer     = integer;
	intern->initialized = true;

	return true;
}
