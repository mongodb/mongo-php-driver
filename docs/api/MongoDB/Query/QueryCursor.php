<?php

namespace MongoDB\Query;

// Note: consider combining implementation with \MongoDB\Command\CommandCursor
final class QueryCursor implements \MongoDB\Cursor
{
    private $server;
    private $batchSize;
    private $cursorId;

    /**
     * @param Server  $server
     * @param integer $cursorId
     */
    public function __construct(Server $server, $cursorId)
    {
        $this->server = $server;
        $this->cursorId = (integer) $cursorId;
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
}
