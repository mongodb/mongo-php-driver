<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Exception;

final class ConnectionTimeoutException extends ConnectionException
{
    /** @var \MongoDB\Driver\WriteResult */
    protected $writeResult;

    final public function getWriteResult(): \MongoDB\Driver\WriteResult {}
}
