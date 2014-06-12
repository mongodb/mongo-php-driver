<?php

namespace MongoDB;

interface Cursor extends ServerResult, \Iterator
{
    /**
     * @return integer Cursor identifier
     */
    function getId();

    /**
     * @param integer $batchSize
     */
    function setBatchSize($batchSize);
}
