<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;
final class DBPointer implements \JsonSerializable, Type, \Serializable, \Stringable
{
    final private function __construct() {}

    final public static function __set_state(array $properties): DBPointer {}

    final public function __toString(): string {}

    final public function serialize(): string {}

    final public function unserialize(string $data): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

    final public function jsonSerialize(): mixed {}
}
