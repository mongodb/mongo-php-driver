<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Exception;

class BulkWriteException extends ServerException
{
    public readonly \MongoDB\Driver\WriteResult $writeResult;

    final public function getWriteResult(): \MongoDB\Driver\WriteResult {}
}
