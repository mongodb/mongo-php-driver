<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class PackedArray implements \IteratorAggregate, \Serializable, \ArrayAccess, Type, \Stringable
{
    private function __construct() {}

    final static public function fromPHP(array $value): PackedArray {}

    final public function get(int $index): mixed {}

    final public function getIterator(): Iterator {}

    final public function has(int $index): bool {}

    final public function toPHP(?array $typeMap = null): array|object {}

    public function offsetExists(mixed $offset): bool {}

    public function offsetGet(mixed $offset): mixed {}

    public function offsetSet(mixed $offset, mixed $value): void {}

    public function offsetUnset(mixed $offset): void {}

    final public function __toString(): string {}

    final public static function __set_state(array $properties): PackedArray {}

    final public function serialize(): string {}

    final public function unserialize(string $data): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}
}
