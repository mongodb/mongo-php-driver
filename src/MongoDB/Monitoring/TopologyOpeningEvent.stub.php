<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class TopologyOpeningEvent
{
    public readonly \MongoDB\BSON\ObjectId $topologyId;

    final private function __construct() {}

    final public function getTopologyId(): \MongoDB\BSON\ObjectId {}
}
