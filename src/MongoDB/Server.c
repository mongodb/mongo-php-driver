/*
 * Copyright 2014-2017 MongoDB, Inc.
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

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <ext/standard/file.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_server_ce;

zend_object_handlers php_phongo_handler_server;


/* {{{ proto void Server::__construct()
   Throws MongoDB\Driver\RuntimeException (can only be created internally) */
PHP_METHOD(Server, __construct)
{
	SUPPRESS_UNUSED_WARNING(return_value) SUPPRESS_UNUSED_WARNING(return_value_used) SUPPRESS_UNUSED_WARNING(return_value_ptr) (void)ZEND_NUM_ARGS(); (void)getThis();

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Accessing private constructor");
}
/* }}} */

/* {{{ proto MongoDB\Driver\Cursor Server::executeCommand(string $db, MongoDB\Driver\Command $command[, MongoDB\Driver\ReadPreference $readPreference = null]))
   Executes a Command on this Server */
PHP_METHOD(Server, executeCommand)
{
	php_phongo_server_t      *intern;
	char                     *db;
	phongo_zpp_char_len       db_len;
	zval                     *command;
	zval                     *readPreference = NULL;
	DECLARE_RETURN_VALUE_USED
	SUPPRESS_UNUSED_WARNING(return_value_ptr)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &db, &db_len, &command, php_phongo_command_ce, &readPreference, php_phongo_readpreference_ce) == FAILURE) {
		return;
	}

#if PHP_VERSION_ID >= 70000
	phongo_execute_command(&intern->manager, db, command, readPreference, intern->server_id, return_value, return_value_used TSRMLS_CC);
#else
	phongo_execute_command(intern->manager, db, command, readPreference, intern->server_id, return_value, return_value_used TSRMLS_CC);
#endif
}
/* }}} */

/* {{{ proto MongoDB\Driver\Cursor Server::executeQuery(string $namespace, MongoDB\Driver\Query $query[, MongoDB\Driver\ReadPreference $readPreference = null]))
   Executes a Query on this Server */
PHP_METHOD(Server, executeQuery)
{
	php_phongo_server_t      *intern;
	char                     *namespace;
	phongo_zpp_char_len       namespace_len;
	zval                     *query;
	zval                     *readPreference = NULL;
	DECLARE_RETURN_VALUE_USED
	SUPPRESS_UNUSED_WARNING(return_value_ptr)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &namespace, &namespace_len, &query, php_phongo_query_ce, &readPreference, php_phongo_readpreference_ce) == FAILURE) {
		return;
	}

#if PHP_VERSION_ID >= 70000
	phongo_execute_query(&intern->manager, namespace, query, readPreference, intern->server_id, return_value, return_value_used TSRMLS_CC);
#else
	phongo_execute_query(intern->manager, namespace, query, readPreference, intern->server_id, return_value, return_value_used TSRMLS_CC);
#endif
}
/* }}} */

/* {{{ proto MongoDB\Driver\WriteResult Server::executeBulkWrite(string $namespace, MongoDB\Driver\BulkWrite $zbulk[, MongoDB\Driver\WriteConcern $writeConcern = null])
   Executes a BulkWrite (i.e. any number of insert, update, and delete ops) on
   this Server */
PHP_METHOD(Server, executeBulkWrite)
{
	php_phongo_server_t      *intern;
	char                     *namespace;
	phongo_zpp_char_len       namespace_len;
	zval                     *zbulk;
	zval                     *zwrite_concern = NULL;
	php_phongo_bulkwrite_t   *bulk;
	DECLARE_RETURN_VALUE_USED
	SUPPRESS_UNUSED_WARNING(return_value_ptr)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &namespace, &namespace_len, &zbulk, php_phongo_bulkwrite_ce, &zwrite_concern, php_phongo_writeconcern_ce) == FAILURE) {
		return;
	}


	bulk = Z_BULKWRITE_OBJ_P(zbulk);
#if PHP_VERSION_ID >= 70000
	phongo_execute_write(&intern->manager, namespace, bulk, phongo_write_concern_from_zval(zwrite_concern TSRMLS_CC), intern->server_id, return_value, return_value_used TSRMLS_CC);
#else
	phongo_execute_write(intern->manager, namespace, bulk, phongo_write_concern_from_zval(zwrite_concern TSRMLS_CC), intern->server_id, return_value, return_value_used TSRMLS_CC);
#endif
}
/* }}} */

/* {{{ proto string Server::getHost()
   Returns the hostname for this Server */
PHP_METHOD(Server, getHost)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)

	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		PHONGO_RETVAL_STRING(mongoc_server_description_host(sd)->host);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ proto array Server::getTags()
   Returns the currently configured tags for this Server */
PHP_METHOD(Server, getTags)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		const bson_t *is_master = mongoc_server_description_ismaster(sd);
		bson_iter_t   iter;

		if (bson_iter_init_find(&iter, is_master, "tags") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
			const uint8_t         *bytes;
			uint32_t               len;
			php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

			state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
			state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

			bson_iter_document(&iter, &len, &bytes);
			phongo_bson_to_zval_ex(bytes, len, &state);
			mongoc_server_description_destroy(sd);

#if PHP_VERSION_ID >= 70000
			RETURN_ZVAL(&state.zchild, 0, 1);
#else
			RETURN_ZVAL(state.zchild, 0, 1);
#endif
		}

		array_init(return_value);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ proto array Server::getInfo()
   Returns the last isMaster result document for this Server */
PHP_METHOD(Server, getInfo)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		const bson_t          *is_master = mongoc_server_description_ismaster(sd);
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		phongo_bson_to_zval_ex(bson_get_data(is_master), is_master->len, &state);
		mongoc_server_description_destroy(sd);

#if PHP_VERSION_ID >= 70000
		RETURN_ZVAL(&state.zchild, 0, 1);
#else
		RETURN_ZVAL(state.zchild, 0, 1);
#endif
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ proto integer Server::getLatency()
   Returns the last messured latency for this Server */
PHP_METHOD(Server, getLatency)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_LONG((phongo_long) mongoc_server_description_round_trip_time(sd));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ proto integer Server::getPort()
   Returns the port for this Server */
PHP_METHOD(Server, getPort)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_LONG(mongoc_server_description_host(sd)->port);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ proto integer Server::getType()
   Returns the node type of this Server */
PHP_METHOD(Server, getType)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_LONG(php_phongo_server_description_type(sd));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ proto bool Server::isPrimary()
   Returns whether this Server is a primary member of a replica set */
PHP_METHOD(Server, isPrimary)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_PRIMARY].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ proto bool Server::isSecondary()
   Returns whether this Server is a secondary member of a replica set */
PHP_METHOD(Server, isSecondary)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_SECONDARY].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ proto bool Server::isArbiter()
   Returns whether this Server is an arbiter member of a replica set */
PHP_METHOD(Server, isArbiter)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_ARBITER].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ proto bool Server::isHidden()
   Returns whether this Server is a hidden member of a replica set */
PHP_METHOD(Server, isHidden)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		bson_iter_t iter;

		RETVAL_BOOL(bson_iter_init_find_case(&iter, mongoc_server_description_ismaster(sd), "hidden") && bson_iter_as_bool(&iter));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ proto bool Server::isPassive()
   Returns whether this Server is a passive member of a replica set */
PHP_METHOD(Server, isPassive)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	SUPPRESS_UNUSED_WARNING(return_value_ptr) SUPPRESS_UNUSED_WARNING(return_value_used)


	intern = Z_SERVER_OBJ_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		bson_iter_t iter;

		RETVAL_BOOL(bson_iter_init_find_case(&iter, mongoc_server_description_ismaster(sd), "passive") && bson_iter_as_bool(&iter));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
}
/* }}} */

/* {{{ MongoDB\Driver\Server */

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\Driver\\ReadPreference, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeQuery, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zquery, MongoDB\\Driver\\Query, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\Driver\\ReadPreference, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeBulkWrite, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zbulk, MongoDB\\Driver\\BulkWrite, 0)
	ZEND_ARG_OBJ_INFO(0, writeConcern, MongoDB\\Driver\\WriteConcern, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_server_me[] = {
	PHP_ME(Server, __construct, ai_Server_void, ZEND_ACC_FINAL|ZEND_ACC_PRIVATE)
	PHP_ME(Server, executeCommand, ai_Server_executeCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, executeQuery, ai_Server_executeQuery, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, executeBulkWrite, ai_Server_executeBulkWrite, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getHost, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getTags, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getInfo, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getLatency, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getPort, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getType, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isPrimary, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isSecondary, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isArbiter, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isHidden, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isPassive, ai_Server_void, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeup, ai_Server_void, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ Other functions */
static int php_phongo_server_compare_objects(zval *o1, zval *o2 TSRMLS_DC) /* {{{ */
{
	php_phongo_server_t         *intern1, *intern2;
	mongoc_server_description_t *sd1, *sd2;
	int                          retval = 0;

	intern1 = Z_SERVER_OBJ_P(o1);
	intern2 = Z_SERVER_OBJ_P(o2);

	sd1 = mongoc_client_get_server_description(intern1->client, intern1->server_id);
	sd2 = mongoc_client_get_server_description(intern2->client, intern2->server_id);

	if (sd1 && sd2) {
		retval = strcasecmp(mongoc_server_description_host(sd1)->host_and_port, mongoc_server_description_host(sd2)->host_and_port);
	} else {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description(s)");
	}

	if (sd1) {
		mongoc_server_description_destroy(sd1);
	}

	if (sd2) {
		mongoc_server_description_destroy(sd2);
	}

	return retval;
} /* }}} */
/* }}} */
/* {{{ php_phongo_server_t object handlers */
static void php_phongo_server_free_object(phongo_free_object_arg *object TSRMLS_DC) /* {{{ */
{
	php_phongo_server_t *intern = Z_OBJ_SERVER(object);

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	zval_ptr_dtor(&intern->manager);

#if PHP_VERSION_ID < 70000
	efree(intern);
#endif
} /* }}} */

phongo_create_object_retval php_phongo_server_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	php_phongo_server_t *intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_server_t, class_type);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

#if PHP_VERSION_ID >= 70000
	intern->std.handlers = &php_phongo_handler_server;

	return &intern->std;
#else
	{
		zend_object_value retval;
		retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_server_free_object, NULL TSRMLS_CC);
		retval.handlers = &php_phongo_handler_server;

		return retval;
	}
#endif
} /* }}} */

HashTable *php_phongo_server_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_server_t         *intern = NULL;
#if PHP_VERSION_ID >= 70000
	zval                         retval;
#else
	zval                         retval = zval_used_for_init;
#endif
	mongoc_server_description_t *sd;

	*is_temp = 1;
	intern = Z_SERVER_OBJ_P(object);


	if (!(sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to get server description");
		return NULL;
	}

	php_phongo_server_to_zval(&retval, sd);
	mongoc_server_description_destroy(sd);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Server)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Server", php_phongo_server_me);
	php_phongo_server_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_server_ce->create_object = php_phongo_server_create_object;
	PHONGO_CE_FINAL(php_phongo_server_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_server_ce);

	memcpy(&php_phongo_handler_server, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_server.compare_objects = php_phongo_server_compare_objects;
	php_phongo_handler_server.get_debug_info = php_phongo_server_get_debug_info;
#if PHP_VERSION_ID >= 70000
	php_phongo_handler_server.free_obj = php_phongo_server_free_object;
	php_phongo_handler_server.offset = XtOffsetOf(php_phongo_server_t, std);
#endif

	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_UNKNOWN"), PHONGO_SERVER_UNKNOWN TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_STANDALONE"), PHONGO_SERVER_STANDALONE TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_MONGOS"), PHONGO_SERVER_MONGOS TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_POSSIBLE_PRIMARY"), PHONGO_SERVER_POSSIBLE_PRIMARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_PRIMARY"), PHONGO_SERVER_RS_PRIMARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_SECONDARY"), PHONGO_SERVER_RS_SECONDARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_ARBITER"), PHONGO_SERVER_RS_ARBITER TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_OTHER"), PHONGO_SERVER_RS_OTHER TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_GHOST"), PHONGO_SERVER_RS_GHOST TSRMLS_CC);


	return SUCCESS;
}
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
