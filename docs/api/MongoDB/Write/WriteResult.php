<?php

namespace MongoDB\Write;

interface WriteResult
{
    /**
     * Returns metadata about the operation, see https://github.com/mongodb/specifications/blob/master/source/server_write_commands.rst#situational-fields
     *
     * @return array Additional metadata for the operation(s) (e.g. lastOp)
     */
    function getInfo();

    /**
     * Returns the Server object that this result originated
     *
     * @return Server Server from which the result originated
     */
    function getServer();

    /**
     * Returns all WriteConcern Errors that occurred
     *
     * @return WriteConcernError[]
     */
    function getWriteConcernErrors();

    /**
     * Returns all Write Errors that occurred
     *
     * @return WriteError[]
     */
    function getWriteErrors();
}
