<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class BSONArray implements \IteratorAggregate
{
    private function __construct() {}

    final static public function fromPHP(array $value): BSONArray {}

    final public function getIterator(): BSONIterator {}

    final public function toPHP(?array $typeMap = null): array {}

    final public function __toString(): string {}
}
