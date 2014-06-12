<?php

namespace MongoDB;

interface ServerResult
{
    /**
     * @return Server Server on which the result originated
     */
    function getServer();
}
