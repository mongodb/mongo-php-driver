<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface Serializable extends Type
{
#if PHP_VERSION_ID >= 80000
    /** @tentative-return-type */
    public function bsonSerialize(): array|\stdClass|Document|PackedArray;
#else
    /** @return array|\stdClass|Document|PackedArray */
    public function bsonSerialize();
#endif
}
