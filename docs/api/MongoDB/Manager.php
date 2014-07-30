<?php

namespace MongoDB;

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
final class Manager
{
    /**
     * Constructs a new Manager
     *
     * @param string $uri           Connection URI
     * @param array  $options       Connection options (e.g. auth, socket timeouts)
     * @param array  $driverOptions Driver options (e.g. stream contexts)
     */
    public function __construct($uri, array $options = array(), array $driverOptions = array())
    {
        // Parse DSN and options and create Server objects
        /*** CEF ***/
/*
	void                   ***ctx = NULL;
	TSRMLS_SET_CTX(ctx);
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	intern->client = mongoc_client_new(uri);
	if (!intern->client) {
		phongo_throw_exception(PHONGO_ERROR_RUNETIME, "Failed to parse MongoDB URI" TSRMLS_CC);
		return;
	}
	mongoc_client_set_stream_initiator(intern->client, phongo_stream_initiator, ctx);
*/
        /*** CIMPL ***/
    }

    /**
     * Execute a command
     *
     * @param string         $db
     * @param Command        $command
     * @param ReadPreference $readPreference
     * @return CommandResult
     */
    public function executeCommand($db, Command $command, ReadPreference $readPreference = null)
    {
        /* Select server based on (1) command requirements and (2) read
         * preference and invoke Server::executeCommand().
         *
         * Read preferences should be injected into the command document if
         * necessary (e.g. mongos).
         *
         * Rely on libmongoc for server selection and read preference injection.
         *
         * Throw exception if no candidate server is found. Throw a special
         * exception the command requirement and read preferences logically
         * conflict (e.g. mapReduce with output collection and secondary RP).
         */
        /*** CEF ***/
/*
	php_phongo_command_t    *cmd;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	cmd = (php_phongo_command_t *)zend_object_store_get_object(command TSRMLS_CC);
	phongo_execute_command(intern->client, db, cmd->bson, phongo_read_preference_from_zval(readPreference TSRMLS_CC), return_value, return_value_used TSRMLS_CC);
*/
        /*** CIMPL ***/
    }

    /**
     * Execute a Query
     *
     * @param string         $namespace
     * @param Query          $zquery
     * @param ReadPreference $readPreference Read preference options (default: "primary")
     * @return QueryResult
     */
    public function executeQuery($namespace, Query $zquery, ReadPreference $readPreference = null)
    {
        /* Select server based on read preference and invoke
         * Server::executeQuery().
         *
         * Read preferences should be injected into the query document if
         * necessary (e.g. mongos).
         *
         * Rely on libmongoc for server selection and read preference injection.
         *
         * Throw exception if no candidate server is found.
         */
        /*** CIMPL ***/
/*
	phongo_execute_query(intern->client, namespace, phongo_query_from_zval(zquery TSRMLS_CC), phongo_read_preference_from_zval(readPreference TSRMLS_CC), return_value, return_value_used TSRMLS_CC);
*/
        /*** CIMPL ***/
    }

    /**
     * Executes a write operation batch (e.g. insert, update, delete)
     *
     * @param string       $namespace
     * @param WriteBatch   $zbatch
     * @param WriteConcern $writeConcern Write concern options (default: {"w": 1})
     * @return WriteResult
     */
    public function executeWriteBatch($namespace, WriteBatch $zbatch, WriteConcern $writeConcern = null)
    {
        /* Select writeable server and invoke Server::executeQuery().
         *
         * Write options are optional, and will be merged into the default,
         * which is {"ordered": true, "w": 1}.
         *
         * On error, we should consider throwing:
         *
         *  - NoCandidateException: no writeable server was found
         *  - WriteException: write failed; a WriteResult should be included.
         *  - ConnectionException: low-level socket error
         *
         * If a WriteException is thrown, its WriteResult may contain multiple
         * write errors and write concern errors.
         */
        /*** CEF ***/
/*
	php_phongo_writebatch_t   *batch;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	batch = (php_phongo_writebatch_t *)zend_object_store_get_object(zbatch TSRMLS_CC);
	phongo_execute_write(intern->client, namespace, batch->batch, 0, return_value, return_value_used TSRMLS_CC);
*/
        /*** CIMPL ***/
    }

    /**
     * Convenience method for single insert operation.
     *
     * @param string       $namespace
     * @param array|object $document     Document to insert
     * @param WriteConcern $writeConcern Write concern options (default: {"w": 1})
     * @return WriteResult
     */
    public function executeInsert($namespace, $document, WriteConcern $writeConcern = null)
    {
        /* Construct and execute a WriteBatch with a single insert operation.
         *
         * Write options are optional, and will be merged into the default,
         * which is {"w": 1}.
         */
        /*** CEF ***/
/*
	bson_t                   *bson;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	bson = bson_new();
	zval_to_bson(document, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);
	phongo_execute_single_insert(intern->client, namespace, bson, return_value, return_value_used TSRMLS_CC);
	bson_destroy(bson);
*/
        /*** CIMPL ***/
    }

    /**
     * Convenience method for single update operation.
     *
     * @param string       $namespace
     * @param array|object $zquery         Update criteria
     * @param array|object $newObj        Update modifier or replacement document
     * @param array        $updateOptions Update options (e.g. "upsert")
     * @param WriteConcern $writeConcern  Write concern options (default: {"w": 1})
     * @return WriteResult
     */
    public function executeUpdate($namespace, $zquery, $newObj, array $updateOptions = null, WriteConcern $writeConcern = null)
    {
        $updateOptions = array_merge(
            array('multi' => false, 'upsert' => false),
            $updateOptions
        );

        /* Construct and execute a WriteBatch with a single update operation.
         *
         * Write options are optional, and will be merged into the default,
         * which is {"w": 1}.
         */
        /*** CEF ***/
/*
	bson_t                   *query;
	bson_t                   *update;
	mongoc_update_flags_t     flags = MONGOC_UPDATE_NONE;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
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
*/
        /*** CIMPL ***/
    }

    /**
     * Convenience method for single delete operation.
     *
     * @param string       $namespace
     * @param array|object $query         Deletion criteria
     * @param array        $deleteOptions Deletion options (e.g. "limit")
     * @param WriteConcern $writeConcern Write concern options (default: {"w": 1})
     * @return WriteResult
     */
    public function executeDelete($namespace, $query, array $deleteOptions = null, WriteConcern $writeConcern = null)
    {
        /* TODO: Decide if we should default to 0 (i.e. "all"), as in 1.x, or if
         * an explicit "limit" option must be required. Keeping $deleteOptions
         * as an array, rather than a single integer option for "limit", seems
         * wise; if the "delete" command ever takes additional options, we would
         * not need to change our API.
         */
        $deleteOptions = array_merge(
            array('limit' => 0),
            $deleteOptions
        );

        /* Construct and execute a WriteBatch with a single delete operation.
         *
         * Write options are optional, and will be merged into the default,
         * which is {"w": 1}.
         */
        /*** CEF ***/
/*
	bson_t                   *bson;
	mongoc_delete_flags_t     flags = MONGOC_DELETE_NONE;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	if (deleteOptions && php_array_fetch_bool(deleteOptions, "limit")) {
		flags |= MONGOC_DELETE_SINGLE_REMOVE;
	}
	bson = bson_new();
	zval_to_bson(query, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);
	phongo_execute_single_delete(intern->client, namespace, bson, flags, return_value, return_value_used TSRMLS_CC);
	bson_destroy(bson);
*/
        /*** CIMPL ***/
    }

    /**
     * Returns the Servers associated with this Manager
     *
     * @return Server[]
     */
    public function getServers() {}
}
$Manager["free"] = <<< EOF
	mongoc_client_destroy(intern->client);

EOF;
$Manager["headers"][] = '"php_array.h"';
