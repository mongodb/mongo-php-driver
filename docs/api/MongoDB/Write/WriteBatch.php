<?php

namespace MongoDB\Write;

interface WriteBatch extends \Countable
{
    /**
     * @param array|object $document Operation/document to add to the batch
     */
    function add($document);
}
