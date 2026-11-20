<?php

/**
 * @generate-class-entries
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\BSON;

interface Serializable extends Type
{
    public function bsonSerialize(): array|\stdClass|Document|PackedArray;
}
