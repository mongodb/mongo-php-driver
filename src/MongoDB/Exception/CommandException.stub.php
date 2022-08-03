<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Exception;

class CommandException extends ServerException
{
    /** @var object */
    protected $resultDocument;

    final public function getResultDocument(): object {}
}
