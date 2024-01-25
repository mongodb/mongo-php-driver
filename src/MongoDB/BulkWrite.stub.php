<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class BulkWrite implements \Countable
{
    final public function __construct(?array $options = null) {}

    public function count(): int {}

#if PHP_VERSION_ID >= 80000
    public function delete(array|object $filter, ?array $deleteOptions = null): void {}
#else
    /** @param array|object $filter */
    public function delete($filter, ?array $deleteOptions = null): void {}
#endif

#if PHP_VERSION_ID >= 80000
    final public function insert(array|object $document): mixed {}
#else
    /**
     * @param array|object $document
     * @return mixed
     */
    final public function insert($document) {}
#endif

#if PHP_VERSION_ID >= 80000
    public function update(array|object $filter, array|object $newObj, ?array $updateOptions = null): void {}
#else
    /**
     * @param array|object $filter
     * @param array|object $newObj
     */
    public function update($filter, $newObj, ?array $updateOptions = null): void {}
#endif
}
