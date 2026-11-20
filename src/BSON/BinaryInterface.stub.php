<?php

/**
 * @generate-class-entries
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\BSON;

interface BinaryInterface
{
    public function getData(): string;

    public function getType(): int;

    public function __toString(): string;
}
