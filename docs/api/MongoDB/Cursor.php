<?php

namespace MongoDB;

interface Cursor extends \Iterator
{
    /**
     * @return CursorId
     */
    function getId();

    /**
     * @return Server Server from which the cursor originated
     */
    function getServer();

    /**
     * @return boolean Whether the cursor is exhausted and has no more results
     */
    function isDead();

    /**
     * @param integer $batchSize
     */
    function setBatchSize($batchSize);
}
