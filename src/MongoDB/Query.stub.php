<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class Query
{
#if PHP_VERSION_ID >= 80000
    final public function __construct(array|object $filter, ?array $queryOptions = null) {}
#else
    /** @param array|object $filter */
    final public function __construct($filter, ?array $queryOptions = null) {}
#endif
}
