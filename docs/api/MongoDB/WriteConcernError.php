<?php

namespace MongoDB;

/**
 * Value object for a write concern error.
 */
final class WriteConcernError
{
    /**
     * Returns the MongoDB error code
     *
     * @return integer Server error code
     */
    public function getCode()
    {
        /*** CIMPL ***/
/*
	RETURN_LONG(intern->code);
*/
        /*** CIMPL ***/
    }

    /**
     * Returns additional metadata for the error
     *
     * @return array Additional metadata for the error (e.g. {"wtimeout": true})
     */
    public function getInfo()
    {
        /*** CIMPL ***/
/*
	if (intern->info && Z_TYPE_P(intern->info) == IS_ARRAY) {
		RETURN_ZVAL(intern->info, 1, 0);
	}

	array_init(return_value);
*/
        /*** CIMPL ***/
    }

    /**
     * Returns the actual error message from the server
     *
     * @return string Server error message
     */
    public function getMessage()
    {
/*
	RETURN_STRING(intern->message, 1);
*/
    }
}

$WriteConcernError["free"] = <<< EOF
	if (intern->message) {
		efree(intern->message);
	}

	if (intern->info) {
		zval_ptr_dtor(&intern->info);
	}

EOF;
