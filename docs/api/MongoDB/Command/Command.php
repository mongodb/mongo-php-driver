<?php

namespace MongoDB\Command;

/**
 * Value object for a database command document.
 */
final class Command
{
    private $document;

    /**
     * Constructs a new MongoDB Command
     *
     * @param array|object $document Command document
     */
    public function __construct($document)
    {
        $this->document = $document;
    }
}
