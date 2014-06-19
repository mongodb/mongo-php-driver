<?php

namespace MongoDB\Write;

/**
 * Result returned by Server and Manager executeWrite() methods processing a
 * DeleteBatch.
 *
 * This class may be constructed internally if it will encapsulate a libmongoc
 * data structure.
 */
final class DeleteResult implements WriteResult
{
    /**
     * Returns the Number of documents that got removed
     *
     * @return integer
     */
    public function getNumRemoved() {}

    /**
     * Returns metadata about the operation, see https://github.com/mongodb/specifications/blob/master/source/server_write_commands.rst#situational-fields
     *
     * @return array Additional metadata for the operation(s) (e.g. lastOp)
     */
    public function getInfo() {}

    /**
     * Returns the Server object that this result originated
     *
     * @return Server Server from which the result originated
     */
    public function getServer() {}

    /**
     * Returns all WriteConcern Errors that occurred
     *
     * @return WriteConcernError[]
     */
    public function getWriteConcernErrors() {}

    /**
     * Returns all Write Errors that occurred
     *
     * @return WriteError[]
     */
    public function getWriteErrors() {}
}
