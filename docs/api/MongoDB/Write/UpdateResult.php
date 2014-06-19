<?php

namespace MongoDB\Write;

/**
 * Result returned by Server and Manager executeWrite() methods processing an
 * UpdateBatch.
 *
 * This class may be constructed internally if it will encapsulate a libmongoc
 * data structure.
 */
final class UpdateResult implements WriteResult
{
    /**
     * Returns the number of documents matching the criteria
     *
     * @return integer
     */
    public function getNumMatched() {}

    /**
     * Returns the number of documents that got physically modified
     *
     * @return integer
     */
    public function getNumModified() {}

    /**
     * Returns the number of new documentes
     *
     * @return integer
     */
    public function getNumUpserted() {}

    /**
     * Returns the GeneratedIds of the upserted documents
     *
     * @return GeneratedId[]
     */
    public function getUpserts()
    {
        /* Return an array of identifiers upserted by the server. Each
         * GeneratedId has a batch index and the ID value.
         */
    }

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
