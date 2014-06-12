<?php

namespace MongoDB\Command;

// Note: consider combining implementation with \MongoDB\Query\QueryCursor
final class CommandCursor implements \MongoDB\Cursor
{
    private $server;
    private $batchSize;
    private $cursorId;
    private $firstBatch;

    /**
     * @param Server  $server
     * @param integer $cursorId
     * @param array   $firstBatch
     */
    public function __construct(Server $server, $cursorId, array $firstBatch)
    {
        $this->server = $server;
        $this->cursorId = (integer) $cursorId;
        $this->firstBatch = $firstBatch;
    }

    // Iterator methods...

    /**
     * @see \MongoDB\Cursor::getId()
     */
    public function getId()
    {
        return $this->cursorId;
    }

    /**
     * @see \MongoDB\ServerResult::getServer()
     */
    public function getServer()
    {
        return $this->server;
    }

    /**
     * @see \MongoDB\Cursor::setBatchSize()
     */
    public function setBatchSize($batchSize)
    {
        $this->batchSize = (integer) $batchSize;
    }

    // Note: this expects consistent command response documents from the server
    static public function createFromCommandResult(CommandResult $result)
    {
        // extract $cursorId and $firstBatch from $result->getResponse()

        return new static($result->getServer(), $cursorId, $firstBatch);
    }
}
