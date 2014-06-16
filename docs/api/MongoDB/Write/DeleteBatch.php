<?php

namespace MongoDB\Write;

/**
 * Aggregates a collection of delete operations, to be executed in batches.
 */
final class DeleteBatch implements WriteBatch
{
    private $documents;

    /**
     * @see WriteBatch::add()
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
