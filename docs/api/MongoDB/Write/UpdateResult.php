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
     * @return integer
     */
    public function getNumMatched() {}

    /**
     * @return integer
     */
    public function getNumModified() {}

    /**
     * @return integer
     */
    public function getNumUpserted() {}

    /**
     * @return GeneratedId[]
     */
    public function getUpserts()
    {
        /* Return an array of identifiers upserted by the server. Each
         * GeneratedId has a batch index and the ID value.
         */
    }

    /**
     * @see WriteResult::getInfo()
     */
    public function getInfo() {}

    /**
     * @see WriteResult::getServer()
     */
    public function getServer() {}

    /**
     * @see WriteResult::getWriteConcernErrors()
     */
    public function getWriteConcernErrors() {}

    /**
     * @see WriteResult::getWriteErrors()
     */
    public function getWriteErrors() {}
}
