<?php

/** @generate-function-entries */

namespace MongoDB\BSON
{
    final class Int64
    {
        private final function __construct() {}

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
