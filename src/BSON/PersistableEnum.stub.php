<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

trait PersistableEnum
{
    public final function bsonSerialize(): array {}

    public final function bsonUnserialize(array $data): void {}
}
