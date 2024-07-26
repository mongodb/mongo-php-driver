<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Document implements \IteratorAggregate, \Serializable, \ArrayAccess, Type
{
    private function __construct() {}

    final static public function fromBSON(string $bson): Document {}

    final static public function fromJSON(string $json): Document {}

    final static public function fromPHP(array|object $value): Document {}

    final public function get(string $key): mixed {}

    final public function getIterator(): Iterator {}

    final public function has(string $key): bool {}

    final public function toPHP(?array $typeMap = null): array|object {}

    final public function toCanonicalExtendedJSON(): string {}

    final public function toRelaxedExtendedJSON(): string {}

    public function offsetExists(mixed $offset): bool {}

    public function offsetGet(mixed $offset): mixed {}

    public function offsetSet(mixed $offset, mixed $value): void {}

    public function offsetUnset(mixed $offset): void {}

    final public function __toString(): string {}

    final public static function __set_state(array $properties): Document {}

    final public function serialize(): string {}

    final public function unserialize(string $data): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}
}
