<?php

/** @generate-function-entries */

namespace MongoDB\BSON
{
    interface RegexInterface
    {
        /** @return string */
        public function getPattern();

        /** @return string */
        public function getFlags();

        /** @return string */
        public function __toString();
    }
}
