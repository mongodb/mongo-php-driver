<?php

namespace MongoDB\Write;

/**
 * Value object for a write error (e.g. duplicate key).
 */
final class WriteError
{
    private $message;
    private $code;
    private $index;
    private $operation;

    /**
     * Constructs a new WriteError object
     *
     * @param string       $message   Server error message
     * @param integer      $code      Server error code
     * @param integer      $index     Batch index of the error
     * @param array|object $operation Operation or document responsible for the error
     */
    public function __construct($message, $code, $index, $operation)
    {
        $this->message = (string) $message;
        $this->code = (integer) $code;
        $this->index = (integer) $index;
        $this->operation = $operation;
    }

    /**
     * Returns the MongoDB error code
     *
     * @return integer Server error code
     */
    public function getCode()
    {
        return $this->code;
    }

    /**
     * Returns the Batch index where this WriteError occurred in
     *
     * @return integer Batch index of the error
     */
    public function getIndex()
    {
        return $this->index;
    }

    /**
     * Returns the actual error message from the server
     *
     * @return string Server error message
     */
    public function getMessage()
    {
        return $this-message;
    }

    /**
     * Returns the batch operation itself that caused the error
     *
     * @return array|object Operation or document responsible for the error
     */
    public function getOperation()
    {
        return $this->operation;
    }
}
