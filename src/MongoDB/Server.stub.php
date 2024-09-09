<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

/** @not-serializable */
final class Server
{
    /**
     * @var int
     * @cvalue PHONGO_SERVER_UNKNOWN
     */
    public const TYPE_UNKNOWN = UNKNOWN;

    /**
     * @var int
     * @cvalue PHONGO_SERVER_STANDALONE
     */
    public const TYPE_STANDALONE = UNKNOWN;

    /**
     * @var int
     * @cvalue PHONGO_SERVER_MONGOS
     */
    public const TYPE_MONGOS = UNKNOWN;

    /**
     * @var int
     * @cvalue PHONGO_SERVER_POSSIBLE_PRIMARY
     */
    public const TYPE_POSSIBLE_PRIMARY = UNKNOWN;

    /**
     * @var int
     * @cvalue PHONGO_SERVER_RS_PRIMARY
     */
    public const TYPE_RS_PRIMARY = UNKNOWN;

    /**
     * @var int
     * @cvalue PHONGO_SERVER_RS_SECONDARY
     */
    public const TYPE_RS_SECONDARY = UNKNOWN;

    /**
     * @var int
     * @cvalue PHONGO_SERVER_RS_ARBITER
     */
    public const TYPE_RS_ARBITER = UNKNOWN;

    /**
     * @var int
     * @cvalue PHONGO_SERVER_RS_OTHER
     */
    public const TYPE_RS_OTHER = UNKNOWN;

    /**
     * @var int
     * @cvalue PHONGO_SERVER_RS_GHOST
     */
    public const TYPE_RS_GHOST = UNKNOWN;

    /**
     * @var int
     * @cvalue PHONGO_SERVER_LOAD_BALANCER
     */
    public const TYPE_LOAD_BALANCER = UNKNOWN;

    final private function __construct() {}

    final public function executeBulkWrite(string $namespace, BulkWrite $bulkWrite, array|WriteConcern|null $options = null): WriteResult {}

    final public function executeCommand(string $db, Command $command, array|ReadPreference|null $options = null): Cursor {}

    final public function executeQuery(string $namespace, Query $query, array|ReadPreference|null $options = null): Cursor {}

    final public function executeReadCommand(string $db, Command $command, ?array $options = null): Cursor {}

    final public function executeReadWriteCommand(string $db, Command $command, ?array $options = null): Cursor {}

    final public function executeWriteCommand(string $db, Command $command, ?array $options = null): Cursor {}

    final public function getHost(): string {}

    final public function getInfo(): array {}

    final public function getLatency(): ?int {}

    final public function getPort(): int {}

    final public function getServerDescription(): ServerDescription {}

    final public function getTags(): array {}

    final public function getType(): int {}

    final public function isArbiter(): bool {}

    final public function isHidden(): bool {}

    final public function isPassive(): bool {}

    final public function isPrimary(): bool {}

    final public function isSecondary(): bool {}
}
