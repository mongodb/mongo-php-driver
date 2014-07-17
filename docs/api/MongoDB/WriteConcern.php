<?php

namespace MongoDB;

/**
 * Value object for write concern used in issuing write operations.
 */
final class WriteConcern
{
    private $w;
    private $wtimeout;
    private $options;

    /**
     * Constructs a new ReadPreference
     *
     * @see http://docs.mongodb.org/manual/reference/write-concern/
     * @param integer|string $w        Number of required acknowledgements or a tag set
     * @param integer        $wtimeout Write concern timeout in milliseconds
     * @param array          $options  Additional options (e.g. "j", "fsync")
     */
    public function __construct($w, $wtimeout = 0, array $options = null)
    {
        // TODO: Eagerly merge "w" and "wtimeout" into $options or empty array
    }
}
