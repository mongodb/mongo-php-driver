/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2015 MongoDB, Inc.                                         |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2014-2015 MongoDB, Inc.                                     |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>
#include <mongoc-client-private.h>

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


/* {{{ proto MongoDB\Driver\Server Server::__construct()
   Throws exception -- can only be created internally */
PHP_METHOD(Server, __construct)
{
	(void)return_value; (void)return_value_used; (void)return_value_ptr; (void)ZEND_NUM_ARGS(); (void)getThis();

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Accessing private constructor");
}
/* }}} */
/* {{{ proto MongoDB\Driver\Cursor Server::executeCommand(string $db, MongoDB\Driver\Command $command[, MongoDB\Driver\ReadPreference $readPreference = null]))
   Executes a command on this server */
PHP_METHOD(Server, executeCommand)
{
	php_phongo_server_t      *intern;
	char                     *db;
	int                       db_len;
	zval                     *command;
	zval                     *readPreference = NULL;
	php_phongo_command_t     *cmd;
	(void)return_value_ptr;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &db, &db_len, &command, php_phongo_command_ce, &readPreference, php_phongo_readpreference_ce) == FAILURE) {
		return;
	}


	cmd = (php_phongo_command_t *)zend_object_store_get_object(command TSRMLS_CC);
	phongo_execute_command(intern->client, db, cmd->bson, phongo_read_preference_from_zval(readPreference TSRMLS_CC), intern->server_id, return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Driver\Cursor Server::executeQuery(string $namespace, MongoDB\Driver\Query $zquery[, MongoDB\Driver\ReadPreference $readPreference = null]))
   Executes a Query */
PHP_METHOD(Server, executeQuery)
{
	php_phongo_server_t      *intern;
	char                     *namespace;
	int                       namespace_len;
	zval                     *zquery;
	zval                     *readPreference = NULL;
	(void)return_value_ptr;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &namespace, &namespace_len, &zquery, php_phongo_query_ce, &readPreference, php_phongo_readpreference_ce) == FAILURE) {
		return;
	}


	phongo_execute_query(intern->client, namespace, phongo_query_from_zval(zquery TSRMLS_CC), phongo_read_preference_from_zval(readPreference TSRMLS_CC), intern->server_id, return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Driver\WriteResult Server::executeBulkWrite(string $namespace, MongoDB\Driver\BulkWrite $zbulk[, MongoDB\Driver\WriteConcern $writeConcern = null])
   Executes a write operation bulk (e.g. insert, update, delete) */
PHP_METHOD(Server, executeBulkWrite)
{
	php_phongo_server_t      *intern;
	char                     *namespace;
	int                       namespace_len;
	zval                     *zbulk;
	zval                     *zwrite_concern = NULL;
	php_phongo_bulkwrite_t  *bulk;
	(void)return_value_ptr;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &namespace, &namespace_len, &zbulk, php_phongo_bulkwrite_ce, &zwrite_concern, php_phongo_writeconcern_ce) == FAILURE) {
		return;
	}


	bulk = (php_phongo_bulkwrite_t *)zend_object_store_get_object(zbulk TSRMLS_CC);
	phongo_execute_write(intern->client, namespace, bulk->bulk, phongo_write_concern_from_zval(zwrite_concern TSRMLS_CC), intern->server_id, return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto string Server::getHost()
   Returns the hostname used to connect to this Server */
PHP_METHOD(Server, getHost)
{
	php_phongo_server_t      *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;

	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		RETURN_STRING(sd->host.host, 1);
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */
/* {{{ proto array Server::getTags()
   Returns the currently configured tags for this node */
PHP_METHOD(Server, getTags)
{
	php_phongo_server_t      *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		MAKE_STD_ZVAL(state.zchild);
		bson_to_zval(bson_get_data(&sd->tags), sd->tags.len, &state);
		RETURN_ZVAL(state.zchild, 0, 1);
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */
/* {{{ proto array Server::getInfo()
   Returns the last isMaster() result document */
PHP_METHOD(Server, getInfo)
{
	php_phongo_server_t      *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;
		state.map.root_type = PHONGO_TYPEMAP_NATIVE_ARRAY;
		state.map.document_type = PHONGO_TYPEMAP_NATIVE_ARRAY;

		MAKE_STD_ZVAL(state.zchild);
		bson_to_zval(bson_get_data(&sd->last_is_master), sd->last_is_master.len, &state);
		RETURN_ZVAL(state.zchild, 0, 1);
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */
/* {{{ proto integer Server::getLatency()
   Returns the last messured latency */
PHP_METHOD(Server, getLatency)
{
	php_phongo_server_t      *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		RETURN_LONG(sd->round_trip_time);
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */
/* {{{ proto integer Server::getPort()
   Returns the port used to create this Server */
PHP_METHOD(Server, getPort)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		RETURN_LONG(sd->host.port);
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */
/* {{{ proto integer Server::getType()
   Returns the node type of this Server */
PHP_METHOD(Server, getType)
{
	php_phongo_server_t      *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		RETURN_LONG(sd->type);
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */
/* {{{ proto bool Server::isPrimary()
   Checks if this is a special "Primary" member of a RepilcaSet */
PHP_METHOD(Server, isPrimary)
{
	php_phongo_server_t      *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		RETURN_BOOL(sd->type == MONGOC_SERVER_RS_PRIMARY);
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */
/* {{{ proto bool Server::isSecondary()
   Checks if this is a special "Secondary" member of a RepilcaSet */
PHP_METHOD(Server, isSecondary)
{
	php_phongo_server_t      *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		RETURN_BOOL(sd->type == MONGOC_SERVER_RS_SECONDARY);
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */
/* {{{ proto bool Server::isArbiter()
   Checks if this is a special "Arbiter" member of a RepilcaSet */
PHP_METHOD(Server, isArbiter)
{
	php_phongo_server_t      *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		RETURN_BOOL(sd->type == MONGOC_SERVER_RS_ARBITER);
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */
/* {{{ proto bool Server::isHidden()
   Checks if this is a special "hidden" member of a RepilcaSet */
PHP_METHOD(Server, isHidden)
{
	php_phongo_server_t      *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		bson_iter_t iter;

		RETURN_BOOL(bson_iter_init_find_case(&iter, &sd->last_is_master, "hidden") && bson_iter_as_bool(&iter));
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */
/* {{{ proto bool Server::isPassive()
   Checks if this is a special passive node member of a ReplicaSet */
PHP_METHOD(Server, isPassive)
{
	php_phongo_server_t         *intern;
	mongoc_server_description_t *sd;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		bson_iter_t iter;

		RETURN_BOOL(bson_iter_init_find_case(&iter, &sd->last_is_master, "passive") && bson_iter_as_bool(&iter));
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
}
/* }}} */

/* {{{ MongoDB\Driver\Server */

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\Driver\\ReadPreference, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeQuery, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zquery, MongoDB\\Driver\\Query, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\Driver\\ReadPreference, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeBulkWrite, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zbulk, MongoDB\\Driver\\BulkWrite, 0)
	ZEND_ARG_OBJ_INFO(0, writeConcern, MongoDB\\Driver\\WriteConcern, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getHost, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getTags, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getInfo, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getLatency, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getPort, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getType, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_isPrimary, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_isSecondary, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_isArbiter, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_isHidden, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_isPassive, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_server_me[] = {
	PHP_ME(Server, __construct, NULL, ZEND_ACC_FINAL|ZEND_ACC_PRIVATE)
	PHP_ME(Server, executeCommand, ai_Server_executeCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, executeQuery, ai_Server_executeQuery, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, executeBulkWrite, ai_Server_executeBulkWrite, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getHost, ai_Server_getHost, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getTags, ai_Server_getTags, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getInfo, ai_Server_getInfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getLatency, ai_Server_getLatency, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getPort, ai_Server_getPort, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getType, ai_Server_getType, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isPrimary, ai_Server_isPrimary, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isSecondary, ai_Server_isSecondary, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isArbiter, ai_Server_isArbiter, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isHidden, ai_Server_isHidden, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isPassive, ai_Server_isPassive, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ Other functions */
static int php_phongo_server_compare_objects(zval *o1, zval *o2 TSRMLS_DC) /* {{{ */
{
    php_phongo_server_t *intern1;
    php_phongo_server_t *intern2;
	mongoc_server_description_t *sd1, *sd2;

    intern1 = (php_phongo_server_t *)zend_object_store_get_object(o1 TSRMLS_CC);
    intern2 = (php_phongo_server_t *)zend_object_store_get_object(o2 TSRMLS_CC);

	sd1 = mongoc_topology_description_server_by_id(&intern1->client->topology->description, intern1->server_id);
	sd2 = mongoc_topology_description_server_by_id(&intern2->client->topology->description, intern2->server_id);

	if (!sd1 || !sd2) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
		return 0;
	}

	return strcasecmp(sd1->host.host_and_port, sd2->host.host_and_port);
} /* }}} */
/* }}} */
/* {{{ php_phongo_server_t object handlers */
static void php_phongo_server_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_server_t *intern = (php_phongo_server_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_server_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_server_t *intern = NULL;

	intern = (php_phongo_server_t *)ecalloc(1, sizeof *intern);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_server_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_server;

	return retval;
} /* }}} */

HashTable *php_phongo_server_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_server_t         *intern = NULL;
	zval                         retval = zval_used_for_init;
	mongoc_server_description_t *sd;


	*is_temp = 1;
	intern = (php_phongo_server_t *)zend_object_store_get_object(object TSRMLS_CC);


	if (!(sd = mongoc_topology_description_server_by_id(&intern->client->topology->description, intern->server_id))) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to get server description, server likely gone");
		return NULL;
	}

	php_phongo_server_to_zval(&retval, sd);

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
	PHONGO_CE_INIT(php_phongo_server_ce);

	memcpy(&php_phongo_handler_server, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_server.compare_objects = php_phongo_server_compare_objects;
	php_phongo_handler_server.get_debug_info = php_phongo_server_get_debug_info;

	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_UNKNOWN"), MONGOC_SERVER_UNKNOWN TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_STANDALONE"), MONGOC_SERVER_STANDALONE TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_MONGOS"), MONGOC_SERVER_MONGOS TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_POSSIBLE_PRIMARY"), MONGOC_SERVER_POSSIBLE_PRIMARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_PRIMARY"), MONGOC_SERVER_RS_PRIMARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_SECONDARY"), MONGOC_SERVER_RS_SECONDARY TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_ARBITER"), MONGOC_SERVER_RS_ARBITER TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_OTHER"), MONGOC_SERVER_RS_OTHER TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_GHOST"), MONGOC_SERVER_RS_GHOST TSRMLS_CC);


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
