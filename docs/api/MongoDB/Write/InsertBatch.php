<?php

namespace MongoDB\Write;

/**
 * Aggregates a collection of insert operations, to be executed in batches.
 */
final class InsertBatch implements WriteBatch
{
    private $documents;

    /**
     * Adds a new operation to be executed
     *
     * @param array|object $document Operation/document to add to the batch
     * @return InsertBatch
     */
    public function add($document)
    {
        $this->documents[] = $document;
    }

    /**
     * Returns how many items have been added to this batch
     *
     * @return int How many operations are total in this batch object
     */
    public function count()
    {
        return count($this->documents);
    }
}
