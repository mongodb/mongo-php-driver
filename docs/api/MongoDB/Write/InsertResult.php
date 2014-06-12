<?php

namespace MongoDB\Write;

interface InsertResult extends WriteResult
{
    /**
     * @return integer
     */
    function getNumInserted();
}
