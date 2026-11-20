<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver;

final class WriteConcern implements \MongoDB\BSON\Serializable
{
    /**
     * @var string
     * @cvalue PHONGO_WRITE_CONCERN_W_MAJORITY
     */
    public const MAJORITY = UNKNOWN;

    public readonly string|int|null $w;

    public readonly bool|null $j;

    public readonly int $wtimeout;

    final public function __construct(string|int $w, ?int $wtimeout = null, ?bool $journal = null) {}

    final public function getJournal(): ?bool {}

    final public function getW(): string|int|null {}

    final public function getWtimeout(): int {}

    final public function isDefault(): bool {}

    final public static function __set_state(array $properties): WriteConcern {}

    final public function bsonSerialize(): \stdClass {}

    final public function __unserialize(array $data): void {}
}
