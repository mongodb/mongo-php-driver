<?php

namespace MongoDB\Write;

interface WriteConcernError extends \MongoDB\ServerError
{
    /**
     * @return array Additional metadata for the error (e.g. {"wtimeout": true})
     */
    function getInfo();
}
