<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class ServerHeartbeatFailedEvent
{
    public readonly string $host;
    public readonly int $port;
    public readonly bool $awaited;
    public readonly int $durationMicros;
    public readonly \Exception $error;

    final private function __construct() {}

    final public function getDurationMicros(): int {}

    final public function getError(): \Exception {}

    final public function getPort(): int {}

    final public function getHost(): string {}

    final public function isAwaited(): bool {}
}
