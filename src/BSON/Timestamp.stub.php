<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Timestamp implements TimestampInterface, \JsonSerializable, Type, \Serializable
{
#if PHP_VERSION_ID >= 80000
    public final function __construct(int|string $increment, int|string $timestamp) {}
#else
    /**
     * @param int|string $increment
     * @param int|string $timestamp
     */
    public final function __construct($increment, $timestamp) {}
#endif

    public final function getTimestamp(): int {}

    public final function getIncrement(): int {}

    public final function __toString(): string {}

    public static function __set_state(array $properties): Timestamp {}

    final public function serialize(): string {}

    /** @param string $serialized */
    final public function unserialize($serialized): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

#if PHP_VERSION_ID >= 80000
    final public function jsonSerialize(): mixed {}
#else
    /** @return mixed */
    final public function jsonSerialize() {}
#endif
}
