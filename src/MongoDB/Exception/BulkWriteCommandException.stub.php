<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Exception;

class BulkWriteCommandException extends ServerException
{
    protected \MongoDB\Driver\BulkWriteCommandResult $bulkWriteCommandResult;

    final public function getBulkWriteCommandResult(): \MongoDB\Driver\BulkWriteCommandResult {}
}
