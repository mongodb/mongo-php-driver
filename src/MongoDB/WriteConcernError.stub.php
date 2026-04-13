<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
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
