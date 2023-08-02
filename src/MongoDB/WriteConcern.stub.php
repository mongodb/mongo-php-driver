<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class WriteConcern implements \MongoDB\BSON\Serializable, \Serializable
{
    /**
     * @var string
     * @cvalue PHONGO_WRITE_CONCERN_W_MAJORITY
     */
    public const MAJORITY = UNKNOWN;

#if PHP_VERSION_ID >= 80000
    final public function __construct(string|int $w, ?int $wtimeout = null, ?bool $journal = null) {}
#else
    /** @param string|int $w */
    final public function __construct($w, ?int $wtimeout = null, ?bool $journal = null) {}
#endif

    final public function getJournal(): ?bool {}

#if PHP_VERSION_ID >= 80000
    final public function getW(): string|int|null {}
#else
    /** @return string|int|null */
    final public function getW() {}
#endif

    final public function getWtimeout(): int {}

    final public function isDefault(): bool {}

    final public static function __set_state(array $properties): WriteConcern {}

    final public function bsonSerialize(): \stdClass {}

    final public function serialize(): string {}

#if PHP_VERSION_ID >= 80000
    final public function unserialize(string $data): void {}
#else
    /** @param string $serialized */
    final public function unserialize($serialized): void {}
#endif

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

}
