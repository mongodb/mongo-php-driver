<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\Driver\Logging;

interface Logger
{
    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_ERROR
     */
    public const LEVEL_ERROR = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_CRITICAL
     */
    public const LEVEL_CRITICAL = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_WARNING
     */
    public const LEVEL_WARNING = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_MESSAGE
     */
    public const LEVEL_MESSAGE = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_INFO
     */
    public const LEVEL_INFO = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_DEBUG
     */
    public const LEVEL_DEBUG = UNKNOWN;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_TRACE
     */
    public const LEVEL_TRACE = UNKNOWN;

    public function log(int $level, string $domain, string $message): void;
}
