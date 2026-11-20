<?php

/**
 * @generate-class-entries
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\BSON;

interface Unserializable
{
    public function bsonUnserialize(array $data): void;
}
