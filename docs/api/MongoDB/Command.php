<?php

namespace MongoDB;

/**
 * Value object for a database command document.
 */
final class Command
{
    private $document;

    /**
     * Constructs a new Command
     *
     * @param array|object $document Command document
     */
    public function __construct($document)
    {
        $this->document = $document;
        /*** CEF ***/
/*
	bson_t                   *bson = bson_new();
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	zval_to_bson(document, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);
	intern->bson = bson;
*/
        /*** CIMPL ***/
    }
}

$Command["free"] = <<< EOF
	if (intern->bson) {
		bson_free(intern->bson);
	}

EOF;
