<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Exception;

final class BulkWriteCommandException extends ServerException
{
    private ?\MongoDB\BSON\Document $errorReply = null;

    private ?\MongoDB\Driver\BulkWriteCommandResult $partialResult = null;

    private array $writeErrors = [];

    private array $writeConcernErrors = [];

    final public function getErrorReply(): ?\MongoDB\BSON\Document {}

    final public function getPartialResult(): ?\MongoDB\Driver\BulkWriteCommandResult {}

    final public function getWriteErrors(): array {}

    final public function getWriteConcernErrors(): array {}
}
