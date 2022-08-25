<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface RegexInterface
{
    /** @tentative-return-type */
    public function getPattern(): string;

    /** @tentative-return-type */
    public function getFlags(): string;

    /** @tentative-return-type */
    public function __toString(): string;
}
