<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Document implements \IteratorAggregate
{
    private function __construct() {}

    final static public function fromBSONString(string $bson): Document {}

    final static public function fromJSON(string $json): Document {}

#if PHP_VERSION_ID >= 80000
    final static public function fromPHP(mixed $value): Document {}
#else
    /** @param mixed $value */
    final static public function fromPHP($value): Document {}
#endif

#if PHP_VERSION_ID >= 80000
    final public function get(string $key): mixed {}
#else
    /** @return mixed */
    final public function get(string $key) {}
#endif

    final public function getIterator(): Iterator {}

    final public function has(string $key): bool {}

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