/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2014 MongoDB, Inc.                                         |
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
  | Copyright (c) 2014, MongoDB, Inc.                                         |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>
#include <mongoc-client-private.h>
#include <mongoc-host-list-private.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <ext/standard/file.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_hash.h>
#include <ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"
#include "php_array.h"


PHONGO_API zend_class_entry *php_phongo_manager_ce;

zend_object_handlers php_phongo_handler_manager;

/* {{{ proto MongoDB\Driver\Manager Manager::__construct(string $uri[, array $options = array()[, array $driverOptions = array()]])
   Constructs a new Manager */
PHP_METHOD(Manager, __construct)
{
	php_phongo_manager_t     *intern;
	zend_error_handling       error_handling;
	char                     *uri;
	int                       uri_len;
	zval                     *options = NULL;
	zval                     *driverOptions = NULL;
	(void)return_value; (void)return_value_ptr; (void)return_value_used;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a!a!", &uri, &uri_len, &options, &driverOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	intern->client = php_phongo_make_mongo_client(uri, driverOptions TSRMLS_CC);
	if (!intern->client) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "Failed to parse MongoDB URI");
		return;
	}
}
/* }}} */
/* {{{ proto MongoDB\Driver\Result Manager::executeCommand(string $db, MongoDB\Driver\Command $command[, MongoDB\Driver\ReadPreference $readPreference = null])
   Execute a command */
PHP_METHOD(Manager, executeCommand)
{
	php_phongo_manager_t     *intern;
	char                     *db;
	int                       db_len;
	zval                     *command;
	zval                     *readPreference = NULL;
	php_phongo_command_t    *cmd;
	(void)return_value_ptr;


	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &db, &db_len, &command, php_phongo_command_ce, &readPreference, php_phongo_readpreference_ce) == FAILURE) {
		return;
	}


	cmd = (php_phongo_command_t *)zend_object_store_get_object(command TSRMLS_CC);
	phongo_execute_command(intern->client, db, cmd->bson, phongo_read_preference_from_zval(readPreference TSRMLS_CC), -1, return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Driver\Result Manager::executeQuery(string $namespace, MongoDB\Driver\Query $zquery[, MongoDB\Driver\ReadPreference $readPreference = null])
   Execute a Query */
PHP_METHOD(Manager, executeQuery)
{
	php_phongo_manager_t     *intern;
	char                     *namespace;
	int                       namespace_len;
	zval                     *zquery;
	zval                     *readPreference = NULL;
	(void)return_value_ptr;


	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &namespace, &namespace_len, &zquery, php_phongo_query_ce, &readPreference, php_phongo_readpreference_ce) == FAILURE) {
		return;
	}


	phongo_execute_query(intern->client, namespace, phongo_query_from_zval(zquery TSRMLS_CC), phongo_read_preference_from_zval(readPreference TSRMLS_CC), -1, return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Driver\WriteResult Manager::executeBulkWrite(string $namespace, MongoDB\Driver\BulkWrite $zbulk[, MongoDB\Driver\WriteConcern $writeConcern = null])
   Executes a write operation bulk (e.g. insert, update, delete) */
PHP_METHOD(Manager, executeBulkWrite)
{
	php_phongo_manager_t      *intern;
	char                      *namespace;
	int                        namespace_len;
	zval                      *zbulk;
	zval                      *zwrite_concern = NULL;
	php_phongo_bulkwrite_t   *bulk;
	(void)return_value_ptr;


	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &namespace, &namespace_len, &zbulk, php_phongo_bulkwrite_ce, &zwrite_concern, php_phongo_writeconcern_ce) == FAILURE) {
		return;
	}


	bulk = (php_phongo_bulkwrite_t *)zend_object_store_get_object(zbulk TSRMLS_CC);
	phongo_execute_write(intern->client, namespace, bulk->bulk, phongo_write_concern_from_zval(zwrite_concern TSRMLS_CC), -1, return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Driver\WriteResult Manager::executeInsert(string $namespace, array|object $document[, MongoDB\Driver\WriteConcern $writeConcern = null])
   Convenience method for single insert operation. */
PHP_METHOD(Manager, executeInsert)
{
	php_phongo_manager_t     *intern;
	char                     *namespace;
	int                       namespace_len;
	zval                     *document;
	zval                     *zwrite_concern = NULL;
	bson_t                   *bson;
	(void)return_value_ptr;


	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sA|O!", &namespace, &namespace_len, &document, &zwrite_concern, php_phongo_writeconcern_ce) == FAILURE) {
		return;
	}


	bson = bson_new();
	zval_to_bson(document, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);
	phongo_execute_single_insert(intern->client, namespace, bson, phongo_write_concern_from_zval(zwrite_concern TSRMLS_CC), -1, return_value, return_value_used TSRMLS_CC);
	bson_clear(&bson);
}
/* }}} */
/* {{{ proto MongoDB\Driver\WriteResult Manager::executeUpdate(string $namespace, array|object $zquery, array|object $newObj[, array $updateOptions = array()[, MongoDB\Driver\WriteConcern $writeConcern = null]])
   Convenience method for single update operation. */
PHP_METHOD(Manager, executeUpdate)
{
	php_phongo_manager_t     *intern;
	char                     *namespace;
	int                       namespace_len;
	zval                     *zquery;
	zval                     *newObj;
	zval                     *updateOptions = NULL;
	zval                     *zwrite_concern = NULL;
	bson_t                   *query;
	bson_t                   *update;
	mongoc_update_flags_t     flags = MONGOC_UPDATE_NONE;
	(void)return_value_ptr;


	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sAA|a!O!", &namespace, &namespace_len, &zquery, &newObj, &updateOptions, &zwrite_concern, php_phongo_writeconcern_ce) == FAILURE) {
		return;
	}


	query = bson_new();
	update = bson_new();
	zval_to_bson(zquery, PHONGO_BSON_NONE, query, NULL TSRMLS_CC);
	zval_to_bson(newObj, PHONGO_BSON_NONE, update, NULL TSRMLS_CC);

	if (updateOptions && php_array_fetch_bool(updateOptions, "upsert")) {
		flags |= MONGOC_UPDATE_UPSERT;
	}
	if (updateOptions && php_array_fetch_bool(updateOptions, "limit")) {
		flags |= MONGOC_UPDATE_MULTI_UPDATE;
	}

	phongo_execute_single_update(intern->client, namespace, query, update, phongo_write_concern_from_zval(zwrite_concern TSRMLS_CC), -1, flags, return_value, return_value_used TSRMLS_CC);
	bson_clear(&query);
	bson_clear(&update);
}
/* }}} */
/* {{{ proto MongoDB\Driver\WriteResult Manager::executeDelete(string $namespace, array|object $query[, array $deleteOptions = array()[, MongoDB\Driver\WriteConcern $writeConcern = null]])
   Convenience method for single delete operation. */
PHP_METHOD(Manager, executeDelete)
{
	php_phongo_manager_t     *intern;
	char                     *namespace;
	int                       namespace_len;
	zval                     *query;
	zval                     *deleteOptions = NULL;
	zval                     *zwrite_concern = NULL;
	bson_t                   *bson;
	mongoc_delete_flags_t     flags = MONGOC_DELETE_NONE;
	(void)return_value_ptr;


	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sA|a!O!", &namespace, &namespace_len, &query, &deleteOptions, &zwrite_concern, php_phongo_writeconcern_ce) == FAILURE) {
		return;
	}


	if (deleteOptions && php_array_fetch_bool(deleteOptions, "limit")) {
		flags |= MONGOC_DELETE_SINGLE_REMOVE;
	}
	bson = bson_new();
	zval_to_bson(query, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);
	phongo_execute_single_delete(intern->client, namespace, bson, phongo_write_concern_from_zval(zwrite_concern TSRMLS_CC), -1, flags, return_value, return_value_used TSRMLS_CC);
	bson_clear(&bson);
}
/* }}} */
/* {{{ proto Server[] Manager::getServers()
   Returns the Servers associated with this Manager */
PHP_METHOD(Manager, getServers)
{
	php_phongo_manager_t         *intern;
	mongoc_set_t                 *set;
	size_t                        i;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	array_init(return_value);
	set = intern->client->topology->description.servers;
	for(i=0; i<set->items_len; i++) {
		zval *obj = NULL;

		MAKE_STD_ZVAL(obj);

		phongo_server_init(obj, intern->client, ((mongoc_server_description_t *)set->items[i].item)->id TSRMLS_CC);
		add_next_index_zval(return_value, obj);
	}
}
/* }}} */

/**
 * Manager abstracts a cluster of Server objects (i.e. socket connections).
 *
 * Typically, users will connect to a cluster using a URI, and the Manager will
 * perform tasks such as replica set discovery and create the necessary Server
 * objects. That said, it is also possible to create a Manager with an arbitrary
 * collection of Server objects using the static factory method (this can be
 * useful for testing or administration).
 *
 * Operation methods do not take socket-level options (e.g. socketTimeoutMS).
 * Those options should be specified during construction.
 */
/* {{{ MongoDB\Driver\Manager */

ZEND_BEGIN_ARG_INFO_EX(ai_Manager___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, uri)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
	ZEND_ARG_ARRAY_INFO(0, driverOptions, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\Driver\\ReadPreference, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeQuery, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zquery, MongoDB\\Driver\\Query, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\Driver\\ReadPreference, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeBulkWrite, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zbulk, MongoDB\\Driver\\BulkWrite, 0)
	ZEND_ARG_OBJ_INFO(0, writeConcern, MongoDB\\Driver\\WriteConcern, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeInsert, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_INFO(0, document)
	ZEND_ARG_OBJ_INFO(0, writeConcern, MongoDB\\Driver\\WriteConcern, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeUpdate, 0, 0, 3)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_INFO(0, zquery)
	ZEND_ARG_INFO(0, newObj)
	ZEND_ARG_ARRAY_INFO(0, updateOptions, 1)
	ZEND_ARG_OBJ_INFO(0, writeConcern, MongoDB\\Driver\\WriteConcern, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeDelete, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_ARRAY_INFO(0, deleteOptions, 1)
	ZEND_ARG_OBJ_INFO(0, writeConcern, MongoDB\\Driver\\WriteConcern, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_getServers, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_manager_me[] = {
	PHP_ME(Manager, __construct, ai_Manager___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeCommand, ai_Manager_executeCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeQuery, ai_Manager_executeQuery, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeBulkWrite, ai_Manager_executeBulkWrite, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeInsert, ai_Manager_executeInsert, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeUpdate, ai_Manager_executeUpdate, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeDelete, ai_Manager_executeDelete, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, getServers, ai_Manager_getServers, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_manager_t object handlers */
static void php_phongo_manager_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_manager_t *intern = (php_phongo_manager_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	mongoc_client_destroy(intern->client);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_manager_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_manager_t *intern = NULL;

	intern = (php_phongo_manager_t *)ecalloc(1, sizeof *intern);

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_manager_free_object, NULL TSRMLS_CC);
	retval.handlers = &php_phongo_handler_manager;

	return retval;
} /* }}} */

/* 
static const char *phongo_cluster_mode_tostring(mongoc_cluster_mode_t mode)
{
	switch (mode) {
		case MONGOC_CLUSTER_DIRECT:
			return "direct";
		case MONGOC_CLUSTER_REPLICA_SET:
			return "replicaset";
		case MONGOC_CLUSTER_SHARDED_CLUSTER:
			return "sharded";
			break;
	}

	return "broken";
}

static const char *phongo_cluster_state_tostring(mongoc_cluster_state_t state)
{
	switch (state) {
		case MONGOC_CLUSTER_STATE_BORN:
			return "born";
		case MONGOC_CLUSTER_STATE_HEALTHY:
			return "healthy";
		case MONGOC_CLUSTER_STATE_DEAD:
			return "dead";
		case MONGOC_CLUSTER_STATE_UNHEALTHY:
			return "unhealthy";
	}

	return "broken";
}
*/

static void add_next_index_node(zval *array, mongoc_cluster_node_t *node)
{
	zval                  *data = NULL;
	php_phongo_bson_state  state = PHONGO_BSON_STATE_INITIALIZER;

	MAKE_STD_ZVAL(state.zchild);

	MAKE_STD_ZVAL(data);
	array_init(data);
	/*FIXME: What data to dump ?
	add_assoc_long_ex(data, ZEND_STRS("ping_avg_msec"), node->ping_avg_msec);
	add_assoc_long_ex(data, ZEND_STRS("stamp"), node->stamp);
	add_assoc_bool_ex(data, ZEND_STRS("primary"), node->primary);
	add_assoc_bool_ex(data, ZEND_STRS("needs_auth"), node->needs_auth);
	add_assoc_bool_ex(data, ZEND_STRS("isdbgrid"), node->needs_auth);
	add_assoc_long_ex(data, ZEND_STRS("min_wire_version"), node->min_wire_version);
	add_assoc_long_ex(data, ZEND_STRS("max_wire_version"), node->max_wire_version);
	add_assoc_long_ex(data, ZEND_STRS("max_write_batch_size"), node->max_write_batch_size);
	if (node->replSet) {
		add_assoc_string_ex(data, ZEND_STRS("replSet"), node->replSet, 0);
	} else {
		add_assoc_null_ex(data, ZEND_STRS("replSet"));
	}
	add_assoc_long_ex(data, ZEND_STRS("last_read_msec"), node->last_read_msec);
	bson_to_zval(bson_get_data(&node->tags), node->tags.len, &state);
	add_assoc_zval_ex(data, ZEND_STRS("tags"), state.zchild);
	add_assoc_string_ex(data, ZEND_STRS("host_and_port"), node->host.host_and_port, 0);
	*/
	/* TODO: Should this contain the actual stream too? we have the mongoc_stream_t... */

	add_next_index_zval(array, data);
}

HashTable *php_phongo_manager_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	zval *retval = NULL;
	php_phongo_manager_t  *intern;

	*is_temp = 0;
	intern = (php_phongo_manager_t *)zend_object_store_get_object(object TSRMLS_CC);


	MAKE_STD_ZVAL(retval);
	array_init(retval);

	add_assoc_long_ex(retval, ZEND_STRS("request_id"), intern->client->request_id);
	add_assoc_string_ex(retval, ZEND_STRS("uri"), (char *)mongoc_uri_get_string(intern->client->uri), 0);

	{
		zval *cluster = NULL;
		MAKE_STD_ZVAL(cluster);
		array_init(cluster);
		/* FIXME: What data to dump ?
		add_assoc_string_ex(cluster, ZEND_STRS("mode"), (char *)phongo_cluster_mode_tostring(intern->client->cluster.mode), 0);
		add_assoc_string_ex(cluster, ZEND_STRS("state"), (char *)phongo_cluster_state_tostring(intern->client->cluster.state), 0);
		add_assoc_long_ex(cluster, ZEND_STRS("request_id"), intern->client->cluster.request_id);
		add_assoc_long_ex(cluster, ZEND_STRS("sockettimeoutms"), intern->client->cluster.sockettimeoutms);
		add_assoc_long_ex(cluster, ZEND_STRS("last_reconnect"), intern->client->cluster.last_reconnect);
		add_assoc_string_ex(cluster, ZEND_STRS("uri"), (char *)mongoc_uri_get_string(intern->client->cluster.uri), 0);
		add_assoc_long_ex(cluster, ZEND_STRS("requires_auth"), intern->client->cluster.requires_auth);

		if (intern->client->cluster.nodes_len) {
			zval *nodes = NULL;
			unsigned int i;

			MAKE_STD_ZVAL(nodes);
			array_init(nodes);
			for (i = 0; i < intern->client->cluster.nodes_len; i++) {
				add_next_index_node(nodes, &intern->client->cluster.nodes[i]);
			}
			add_assoc_zval_ex(cluster, ZEND_STRS("nodes"), nodes);
		}
		add_assoc_long_ex(cluster, ZEND_STRS("max_bson_size"), intern->client->cluster.max_bson_size);
		add_assoc_long_ex(cluster, ZEND_STRS("max_msg_size"), intern->client->cluster.max_msg_size);
		add_assoc_long_ex(cluster, ZEND_STRS("sec_latency_ms"), intern->client->cluster.sec_latency_ms);
		{
			mongoc_host_list_t host;
			zval *peers = NULL;
			mongoc_list_t *list;
			mongoc_list_t *liter;

			MAKE_STD_ZVAL(peers);
			array_init(peers);

			list = intern->client->cluster.peers;
			for (liter = list; liter ; liter = liter->next) {

				if (_mongoc_host_list_from_string(&host, liter->data)) {
					add_next_index_string(peers, host.host_and_port, 1);
				}
			}
			add_assoc_zval_ex(cluster, ZEND_STRS("peers"), peers);
		}
		if (intern->client->cluster.replSet) {
			add_assoc_string_ex(cluster, ZEND_STRS("replSet"), intern->client->cluster.replSet, 0);
		} else {
			add_assoc_null_ex(cluster, ZEND_STRS("replSet"));
		}
		*/

		add_assoc_zval_ex(retval, ZEND_STRS("cluster"), cluster);
	}

	return Z_ARRVAL_P(retval);
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Manager)
{
	zend_class_entry ce;
	(void)type;(void)module_number;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Manager", php_phongo_manager_me);
	php_phongo_manager_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_manager_ce->create_object = php_phongo_manager_create_object;
	PHONGO_CE_INIT(php_phongo_manager_ce);

	memcpy(&php_phongo_handler_manager, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_manager.get_debug_info = php_phongo_manager_get_debug_info;

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
