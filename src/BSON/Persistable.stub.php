<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface Persistable extends Serializable, Unserializable
{
#if PHP_VERSION_ID >= 80000
    /** @tentative-return-type */
    public function bsonSerialize(): array|\stdClass|Document;
#else
    /** @return array|\stdClass|Document */
    public function bsonSerialize();
#endif
}
