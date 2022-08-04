<?php

/**
  * @generate-class-entries static
  * @generate-function-entries static
  */

namespace MongoDB\BSON;

final class Decimal128 implements Decimal128Interface, \JsonSerializable, Type, \Serializable
{
    final public function __construct(string $value = '') {}

    final public function __toString(): string {}

    public static function __set_state(array $properties): Decimal128 {}

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
