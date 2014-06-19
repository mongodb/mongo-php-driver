<?php

namespace MongoDB\Command;

use MongoDB\Server;

/**
 * Result returned by Server and Manager executeCommand() methods.
 */
final class CommandResult
{
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
     * Returns the original response document from the server
     *
     * @return array Original response document from the server
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
