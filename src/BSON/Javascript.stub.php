<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Javascript implements JavascriptInterface, \JsonSerializable, Type, \Stringable
{
    final public function __construct(string $code, array|object|null $scope = null) {}

    final public static function __set_state(array $properties): Javascript {}

    final public function getCode(): string {}

    final public function getScope(): ?object {}

    final public function __toString(): string {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

    final public function jsonSerialize(): mixed {}
}
