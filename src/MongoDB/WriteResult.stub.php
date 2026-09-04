<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver;

/** @not-serializable */
final class WriteResult
{
    public readonly int|null $insertedCount;

    public readonly int|null $matchedCount;

    public readonly int|null $modifiedCount;

    public readonly int|null $deletedCount;

    public readonly int|null $upsertedCount;

    public readonly Server $server;

    public readonly array $upsertedIds;

    public readonly array $writeErrors;

    public readonly WriteConcernError|null $writeConcernError;

    public readonly WriteConcern|null $writeConcern;

    public readonly array $errorReplies;

    final private function __construct() {}

    final public function getInsertedCount(): int {}

    final public function getMatchedCount(): int {}

    final public function getModifiedCount(): int {}

    final public function getDeletedCount(): int {}

    final public function getUpsertedCount(): int {}

    final public function getServer(): Server {}

    final public function getUpsertedIds(): array {}

    final public function getWriteConcernError(): ?WriteConcernError {}

    final public function getWriteErrors(): array {}

    final public function getErrorReplies(): array {}

    final public function isAcknowledged(): bool {}
}
