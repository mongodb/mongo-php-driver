<?php

namespace MongoDB;

use Countable;

class WriteBatch implements Countable
{
    /**
     * Constructs a new WriteBatch
     *
     * @return WriteBatch
     */
    function __construct() {}

    /**
     * Adds an insert operation to the batch
     *
     * @param array|object $document Operation/document to add to insert
     * @return self
     */
    function insert($document) {}

    /**
     * Adds an update operation to batch
     *
     * @param array|object $query         Update criteria
     * @param array|object $newObj        Update modifier or replacement document
     * @param array        $updateOptions Update options (e.g. "upsert")
     * @return self
     */
    function update($query, $newObj, array $updateOptions = null)
    {
        $updateOptions = array_merge(
            array('multi' => false, 'upsert' => false),
            $updateOptions
        );
    }

    /**
     * Adds a delete operation to the batch
     *
     * @param array|object $query         Deletion criteria
     * @param array        $deleteOptions Deletion options (e.g. "limit")
     * @return self
     */
    function delete($query, array $deleteOptions = null)
    {
        /* TODO: Decide if we should default to 0 (i.e. "all"), as in 1.x, or if
         * an explicit "limit" option must be required. Keeping $deleteOptions
         * as an array, rather than a single integer option for "limit", seems
         * wise; if the "delete" command ever takes additional options, we would
         * not need to change our API.
         */
        $deleteOptions = array_merge(
            array('limit' => 0),
            $deleteOptions
        );
    }

    /**
     * Returns the number of operations that have been added to the batch
     *
     * @return integer
     */
    function count() {}
}
