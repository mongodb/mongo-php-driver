<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class ServerClosedEvent
{
    public readonly string $host;
    public readonly int $port;
    public readonly \MongoDB\BSON\ObjectId $topologyId;

    final private function __construct() {}

    final public function getPort(): int {}

    final public function getHost(): string {}

    final public function getTopologyId(): \MongoDB\BSON\ObjectId {}
}
