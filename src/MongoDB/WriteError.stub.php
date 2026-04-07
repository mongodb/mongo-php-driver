<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

/** @not-serializable */
final class WriteError
{
    public readonly string $message;

    public readonly int $code;

    public readonly int $index;

    public readonly object|null $info;

    final private function __construct() {}

    final public function getCode(): int {}

    final public function getIndex(): int {}

    final public function getInfo(): ?object {}

    final public function getMessage(): string {}
}
