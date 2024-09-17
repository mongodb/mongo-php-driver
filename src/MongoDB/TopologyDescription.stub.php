<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

/** @not-serializable */
final class TopologyDescription
{
    /**
     * @var string
     * @cvalue PHONGO_TOPOLOGY_UNKNOWN
     */
    public const TYPE_UNKNOWN = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_TOPOLOGY_SINGLE
     */
    public const TYPE_SINGLE = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_TOPOLOGY_SHARDED
     */
    public const TYPE_SHARDED = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_TOPOLOGY_REPLICA_SET_NO_PRIMARY
     */
    public const TYPE_REPLICA_SET_NO_PRIMARY = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_TOPOLOGY_REPLICA_SET_WITH_PRIMARY
     */
    public const TYPE_REPLICA_SET_WITH_PRIMARY = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_TOPOLOGY_LOAD_BALANCED
     */
    public const TYPE_LOAD_BALANCED = UNKNOWN;

    final private function __construct() {}

    final public function getServers(): array {}

    final public function getType(): string {}

    final public function hasReadableServer(?ReadPreference $readPreference = null): bool {}

    final public function hasWritableServer(): bool {}
}
