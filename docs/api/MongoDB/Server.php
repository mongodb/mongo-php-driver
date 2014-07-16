<?php

namespace MongoDB;

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
     * Constructs a new Server
     *
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
        /*** CEF ***/
/*
	mongoc_uri_t             *uri;
	void                   ***ctx = NULL;
	TSRMLS_SET_CTX(ctx);
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	uri = mongoc_uri_new_for_host_port(host, port);
	intern->client = mongoc_client_new_from_uri(uri);
	if (!intern->client) {
		phongo_throw_exception(PHONGO_ERROR_RUNETIME, "Failed to parse MongoDB URI" TSRMLS_CC);
		return;
	}

	intern->host = mongoc_uri_get_hosts(uri);
	mongoc_client_set_stream_initiator(intern->client, phongo_stream_initiator, ctx);
*/
        /*** CIMPL ***/
    }

    /**
     * Executes a command on this server
     *
     * @param string  $db
     * @param Command $command
     * @return CommandResult
     */
    public function executeCommand($db, Command $command)
    {
        /* This method does not take a ReadPreference. If connected to mongos,
         * it the user's responsibility to set a read preference on the command
         * document (if applicable).
         */
        /*** CEF ***/
/*
	php_phongo_command_t     *cmd;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	cmd = (php_phongo_command_t *)zend_object_store_get_object(command TSRMLS_CC);
	phongo_execute_command(intern->client, db, cmd->bson, NULL, return_value, return_value_used TSRMLS_CC);
*/
        /*** CIMPL ***/
    }

    /**
     * Executes a Query
     *
     * @param string $namespace
     * @param Query  $zquery
     * @return QueryResult
     */
    public function executeQuery($namespace, Query $zquery)
    {
        /* This method does not take a ReadPreference. If connected to mongos,
         * it the user's responsibility to set a read preference on the command
         * document (if applicable).
         */
        /*** CIMPL ***/
/*
	phongo_execute_query(intern->client, namespace, phongo_query_from_zval(zquery TSRMLS_CC), NULL, return_value, return_value_used TSRMLS_CC);
*/
        /*** CIMPL ***/
    }

    /**
     * Executes a write operation batch (e.g. insert, update, delete)
     *
     * @param string     $namespace
     * @param WriteBatch $zbatch
     * @return WriteResult
     */
    public function executeWrite($namespace, WriteBatch $zbatch)
    {
        /* Write options are not taken as an argument, since they are specified
         * during Batch construction.
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
        /*** CEF ***/
/*
	php_phongo_writebatch_t  *batch;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	batch = (php_phongo_writebatch_t *)zend_object_store_get_object(zbatch TSRMLS_CC);
	phongo_execute_write(intern->client, namespace, batch->batch, intern->hint, return_value, return_value_used TSRMLS_CC);
*/
        /*** CIMPL ***/
    }

    /**
     * Returns the hostname used to connect to this Server
     * 
     * @return string
     */
    public function getHost()
    {
        /* This does not return the host name from isMaster, since the "me"
         * field is only present in replica set connections. There is also less
         * value in providing the driver with the host name by which the server
         * identifies (vs. the host name used to connect to it).
         */
        return $this->host;
        /*** CIMPL ***/
/*
    // FIXME: BUGBUG: this is a workaround as its not implemented yet :)
	server_populate(intern);
	RETURN_STRING(intern->host->host, 1);
*/
        /*** CIMPL ***/
    }

    /**
     * Returns the last isMaster() result document
     *
     * @return array Connection metadata (e.g. min/maxWireVersion, RS hosts)
     */
    public function getInfo()
    {
        // Return last isMaster result document
    }

    /**
     * Returns the last messured latency
     *
     * @return integer Server latency in milliseconds
     */
    public function getLatency()
    {
        // Return last ping time in milliseconds (calculated value)
    }

    /**
     * Returns the port used to create this Server
     *
     * @return integer
     */
    public function getPort()
    {
        return $this->port;
        /*** CIMPL ***/
/*
	RETURN_LONG(intern->host->port);
*/
        /*** CIMPL ***/
    }

    /**
     * Returns the current state of the node (maintenece/startup/...)
     *
     * @return integer Replica set node state
     */
    public function getState()
    {
        /* Return the replica set node's state via replSetGetStatus
         * http://docs.mongodb.org/manual/reference/replica-states/
         *
         * We may want to create class constants for documented states.
         *
         * What should this return if the node isn't in a replica set?
         */
    }

    /**
     * Returns the node type of this Server
     *
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

    /**
     * Checks if this is a special "delayed" member of a RepilcaSet
     *
     * @return bool true if delayed node, false otherwise
     */
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

    /**
     * Checks if this is a special passive node member of a ReplicaSet
     *
     * @return bool true if this node is passive, false otherwise
     */
    public function isPassive()
    {
        /* Return inferred value from isMaster data.
         *
         * Is this really useful in light of getInfo(), which provides the same
         * information, and more?
         */
    }
}

$Server["funcs"] = <<< EOT
inline int server_populate(php_phongo_server_t *server)
{
	mongoc_host_list_t *host = NULL;
	host = (mongoc_host_list_t *) emalloc(sizeof(mongoc_host_list_t));

	strcpy(host->host, "localhost");
	host->port = 27017;

	server->host = host;

	return true;
}

zend_object_handlers* php_phongo_handlers_server() /* {{{ */
{
	return &php_phongo_handler_server;
} /* }}} */
static int php_phongo_server_compare_objects(zval *o1, zval *o2 TSRMLS_DC) /* {{{ */
{
    php_phongo_server_t *intern1;
    php_phongo_server_t *intern2;

    intern1 = (php_phongo_server_t *)zend_object_store_get_object(o1 TSRMLS_CC);
    intern2 = (php_phongo_server_t *)zend_object_store_get_object(o2 TSRMLS_CC);

	/* FIXME: BUGBUG: We need a way to get mongoc_host from WriteResults */
	if (intern1 && intern2) {
		return 0;
	}
    /*
	if (!strcmp(intern1->host->host_and_port, intern2->host->host_and_port)) {
		return 0;
	}
     */

	return 1;
} /* }}} */

EOT;

$Server["forward_declarations"] = <<< EOT

inline int server_populate(php_phongo_server_t *server);
zend_object_handlers php_phongo_handler_server;


EOT;

$Server["handlers_callback"] = "php_phongo_handlers_server";
$Server["handlers_init"]     = "
	memcpy(&php_phongo_handler_server, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_server.compare_objects = php_phongo_server_compare_objects;
";
