<?php

/** @generate-function-entries */

namespace MongoDB\BSON
{
    final class UTCDateTime
    {
#if PHP_VERSION_ID >= 80000
        public final function __construct(int|string|float|\DateTime|\DateTimeImmutable|null $milliseconds = null) {}
#else
        /** @param int|string|float|\DateTime|\DateTimeImmutable|null $milliseconds */
        public final function __construct($milliseconds = null) {}
#endif

        public final function toDateTime(): \DateTime {}

        public final function __toString(): string {}

        public static function __set_state(array $properties): object {}

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
