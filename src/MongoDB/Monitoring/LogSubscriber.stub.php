<?php

/**
 * @generate-class-entries static
 * @generate-function-entries
 */

namespace MongoDB\Driver\Monitoring;

interface LogSubscriber extends Subscriber
{
    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_ERROR
     */
    public const LEVEL_ERROR = 0;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_CRITICAL
     */
    public const LEVEL_CRITICAL = 1;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_WARNING
     */
    public const LEVEL_WARNING = 2;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_MESSAGE
     */
    public const LEVEL_MESSAGE = 3;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_INFO
     */
    public const LEVEL_INFO = 4;

    /**
     * @var int
     * @cvalue MONGOC_LOG_LEVEL_DEBUG
     */
    public const LEVEL_DEBUG = 5;

    /* MONGOC_LOG_LEVEL_TRACE is intentionally omitted. Trace logs are only
     * reported via streams (i.e. mongodb.debug INI), so the constant is not
     * relevant to LogSubscriber implementations. */

    public function log(int $level, string $domain, string $message): void;
}
