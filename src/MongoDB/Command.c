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

#include "php_array_api.h"

#include "phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"
#include "Command_arginfo.h"

zend_class_entry* phongo_command_ce;

/* Initialize the "maxAwaitTimeMS" option. Returns true on success; otherwise,
 * false is returned and an exception is thrown.
 *
 * The "maxAwaitTimeMS" option is assigned to the cursor after query execution
 * via mongoc_cursor_set_max_await_time_ms(). */
static bool phongo_command_init_max_await_time_ms(phongo_command_t* intern, zval* options)
{
	int64_t max_await_time_ms;

	if (!php_array_existsc(options, "maxAwaitTimeMS")) {
		return true;
	}

	max_await_time_ms = php_array_fetchc_long(options, "maxAwaitTimeMS");

	if (max_await_time_ms < 0) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"maxAwaitTimeMS\" option to be >= 0, %" PRId64 " given", max_await_time_ms);
		return false;
	}

	if (max_await_time_ms > UINT32_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"maxAwaitTimeMS\" option to be <= %" PRIu32 ", %" PRId64 " given", UINT32_MAX, max_await_time_ms);
		return false;
	}

	intern->max_await_time_ms = (uint32_t) max_await_time_ms;

	return true;
}

/* Initializes the phongo_command_init from options argument. This
 * function will fall back to a modifier in the absence of a top-level option
 * (where applicable). */
static bool phongo_command_init(phongo_command_t* intern, zval* filter, zval* options)
{
	bson_iter_t iter;
	bson_iter_t sub_iter;

	intern->bson              = bson_new();
	intern->batch_size        = 0;
	intern->max_await_time_ms = 0;

	phongo_zval_to_bson(filter, PHONGO_BSON_NONE, intern->bson, NULL);

	/* Note: if any exceptions are thrown, we can simply return as PHP will
	 * invoke phongo_query_free_object to destruct the object. */
	if (EG(exception)) {
		return false;
	}

	if (bson_iter_init(&iter, intern->bson) && bson_iter_find_descendant(&iter, "cursor.batchSize", &sub_iter) && BSON_ITER_HOLDS_INT(&sub_iter)) {
		int64_t batch_size = bson_iter_as_int64(&sub_iter);

		if (batch_size >= 0 && batch_size <= UINT32_MAX) {
			intern->batch_size = (uint32_t) batch_size;
		}
	}

	if (!options) {
		return true;
	}

	if (!phongo_command_init_max_await_time_ms(intern, options)) {
		return false;
	}

	return true;
}

/* Constructs a new Command */
static PHP_METHOD(MongoDB_Driver_Command, __construct)
{
	PHONGO_INTERN_FROM_THIS(command);

	zval* document;
	zval* options = NULL;

	PHONGO_PARSE_PARAMETERS_START(1, 2)
	Z_PARAM_ARRAY_OR_OBJECT(document)
	Z_PARAM_OPTIONAL
	Z_PARAM_ARRAY_OR_NULL(options)
	PHONGO_PARSE_PARAMETERS_END();

	phongo_command_init(intern, document, options);
}

/* MongoDB\Driver\Command object handlers */
static zend_object_handlers phongo_handler_command;

static void phongo_command_free_object(zend_object* object)
{
	PHONGO_INTERN_FROM_Z_OBJ(command, object);

	zend_object_std_dtor(&intern->std);

	if (intern->bson) {
		bson_clear(&intern->bson);
	}
}

static zend_object* phongo_command_create_object(zend_class_entry* class_type)
{
	PHONGO_INTERN_OBJECT_ALLOC(command, class_type);

	intern->std.handlers = &phongo_handler_command;

	return &intern->std;
}

static HashTable* phongo_command_get_debug_info(zend_object* object, int* is_temp)
{
	PHONGO_INTERN_FROM_Z_OBJ(command, object);

	zval retval = ZVAL_STATIC_INIT;

	*is_temp = 1;

	array_init_size(&retval, 1);

	if (intern->bson) {
		zval zv;

		if (!phongo_bson_to_zval(intern->bson, &zv)) {
			zval_ptr_dtor(&zv);
			goto done;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "command", &zv);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "command");
	}

done:
	return Z_ARRVAL(retval);
}

void phongo_command_init_ce(INIT_FUNC_ARGS)
{
	phongo_command_ce                = register_class_MongoDB_Driver_Command();
	phongo_command_ce->create_object = phongo_command_create_object;

	memcpy(&phongo_handler_command, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	phongo_handler_command.get_debug_info = phongo_command_get_debug_info;
	phongo_handler_command.free_obj       = phongo_command_free_object;
	phongo_handler_command.offset         = XtOffsetOf(phongo_command_t, std);
}
