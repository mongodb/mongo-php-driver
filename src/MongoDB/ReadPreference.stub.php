<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class ReadPreference implements \MongoDB\BSON\Serializable, \Serializable
{
    /**
     * @var int
     * @cvalue MONGOC_READ_PRIMARY
     * @deprecated
     */
    public const RP_PRIMARY = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_READ_PRIMARY_PREFERRED
     * @deprecated
     */
    public const RP_PRIMARY_PREFERRED = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_READ_SECONDARY
     * @deprecated
     */
    public const RP_SECONDARY = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_READ_SECONDARY_PREFERRED
     * @deprecated
     */
    public const RP_SECONDARY_PREFERRED = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_READ_NEAREST
     * @deprecated
     */
    public const RP_NEAREST = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_READ_PRIMARY
     */
    public const PRIMARY = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_READ_PRIMARY_PREFERRED
     */
    public const PRIMARY_PREFERRED = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_READ_SECONDARY
     */
    public const SECONDARY = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_READ_SECONDARY_PREFERRED
     */
    public const SECONDARY_PREFERRED = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_READ_NEAREST
     */
    public const NEAREST = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_NO_MAX_STALENESS
     */
    public const NO_MAX_STALENESS = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_SMALLEST_MAX_STALENESS_SECONDS
     */
    public const SMALLEST_MAX_STALENESS_SECONDS = UNKNOWN;

    final public function __construct(string|int $mode, ?array $tagSets = null, ?array $options = null) {}

    final public function getHedge(): ?object {}

    final public function getMaxStalenessSeconds(): int {}

    /** @deprecated */
    final public function getMode(): int {}

    final public function getModeString(): string {}

    final public function getTagSets(): array {}

    final public static function __set_state(array $properties): ReadPreference {}

    final public function bsonSerialize(): \stdClass {}

    final public function serialize(): string {}

    final public function unserialize(string $data): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}
}
