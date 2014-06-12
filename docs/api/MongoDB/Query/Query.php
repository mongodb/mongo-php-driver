<?php

namespace MongoDB\Query;

final class Query
{
    /**
     * @param array|object $query    Query document
     * @param array|object $selector Selector document
     * @param integer      $flags    Query flags
     * @param integer      $limit    Limit
     * @param integer      $skip     Skip
     */
    public function __construct($query, $selector, $flags, $skip, $limit)
    {
        // ...
    }
}
