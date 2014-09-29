<?php

namespace MongoDB;

/**
 * Value object for write concern used in issuing write operations.
 */
final class WriteException extends \RuntimeException
{
    private $writeResult;

    /**
     * Returns the WriteResult from the failed write operation.
     *
     * In the event of a batch write, WriteResult may have relevant information
     * on the successful writes in the batch.
     *
     * @return WriteResult
     */
    public function getWriteResult()
    {
        /*** CIMPL ***/
/*
	if (intern->writeResult && Z_TYPE_P(intern->writeResult) == IS_OBJECT && Z_OBJCE_P(intern->writeResult) == php_phongo_writeresult_ce) {
		RETURN_ZVAL(intern->writeResult, 1, 0);
	}

	RETURN_NULL();
*/
        /*** CIMPL ***/
    }
}

$WriteException["headers"][] = '<ext/spl/spl_exceptions.h>';

$WriteException["free"] = <<< EOF
	if (intern->writeResult) {
		zval_ptr_dtor(&intern->writeResult);
	}

EOF;
