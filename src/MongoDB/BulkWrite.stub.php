<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver;

/** @not-serializable */
final class BulkWrite implements \Countable
{
    final public function __construct(?array $options = null) {}

    public function count(): int {}

    public function delete(array|object $filter, ?array $deleteOptions = null): void {}

    final public function insert(array|object $document): mixed {}

    public function update(array|object $filter, array|object $newObj, ?array $updateOptions = null): void {}
}
