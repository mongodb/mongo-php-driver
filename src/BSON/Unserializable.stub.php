<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface Unserializable
{
    public function bsonUnserialize(array $data): void;
}
