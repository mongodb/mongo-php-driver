<?php

namespace MongoDB;

use MongoDB\Command\Command;
use MongoDB\Command\CommandResult;
use MongoDB\Query\Query;
use MongoDB\Query\QueryCursor;
use MongoDB\Write\WriteBatch;
use MongoDB\Write\WriteOptions;
use MongoDB\Write\WriteResult;

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
     * @param string $uri           Connection URI
     * @param array  $options       Connection options (e.g. auth, socket timeouts)
     * @param array  $driverOptions Driver options (e.g. stream contexts)
     */
    public function __construct($uri, array $options = array(), array $driverOptions = array())
    {
        // Parse DSN and options and create Server objects
    }

    /**
     * @param Server[] $servers
     * @return Manager
     */
    static public function createFromServers(array $servers)
    {
        /* Instantiate a new Manager instance with the provided Servers
         *
         * Should this method take $driverOptions as a second parameter? The
         * Servers would already be constructed, so stream context options
         * would not apply; however, we may want to take other options, such as
         * event callbacks, down the line.
         */
    }

    /**
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
    }

    /**
     * @param string         $namespace
     * @param Query          $query
     * @param ReadPreference $readPreference
     * @return QueryCursor
     */
    public function executeQuery($namespace, Query $query, ReadPreference $readPreference = null)
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
    }

    /**
     * @param string     $namespace
     * @param WriteBatch $batch
     * @param array      $writeOptions Ordering and write concern options (default: {"ordered": true, "w": 1})
     * @return WriteResult
     */
    public function executeWrite($namespace, WriteBatch $batch, array $writeOptions = null)
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
    }

    /**
     * Convenience method for single insert operation.
     *
     * @param string       $namespace
     * @param array|object $document     Document to insert
     * @param array        $writeOptions Write concern options (default: {"w": 1})
     * @return InsertResult
     */
    public function executeInsert($namespace, $document, array $writeOptions = null)
    {
        /* Construct and execute an InsertBatch
         *
         * Write options are optional, and will be merged into the default,
         * which is {"w": 1}.
         */
    }

    /**
     * Convenience method for single update operation.
     *
     * @param string       $namespace
     * @param array|object $query         Update criteria
     * @param array|object $newObj        Update modifier or replacement document
     * @param array        $updateOptions Update options (e.g. "upsert")
     * @param array        $writeOptions  Write concern options (default: {"w": 1})
     * @return UpdateResult
     */
    public function executeUpdate($namespace, $query, $newObj, array $updateOptions = null, array $writeOptions = null)
    {
        /* Construct and execute an UpdateBatch
         *
         * Write options are optional, and will be merged into the default,
         * which is {"w": 1}.
         *
         * What should be the default value for $options? No multi, no upsert?
         */
    }

    /**
     * Convenience method for single delete operation.
     *
     * @param string       $namespace
     * @param array|object $query         Deletion criteria
     * @param array        $deleteOptions Deletion options (e.g. "limit")
     * @param array        $writeOptions  Write concern options (default: {"w": 1})
     * @return DeleteResult
     */
    public function executeDelete($namespace, $query, array $deleteOptions = null, array $writeOptions = null)
    {
        /* Construct and execute an DeleteBatch
         *
         * Write options are optional, and will be merged into the default,
         * which is {"w": 1}.
         *
         * What should be the default value for $options? No limit?
         */
    }
}
