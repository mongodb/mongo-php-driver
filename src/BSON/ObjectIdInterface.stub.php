<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface ObjectIdInterface
{
    public function getTimestamp(): int;

    public function __toString(): string;
}
