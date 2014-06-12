<?php

namespace MongoDB\Write;

final class Upsert
{
    /**
     * @return mixed
     */
    function getId();

    /**
     * @return integer Batch index of the upsert
     */
    function getIndex();
}
