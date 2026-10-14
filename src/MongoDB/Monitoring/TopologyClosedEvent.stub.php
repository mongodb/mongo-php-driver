<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class TopologyClosedEvent
{
    public readonly \MongoDB\BSON\ObjectId $topologyId;

    final private function __construct() {}

    final public function getTopologyId(): \MongoDB\BSON\ObjectId {}
}
