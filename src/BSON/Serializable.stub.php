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
    public function bsonSerialize(): array|\stdClass;
#else
    /** @return array|\stdClass */
    public function bsonSerialize();
#endif
}
