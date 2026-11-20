<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Exception;

class CommandException extends ServerException
{
    public readonly object $resultDocument;

    final public function getResultDocument(): object {}
}
