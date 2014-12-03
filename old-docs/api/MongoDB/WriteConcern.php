<?php

namespace MongoDB;

define('PHONGO_WRITE_CONCERN_W_MAJORITY', 'PHONGO_WRITE_CONCERN_W_MAJORITY');

/**
 * Value object for write concern used in issuing write operations.
 */
final class WriteConcern
{
    const MAJORITY = PHONGO_WRITE_CONCERN_W_MAJORITY;

    /**
     * Constructs a new WriteConcern
     *
     * @see http://docs.mongodb.org/manual/reference/write-concern/
     * @param integer|string $wstring  Number of required acknowledgements or a tag set
     * @param integer        $wtimeout Write concern timeout in milliseconds
     * @param boolean        $journal  = false
     * @param boolean        $fsync    = false
     */
    public function __construct($wstring, $wtimeout = 0, $journal = false, $fsync = false)
    {
        /*** CEF ***/
/*
	long                      w;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	intern->write_concern = mongoc_write_concern_new();

	if (IS_LONG == is_numeric_string(wstring, wstring_len, &w, NULL, 0)) {
		mongoc_write_concern_set_w(intern->write_concern, w);
	} else {
		if (strcmp(wstring, PHONGO_WRITE_CONCERN_W_MAJORITY) == 0) {
			mongoc_write_concern_set_w(intern->write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);
		} else {
			mongoc_write_concern_set_wtag(intern->write_concern, wstring);
		}
	}

	switch(ZEND_NUM_ARGS()) {
		case 4:
			if (fsync) {
				mongoc_write_concern_set_fsync(intern->write_concern, true);
			}
			// fallthrough
		case 3:
			if (journal) {
				mongoc_write_concern_set_journal(intern->write_concern, true);
			}
			// fallthrough
		case 2:
			if (wtimeout > 0) {
				mongoc_write_concern_set_wtimeout(intern->write_concern, wtimeout);
			}
	}
*/
        /*** CIMPL ***/
    }
}

$WriteConcern["forward_declarations"] = <<< EOT

#define PHONGO_WRITE_CONCERN_W_MAJORITY "majority"

EOT;

$WriteConcern["free"] = <<< EOF
	if (intern->write_concern) {
		mongoc_write_concern_destroy(intern->write_concern);
	}

EOF;
