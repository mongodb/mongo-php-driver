<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface UTCDateTimeInterface
{
    public function toDateTime(): \DateTime;

    public function __toString(): string;
}
