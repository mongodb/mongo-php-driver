<?php

/**
  * @generate-class-entries static
  * @generate-function-entries static
  */

namespace MongoDB\BSON;

final class Iterator implements \Iterator
{
    final private function __construct() {}

    final public function current(): Value {}

#if PHP_VERSION_ID >= 80000
    final public function key(): string|int {}
#else
    /** @return string|int */
    final public function key() {}
#endif

    final public function next(): void {}

    final public function rewind(): void {}

    final public function valid(): bool {}

    final public function __wakeup(): void {}
}
