<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Decimal128 implements Decimal128Interface, \JsonSerializable, Type, \Stringable
{
    final public function __construct(string $value) {}

    final public function __toString(): string {}

    final public static function __set_state(array $properties): Decimal128 {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

    final public function jsonSerialize(): mixed {}
}
