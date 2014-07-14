<?php

namespace MongoDB;

use Iterator;

/**
 * Cursor used to iterate through results of an executed Query or Command.
 *
 * The iteration and internal logic of Query and Command cursors is very
 * similar. The cursor ID and first batch of results, originating from either
 * the OP_REPLY message or command result document), will be used to construct
 * this object.
 *
 * While this Cursor object must be initialized internally, the class itself may
 * be extended to provide custom Cursor behaviors (e.g. return documents as
 * BSON, hydrated classes, stdClass objects).
 */
class Cursor implements Iterator
{
    private $batchSize;
    private $cursorId;
    private $server;

    /**
     * Construct a new Cursor
     *
     * @param Server   $server
     * @param CursorId $cursorId
     * @param array    $firstBatch
     */
    final private function __construct(Server $server, CursorId $cursorId, array $firstBatch)
    {
        $this->server = $server;
        $this->cursorId = $cursorId;

        // $firstBatch will be used internally to jump-start iteration

        /* TODO: Should a cursor be constructed with OP_REPLY response flags?
         * AwaitCapable may be relevant.
         */
    }

    /**
     * Returns the CursorId
     *
     * @return CursorId
     */
    final public function getId()
    {
        return $this->cursorId;
    }

    /**
     * Returns the Server object that this cursor is attached to
     *
     * @return Server Server from which the cursor originated
     */
    final public function getServer()
    {
        return $this->server;
    }

    /**
     * Checks if a cursor is still alive
     *
     * @return boolean Whether the cursor is exhausted and has no more results
     */
    final public function isDead()
    {
        // Return whether the cursor is exhausted and has no more results
    }

    /**
     * Sets a batch size for the cursor
     *
     * @param integer $batchSize
     * @return boolean true on success, false on failure
     */
    final public function setBatchSize($batchSize)
    {
        $this->batchSize = (integer) $batchSize;
    }

    /**
     * Returns the current element
     *
     * @see Iterator::current()
     * @return mixed
     */
    public function current()
    {
        /* The default cursor implementation should return a BSON object.
         * Alternative implementations can then optionally convert that object
         * into an associative array, stdClass object, etc.
         *
         * For performance reasons, some common implementations should be
         * implemented in C and packaged with the core extension.
         */
    }

    /**
     * Returns the key of the current element
     *
     * @see Iterator::key()
     * @return mixed
     */
    public function key()
    {
        /* The default cursor implementation should return an integer, which
         * corresponds to the current elements offset in the result set. We
         * should not return "_id" as in the 1.x driver, because it is
         * unreliable for non-scalar identifiers.
         *
         * See: https://jira.mongodb.org/browse/PHP-570 for additional context.
         *
         * Nevertheless, it alternative cursor implementations can easily
         * extend and override this method to return "_id" (obtained by calling
         * current()). On PHP versions before 5.5, they may want to degrade
         * gracefully on non-scalar values and simply call parent::key().
         */
    }

    /**
     * Move forward to next element
     *
     * @see Iterator::next()
     * @throws RuntimeException if an error occurs during an OP_GET_MORE
     */
    final public function next()
    {
        // This method may block if the cursor is tailable.
    }

    /**
     * Rewind the Iterator to the first element
     *
     * @see Iterator::rewind()
     * @throws LogicException if the cursor has already been advanced
     */
    final public function rewind()
    {
        /* Cursors cannot be rewound. In the 1.x driver, MongoCursor::rewind()
         * would reset the cursor and execute the query again. That API no
         * longer makes sense, since Cursor is constructed after a query or
         * command has already executed.
         *
         * The rewind() method should be a NOP, unless the Cursor has started
         * iterating, in which case we should throw an exception. Therefore, it
         * will not be possible to use the same Cursor in successive foreach
         * loops. Users that would like to do so (e.g. maybe they break during
         * the first foreach loop and want to continue in another) should
         * decorate the Cursor with NoRewindIterator first.
         */
    }

    /**
     * Checks if current position is valid
     *
     * @see Iterator::valid()
     * @return boolean
     */
    final public function valid() {}
}
