<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class ServerChangedEvent
{
    final private function __construct() {}

    final public function getPort(): int {}

    final public function getHost(): string {}

    final public function getNewDescription(): \MongoDB\Driver\ServerDescription {}

    final public function getPreviousDescription(): \MongoDB\Driver\ServerDescription {}

    final public function getTopologyId(): \MongoDB\BSON\ObjectId {}
}
