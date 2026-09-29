<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface BinaryInterface
{
    public function getData(): string;

    public function getType(): int;

    public function __toString(): string;
}
