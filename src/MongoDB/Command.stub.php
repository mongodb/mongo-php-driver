<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class Command
{
#if PHP_VERSION_ID >= 80000
    final public function __construct(array|object $document, ?array $commandOptions = null) {}
#else
    /** @param array|object $document */
    final public function __construct($document, ?array $commandOptions = null) {}
#endif
}
