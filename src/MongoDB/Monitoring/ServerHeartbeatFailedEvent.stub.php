<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class ServerHeartbeatFailedEvent
{
    public readonly string $host;
    public readonly int $port;
    public readonly bool $awaited;
    public readonly int $duration;
    public readonly \Exception $error;

    final private function __construct() {}

    final public function getDurationMicros(): int {}

    final public function getError(): \Exception {}

    final public function getPort(): int {}

    final public function getHost(): string {}

    final public function isAwaited(): bool {}
}
