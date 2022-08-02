<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class Cursor implements \Iterator, CursorInterface
{
    final private function __construct() {}

#if PHP_VERSION_ID >= 80000
    public function current(): array|object|null {}
#else
    /** @return array|object|null */
    public function current() {}
#endif

    final public function getId(): CursorId {}

    final public function getServer(): Server {}

    final public function isDead(): bool {}

    public function key(): ?int {}

    public function next(): void {}

    public function rewind(): void {}

    final public function setTypeMap(array $typemap): void {}

    final public function toArray(): array {}

    public function valid(): bool {}
}
