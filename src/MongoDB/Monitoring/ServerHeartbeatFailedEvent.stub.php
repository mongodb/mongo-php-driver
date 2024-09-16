<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class ServerHeartbeatFailedEvent
{
    final private function __construct() {}

    final public function getDurationMicros() : int{}

    final public function getError(): \Exception {}

    final public function getPort(): int {}

    final public function getHost(): string {}

    final public function isAwaited(): bool {}
}
