<?php

namespace MongoDB\Write;

interface WriteBatch extends \Countable
{
    /**
     * @param array|object Operation/document to add to the batch
     */
    function add($document);

    /**
     * @return array Array of operations/documents in the batch
     */
    function getDocuments();
}
