<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface JavascriptInterface
{
    public function getCode(): string;

    public function getScope(): ?object;

    public function __toString(): string;
}
