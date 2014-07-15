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
	php_phongo_bson_encode_array(bson, document TSRMLS_CC);
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
