<?php

namespace MongoDB\Write;

final class UpdateBatch extends WriteBatch
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

    /**
     * @see WriteBatch::getDocuments()
     */
    public function getDocuments()
    {
        return $this->documents;
    }
}
