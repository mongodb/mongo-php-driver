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
     * @param Server       $server
     * @param array|object $responseDocument
     */
    public function __construct(Server $server, $responseDocument)
    {
        $this->server = $server;
        $this->responseDocument = $responseDocument;
    }

    /**
     * @return array Original response document from the server
     */
    public function getResponseDocument()
    {
        return $this->responseDocument;
    }

    /**
     * @return Server Server from which the result originated
     */
    public function getServer()
    {
        return $this->server;
    }
}
