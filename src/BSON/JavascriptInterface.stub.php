<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface JavascriptInterface
{
    /** @tentative-return-type */
    public function getCode(): string;

    /** @tentative-return-type */
    public function getScope(): ?object;

    /** @tentative-return-type */
    public function __toString(): string;
}
