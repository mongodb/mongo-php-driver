<?php

/** @generate-function-entries */

namespace MongoDB\BSON
{
    interface Serializable
    {
#if PHP_VERSION_ID >= 80000
        /** @tentative-return-type */
        public function bsonSerialize(): array|object;
#else
        /** @return array|object */
        public function bsonSerialize();
#endif
    }
}
