<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class PackedArray implements \IteratorAggregate, \Serializable, \ArrayAccess, Type
{
    private function __construct() {}

    final static public function fromPHP(array $value): PackedArray {}

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

#if PHP_VERSION_ID >= 80000
    public function offsetExists(mixed $key): bool {}
# else
    /** @param mixed $key */
    public function offsetExists($key): bool {}
# endif

#if PHP_VERSION_ID >= 80000
    public function offsetGet(mixed $key): mixed {}
# else
    /**
     * @param mixed $key
     * @return mixed
     */
    public function offsetGet($key) {}
# endif

#if PHP_VERSION_ID >= 80000
    public function offsetSet(mixed $key, mixed $value): void {}
# else
    /**
     * @param mixed $key
     * @param mixed $value
     */
    public function offsetSet($key, $value): void {}
# endif

#if PHP_VERSION_ID >= 80000
    public function offsetUnset(mixed $key): void {}
# else
    /** @param mixed $key */
    public function offsetUnset($key): void {}
# endif

    final public function __toString(): string {}

    final public static function __set_state(array $properties): PackedArray {}

    final public function serialize(): string {}

#if PHP_VERSION_ID >= 80000
    final public function unserialize(string $data): void {}
#else
    /** @param string $serialized */
    final public function unserialize($serialized): void {}
#endif

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}
}
