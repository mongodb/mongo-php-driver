<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Regex implements RegexInterface, \JsonSerializable, Type, \Serializable
{
    final public function __construct(string $pattern, string $flags = '') {}

    final public function getPattern(): string {}

    final public function getFlags(): string {}

    final public function __toString(): string {}

    final public static function __set_state(array $properties): Regex {}

    final public function serialize(): string {}

#if PHP_VERSION_ID >= 80000
    final public function unserialize(string $data): void {}
#else
    /** @param string $serialized */
    final public function unserialize($serialized): void {}
#endif

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

#if PHP_VERSION_ID >= 80000
    final public function jsonSerialize(): mixed {}
#else
    /** @return mixed */
    final public function jsonSerialize() {}
#endif
}
