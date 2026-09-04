<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Exception;

class BulkWriteException extends ServerException
{
    public readonly \MongoDB\Driver\WriteResult $writeResult;

    final public function getWriteResult(): \MongoDB\Driver\WriteResult {}
}
