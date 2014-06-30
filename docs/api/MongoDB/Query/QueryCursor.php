<?php

namespace MongoDB\Query;

use MongoDB\Cursor;
use MongoDB\CursorId;
use Mongodb\Server;

/**
 * Cursor implementation that is returned after executing a Query.
 *
 * The iteration and internal logic is very similar to CommandCursor, so both
 * classes should likely share code. The documents in the OP_REPLY message
 * returned by the original OP_QUERY is comparable to the first batch of a
 * command cursor, in that both may be available at the time the cursor is
 * constructed.
 */
final class QueryCursor implements Cursor
{
    private $server;
    private $batchSize;
    private $cursorId;

    /**
     * Construct a new QueryCursor
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
