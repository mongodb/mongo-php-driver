<?php

namespace MongoDB\Write;

interface WriteResult
{
    /**
     * @see https://github.com/mongodb/specifications/blob/master/source/server_write_commands.rst#situational-fields
     * @return array Additional metadata for the operation(s) (e.g. lastOp)
     */
    function getInfo();

    /**
     * @return Server Server from which the result originated
     */
    function getServer();

    /**
     * @return WriteConcernError[]
     */
    function getWriteConcernErrors();

    /**
     * @return WriteError[]
     */
    function getWriteErrors();
}
