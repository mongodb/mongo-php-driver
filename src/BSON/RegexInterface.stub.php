<?php

/**
 * @generate-class-entries
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\BSON;

interface RegexInterface
{
    public function getPattern(): string;

    public function getFlags(): string;

    public function __toString(): string;
}
