/*
 * Copyright 2022-present MongoDB, Inc.
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
#include "mongoc/mongoc.h"

#include <php.h>
#include <Zend/zend_exceptions.h>

#include "php_phongo.h"
#include "phongo_error.h"

/* This constant is used for determining if a server error for an exceeded query
 * or command should select ExecutionTimeoutException. */
#define PHONGO_SERVER_ERROR_EXCEEDED_TIME_LIMIT 50

void phongo_add_exception_prop(const char* prop, int prop_len, zval* value)
{
	if (EG(exception)) {
		zval ex;
		ZVAL_OBJ(&ex, EG(exception));
		zend_update_property(Z_OBJCE(ex), PHONGO_COMPAT_OBJ_P(&ex), prop, prop_len, value);
	}
}

zend_class_entry* phongo_exception_from_phongo_domain(php_phongo_error_domain_t domain)
{
	switch (domain) {
		case PHONGO_ERROR_INVALID_ARGUMENT:
			return php_phongo_invalidargumentexception_ce;
		case PHONGO_ERROR_LOGIC:
			return php_phongo_logicexception_ce;
		case PHONGO_ERROR_RUNTIME:
			return php_phongo_runtimeexception_ce;
		case PHONGO_ERROR_UNEXPECTED_VALUE:
			return php_phongo_unexpectedvalueexception_ce;
		case PHONGO_ERROR_MONGOC_FAILED:
			return php_phongo_runtimeexception_ce;
		case PHONGO_ERROR_CONNECTION_FAILED:
			return php_phongo_connectionexception_ce;
	}

	MONGOC_ERROR("Resolving unknown phongo error domain: %d", domain);
	return php_phongo_runtimeexception_ce;
}

zend_class_entry* phongo_exception_from_mongoc_domain(mongoc_error_domain_t domain, mongoc_error_code_t code)
{
	if (domain == MONGOC_ERROR_CLIENT) {
		if (code == MONGOC_ERROR_CLIENT_AUTHENTICATE) {
			return php_phongo_authenticationexception_ce;
		}

		if (code == MONGOC_ERROR_CLIENT_INVALID_ENCRYPTION_ARG) {
			return php_phongo_invalidargumentexception_ce;
		}
	}

	if (domain == MONGOC_ERROR_COMMAND && code == MONGOC_ERROR_COMMAND_INVALID_ARG) {
		return php_phongo_invalidargumentexception_ce;
	}

	if (domain == MONGOC_ERROR_SERVER) {
		if (code == PHONGO_SERVER_ERROR_EXCEEDED_TIME_LIMIT) {
			return php_phongo_executiontimeoutexception_ce;
		}

		return php_phongo_serverexception_ce;
	}

	if (domain == MONGOC_ERROR_SERVER_SELECTION && code == MONGOC_ERROR_SERVER_SELECTION_FAILURE) {
		return php_phongo_connectiontimeoutexception_ce;
	}

	if (domain == MONGOC_ERROR_STREAM) {
		if (code == MONGOC_ERROR_STREAM_SOCKET) {
			return php_phongo_connectiontimeoutexception_ce;
		}

		return php_phongo_connectionexception_ce;
	}

	if (domain == MONGOC_ERROR_WRITE_CONCERN) {
		return php_phongo_serverexception_ce;
	}

	if (domain == MONGOC_ERROR_PROTOCOL && code == MONGOC_ERROR_PROTOCOL_BAD_WIRE_VERSION) {
		return php_phongo_connectionexception_ce;
	}

	if (domain == MONGOC_ERROR_CLIENT_SIDE_ENCRYPTION) {
		return php_phongo_encryptionexception_ce;
	}

	return php_phongo_runtimeexception_ce;
}

void phongo_throw_exception(php_phongo_error_domain_t domain, const char* format, ...)
{
	va_list args;
	char*   message;
	int     message_len;

	va_start(args, format);
	message_len = vspprintf(&message, 0, format, args);
	zend_throw_exception(phongo_exception_from_phongo_domain(domain), message, 0);
	efree(message);
	va_end(args);
}

static int phongo_exception_append_error_labels(zval* labels, const bson_iter_t* iter)
{
	bson_iter_t error_labels;
	uint32_t    label_count = 0;

	if (!BSON_ITER_HOLDS_ARRAY(iter) || !bson_iter_recurse(iter, &error_labels)) {
		return label_count;
	}

	while (bson_iter_next(&error_labels)) {
		if (BSON_ITER_HOLDS_UTF8(&error_labels)) {
			const char* error_label;
			uint32_t    error_label_len;

			error_label = bson_iter_utf8(&error_labels, &error_label_len);
			ADD_NEXT_INDEX_STRINGL(labels, error_label, error_label_len);
			label_count++;
		}
	}

	return label_count;
}

void phongo_exception_add_error_labels(const bson_t* reply)
{
	bson_iter_t iter, child;
	zval        labels;
	uint32_t    label_count = 0;

	if (!reply) {
		return;
	}

	array_init(&labels);

	if (bson_iter_init_find(&iter, reply, "errorLabels")) {
		label_count += phongo_exception_append_error_labels(&labels, &iter);
	}

	if (bson_iter_init_find(&iter, reply, "writeConcernError") && BSON_ITER_HOLDS_DOCUMENT(&iter) &&
		bson_iter_recurse(&iter, &child) && bson_iter_find(&child, "errorLabels")) {
		label_count += phongo_exception_append_error_labels(&labels, &child);
	}

	/* mongoc_write_result_t always reports writeConcernErrors in an array, so
	 * we must iterate this to collect WCE labels for BulkWrite replies. */
	if (bson_iter_init_find(&iter, reply, "writeConcernErrors") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
		bson_iter_t wce;

		while (bson_iter_next(&child)) {
			if (BSON_ITER_HOLDS_DOCUMENT(&child) && bson_iter_recurse(&child, &wce) && bson_iter_find(&wce, "errorLabels")) {
				label_count += phongo_exception_append_error_labels(&labels, &wce);
			}
		}
	}

	if (label_count > 0) {
		phongo_add_exception_prop(ZEND_STRL("errorLabels"), &labels);
	}

	zval_ptr_dtor(&labels);
}

void phongo_throw_exception_from_bson_error_t_and_reply(bson_error_t* error, const bson_t* reply)
{
	/* Server errors (other than ExceededTimeLimit) and write concern errors
	 * may use CommandException and report the result document for the
	 * failed command. For BC, ExceededTimeLimit errors will continue to use
	 * ExcecutionTimeoutException and omit the result document. */
	if (reply && ((error->domain == MONGOC_ERROR_SERVER && error->code != PHONGO_SERVER_ERROR_EXCEEDED_TIME_LIMIT) || error->domain == MONGOC_ERROR_WRITE_CONCERN)) {
		zval zv;

		zend_throw_exception(php_phongo_commandexception_ce, error->message, error->code);
		if (php_phongo_bson_to_zval(reply, &zv)) {
			phongo_add_exception_prop(ZEND_STRL("resultDocument"), &zv);
		}

		zval_ptr_dtor(&zv);
	} else {
		zend_throw_exception(phongo_exception_from_mongoc_domain(error->domain, error->code), error->message, error->code);
	}
	phongo_exception_add_error_labels(reply);
}

void phongo_throw_exception_from_bson_error_t(bson_error_t* error)
{
	phongo_throw_exception_from_bson_error_t_and_reply(error, NULL);
}

#ifndef MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION
void phongo_throw_exception_no_cse(php_phongo_error_domain_t domain, const char* message)
{
	phongo_throw_exception(domain, "%s Please recompile with support for libmongocrypt using the with-mongodb-client-side-encryption configure switch.", message);
}
#endif /* MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION */
