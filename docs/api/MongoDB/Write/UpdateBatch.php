<?php

namespace MongoDB\Write;

/**
 * Aggregates a collection of update operations, to be executed in batches.
 */
final class UpdateBatch implements WriteBatch
{
    private $documents;
    private $writeOptions;

    /**
     * @param array $writeOptions Ordering and write concern options
     */
    public function __construct(array $writeOptions)
    {
        $this->writeOptions = $writeOptions;
    }

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
