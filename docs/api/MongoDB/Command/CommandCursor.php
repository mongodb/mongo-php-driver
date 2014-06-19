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
     * Constructs a new CommandCursor object
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
     * Returns the CursorId
     *
     * @return CursorId
     */
    public function getId()
    {
        return $this->cursorId;
    }

    /**
     * Returns the Server object that this cursor is attached to
     *
     * @return Server Server from which the cursor originated
     */
    public function getServer()
    {
        return $this->server;
    }

    /**
     * Checks if a cursor is still alive
     *
     * @return boolean Whether the cursor is exhausted and has no more results
     */
    public function isDead()
    {
        // Return whether the cursor is exhausted and has no more results
    }

    /**
     * Sets a batch size for the cursor
     *
     * @param integer $batchSize
     * @return boolean true on success, false on failure
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
