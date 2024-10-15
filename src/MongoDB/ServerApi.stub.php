<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class ServerApi implements \MongoDB\BSON\Serializable, \Serializable
{
    /** @var string */
    public const V1 = '1';

    final public function __construct(string $version, ?bool $strict = null, ?bool $deprecationErrors = null) {}

    final public static function __set_state(array $properties): ServerApi {}

    final public function bsonSerialize(): \stdClass {}

    final public function serialize(): string {}

    final public function unserialize(string $data): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}
}
