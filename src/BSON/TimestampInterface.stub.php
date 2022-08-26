<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface TimestampInterface
{
    /** @tentative-return-type */
    public function getTimestamp(): int;

    /** @tentative-return-type */
    public function getIncrement(): int;

    /** @tentative-return-type */
    public function __toString(): string;
}
