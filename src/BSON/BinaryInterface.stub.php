<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface BinaryInterface
{
    /** @tentative-return-type */
    public function getData(): string;

    /** @tentative-return-type */
    public function getType(): int;

    public function __toString(): string;
}
