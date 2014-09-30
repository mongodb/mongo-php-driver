<?php

namespace MongoDB;

/**
 * Value object for a write error (e.g. duplicate key).
 */
final class WriteError
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
     * Returns the Batch index where this WriteError occurred in
     *
     * @return integer Batch index of the error
     */
    public function getIndex()
    {
        /*** CIMPL ***/
/*
	RETURN_LONG(intern->index);
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
        /*** CIMPL ***/
/*
	RETURN_STRING(intern->message, 1);
*/
        /*** CIMPL ***/
    }
}

$WriteError["free"] = <<< EOF
	if (intern->message) {
		efree(intern->message);
	}

EOF;
