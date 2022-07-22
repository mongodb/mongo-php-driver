<?php

/** @generate-function-entries */

namespace MongoDB\BSON
{
    interface BinaryInterface
    {
        /** @return string */
        public function getData();

        /** @return int */
        public function getType();

        /** @return string */
        public function __toString();
    }
}
