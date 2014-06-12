<?php

namespace MongoDB\Command;

interface CommandResult extends \MongoDB\ServerResult
{
    /**
     * @return array Original response document from the server
     */
    function getResponse();
}
