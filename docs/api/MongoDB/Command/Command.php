<?php

namespace MongoDB\Command;

/**
 * Value object for a database command document.
 */
final class Command
{
    private $document;

    /**
     * @param array|object $document Command document
     */
    public function __construct($document)
    {
        $this->document = $document;
    }
}
