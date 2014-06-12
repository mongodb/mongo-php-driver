<?php

namespace MongoDB\Write;

interface WriteResult extends \MongoDB\ServerResult
{
    /**
     * @return array Additional metadata for the operation(s) (e.g. timings)
     */
    function getInfo();

    /**
     * @return integer Number of operations that were executed
     */
    function getOpCount();

    /**
     * @return WriteError[]
     */
    function getWriteErrors();

    /**
     * @return WriteConcernError[]
     */
    function getWriteConcernErrors();
}
