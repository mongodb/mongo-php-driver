<?php

/**
  * @generate-class-entries static
  * @generate-function-entries
  */

namespace MongoDB\BSON
{
    interface Serializable extends Type
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
