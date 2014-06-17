<?php

namespace MongoDB\Write;

/**
 * Aggregates a collection of insert operations, to be executed in batches.
 */
final class InsertBatch implements WriteBatch
{
    private $documents;

    /**
     * @param array|object $document Operation/document to add to the batch
     */
    public function add($document)
    {
        $this->documents[] = $document;
    }

    /**
     * @see Countable::count()
     */
    public function count()
    {
        return count($this->documents);
    }
}
