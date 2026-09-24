<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface RegexInterface
{
    public function getPattern(): string;

    public function getFlags(): string;

    public function __toString(): string;
}
