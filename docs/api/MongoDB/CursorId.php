<?php

namespace MongoDB;

/**
 * Value object for the 64-bit cursor identifier.
 *
 * This is useful for compatibility with 32-bit platforms, and also allows
 * Cursor constructors to type-hint against a class.
 */
final class CursorId
{
    // $id is an internal uint64_t value instead of a class property

    /**
     * @param integer|string $id
     */
    public function __construct($id)
    {
        // Set internal uint64_t value
    }

    /**
     * @return string
     */
    public function __toString()
    {
        /* Return string representation of internal uint64_t value.
         *
         * This can be used to compare two CursorIds on a 32-bit platform. Is
         * this functionality necessary, or should we do without any "getter"
         * method?
         */
    }
}
