<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class BSONDocument implements \IteratorAggregate
{
    private function __construct() {}

    final static public function fromJSON(string $json): BSONDocument {}

#if PHP_VERSION_ID >= 80000
    final static public function fromPHP(mixed $value): BSONDocument {}
#else
    /** @param mixed $value */
    final static public function fromPHP($value): BSONDocument {}
#endif

    final public function getIterator(): BSONIterator {}

#if PHP_VERSION_ID >= 80000
    final public function toPHP(?array $typeMap = null): mixed {}
#else
    /** @return mixed */
    final public function toPHP(?array $typeMap = null) {}
#endif

    final public function toCanonicalExtendedJSON(): string {}

    final public function toRelaxedExtendedJSON(): string {}

    final public function __toString(): string {}
}
