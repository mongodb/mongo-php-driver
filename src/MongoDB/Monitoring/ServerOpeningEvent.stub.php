<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class ServerOpeningEvent
{
    final private function __construct() {}

    final public function getPort(): int {}

    final public function getHost(): string {}

    final public function getTopologyId(): \MongoDB\BSON\ObjectId {}
}
