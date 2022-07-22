<?php

/** @generate-function-entries */

namespace MongoDB\BSON
{
    interface Unserializable
    {
        /** @tentative-return-type */
        public function bsonUnserialize(array $data): void;
    }
}
