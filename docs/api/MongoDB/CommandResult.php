<?php

namespace MongoDB;

use IteratorAggregate;

/**
 * Result returned by Server and Manager executeCommand() methods.
 *
 * This object wraps an OP_REPLY. It is constructed after a command is executed
 * on the server but before a Cursor is created in the driver (if applicable).
 * This allows the Cursor implementation to be customized.
 *
 * For commands that do not support cursors (i.e. most commands), getIterator()
 * should return a cursor consisting of a single document, the command result.
 */
final class CommandResult implements IteratorAggregate
{
    private $iteratorClass = 'MongoDB\Cursor';
    private $iteratorInitCallback;

    private $server;
    private $responseDocument;

    /**
     * Constructs a new CommandResult
     *
     * @param Server       $server
     * @param array|object $responseDocument
     */
    public function __construct(Server $server, $responseDocument)
    {
        $this->server = $server;
        $this->responseDocument = $responseDocument;
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
     * Returns the original response document from the server
     *
     * For commands that do not support cursors, this will be the same document
     * that is accessible by the Cursor returned by getIterator(). The commands
     * that do support cursors, this will contain the data used to create the
     * Cursor returned by getIterator() (e.g. cursor ID, first batch).
     *
     * @return array
     */
    public function getResponseDocument()
    {
        return $this->responseDocument;
    }

    /**
     * Returns the Server object that this cursor is attached to
     *
     * @return Server Server from which the result originated
     */
    public function getServer()
    {
        return $this->server;
    }
}

