<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Monitoring;

final class CommandStartedEvent
{
    final private function __construct() {}

    final public function getCommand(): object {}

    final public function getCommandName(): string {}

    final public function getDatabaseName(): string {}

    final public function getOperationId(): string {}

    final public function getRequestId(): string {}

    final public function getServer(): \MongoDB\Driver\Server {}

    final public function getServiceId(): ?\MongoDB\BSON\ObjectId {}

    final public function getServerConnectionId(): ?int {}
}
