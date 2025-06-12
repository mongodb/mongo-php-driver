<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface Serializable extends Type
{
    public function bsonSerialize(): array|\stdClass|Document|PackedArray;
}
