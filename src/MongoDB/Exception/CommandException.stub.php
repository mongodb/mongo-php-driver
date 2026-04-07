<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Exception;

class CommandException extends ServerException
{
    public readonly object $resultDocument;

    final public function getResultDocument(): object {}
}
