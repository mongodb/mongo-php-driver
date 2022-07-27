<?php

/**
  * @generate-class-entries static
  * @generate-function-entries static
  */

namespace MongoDB\BSON
{
    final class DBPointer implements JsonSerializable, Type, \Serializable
    {
        final private function __construct() {}

        final public function __toString(): string {}

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
}
