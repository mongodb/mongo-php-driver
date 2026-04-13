<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\BSON;

final class Int64 implements \JsonSerializable, Type, \Stringable
{
    final public function __construct(int|string $value) {}

    final public function __toString(): string {}

    final public static function __set_state(array $properties): Int64 {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

    final public function jsonSerialize(): mixed {}
}
