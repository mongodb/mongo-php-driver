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

    final public function __construct(string|int $w, ?int $wtimeout = null, ?bool $journal = null) {}

    final public function getJournal(): ?bool {}

    final public function getW(): string|int|null {}

    final public function getWtimeout(): int {}

    final public function isDefault(): bool {}

    final public static function __set_state(array $properties): WriteConcern {}

    final public function bsonSerialize(): \stdClass {}

    final public function serialize(): string {}

    final public function unserialize(string $data): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

}
