<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

trait PersistableEnum
{
    final public function bsonSerialize(): array {}

    final public function bsonUnserialize(array $data): void {}
}
