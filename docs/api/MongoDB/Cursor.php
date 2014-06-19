<?php

namespace MongoDB;

interface Cursor extends \Iterator
{
    /**
     * Returns the CursorId
     *
     * @return CursorId
     */
    function getId();

    /**
     * Returns the Server object that this cursor is attached to
     *
     * @return Server Server from which the cursor originated
     */
    function getServer();

    /**
     * Checks if a cursor is still alive
     *
     * @return boolean Whether the cursor is exhausted and has no more results
     */
    function isDead();

    /**
     * Sets the BatchSize of the Cursor
     *
     * @param integer $batchSize
     * @return boolean true on success, false on failure
     */
    function setBatchSize($batchSize);
}
