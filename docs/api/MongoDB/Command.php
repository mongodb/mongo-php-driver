<?php

namespace MongoDB;

/**
 * Value object for a database command document.
 */
final class Command
{
    private $document;

    /**
     * Constructs a new Command
     *
     * @param array|object $document Command document
     */
    public function __construct($document)
    {
        $this->document = $document;
    }
}
