<?php

namespace MongoDB;

use Countable;

class WriteBatch implements Countable
{
    /**
     * Constructs a new WriteBatch
     *
     * @param boolean $ordered Initialize (un-)ordered batch
     * @return WriteBatch
     */
    function __construct($ordered = true) {
        /*** CIMPL ***/
/*
	intern->batch = phongo_writebatch_init(ordered);
*/
        /*** CIMPL ***/
    }

    /**
     * Adds an insert operation to the batch
     *
     * The document's "_id" will be returned (either from the provided document
     * or a generated value from the driver).
     *
     * @param array|object $document Operation/document to add to insert
     * @return mixed Document identifier
     */
    function insert($document) {
        /*** CEF ***/
/*
	bson_t                   *bson;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	bson = bson_new();
	php_phongo_bson_encode_array(bson, document TSRMLS_CC);
	mongoc_bulk_operation_insert(intern->batch, bson);
	bson_destroy(bson);
*/
        /*** CIMPL ***/
    }

    /**
     * Adds an update operation to batch
     *
     * @param array|object $query         Update criteria
     * @param array|object $newObj        Update modifier or replacement document
     * @param array        $updateOptions Update options (e.g. "upsert")
     */
    function update($query, $newObj, array $updateOptions = null)
    {
        /*** CEF ***/
/*
	mongoc_update_flags_t     limit = 0;
	zend_bool                 upsert = 0;
	bson_t                   *bquery;
	bson_t                   *bupdate;
*/
        /*** CEF ***/
        $updateOptions = array_merge(
            array('multi' => false, 'upsert' => false),
            $updateOptions
        );
        /*** CIMPL ***/
/*
	bquery = bson_new();
	bupdate = bson_new();

	php_phongo_bson_encode_array(bquery, query TSRMLS_CC);
	php_phongo_bson_encode_array(bupdate, newObj TSRMLS_CC);

	if (updateOptions) {
		limit = php_array_fetch_bool(updateOptions, "limit");
		upsert = php_array_fetch_bool(updateOptions, "upsert");
	}

	if (limit) {
		mongoc_bulk_operation_update_one(intern->batch, bquery, bupdate, upsert);
	} else {
		mongoc_bulk_operation_update(intern->batch, bquery, bupdate, upsert);
	}

	bson_destroy(bquery);
	bson_destroy(bupdate);
*/
        /*** CIMPL ***/
    }

    /**
     * Adds a delete operation to the batch
     *
     * @param array|object $query         Deletion criteria
     * @param array        $deleteOptions Deletion options (e.g. "limit")
     */
    function delete($query, array $deleteOptions = null)
    {
        /*** CEF ***/
/*
	bson_t                   *bson;
*/
        /*** CEF ***/
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
        /*** CIMPL ***/
/*
	bson = bson_new();
	php_phongo_bson_encode_array(bson, query TSRMLS_CC);

	if (deleteOptions && php_array_fetch_bool(deleteOptions, "limit")) {
		mongoc_bulk_operation_remove_one(intern->batch, bson);
	} else {
		mongoc_bulk_operation_remove(intern->batch, bson);
	}

	bson_destroy(bson);
*/
        /*** CIMPL ***/
    }

    /**
     * Returns the number of operations that have been added to the batch
     *
     * @return integer
     */
    function count() {}
}

$WriteBatch["headers"][] = '"php_array.h"';
