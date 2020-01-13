/*
 * Copyright 2017 MongoDB, Inc.
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
#include <Zend/zend_interfaces.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"
#include "php_bson.h"
#include "php_array_api.h"
#include "Session.h"

zend_class_entry* php_phongo_session_ce;

#define PHONGO_TRANSACTION_NONE "none"
#define PHONGO_TRANSACTION_STARTING "starting"
#define PHONGO_TRANSACTION_IN_PROGRESS "in_progress"
#define PHONGO_TRANSACTION_COMMITTED "committed"
#define PHONGO_TRANSACTION_ABORTED "aborted"

#define SESSION_CHECK_LIVELINESS(i, m)                                  \
	if (!(i)->client_session) {                                         \
		phongo_throw_exception(                                         \
			PHONGO_ERROR_LOGIC TSRMLS_CC,                               \
			"Cannot call '%s', as the session has already been ended.", \
			(m));                                                       \
		return;                                                         \
	}

static bool php_phongo_session_get_timestamp_parts(zval* obj, uint32_t* timestamp, uint32_t* increment TSRMLS_DC)
{
	bool retval = false;
#if PHP_VERSION_ID >= 70000
	zval ztimestamp = ZVAL_STATIC_INIT;
	zval zincrement = ZVAL_STATIC_INIT;

	zend_call_method_with_0_params(obj, NULL, NULL, "getTimestamp", &ztimestamp);

	if (Z_ISUNDEF(ztimestamp) || EG(exception)) {
		goto cleanup;
	}

	zend_call_method_with_0_params(obj, NULL, NULL, "getIncrement", &zincrement);

	if (Z_ISUNDEF(zincrement) || EG(exception)) {
		goto cleanup;
	}

	*timestamp = Z_LVAL(ztimestamp);
	*increment = Z_LVAL(zincrement);
#else
	zval* ztimestamp = NULL;
	zval* zincrement = NULL;

	zend_call_method_with_0_params(&obj, NULL, NULL, "getTimestamp", &ztimestamp);

	if (Z_ISUNDEF(ztimestamp) || EG(exception)) {
		goto cleanup;
	}

	zend_call_method_with_0_params(&obj, NULL, NULL, "getIncrement", &zincrement);

	if (Z_ISUNDEF(zincrement) || EG(exception)) {
		goto cleanup;
	}

	*timestamp = Z_LVAL_P(ztimestamp);
	*increment = Z_LVAL_P(zincrement);
#endif

	retval = true;

cleanup:
	if (!Z_ISUNDEF(ztimestamp)) {
		zval_ptr_dtor(&ztimestamp);
	}

	if (!Z_ISUNDEF(zincrement)) {
		zval_ptr_dtor(&zincrement);
	}

	return retval;
}

static const char* php_phongo_get_transaction_state_string(mongoc_transaction_state_t state TSRMLS_DC)
{
	switch (state) {
		case MONGOC_TRANSACTION_NONE:
			return PHONGO_TRANSACTION_NONE;
		case MONGOC_TRANSACTION_STARTING:
			return PHONGO_TRANSACTION_STARTING;
		case MONGOC_TRANSACTION_IN_PROGRESS:
			return PHONGO_TRANSACTION_IN_PROGRESS;
		case MONGOC_TRANSACTION_COMMITTED:
			return PHONGO_TRANSACTION_COMMITTED;
		case MONGOC_TRANSACTION_ABORTED:
			return PHONGO_TRANSACTION_ABORTED;
		default:
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Invalid transaction state %d given", (int) state);
			return NULL;
	}
}

/* {{{ proto void MongoDB\Driver\Session::advanceClusterTime(array|object $clusterTime)
   Advances the cluster time for this Session */
static PHP_METHOD(Session, advanceClusterTime)
{
	php_phongo_session_t* intern;
	zval*                 zcluster_time;
	bson_t                cluster_time = BSON_INITIALIZER;

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "advanceClusterTime")

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "A", &zcluster_time) == FAILURE) {
		return;
	}

	php_phongo_zval_to_bson(zcluster_time, PHONGO_BSON_NONE, &cluster_time, NULL TSRMLS_CC);

	/* An exception may be thrown during BSON conversion */
	if (EG(exception)) {
		goto cleanup;
	}

	mongoc_client_session_advance_cluster_time(intern->client_session, &cluster_time);

cleanup:
	bson_destroy(&cluster_time);
} /* }}} */

/* {{{ proto void MongoDB\Driver\Session::advanceOperationTime(MongoDB\BSON\TimestampInterface $timestamp)
   Advances the operation time for this Session */
static PHP_METHOD(Session, advanceOperationTime)
{
	php_phongo_session_t* intern;
	zval*                 ztimestamp;
	uint32_t              timestamp = 0;
	uint32_t              increment = 0;

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "advanceOperationTime")

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &ztimestamp, php_phongo_timestamp_interface_ce) == FAILURE) {
		return;
	}

	if (!php_phongo_session_get_timestamp_parts(ztimestamp, &timestamp, &increment TSRMLS_CC)) {
		return;
	}

	mongoc_client_session_advance_operation_time(intern->client_session, timestamp, increment);
} /* }}} */

/* {{{ proto object|null MongoDB\Driver\Session::getClusterTime()
   Returns the cluster time for this Session */
static PHP_METHOD(Session, getClusterTime)
{
	php_phongo_session_t* intern;
	const bson_t*         cluster_time;
	php_phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "getClusterTime")

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	cluster_time = mongoc_client_session_get_cluster_time(intern->client_session);

	if (!cluster_time) {
		RETURN_NULL();
	}

	if (!php_phongo_bson_to_zval_ex(bson_get_data(cluster_time), cluster_time->len, &state)) {
		/* Exception should already have been thrown */
		zval_ptr_dtor(&state.zchild);
		return;
	}

#if PHP_VERSION_ID >= 70000
	RETURN_ZVAL(&state.zchild, 0, 1);
#else
	RETURN_ZVAL(state.zchild, 0, 1);
#endif
} /* }}} */

/* {{{ proto object MongoDB\Driver\Session::getLogicalSessionId()
   Returns the logical session ID for this Session */
static PHP_METHOD(Session, getLogicalSessionId)
{
	php_phongo_session_t* intern;
	const bson_t*         lsid;
	php_phongo_bson_state state;

	PHONGO_BSON_INIT_STATE(state);

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "getLogicalSessionId")

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	lsid = mongoc_client_session_get_lsid(intern->client_session);

	if (!php_phongo_bson_to_zval_ex(bson_get_data(lsid), lsid->len, &state)) {
		/* Exception should already have been thrown */
		zval_ptr_dtor(&state.zchild);
		return;
	}

#if PHP_VERSION_ID >= 70000
	RETURN_ZVAL(&state.zchild, 0, 1);
#else
	RETURN_ZVAL(state.zchild, 0, 1);
#endif
} /* }}} */

/* {{{ proto MongoDB\BSON\Timestamp|null MongoDB\Driver\Session::getOperationTime()
   Returns the operation time for this Session */
static PHP_METHOD(Session, getOperationTime)
{
	php_phongo_session_t* intern;
	uint32_t              timestamp, increment;

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "getOperationTime")

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	mongoc_client_session_get_operation_time(intern->client_session, &timestamp, &increment);

	/* mongoc_client_session_get_operation_time() returns 0 for both parts if
	 * the session has not been used. According to the causal consistency spec,
	 * the operation time for an unused session is null. */
	if (timestamp == 0 && increment == 0) {
		RETURN_NULL();
	}

	php_phongo_bson_new_timestamp_from_increment_and_timestamp(return_value, increment, timestamp TSRMLS_CC);
} /* }}} */

/* {{{ proto MongoDB\Driver\Server|null MongoDB\Driver\Session::getServer()
   Returns the server this session is pinned to */
static PHP_METHOD(Session, getServer)
{
	php_phongo_session_t* intern;
	uint32_t              server_id = 0;

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "getServer")

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	server_id = mongoc_client_session_get_server_id(intern->client_session);

	/* For sessions without a pinned server, 0 is returned. */
	if (!server_id) {
		RETURN_NULL();
	}

	phongo_server_init(return_value, intern->client, server_id TSRMLS_CC);
} /* }}} */

/* {{{ proto array|null MongoDB\Driver\Session::getTransactionOptions()
   Returns options for the currently running transaction */
static PHP_METHOD(Session, getTransactionOptions)
{
	php_phongo_session_t*         intern;
	mongoc_transaction_opt_t*     opts;
	int64_t                       max_commit_time_ms;
	const mongoc_read_concern_t*  read_concern;
	const mongoc_read_prefs_t*    read_preference;
	const mongoc_write_concern_t* write_concern;

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "getTransactionOptions")

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	opts = mongoc_session_opts_get_transaction_opts(intern->client_session);

	if (!opts) {
		return;
	}

	max_commit_time_ms = mongoc_transaction_opts_get_max_commit_time_ms(opts);
	read_concern       = mongoc_transaction_opts_get_read_concern(opts);
	read_preference    = mongoc_transaction_opts_get_read_prefs(opts);
	write_concern      = mongoc_transaction_opts_get_write_concern(opts);

	array_init_size(return_value, 4);

	if (max_commit_time_ms) {
		ADD_ASSOC_LONG_EX(return_value, "maxCommitTimeMS", max_commit_time_ms);
	}

	if (!mongoc_read_concern_is_default(read_concern)) {
#if PHP_VERSION_ID >= 70000
		zval zread_concern;

		phongo_readconcern_init(&zread_concern, read_concern TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(return_value, "readConcern", &zread_concern);
#else
		zval* zread_concern = NULL;
		MAKE_STD_ZVAL(zread_concern);

		phongo_readconcern_init(zread_concern, read_concern TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(return_value, "readConcern", zread_concern);
#endif
	}

	if (read_preference) {
#if PHP_VERSION_ID >= 70000
		zval zread_preference;

		phongo_readpreference_init(&zread_preference, read_preference TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(return_value, "readPreference", &zread_preference);
#else
		zval* zread_preference = NULL;
		MAKE_STD_ZVAL(zread_preference);

		phongo_readpreference_init(zread_preference, read_preference TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(return_value, "readPreference", zread_preference);
#endif
	}

	if (!mongoc_write_concern_is_default(write_concern)) {
#if PHP_VERSION_ID >= 70000
		zval zwrite_concern;

		phongo_writeconcern_init(&zwrite_concern, write_concern TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(return_value, "writeConcern", &zwrite_concern);
#else
		zval* zwrite_concern = NULL;
		MAKE_STD_ZVAL(zwrite_concern);

		phongo_writeconcern_init(zwrite_concern, write_concern TSRMLS_CC);
		ADD_ASSOC_ZVAL_EX(return_value, "writeConcern", zwrite_concern);
#endif
	}
} /* }}} */

/* {{{ proto string MongoDB\Driver\Session::getTransactionState()
   Returns the current transaction state for this session */
static PHP_METHOD(Session, getTransactionState)
{
	php_phongo_session_t* intern;
	const char*           state;

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "getTransactionState")

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	state = php_phongo_get_transaction_state_string(mongoc_client_session_get_transaction_state(intern->client_session) TSRMLS_CC);
	if (!state) {
		/* Exception already thrown */
		return;
	}

	PHONGO_RETURN_STRING(state);
} /* }}} */

/* Creates a opts structure from an array optionally containing an RP, RC,
 * WC object, and/or maxCommitTimeMS int. Returns NULL if no options were found,
 * or there was an invalid option. If there was an invalid option or structure,
 * an exception will be thrown too. */
mongoc_transaction_opt_t* php_mongodb_session_parse_transaction_options(zval* options TSRMLS_DC)
{
	mongoc_transaction_opt_t* opts = NULL;

	if (php_array_existsc(options, "maxCommitTimeMS")) {
		int64_t max_commit_time_ms = php_array_fetchc_long(options, "maxCommitTimeMS");

		if (max_commit_time_ms < 0) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected \"maxCommitTimeMS\" option to be >= 0, %" PRId64 " given", max_commit_time_ms);
			/* Freeing opts is not needed here, as it can't be set yet. The
			 * code is here to keep it consistent with the others in case more
			 * options are added before this one. */
			if (opts) {
				mongoc_transaction_opts_destroy(opts);
			}
			return NULL;
		}

		if (max_commit_time_ms > UINT32_MAX) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected \"maxCommitTimeMS\" option to be <= %" PRIu32 ", %" PRId64 " given", UINT32_MAX, max_commit_time_ms);
			/* Freeing opts is not needed here, as it can't be set yet. The
			 * code is here to keep it consistent with the others in case more
			 * options are added before this one. */
			if (opts) {
				mongoc_transaction_opts_destroy(opts);
			}
			return NULL;
		}

		if (!opts) {
			opts = mongoc_transaction_opts_new();
		}

		mongoc_transaction_opts_set_max_commit_time_ms(opts, max_commit_time_ms);
	}

	if (php_array_existsc(options, "readConcern")) {
		zval* read_concern = php_array_fetchc(options, "readConcern");

		if (Z_TYPE_P(read_concern) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(read_concern), php_phongo_readconcern_ce TSRMLS_CC)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected \"readConcern\" option to be %s, %s given", ZSTR_VAL(php_phongo_readconcern_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(read_concern));
			if (opts) {
				mongoc_transaction_opts_destroy(opts);
			}
			return NULL;
		}

		if (!opts) {
			opts = mongoc_transaction_opts_new();
		}

		mongoc_transaction_opts_set_read_concern(opts, phongo_read_concern_from_zval(read_concern TSRMLS_CC));
	}

	if (php_array_existsc(options, "readPreference")) {
		zval* read_preference = php_array_fetchc(options, "readPreference");

		if (Z_TYPE_P(read_preference) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(read_preference), php_phongo_readpreference_ce TSRMLS_CC)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected \"readPreference\" option to be %s, %s given", ZSTR_VAL(php_phongo_readpreference_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(read_preference));
			if (opts) {
				mongoc_transaction_opts_destroy(opts);
			}
			return NULL;
		}

		if (!opts) {
			opts = mongoc_transaction_opts_new();
		}

		mongoc_transaction_opts_set_read_prefs(opts, phongo_read_preference_from_zval(read_preference TSRMLS_CC));
	}

	if (php_array_existsc(options, "writeConcern")) {
		zval* write_concern = php_array_fetchc(options, "writeConcern");

		if (Z_TYPE_P(write_concern) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(write_concern), php_phongo_writeconcern_ce TSRMLS_CC)) {
			phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Expected \"writeConcern\" option to be %s, %s given", ZSTR_VAL(php_phongo_writeconcern_ce->name), PHONGO_ZVAL_CLASS_OR_TYPE_NAME_P(write_concern));
			if (opts) {
				mongoc_transaction_opts_destroy(opts);
			}
			return NULL;
		}

		if (!opts) {
			opts = mongoc_transaction_opts_new();
		}

		mongoc_transaction_opts_set_write_concern(opts, phongo_write_concern_from_zval(write_concern TSRMLS_CC));
	}

	return opts;
}

/* {{{ proto void MongoDB\Driver\Session::startTransaction([array $options = null])
   Starts a new transaction */
static PHP_METHOD(Session, startTransaction)
{
	php_phongo_session_t*     intern;
	zval*                     options     = NULL;
	mongoc_transaction_opt_t* txn_options = NULL;
	bson_error_t              error;

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "startTransaction")

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a!", &options) == FAILURE) {
		return;
	}

	if (options) {
		txn_options = php_mongodb_session_parse_transaction_options(options TSRMLS_CC);
	}
	if (EG(exception)) {
		return;
	}

	if (!mongoc_client_session_start_transaction(intern->client_session, txn_options, &error)) {
		phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
	}

	if (txn_options) {
		mongoc_transaction_opts_destroy(txn_options);
	}
} /* }}} */

/* {{{ proto void MongoDB\Driver\Session::commitTransaction(void)
   Commits an existing transaction */
static PHP_METHOD(Session, commitTransaction)
{
	php_phongo_session_t* intern;
	bson_error_t          error;
	bson_t                reply;

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "commitTransaction")

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!mongoc_client_session_commit_transaction(intern->client_session, &reply, &error)) {
		phongo_throw_exception_from_bson_error_t_and_reply(&error, &reply TSRMLS_CC);
		bson_destroy(&reply);
	}
} /* }}} */

/* {{{ proto void MongoDB\Driver\Session::abortTransaction(void)
   Aborts (rolls back) an existing transaction */
static PHP_METHOD(Session, abortTransaction)
{
	php_phongo_session_t* intern;
	bson_error_t          error;

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "abortTransaction")

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!mongoc_client_session_abort_transaction(intern->client_session, &error)) {
		phongo_throw_exception_from_bson_error_t(&error TSRMLS_CC);
	}
} /* }}} */

/* {{{ proto void MongoDB\Driver\Session::endSession(void)
   Ends the session, and a running transaction if active */
static PHP_METHOD(Session, endSession)
{
	php_phongo_session_t* intern;

	intern = Z_SESSION_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	mongoc_client_session_destroy(intern->client_session);
	intern->client_session = NULL;
} /* }}} */

/* {{{ proto void MongoDB\Driver\Session::isInTransaction(void)
   Returns whether a multi-document transaction is in progress */
static PHP_METHOD(Session, isInTransaction)
{
	php_phongo_session_t* intern;

	intern = Z_SESSION_OBJ_P(getThis());
	SESSION_CHECK_LIVELINESS(intern, "isInTransaction")

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_BOOL(mongoc_client_session_in_transaction(intern->client_session));
} /* }}} */

/* {{{ MongoDB\Driver\Session function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Session_advanceClusterTime, 0, 0, 1)
	ZEND_ARG_INFO(0, clusterTime)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Session_advanceOperationTime, 0, 0, 1)
	ZEND_ARG_INFO(0, timestamp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Session_startTransaction, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Session_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_session_me[] = {
	/* clang-format off */
	PHP_ME(Session, abortTransaction, ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, advanceClusterTime, ai_Session_advanceClusterTime, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, advanceOperationTime, ai_Session_advanceOperationTime, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, commitTransaction, ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, endSession, ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, getClusterTime, ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, getLogicalSessionId, ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, getOperationTime, ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, getServer, ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, getTransactionOptions, ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, getTransactionState, ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, isInTransaction, ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Session, startTransaction, ai_Session_startTransaction, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_Session_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_Session_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Session object handlers */
static zend_object_handlers php_phongo_handler_session;

static void php_phongo_session_free_object(phongo_free_object_arg* object TSRMLS_DC) /* {{{ */
{
	php_phongo_session_t* intern = Z_OBJ_SESSION(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	/* If this Session was created in a different process, reset the client so
	 * that its session pool is cleared and mongoc_client_session_destroy will
	 * destroy the corresponding server session rather than return it to the
	 * now-empty pool. This will ensure that we do not re-use a server session
	 * (i.e. LSID) created by a parent process. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	if (intern->client_session) {
		mongoc_client_session_destroy(intern->client_session);
	}

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

static phongo_create_object_retval php_phongo_session_create_object(zend_class_entry* class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_session_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_session_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	PHONGO_SET_CREATED_BY_PID(intern);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_session;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle   = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_session_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_session;

		return retval;
	}
#endif
} /* }}} */

static HashTable* php_phongo_session_get_debug_info(zval* object, int* is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_session_t*       intern = NULL;
	const mongoc_session_opt_t* cs_opts;
	zval                        retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_SESSION_OBJ_P(object);

	array_init(&retval);

	if (intern->client_session) {
		const bson_t*         lsid;
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		lsid = mongoc_client_session_get_lsid(intern->client_session);

		if (!php_phongo_bson_to_zval_ex(bson_get_data(lsid), lsid->len, &state)) {
			zval_ptr_dtor(&state.zchild);
			goto done;
		}

#if PHP_VERSION_ID >= 70000
		ADD_ASSOC_ZVAL_EX(&retval, "logicalSessionId", &state.zchild);
#else
		ADD_ASSOC_ZVAL_EX(&retval, "logicalSessionId", state.zchild);
#endif
	} else {
		ADD_ASSOC_NULL_EX(&retval, "logicalSessionId");
	}

	if (intern->client_session) {
		const bson_t* cluster_time;

		cluster_time = mongoc_client_session_get_cluster_time(intern->client_session);

		if (cluster_time) {
			php_phongo_bson_state state;

			PHONGO_BSON_INIT_DEBUG_STATE(state);

			if (!php_phongo_bson_to_zval_ex(bson_get_data(cluster_time), cluster_time->len, &state)) {
				zval_ptr_dtor(&state.zchild);
				goto done;
			}

#if PHP_VERSION_ID >= 70000
			ADD_ASSOC_ZVAL_EX(&retval, "clusterTime", &state.zchild);
#else
			ADD_ASSOC_ZVAL_EX(&retval, "clusterTime", state.zchild);
#endif
		} else {
			ADD_ASSOC_NULL_EX(&retval, "clusterTime");
		}
	} else {
		ADD_ASSOC_NULL_EX(&retval, "clusterTime");
	}

	if (intern->client_session) {
		cs_opts = mongoc_client_session_get_opts(intern->client_session);
		ADD_ASSOC_BOOL_EX(&retval, "causalConsistency", mongoc_session_opts_get_causal_consistency(cs_opts));
	} else {
		ADD_ASSOC_NULL_EX(&retval, "causalConsistency");
	}

	if (intern->client_session) {
		uint32_t timestamp, increment;

		mongoc_client_session_get_operation_time(intern->client_session, &timestamp, &increment);

		if (timestamp && increment) {
#if PHP_VERSION_ID >= 70000
			zval ztimestamp;

			php_phongo_bson_new_timestamp_from_increment_and_timestamp(&ztimestamp, increment, timestamp TSRMLS_CC);
			ADD_ASSOC_ZVAL_EX(&retval, "operationTime", &ztimestamp);
#else
			zval* ztimestamp;

			MAKE_STD_ZVAL(ztimestamp);
			php_phongo_bson_new_timestamp_from_increment_and_timestamp(ztimestamp, increment, timestamp TSRMLS_CC);
			ADD_ASSOC_ZVAL_EX(&retval, "operationTime", ztimestamp);
#endif
		} else {
			ADD_ASSOC_NULL_EX(&retval, "operationTime");
		}
	} else {
		ADD_ASSOC_NULL_EX(&retval, "operationTime");
	}

	if (intern->client_session) {
		uint32_t server_id = mongoc_client_session_get_server_id(intern->client_session);

		if (server_id) {

#if PHP_VERSION_ID >= 70000
			zval server;

			phongo_server_init(&server, intern->client, server_id TSRMLS_CC);
			ADD_ASSOC_ZVAL_EX(&retval, "server", &server);
#else
			zval* server = NULL;

			MAKE_STD_ZVAL(server);
			phongo_server_init(server, intern->client, server_id TSRMLS_CC);
			ADD_ASSOC_ZVAL_EX(&retval, "server", server);
#endif
		} else {
			ADD_ASSOC_NULL_EX(&retval, "server");
		}
	} else {
		ADD_ASSOC_NULL_EX(&retval, "server");
	}

done:
	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_session_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Session", php_phongo_session_me);
	php_phongo_session_ce                = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_session_ce->create_object = php_phongo_session_create_object;
	PHONGO_CE_FINAL(php_phongo_session_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_session_ce);

	memcpy(&php_phongo_handler_session, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_session.get_debug_info = php_phongo_session_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_session.free_obj = php_phongo_session_free_object;
	php_phongo_handler_session.offset   = XtOffsetOf(php_phongo_session_t, std);
#endif

	zend_declare_class_constant_string(php_phongo_session_ce, ZEND_STRL("TRANSACTION_NONE"), PHONGO_TRANSACTION_NONE TSRMLS_CC);
	zend_declare_class_constant_string(php_phongo_session_ce, ZEND_STRL("TRANSACTION_STARTING"), PHONGO_TRANSACTION_STARTING TSRMLS_CC);
	zend_declare_class_constant_string(php_phongo_session_ce, ZEND_STRL("TRANSACTION_IN_PROGRESS"), PHONGO_TRANSACTION_IN_PROGRESS TSRMLS_CC);
	zend_declare_class_constant_string(php_phongo_session_ce, ZEND_STRL("TRANSACTION_COMMITTED"), PHONGO_TRANSACTION_COMMITTED TSRMLS_CC);
	zend_declare_class_constant_string(php_phongo_session_ce, ZEND_STRL("TRANSACTION_ABORTED"), PHONGO_TRANSACTION_ABORTED TSRMLS_CC);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
