<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

/** @not-serializable */
final class WriteConcernError
{
    final private function __construct() {}

    final public function getCode(): int {}

    final public function getInfo(): ?object {}

    final public function getMessage(): string {}
}
