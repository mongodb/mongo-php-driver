<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\BSON;

final class Undefined implements \JsonSerializable, Type, \Stringable
{
    final private function __construct() {}

    final public function __toString(): string {}

    final public static function __set_state(array $properties): Undefined {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

    final public function jsonSerialize(): mixed {}
}
