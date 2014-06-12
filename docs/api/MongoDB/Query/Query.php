<?php

namespace MongoDB\Query;

/**
 * Value object corresponding to a wire protocol OP_QUERY message.
 *
 * If and when queries become commands, we will need to introduce a new Query
 * object, such as QueryCommand. At that point, the query will likely be
 * constructed from a single document, which includes the arguments below in a
 * similar fashion to findAndModify.
 */
final class Query
{
    private $query;
    private $selector;
    private $flags;
    private $skip;
    private $limit;

    /**
     * @param array|object $query    Query document
     * @param array|object $selector Selector document
     * @param integer      $flags    Query flags
     * @param integer      $limit    Limit
     * @param integer      $skip     Skip
     */
    public function __construct($query, $selector, $flags, $skip, $limit)
    {
        $this->query = $query;
        $this->selector = $selector;
        $this->flags = (integer) $flags;
        $this->skip = (integer) $skip;
        $this->limit = (integer) $limit;
    }
}
