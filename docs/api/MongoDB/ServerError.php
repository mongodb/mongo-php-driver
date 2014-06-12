<?php

namespace MongoDB;

interface ServerError extends ServerResult
{
    /**
     * @return integer Server error code
     */
    function getCode();

    /**
     * @return string Server error message
     */
    function getMessage();
}
