<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class ArrayList implements \IteratorAggregate, \Serializable
{
    private function __construct() {}

    final static public function fromPHP(array $value): ArrayList {}

#if PHP_VERSION_ID >= 80000
    final public function get(int $index): mixed {}
#else
    /** @return mixed */
    final public function get(int $index) {}
#endif

    final public function getIterator(): Iterator {}

    final public function has(int $index): bool {}

#if PHP_VERSION_ID >= 80000
    final public function toPHP(?array $typeMap = null): array|object {}
#else
    /** @return array|object */
    final public function toPHP(?array $typeMap = null) {}
#endif

    final public function __toString(): string {}

    final public static function __set_state(array $properties): ArrayList {}

    final public function serialize(): string {}

    /** @param string $serialized */
    final public function unserialize($serialized): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}
}
