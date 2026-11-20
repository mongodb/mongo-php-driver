<?php

/**
 * @generate-class-entries
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\BSON;

interface Persistable extends Serializable, Unserializable
{
    public function bsonSerialize(): array|\stdClass|Document;
}
