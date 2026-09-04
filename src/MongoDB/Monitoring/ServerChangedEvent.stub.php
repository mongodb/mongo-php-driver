<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class ServerChangedEvent
{
    public readonly string $host;
    public readonly int $port;
    public readonly \MongoDB\BSON\ObjectId $topologyId;
    public readonly \MongoDB\Driver\ServerDescription $newDescription;
    public readonly \MongoDB\Driver\ServerDescription $previousDescription;

    final private function __construct() {}

    final public function getPort(): int {}

    final public function getHost(): string {}

    final public function getNewDescription(): \MongoDB\Driver\ServerDescription {}

    final public function getPreviousDescription(): \MongoDB\Driver\ServerDescription {}

    final public function getTopologyId(): \MongoDB\BSON\ObjectId {}
}
