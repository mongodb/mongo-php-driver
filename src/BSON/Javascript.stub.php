<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class Javascript implements JavascriptInterface, \JsonSerializable, Type, \Serializable
{
#if PHP_VERSION_ID >= 80000
    final public function __construct(string $code, array|object|null $scope = null) {}
#else
    /** @param array|object|null $scope */
    final public function __construct(string $code, $scope = null) {}
#endif

    final public static function __set_state(array $properties): Javascript {}

    final public function getCode(): string {}

    final public function getScope(): ?object {}

    final public function __toString(): string {}

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
