<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Monitoring;

final class TopologyChangedEvent
{
    final private function __construct() {}

    final public function getNewDescription(): \MongoDB\Driver\TopologyDescription {}

    final public function getPreviousDescription(): \MongoDB\Driver\TopologyDescription {}

    final public function getTopologyId(): \MongoDB\BSON\ObjectId {}
}
