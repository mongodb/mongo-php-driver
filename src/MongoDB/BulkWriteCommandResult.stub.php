<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

/** @not-serializable */
final class BulkWriteCommandResult
{
    final private function __construct() {}

    final public function getInsertedCount(): int {}

    final public function getMatchedCount(): int {}

    final public function getModifiedCount(): int {}

    final public function getUpsertedCount(): int {}

    final public function getDeletedCount(): int {}

    final public function getServer(): Server {}

    final public function getInsertResults(): ?\MongoDB\BSON\Document {}
    
    final public function getUpdateResults(): ?\MongoDB\BSON\Document {}
    
    final public function getDeleteResults(): ?\MongoDB\BSON\Document {}

    final public function getWriteErrors(): ?\MongoDB\BSON\Document {}

    final public function getWriteConcernErrors(): ?\MongoDB\BSON\PackedArray {}

    final public function getErrorReply(): ?\MongoDB\BSON\Document {}

    final public function isAcknowledged(): bool {}
}
