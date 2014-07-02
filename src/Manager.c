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


PHONGO_API zend_class_entry *php_phongo_manager_ce;

/* {{{ proto MongoDB\Manager Manager::__construct(string $uri[, array $options = array()[, array $driverOptions = array()]])
   Constructs a new Manager */
PHP_METHOD(Manager, __construct)
{
	php_phongo_manager_t  *intern;
	zend_error_handling	error_handling;
	char                  *uri;
	int                    uri_len;
	zval                  *options;
	zval                  *driverOptions;
	void                ***ctx = NULL;
	TSRMLS_SET_CTX(ctx);

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|aa", &uri, &uri_len, &options, &driverOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	intern->client = mongoc_client_new(uri);
	if (!intern->client) {
		phongo_throw_exception(PHONGO_ERROR_RUNETIME TSRMLS_CC, "Failed to parse MongoDB URI");
		return;
	}
	mongoc_client_set_stream_initiator(intern->client, phongo_stream_initiator, ctx);
}
/* }}} */
/* {{{ proto MongoDB\Manager Manager::createFromServers(array $servers)
   Creates new Manager from a list of servers */
PHP_METHOD(Manager, createFromServers)
{
	php_phongo_manager_t  *intern;
	zend_error_handling	error_handling;
	zval                  *servers;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &servers) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Command\CommandResult Manager::executeCommand(string $db, MongoDB\Command\Command $command[, MongoDB\ReadPreference $readPreference = null])
   Execute a command */
PHP_METHOD(Manager, executeCommand)
{
	php_phongo_manager_t  *intern;
	zend_error_handling	error_handling;
	char                  *db;
	int                    db_len;
	zval                  *command;
	zval                  *readPreference;
	php_phongo_command_t  *cmd;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O", &db, &db_len, &command, php_phongo_command_ce, &readPreference, php_phongo_readpreference_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	cmd = (php_phongo_command_t *)zend_object_store_get_object(command TSRMLS_CC);
	phongo_execute_command(intern->client, mongoc_client_get_database(intern->client, db), cmd->bson, readPreference, return_value, return_value_used);
}
/* }}} */
/* {{{ proto MongoDB\Query\QueryCursor Manager::executeQuery(string $namespace, MongoDB\Query\Query $query[, MongoDB\ReadPreference $readPreference = null])
   Execute a Query */
PHP_METHOD(Manager, executeQuery)
{
	php_phongo_manager_t  *intern;
	zend_error_handling	error_handling;
	char                  *namespace;
	int                    namespace_len;
	zval                  *zquery;
	zval                  *readPreference;
	mongoc_collection_t   *collection;
	php_phongo_query_t    *query;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|O", &namespace, &namespace_len, &zquery, php_phongo_query_ce, &readPreference, php_phongo_readpreference_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	query = (php_phongo_query_t *)zend_object_store_get_object(zquery TSRMLS_CC);
	collection = phongo_get_collection_from_namespace(intern->client, namespace, namespace_len);
	phongo_execute_query(intern->client, collection, query->bson, return_value, return_value_used);
	mongoc_collection_destroy(collection);
}
/* }}} */
/* {{{ proto MongoDB\Write\WriteResult Manager::executeWrite(string $namespace, MongoDB\Write\WriteBatch $batch[, array $writeOptions = array()])
   Executes a write operation batch (e.g. insert, update, delete) */
PHP_METHOD(Manager, executeWrite)
{
	php_phongo_manager_t  *intern;
	zend_error_handling    error_handling;
	char                  *namespace;
	int                    namespace_len;
	zval                  *batch;
	zval                  *writeOptions;
	mongoc_collection_t   *collection;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO|a", &namespace, &namespace_len, &batch, php_phongo_writebatch_ce, &writeOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	collection = phongo_get_collection_from_namespace(intern->client, namespace, namespace_len);
	phongo_execute_write(intern->client, collection, batch, return_value, return_value_used);
	mongoc_collection_destroy(collection);
}
/* }}} */
/* {{{ proto MongoDB\Write\InsertResult Manager::executeInsert(string $namespace, array|object $document[, array $writeOptions = array()])
   Convenience method for single insert operation. */
PHP_METHOD(Manager, executeInsert)
{
	php_phongo_manager_t  *intern;
	zend_error_handling    error_handling;
	char                  *namespace;
	int                    namespace_len;
	zval                  *document;
	zval                  *writeOptions;
	mongoc_collection_t   *collection;
	bson_t                 *bson;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sA|a", &namespace, &namespace_len, &document, &writeOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

	collection = phongo_get_collection_from_namespace(intern->client, namespace, namespace_len);

	bson = bson_new();
	php_phongo_bson_encode_array(bson, document TSRMLS_CC);
	phongo_crud_insert(intern->client, collection, bson, return_value, return_value_used);
	mongoc_collection_destroy(collection);
	bson_destroy(bson);
}
/* }}} */
/* {{{ proto MongoDB\Write\UpdateResult Manager::executeUpdate(string $namespace, array|object $query, array|object $newObj[, array $updateOptions = array()[, array $writeOptions = array()]])
   Convenience method for single update operation. */
PHP_METHOD(Manager, executeUpdate)
{
	php_phongo_manager_t  *intern;
	zend_error_handling	error_handling;
	char                  *namespace;
	int                    namespace_len;
	zval                  *query;
	zval                  *newObj;
	zval                  *updateOptions;
	zval                  *writeOptions;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sAA|aa", &namespace, &namespace_len, &query, &newObj, &updateOptions, &writeOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\Write\DeleteResult Manager::executeDelete(string $namespace, array|object $query[, array $deleteOptions = array()[, array $writeOptions = array()]])
   Convenience method for single delete operation. */
PHP_METHOD(Manager, executeDelete)
{
	php_phongo_manager_t  *intern;
	zend_error_handling	error_handling;
	char                  *namespace;
	int                    namespace_len;
	zval                  *query;
	zval                  *deleteOptions;
	zval                  *writeOptions;

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_manager_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sA|aa", &namespace, &namespace_len, &query, &deleteOptions, &writeOptions) == FAILURE) {
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

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_createFromServers, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, servers, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db) 
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Command\\Command, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\ReadPreference, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeQuery, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace) 
	ZEND_ARG_OBJ_INFO(0, query, MongoDB\\Query\\Query, 0)
	ZEND_ARG_OBJ_INFO(0, readPreference, MongoDB\\ReadPreference, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeWrite, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace) 
	ZEND_ARG_OBJ_INFO(0, batch, MongoDB\\Write\\WriteBatch, 0)
	ZEND_ARG_ARRAY_INFO(0, writeOptions, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeInsert, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace) 
	ZEND_ARG_INFO(0, document) 
	ZEND_ARG_ARRAY_INFO(0, writeOptions, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeUpdate, 0, 0, 3)
	ZEND_ARG_INFO(0, namespace) 
	ZEND_ARG_INFO(0, query) 
	ZEND_ARG_INFO(0, newObj) 
	ZEND_ARG_ARRAY_INFO(0, updateOptions, 1)
	ZEND_ARG_ARRAY_INFO(0, writeOptions, 1)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Manager_executeDelete, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace) 
	ZEND_ARG_INFO(0, query) 
	ZEND_ARG_ARRAY_INFO(0, deleteOptions, 1)
	ZEND_ARG_ARRAY_INFO(0, writeOptions, 1)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_manager_me[] = {
	PHP_ME(Manager, __construct, ai_Manager___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Manager, createFromServers, ai_Manager_createFromServers, ZEND_ACC_PUBLIC)
	PHP_ME(Manager, executeCommand, ai_Manager_executeCommand, ZEND_ACC_PUBLIC)
	PHP_ME(Manager, executeQuery, ai_Manager_executeQuery, ZEND_ACC_PUBLIC)
	PHP_ME(Manager, executeWrite, ai_Manager_executeWrite, ZEND_ACC_PUBLIC)
	PHP_ME(Manager, executeInsert, ai_Manager_executeInsert, ZEND_ACC_PUBLIC)
	PHP_ME(Manager, executeUpdate, ai_Manager_executeUpdate, ZEND_ACC_PUBLIC)
	PHP_ME(Manager, executeDelete, ai_Manager_executeDelete, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_manager_free_object && php_phongo_manager_create_object */
static void php_phongo_manager_free_object(void *object TSRMLS_DC)
{
	php_phongo_manager_t *intern = (php_phongo_manager_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	mongoc_client_destroy(intern->client);

	efree(intern);
}

zend_object_value php_phongo_manager_create_object(zend_class_entry *class_type TSRMLS_DC)
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
}
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
