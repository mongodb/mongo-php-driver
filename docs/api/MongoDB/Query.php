<?php

namespace MongoDB;
define("FLAG_NONE", "MONGOC_QUERY_NONE");
define("FLAG_TAILABLE_CURSOR", MONGOC_QUERY_TAILABLE_CURSOR);
define("FLAG_SLAVE_OK", MONGOC_QUERY_SLAVE_OK);
define("FLAG_OPLOG_REPLAY", MONGOC_QUERY_OPLOG_REPLAY);
define("FLAG_NO_CURSOR_TIMEOUT", MONGOC_QUERY_NO_CURSOR_TIMEOUT);
define("FLAG_AWAIT_DATA", MONGOC_QUERY_AWAIT_DATA);
define("FLAG_EXHAUST", MONGOC_QUERY_EXHAUST);
define("FLAG_PARTIAL", MONGOC_QUERY_PARTIAL);

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
    const FLAG_NONE              = FLAG_NONE;
    const FLAG_TAILABLE_CURSOR   = FLAG_TAILABLE_CURSOR;
    const FLAG_SLAVE_OK          = FLAG_SLAVE_OK;
    const FLAG_OPLOG_REPLAY      = FLAG_OPLOG_REPLAY;
    const FLAG_NO_CURSOR_TIMEOUT = FLAG_NO_CURSOR_TIMEOUT;
    const FLAG_AWAIT_DATA        = FLAG_AWAIT_DATA;
    const FLAG_EXHAUST           = FLAG_EXHAUST;
    const FLAG_PARTIAL           = FLAG_PARTIAL;

    private $query;
    private $selector;
    private $flags;
    private $skip;
    private $limit;

    /**
     * Constructs a new Query
     *
     * @param array|object $query    Query document
     * @param array|object $selector Selector document
     * @param integer      $flags    Query flags
     * @param integer      $skip     Skip
     * @param integer      $limit    Limit
     */
    public function __construct($query, $selector = array(), $flags = self::FLAG_NONE, $skip = 0, $limit = 0)
    {
        $this->query = $query;
        $this->selector = $selector;
        $this->flags = (integer) $flags;
        $this->skip = (integer) $skip;
        $this->limit = (integer) $limit;
        /*** CIMPL ***/
/*
	phongo_query_init(intern, query, selector, flags, skip, limit TSRMLS_CC);
*/
        /*** CIMPL ***/
    }
}
