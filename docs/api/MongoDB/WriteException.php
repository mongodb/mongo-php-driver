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
        /*** CEF ***/
/*
	zval *writeresult;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	writeresult = zend_read_property(php_phongo_writeexception_ce, getThis(), ZEND_STRL("writeResult"), 0 TSRMLS_CC);

	RETURN_ZVAL(writeresult, 1, 0);
*/
        /*** CIMPL ***/
    }
}

$WriteException["headers"][] = '<ext/spl/spl_exceptions.h>';
