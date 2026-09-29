<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Regex implements RegexInterface, \JsonSerializable, Type, \Stringable
{
    final public function __construct(string $pattern, string $flags = '') {}

    final public function getPattern(): string {}

    final public function getFlags(): string {}

    final public function __toString(): string {}

    final public static function __set_state(array $properties): Regex {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

    final public function jsonSerialize(): mixed {}
}
