<?php

namespace MongoDB;

use IteratorAggregate;

/**
 * Result returned by Server and Manager executeQuery() methods.
 *
 * This object wraps an OP_REPLY. It is constructed after a query is executed
 * on the server but before a Cursor is created in the driver. This allows the
 * Cursor implementation to be customized.
 */
final class QueryResult implements IteratorAggregate
{
    private $iteratorClass = 'MongoDB\Cursor';
    private $iteratorInitCallback;

    private $server;

    /**
     * Construct a new QueryResult
     *
     * @param Server   $server
     * @param CursorId $cursorId
     * @param array    $firstBatch
     */
    public function __construct(Server $server, CursorId $cursorId, array $firstBatch)
    {
        $this->server = $server;
        $this->cursorId = $cursorId;
        $this->firstBatch = $firstBatch;
    }

    /**
     * Returns the Cursor iterator
     *
     * @see IteratorAggregate::getIterator()
     * @return Cursor
     */
    public function getIterator()
    {
        // Construct a Cursor instance based on the specified iterator class
    }

    /**
     * Sets the class name of the Cursor implementation to be used
     *
     * @param string $class
     * @return self
     * @throws InvalidArgumentException if the class does not extend MongoDB\Cursor
     */
    public function setIteratorClass($class)
    {
        $this->iteratorClass = $class;
        return $this;
    }

    /**
     * Sets a callback to invoke for initializing a custom Cursor
     *
     * Since Cursors are constructed internally (its constructor is also private
     * and final), a callback may be registered so that custom Cursor
     * implementations can be initialized (e.g. inject dependencies) before
     * being returned by getIterator(). The Cursor instance will be passed as
     * the first and only argument to the callback.
     *
     * The callback should not invoke Iterator methods on the Cursor, although
     * we cannot actually enforce that.
     *
     * @param callable $callback
     * @return self
     */
    public function setIteratorInitCallback(callable $callback)
    {
        $this->iteratorInitCallback = $callback;
        return $this;
    }

    /**
     * Returns the Server object that this cursor is attached to
     *
     * @return Server Server from which the result originated
     */
    public function getServer()
    {
        return $this->server;
        /*** CEF ***/
/*
	mongoc_host_list_t       *host;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	host = (mongoc_host_list_t *) emalloc(sizeof(mongoc_host_list_t));
	mongoc_cursor_get_host(intern->result.cursor, host);
	phongo_server_init(return_value, intern->result.hint, host TSRMLS_CC);
*/
        /*** CIMPL ***/
    }
}


$QueryResult["internwrapper"] = "result.";
$QueryResult["ce"]["get_iterator"] = "phongo_result_get_iterator";

