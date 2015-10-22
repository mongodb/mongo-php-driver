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
#include <mongoc-host-list-private.h>
#include <mongoc-server-description-private.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <ext/standard/file.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_hash.h>
#include <ext/spl/spl_iterators.h>
/* PHP array helpers */
#include "php_array_api.h"
/* Our Compatability header */
#include "phongo_compat.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_manager_ce;

zend_object_handlers php_phongo_handler_manager;

/* {{{ proto MongoDB\Driver\Manager Manager::__construct(string $uri[, array $options = array()[, array $driverOptions = array()]])
   Constructs a new Manager */
PHP_METHOD(Manager, __construct)
{
	php_phongo_manager_t     *intern;
	zend_error_handling       error_handling;
	mongoc_uri_t             *uri;
	char                     *uri_string;
	int                       uri_string_len;
	zval                     *options = NULL;
	bson_t                    bson_options = BSON_INITIALIZER;
	zval                     *driverOptions = NULL;
	(void)return_value; (void)return_value_ptr; (void)return_value_used;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a!a!", &uri_string, &uri_string_len, &options, &driverOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	if (options) {
		zval_to_bson(options, PHONGO_BSON_NONE, &bson_options, NULL TSRMLS_CC);
	}

	if (!(uri = php_phongo_make_uri(uri_string, &bson_options))) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT TSRMLS_CC, "Failed to parse MongoDB URI: '%s'", uri_string);
		bson_destroy(&bson_options);

		return;
	}

	intern->client = php_phongo_make_mongo_client(uri, driverOptions TSRMLS_CC);
	mongoc_uri_destroy(uri);

	if (!intern->client) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "Failed to create Manager from URI: '%s'", uri_string);
		bson_destroy(&bson_options);

		return;
	}

	if (!php_phongo_apply_rp_options_to_client(intern->client, &bson_options TSRMLS_CC) ||
	    !php_phongo_apply_wc_options_to_client(intern->client, &bson_options TSRMLS_CC)) {
		/* Exception should already have been thrown */
		bson_destroy(&bson_options);

		return;
	}

	bson_destroy(&bson_options);
}
/* }}} */
/* {{{ proto MongoDB\Driver\Cursor Manager::executeCommand(string $db, MongoDB\Driver\Command $command[, MongoDB\Driver\ReadPreference $readPreference = null])
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
/* {{{ proto MongoDB\Driver\Cursor Manager::executeQuery(string $namespace, MongoDB\Driver\Query $zquery[, MongoDB\Driver\ReadPreference $readPreference = null])
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
/* {{{ proto MongoDB\Driver\ReadPreference Manager::getReadPreference()
   Returns the ReadPreference associated with this Manager */
PHP_METHOD(Manager, getReadPreference)
{
	php_phongo_manager_t *intern;
	(void)return_value_ptr;

	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (return_value_used) {
		phongo_readpreference_init(return_value, mongoc_client_get_read_prefs(intern->client) TSRMLS_CC);
	}
}
/* }}} */
/* {{{ proto MongoDB\Driver\Server[] Manager::getServers()
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
		mongoc_server_description_t *sd = (mongoc_server_description_t *) set->items[i].item;
		zval *obj = NULL;

		if (sd->type == MONGOC_SERVER_UNKNOWN) {
			continue;
		}

		MAKE_STD_ZVAL(obj);

		phongo_server_init(obj, intern->client, sd->id TSRMLS_CC);
		add_next_index_zval(return_value, obj);
	}
}
/* }}} */
/* {{{ proto MongoDB\Driver\WriteConcern Manager::getWriteConcern()
   Returns the WriteConcern associated with this Manager */
PHP_METHOD(Manager, getWriteConcern)
{
	php_phongo_manager_t *intern;
	(void)return_value_ptr;

	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (return_value_used) {
		phongo_writeconcern_init(return_value, mongoc_client_get_write_concern(intern->client) TSRMLS_CC);
	}
}
/* }}} */
/* {{{ proto MongoDB\Driver\Server Manager::selectServers(MongoDB\Driver\ReadPreference $readPreference)
   Returns a suitable Server for the given $readPreference */
PHP_METHOD(Manager, selectServer)
{
	php_phongo_manager_t         *intern;
	zval                         *zreadPreference = NULL;
	const mongoc_read_prefs_t    *readPreference;
	uint32_t                      server_id;
	(void)return_value_ptr; (void)return_value_used;


	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &zreadPreference, php_phongo_readpreference_ce) == FAILURE) {
		return;
	}

	readPreference = phongo_read_preference_from_zval(zreadPreference TSRMLS_CC);
	server_id = mongoc_cluster_preselect(&intern->client->cluster, MONGOC_OPCODE_QUERY, readPreference, NULL);
	phongo_server_init(return_value, intern->client, server_id TSRMLS_CC);
}
/* }}} */
/* {{{ proto void MongoDB\Driver\Manager::__wakeUp()
 * Throws MongoDB\Driver\RuntimeException as it cannot be serialized */
PHP_METHOD(Manager, __wakeUp)
{
	(void)return_value_ptr; (void)return_value_used; (void)return_value; (void)this_ptr;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME TSRMLS_CC, "%s", "MongoDB\\Driver objects cannot be serialized");
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

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_getReadPreference, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_getServers, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_getWriteConcern, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_selectServer, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\Driver\\ReadPreference, 1)
ZEND_END_ARG_INFO();

static zend_function_entry php_phongo_manager_me[] = {
	PHP_ME(Manager, __construct, ai_Manager___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeCommand, ai_Manager_executeCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeQuery, ai_Manager_executeQuery, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeBulkWrite, ai_Manager_executeBulkWrite, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, getReadPreference, ai_Manager_getReadPreference, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, getServers, ai_Manager_getServers, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, getWriteConcern, ai_Manager_getWriteConcern, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, selectServer, ai_Manager_selectServer, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, __wakeUp, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_manager_t object handlers */
static void php_phongo_manager_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_manager_t *intern = (php_phongo_manager_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->client) {
		mongoc_client_destroy(intern->client);
	}

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

bool phongo_add_server_debug(void *item, void *ctx)
{
	mongoc_server_description_t *server = item;
	zval                        *retval = ctx;
	zval *entry = NULL;

	MAKE_STD_ZVAL(entry);

	php_phongo_server_to_zval(entry, server);

	add_next_index_zval(retval, entry);

	return true;
}

HashTable *php_phongo_manager_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_phongo_manager_t  *intern;
	zval                   retval = zval_used_for_init;


	*is_temp = 1;
	intern = (php_phongo_manager_t *)zend_object_store_get_object(object TSRMLS_CC);


	array_init(&retval);

	add_assoc_long_ex(&retval, ZEND_STRS("request_id"), intern->client->request_id);
	add_assoc_string_ex(&retval, ZEND_STRS("uri"), (char *)mongoc_uri_get_string(intern->client->uri), 1);


	{
		zval *cluster = NULL;
		MAKE_STD_ZVAL(cluster);
		array_init(cluster);

		mongoc_set_for_each(intern->client->topology->description.servers, phongo_add_server_debug, cluster);
		add_assoc_zval_ex(&retval, ZEND_STRS("cluster"), cluster);
	}

	return Z_ARRVAL(retval);
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
