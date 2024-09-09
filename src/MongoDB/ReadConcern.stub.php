<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class ReadConcern implements \MongoDB\BSON\Serializable, \Serializable
{
    /**
     * @var string
     * @cvalue MONGOC_READ_CONCERN_LEVEL_LINEARIZABLE
     */
    public const LINEARIZABLE = UNKNOWN;

    /**
     * @var string
     * @cvalue MONGOC_READ_CONCERN_LEVEL_LOCAL
     */
    public const LOCAL = UNKNOWN;

    /**
     * @var string
     * @cvalue MONGOC_READ_CONCERN_LEVEL_MAJORITY
     */
    public const MAJORITY = UNKNOWN;

    /**
     * @var string
     * @cvalue MONGOC_READ_CONCERN_LEVEL_AVAILABLE
     */
    public const AVAILABLE = UNKNOWN;

    /**
     * @var string
     * @cvalue MONGOC_READ_CONCERN_LEVEL_SNAPSHOT
     */
    public const SNAPSHOT = UNKNOWN;

    final public function __construct(?string $level = null) {}

    final public function getLevel(): ?string {}

    final public function isDefault(): bool {}

    final public static function __set_state(array $properties): ReadConcern {}

    final public function bsonSerialize(): \stdClass {}

    final public function serialize(): string {}

    final public function unserialize(string $data): void {}

    final public function __unserialize(array $data): void {}

    final public function __serialize(): array {}

}
