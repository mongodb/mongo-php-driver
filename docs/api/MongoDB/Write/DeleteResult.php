<?php

namespace MongoDB\Write;

interface DeleteResult extends WriteResult
{
    /**
     * @return integer
     */
    function getNumRemoved();
}
