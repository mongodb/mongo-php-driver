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
    // See: http://docs.mongodb.org/meta-driver/latest/legacy/mongodb-wire-protocol/#op-query
    const FLAG_TAILABLE_CURSOR   = 0x01;
    const FLAG_SLAVE_OK          = 0x02;
    const FLAG_OPLOG_REPLAY      = 0x04;
    const FLAG_NO_CURSOR_TIMEOUT = 0x08;
    const FLAG_AWAIT_DATA        = 0x10;
    const FLAG_EXHAUST           = 0x20;
    const FLAG_PARTIAL           = 0x40;

    private $query;
    private $selector;
    private $flags;
    private $skip;
    private $limit;

    /**
     * Sets the query criteria
     *
     * @param array $query Query document
     */
    public function setQuery(array $query)
    {
        $this->query = $query;
    }

    /**
     * Merges a query with the current one.
     *
     * @param array $criteria The query.
     */
    public function mergeQuery(array $query)
    {
        if (!$this->query) {
            $this->query = $query;
        } else {
            $this->criteria = array_merge($this->query, $query);
        }
    }

    /**
     * Sets the query projection
     *
     * @param array $selector Selector document
     */
    public function setSelector(array $selector)
    {
        $this->selector = $selector;
    }

    /**
     * Sets the query flags
     *
     * @param integer $flags Query flags
     */
    public function setFlags($flags)
    {
        $this->flags = (int) $flags;
    }

    /**
     * Sets the skip param
     *
     * @param integer $skip Skip
     */
    public function setSkip($skip)
    {
        $this->skip = (int) $skip;
    }

    /**
     * Sets the limit param
     *
     * @param integer $limit Limit
     */
    public function setLimit($limit)
    {
        $this->limit = (int) $limit;
    }
}
