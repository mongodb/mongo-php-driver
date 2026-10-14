<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\BSON;

final class UTCDateTime implements UTCDateTimeInterface, \JsonSerializable, Type, \Stringable
{
    final public function __construct(int|\DateTimeInterface|Int64|null $milliseconds = null) {}

    final public function toDateTime(): \DateTime {}

    final public function toDateTimeImmutable(): \DateTimeImmutable {}

    final public function __toString(): string {}

    final public static function __set_state(array $properties): UTCDateTime {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

    final public function jsonSerialize(): mixed {}
}
