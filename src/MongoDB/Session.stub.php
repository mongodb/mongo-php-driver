<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

/** @not-serializable */
final class Session
{
    /**
     * @var string
     * @cvalue PHONGO_TRANSACTION_NONE
     */
    public const TRANSACTION_NONE = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_TRANSACTION_STARTING
     */
    public const TRANSACTION_STARTING = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_TRANSACTION_IN_PROGRESS
     */
    public const TRANSACTION_IN_PROGRESS = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_TRANSACTION_COMMITTED
     */
    public const TRANSACTION_COMMITTED = UNKNOWN;

    /**
     * @var string
     * @cvalue PHONGO_TRANSACTION_ABORTED
     */
    public const TRANSACTION_ABORTED = UNKNOWN;

    final private function __construct() {}

    final public function abortTransaction(): void {}

    final public function advanceClusterTime(array|object $clusterTime): void {}

    final public function advanceOperationTime(\MongoDB\BSON\TimestampInterface $operationTime): void {}

    final public function commitTransaction(): void {}

    final public function endSession(): void {}

    final public function getClusterTime(): ?object {}

    final public function getLogicalSessionId(): object {}

    final public function getOperationTime(): ?\MongoDB\BSON\Timestamp {}

    final public function getServer(): ?Server {}

    final public function getTransactionOptions(): ?array {}

    final public function getTransactionState(): string {}

    final public function isDirty(): bool {}

    final public function isInTransaction(): bool {}

    final public function startTransaction(?array $options = null): void {}
}
