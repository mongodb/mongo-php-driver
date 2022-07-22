<?php

/** @generate-function-entries */

namespace MongoDB\BSON
{
    interface TimestampInterface
    {
        /** @return int */
        public function getTimestamp();

        /** @return int */
        public function getIncrement();

        /** @return string */
        public function __toString();
    }
}
