<?php

namespace MongoDB\Write;

interface WriteBatch extends \Countable
{
    /**
     * Adds a new operation to be executed
     *
     * @param array|object $document Operation/document to add to the batch
     * @return DeleteBatch
     */
    function add($document);
}
