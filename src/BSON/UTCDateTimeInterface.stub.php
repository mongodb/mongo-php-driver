<?php

/**
 * @generate-class-entries
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\BSON;

interface UTCDateTimeInterface
{
    public function toDateTime(): \DateTime;

    public function toDateTimeImmutable(): \DateTimeImmutable;

    public function __toString(): string;
}
