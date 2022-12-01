<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class ArrayList implements \IteratorAggregate
{
    private function __construct() {}

    final static public function fromPHP(array $value): ArrayList {}

#if PHP_VERSION_ID >= 80000
    final public function get(int $index): mixed {}
#else
    /** @return mixed */
    final public function get(int $index) {}
#endif

    final public function getIterator(): BSONIterator {}

    final public function has(int $index): bool {}

    final public function toPHP(?array $typeMap = null): array {}

    final public function __toString(): string {}
}
