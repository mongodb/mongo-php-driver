<?php
namespace MongoDB;

define("MONGOC_QUERY_NONE", "MONGOC_QUERY_NONE");
define("MONGOC_QUERY_TAILABLE_CURSOR", "MONGOC_QUERY_TAILABLE_CURSOR");
define("MONGOC_QUERY_SLAVE_OK", "MONGOC_QUERY_SLAVE_OK");
define("MONGOC_QUERY_OPLOG_REPLAY", "MONGOC_QUERY_OPLOG_REPLAY");
define("MONGOC_QUERY_NO_CURSOR_TIMEOUT", "MONGOC_QUERY_NO_CURSOR_TIMEOUT");
define("MONGOC_QUERY_AWAIT_DATA", "MONGOC_QUERY_AWAIT_DATA");
define("MONGOC_QUERY_EXHAUST", "MONGOC_QUERY_EXHAUST");
define("MONGOC_QUERY_PARTIAL", "MONGOC_QUERY_PARTIAL");

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
    const FLAG_NONE              = MONGOC_QUERY_NONE;
    const FLAG_TAILABLE_CURSOR   = MONGOC_QUERY_TAILABLE_CURSOR;
    const FLAG_SLAVE_OK          = MONGOC_QUERY_SLAVE_OK;
    const FLAG_OPLOG_REPLAY      = MONGOC_QUERY_OPLOG_REPLAY;
    const FLAG_NO_CURSOR_TIMEOUT = MONGOC_QUERY_NO_CURSOR_TIMEOUT;
    const FLAG_AWAIT_DATA        = MONGOC_QUERY_AWAIT_DATA;
    const FLAG_EXHAUST           = MONGOC_QUERY_EXHAUST;
    const FLAG_PARTIAL           = MONGOC_QUERY_PARTIAL;

    private $query;
    private $selector;
    private $flags;
    private $skip;
    private $limit;

    /**
     * Constructs a new Query
     *
     * Optional arguments include:
     *  - batchSize (integer): The number of documents to return per batch
     *  - cursorFlags (integer): The cursor flags to use
     *  - limit (integer): The maximum number of documents to return
     *  - modifiers (array|object): Meta-operators modifying the output or behavior of a query
     *  - projection (array|object): Limits the fields to return for all matching documents
     *  - skip (integer): The number of documents to skip before returning
     *  - sort (array|object): The order in which to return matching documents
     *
     * @param array|object $filter  Filter criteria (i.e. "$query" field in query document)
     * @param array        $options Optional arguments
     */
    public function __construct($filter, array $options = array())
    {
        /*** CIMPL ***/
/*
	phongo_query_init(intern, filter, options TSRMLS_CC);
*/
        /*** CIMPL ***/
    }
}
