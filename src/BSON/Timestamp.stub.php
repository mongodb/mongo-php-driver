<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Timestamp implements TimestampInterface, \JsonSerializable, Type, \Serializable
{
    final public function __construct(int|string $increment, int|string $timestamp) {}

    final public function getTimestamp(): int {}

    final public function getIncrement(): int {}

    final public function __toString(): string {}

    final public static function __set_state(array $properties): Timestamp {}

    final public function serialize(): string {}

    final public function unserialize(string $data): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

    final public function jsonSerialize(): mixed {}
}
