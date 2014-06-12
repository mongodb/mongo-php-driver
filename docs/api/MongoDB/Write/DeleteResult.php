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
     * @return integer
     */
    public function getNumRemoved() {}

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
