<?php

namespace MongoDB;

use MongoDB\Command\Command;
use MongoDB\Command\CommandResult;
use MongoDB\Query\Query;
use MongoDB\Query\QueryCursor;
use MongoDB\Write\WriteBatch;
use MongoDB\Write\WriteResult;

/**
 * Server abstracts a socket connection to a single MongoDB server. The server
 * itself may be a mongod (stand-alone or replica set node) or mongos process.
 *
 * Users will typically not construct this class directly. The common use case
 * will be connection to a cluster of servers via a URI with the Manager class.
 *
 * This class does not utilize read preferences, since there is only a single
 * single socket on which to send a command, query, or write.
 *
 * Operation methods do not take socket-level options (e.g. socketTimeoutMS).
 * Those options should be specified during construction.
 */
final class Server
{
    const TYPE_MONGOS     = 0x01;
    const TYPE_STANDALONE = 0x02;
    const TYPE_ARBITER    = 0x03;
    const TYPE_SECONDARY  = 0x04;
    const TYPE_PRIMARY    = 0x05;

    private $host;
    private $port;
    private $options;
    private $driverOptions;

    /**
     * @param string  $host          Server host
     * @param integer $port          Server port
     * @param array   $options       Connection options (e.g. auth, socket timeouts)
     * @param array   $driverOptions Driver options (e.g. stream contexts)
     */
    public function __construct($host, $port, array $options = array(), array $driverOptions = array())
    {
        $this->host = (string) $host;
        $this->port = (integer) $port;
        $this->options = $options;
        $this->driverOptions = $driverOptions;

        /* Server connections may use a persistent socket connection. Sockets
         * will need to be hashed by all relevant arguments, which likely means
         * all of Server's constructor arguments. Argument arrays will need to 
         * be normalized before we compute the hash (i.e. option order shouldn't
         * matter).
         *
         * Actual socket connections should be created lazily.
         */
    }

    /**
     * @param string  $namespace
     * @param Command $command
     * @return CommandResult
     */
    public function executeCommand($namespace, Command $command)
    {
        /* This method does not take a ReadPreference. If connected to mongos,
         * it the user's responsibility to set a read preference on the command
         * document (if applicable).
         */
    }

    /**
     * @param string $namespace
     * @param Query  $query
     * @return QueryCursor
     */
    public function executeQuery($namespace, Query $query)
    {
        /* This method does not take a ReadPreference. If connected to mongos,
         * it the user's responsibility to set a read preference on the command
         * document (if applicable).
         */
    }

    /**
     * @param string     $namespace
     * @param WriteBatch $batch
     * @return WriteResult
     */
    public function executeWrite($namespace, WriteBatch $batch)
    {
        /* Write options are not taken as an argument, since they are specified
         * during WriteBatch construction.
         *
         * On error, we should consider throwing:
         *
         *  - WriteException: write failed; a WriteResult should be included.
         *  - ConnectionException: low-level socket error
         *
         * If a WriteException is thrown, its WriteResult may contain multiple
         * write errors and write concern errors.
         *
         * What exception is used for sending a write to a secondary? Would that
         * be a WriteException without a WriteResult attached?
         */
    }

    public function getHost()
    {
        /* This does not return the host name from isMaster, since the "me"
         * field is only present in replica set connections. There is also less
         * value in providing the driver with the host name by which the server
         * identifies (vs. the host name used to connect to it).
         */
        return $this->host;
    }

    /**
     * @return array Connection metadata (e.g. min/maxWireVersion, RS hosts)
     */
    public function getInfo()
    {
        // Return last isMaster result document
    }

    /**
     * @return integer Server latency in milliseconds
     */
    public function getLatency()
    {
        // Return last ping time in milliseconds (calculated value)
    }

    /**
     * @return integer
     */
    public function getPort()
    {
        return $this->port;
    }

    /**
     * @see http://docs.mongodb.org/manual/reference/replica-states/
     * @return integer Replica set node state
     */
    public function getState()
    {
        /* Return the replica set node's state via replSetGetStatus
         *
         * We may want to create class constants for documented states.
         *
         * What should this return if the node isn't in a replica set?
         */
    }

    /**
     * @return integer Server type code
     */
    public function getType()
    {
        /* Return the server's current type code.
         *
         * The fact that Servers are mutable and this return value is subject to
         * change is not a problem, as libmongoc should have its own snapshot of
         * the cluster topology for evaluating read preferences.
         */
    }

    public function isDelayed()
    {
        /* Return whether the secondary is delayed.
         *
         * What should this return for a node that isn't in a replica set? Isn't
         * the actual delay time more useful than a boolean? This isn't reported
         * in isMaster, so how do we calculate this short of reading the replica
         * set configuration?
         */
    }

    public function isPassive()
    {
        /* Return inferred value from isMaster data.
         *
         * Is this really useful in light of getInfo(), which provides the same
         * information, and more?
         */
    }
}
