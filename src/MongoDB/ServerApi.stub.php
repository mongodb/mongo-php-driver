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

#if PHP_VERSION_ID >= 80000
    final public function bsonSerialize(): array|object {}
#else
    /** @return array|object */
    final public function bsonSerialize() {}
#endif

    final public function serialize(): string {}

    /** @param string $serialized */
    final public function unserialize($serialized): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}
}
