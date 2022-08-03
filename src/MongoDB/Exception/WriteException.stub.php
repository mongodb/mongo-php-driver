<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Exception;

abstract class WriteException extends RuntimeException
{
    /** @var \MongoDB\Driver\WriteResult */
    protected $writeResult;

    final public function getWriteResult(): \MongoDB\Driver\WriteResult {}
}
