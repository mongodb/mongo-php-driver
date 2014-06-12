<?php

namespace MongoDB\Write;

interface WriteError extends \MongoDB\ServerError
{
    /**
     * @return integer Batch index of the error
     */
    function getIndex();

    /**
     * @return array|object Operation or document responsible for the error
     */
    function getOperation();
}
