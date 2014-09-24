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

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"
#include "php_array.h"


PHONGO_API zend_class_entry *php_phongo_manager_ce;

/* {{{ proto MongoDB\Manager Manager::__construct(string $uri[, array $options = array()[, array $driverOptions = array()]])
   Constructs a new Manager */
PHP_METHOD(Manager, __construct)
{
	php_phongo_manager_t     *intern;
	zend_error_handling       error_handling;
	char                     *uri;
	int                       uri_len;
	zval                     *options = NULL;
	zval                     *driverOptions = NULL;
	void                   ***ctx = NULL;
	TSRMLS_SET_CTX(ctx);


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a!a!", &uri, &uri_len, &options, &driverOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	intern->client = mongoc_client_new(uri);
	if (!intern->client) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to parse MongoDB URI" TSRMLS_CC);
		return;
	}
	mongoc_client_set_stream_initiator(intern->client, phongo_stream_initiator, ctx);
}
/* }}} */
/* {{{ proto MongoDB\CommandResult Manager::executeCommand(string $db, MongoDB\Command $command[, MongoDB\ReadPreference $readPreference = null])
   Execute a command */
PHP_METHOD(Manager, executeCommand)
{
	php_phongo_manager_t     *intern;
	zend_error_handling       error_handling;
	char                     *db;
	int                       db_len;
	zval                     *command;
	zval                     *readPreference = NULL;
	php_phongo_command_t    *cmd;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &db, &db_len, &command, php_phongo_command_ce, &readPreference, php_phongo_readpreference_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	cmd = (php_phongo_command_t *)zend_object_store_get_object(command TSRMLS_CC);
	phongo_execute_command(intern->client, db, cmd->bson, phongo_read_preference_from_zval(readPreference TSRMLS_CC), return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\QueryResult Manager::executeQuery(string $namespace, MongoDB\Query $zquery[, MongoDB\ReadPreference $readPreference = null])
   Execute a Query */
PHP_METHOD(Manager, executeQuery)
{
	php_phongo_manager_t     *intern;
	zend_error_handling       error_handling;
	char                     *namespace;
	int                       namespace_len;
	zval                     *zquery;
	zval                     *readPreference = NULL;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &namespace, &namespace_len, &zquery, php_phongo_query_ce, &readPreference, php_phongo_readpreference_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	phongo_execute_query(intern->client, namespace, phongo_query_from_zval(zquery TSRMLS_CC), phongo_read_preference_from_zval(readPreference TSRMLS_CC), return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\WriteResult Manager::executeWriteBatch(string $namespace, MongoDB\WriteBatch $zbatch[, MongoDB\WriteConcern $writeConcern = null])
   Executes a write operation batch (e.g. insert, update, delete) */
PHP_METHOD(Manager, executeWriteBatch)
{
	php_phongo_manager_t     *intern;
	zend_error_handling       error_handling;
	char                     *namespace;
	int                       namespace_len;
	zval                     *zbatch;
	zval                     *writeConcern = NULL;
	php_phongo_writebatch_t   *batch;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O!", &namespace, &namespace_len, &zbatch, php_phongo_writebatch_ce, &writeConcern, php_phongo_writeconcern_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	batch = (php_phongo_writebatch_t *)zend_object_store_get_object(zbatch TSRMLS_CC);
	phongo_execute_write(intern->client, namespace, batch->batch, 0, return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\WriteResult Manager::executeInsert(string $namespace, array|object $document[, MongoDB\WriteConcern $writeConcern = null])
   Convenience method for single insert operation. */
PHP_METHOD(Manager, executeInsert)
{
	php_phongo_manager_t     *intern;
	zend_error_handling       error_handling;
	char                     *namespace;
	int                       namespace_len;
	zval                     *document;
	zval                     *writeConcern = NULL;
	bson_t                   *bson;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sA|O!", &namespace, &namespace_len, &document, &writeConcern, php_phongo_writeconcern_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	bson = bson_new();
	zval_to_bson(document, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);
	phongo_execute_single_insert(intern->client, namespace, bson, return_value, return_value_used TSRMLS_CC);
	bson_destroy(bson);
}
/* }}} */
/* {{{ proto MongoDB\WriteResult Manager::executeUpdate(string $namespace, array|object $zquery, array|object $newObj[, array $updateOptions = array()[, MongoDB\WriteConcern $writeConcern = null]])
   Convenience method for single update operation. */
PHP_METHOD(Manager, executeUpdate)
{
	php_phongo_manager_t     *intern;
	zend_error_handling       error_handling;
	char                     *namespace;
	int                       namespace_len;
	zval                     *zquery;
	zval                     *newObj;
	zval                     *updateOptions = NULL;
	zval                     *writeConcern = NULL;
	bson_t                   *query;
	bson_t                   *update;
	mongoc_update_flags_t     flags = MONGOC_UPDATE_NONE;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sAA|a!O!", &namespace, &namespace_len, &zquery, &newObj, &updateOptions, &writeConcern, php_phongo_writeconcern_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


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

	phongo_execute_single_update(intern->client, namespace, query, update, flags, return_value, return_value_used TSRMLS_CC);
	bson_destroy(query);
	bson_destroy(update);
}
/* }}} */
/* {{{ proto MongoDB\WriteResult Manager::executeDelete(string $namespace, array|object $query[, array $deleteOptions = array()[, MongoDB\WriteConcern $writeConcern = null]])
   Convenience method for single delete operation. */
PHP_METHOD(Manager, executeDelete)
{
	php_phongo_manager_t     *intern;
	zend_error_handling       error_handling;
	char                     *namespace;
	int                       namespace_len;
	zval                     *query;
	zval                     *deleteOptions = NULL;
	zval                     *writeConcern = NULL;
	bson_t                   *bson;
	mongoc_delete_flags_t     flags = MONGOC_DELETE_NONE;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sA|a!O!", &namespace, &namespace_len, &query, &deleteOptions, &writeConcern, php_phongo_writeconcern_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	if (deleteOptions && php_array_fetch_bool(deleteOptions, "limit")) {
		flags |= MONGOC_DELETE_SINGLE_REMOVE;
	}
	bson = bson_new();
	zval_to_bson(query, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);
	phongo_execute_single_delete(intern->client, namespace, bson, flags, return_value, return_value_used TSRMLS_CC);
	bson_destroy(bson);
}
/* }}} */
/* {{{ proto Server[] Manager::getServers()
   Returns the Servers associated with this Manager */
PHP_METHOD(Manager, getServers)
{
	php_phongo_manager_t     *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

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
/* {{{ MongoDB\Manager */

ZEND_BEGIN_ARG_INFO_EX(ai_Manager___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, uri)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
	ZEND_ARG_ARRAY_INFO(0, driverOptions, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Command, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\ReadPreference, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeQuery, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zquery, MongoDB\\Query, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\ReadPreference, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeWriteBatch, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zbatch, MongoDB\\WriteBatch, 0)
	ZEND_ARG_OBJ_INFO(0, writeConcern, MongoDB\\WriteConcern, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeInsert, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_INFO(0, document)
	ZEND_ARG_OBJ_INFO(0, writeConcern, MongoDB\\WriteConcern, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeUpdate, 0, 0, 3)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_INFO(0, zquery)
	ZEND_ARG_INFO(0, newObj)
	ZEND_ARG_ARRAY_INFO(0, updateOptions, 1)
	ZEND_ARG_OBJ_INFO(0, writeConcern, MongoDB\\WriteConcern, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeDelete, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_ARRAY_INFO(0, deleteOptions, 1)
	ZEND_ARG_OBJ_INFO(0, writeConcern, MongoDB\\WriteConcern, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_getServers, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_manager_me[] = {
	PHP_ME(Manager, __construct, ai_Manager___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeCommand, ai_Manager_executeCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeQuery, ai_Manager_executeQuery, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Manager, executeWriteBatch, ai_Manager_executeWriteBatch, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
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
	php_phongo_manager_t *intern;

	intern = (php_phongo_manager_t *)emalloc(sizeof(php_phongo_manager_t));
	memset(intern, 0, sizeof(php_phongo_manager_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_manager_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Manager)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB", "Manager", php_phongo_manager_me);
	ce.create_object = php_phongo_manager_create_object;
	php_phongo_manager_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_manager_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;


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
