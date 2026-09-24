<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\BSON;

interface Persistable extends Serializable, Unserializable
{
    public function bsonSerialize(): array|\stdClass|Document;
}
