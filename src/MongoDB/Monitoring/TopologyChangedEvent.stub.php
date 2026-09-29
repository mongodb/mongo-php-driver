<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class TopologyChangedEvent
{
    public readonly \MongoDB\BSON\ObjectId $topologyId;
    public readonly \MongoDB\Driver\TopologyDescription $newDescription;
    public readonly \MongoDB\Driver\TopologyDescription $previousDescription;

    final private function __construct() {}

    final public function getNewDescription(): \MongoDB\Driver\TopologyDescription {}

    final public function getPreviousDescription(): \MongoDB\Driver\TopologyDescription {}

    final public function getTopologyId(): \MongoDB\BSON\ObjectId {}
}
