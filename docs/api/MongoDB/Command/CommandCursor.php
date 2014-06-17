<?php

namespace MongoDB\Command;

use MongoDB\Cursor;
use MongoDB\CursorId;
use Mongodb\Server;

/**
 * Cursor implementation that may be constructed from values found in a
 * CommandResult's response document.
 *
 * The iteration and internal logic is very similar to QueryCursor, so both
 * classes should likely share code. The first batch is comparable to the
 * documents found in the OP_REPLY message returned by a QueryCursor's original
 * OP_QUERY, in that both may be available at the time the cursor is
 * constructed.
 */
final class CommandCursor implements Cursor
{
    private $server;
    private $batchSize;
    private $cursorId;
    private $firstBatch;

    /**
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
     * @return Cursor::getId()
     */
    public function getId()
    {
        return $this->cursorId;
    }

    /**
     * @see Cursor::getServer()
     */
    public function getServer()
    {
        return $this->server;
    }

    /**
     * @see Cursor::isDead()
     */
    public function isDead()
    {
        // Return whether the cursor is exhausted and has no more results
    }

    /**
     * @see \MongoDB\Cursor::setBatchSize()
     */
    public function setBatchSize($batchSize)
    {
        $this->batchSize = (integer) $batchSize;
    }

    /* Cursor is an iterator */
    public function current() {}
    public function next() {}
    public function key() {}
    public function valid() {}
    public function rewind() {}
}
