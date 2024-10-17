<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

/** @not-serializable */
final class Cursor implements \Iterator, CursorInterface
{
    final private function __construct() {}

    public function current(): array|object|null {}

    /** @tentative-return-type */
    final public function getId(bool $asInt64 = false): CursorId|\MongoDB\BSON\Int64 {}

    final public function getServer(): Server {}

    final public function isDead(): bool {}

    public function key(): ?int {}

    public function next(): void {}

    public function rewind(): void {}

    final public function setTypeMap(array $typemap): void {}

    final public function toArray(): array {}

    public function valid(): bool {}
}
