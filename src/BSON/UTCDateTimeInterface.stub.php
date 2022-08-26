<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface UTCDateTimeInterface
{
    /** @tentative-return-type */
    public function toDateTime(): \DateTime;

    /** @tentative-return-type */
    public function __toString(): string;
}
