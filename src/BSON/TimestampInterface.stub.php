<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface TimestampInterface
{
    public function getTimestamp(): int;

    public function getIncrement(): int;

    public function __toString(): string;
}
