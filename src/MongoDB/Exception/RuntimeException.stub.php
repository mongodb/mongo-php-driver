<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Exception;

class RuntimeException extends \RuntimeException implements Exception
{
    /** @var bool */
    protected $errorLabels;

    final public function hasErrorLabel(string $errorLabel): bool {}
}
