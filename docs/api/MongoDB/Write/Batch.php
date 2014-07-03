<?php

namespace MongoDB\Write;

class Batch implements \Countable
{
    /**
     * Adds a new document to be inserted
     *
     * @param array|object $document Operation/document to add to insert
     * @return Batch
     */
    function insert($document) {
    }

    /**
     * Add a update operation to batch
     *
     * @param array|object $query Criteria to search for
     * @param array|object $update Updated document
     * @param integer $limit One or all matching $query
     * @param boolean $upsert Insert document if not exist
     * @return Batch
     */
    function update($query, $update, $limit, $upsert) {
    }

    /**
     * Add a delete operation to batch
     *
     * @param array|object $query Which docs to delete
     * @param boolean $limit One or all matching $query
     * @return Batch
     */
    function delete($query, $limit) {
    }

    /**
     * Counts how many operations are in the patch
     *
     * @return integer
     */
    function count() {
    }


}
