<?php

namespace MongoDB\Write;

/**
 * Value object for a write concern error.
 */
final class WriteConcernError
{
    private $message;
    private $code;
    private $info;

    /**
     * Constructs a new WriteConcernError object
     * 
     * @param string  $message Server error message
     * @param integer $code    Server error code
     * @param array   $info    Additional metadat for the error (e.g. {"wtimeout": true})
     */
    public function __construct($message, $code, array $info)
    {
        $this->message = (string) $message;
        $this->code = (integer) $code;
        $this->info = $info;
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
     * Returns additional metadata for the error
     *
     * @return array Additional metadata for the error (e.g. {"wtimeout": true})
     */
    public function getInfo()
    {
        return $this->info;
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
}
