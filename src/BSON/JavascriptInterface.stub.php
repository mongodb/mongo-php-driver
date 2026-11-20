<?php

/**
 * @generate-class-entries
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\BSON;

interface JavascriptInterface
{
    public function getCode(): string;

    public function getScope(): ?object;

    public function __toString(): string;
}
