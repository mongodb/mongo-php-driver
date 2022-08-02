<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Exception;

class RuntimeException extends \RuntimeException implements Exception
{
    /** @var bool */
    protected $errorLabels;

    final public function hasErrorLabel(string $errorLabel): bool {}
}
