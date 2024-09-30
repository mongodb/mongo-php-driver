<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class CommandStartedEvent
{
    final private function __construct() {}

    final public function getCommand(): object {}

    final public function getCommandName(): string {}

    final public function getDatabaseName(): string {}

    final public function getHost(): string {}

    final public function getOperationId(): string {}

    final public function getPort(): int {}

    final public function getRequestId(): string {}

    /** @deprecated */
    final public function getServer(): \MongoDB\Driver\Server {}

    final public function getServiceId(): ?\MongoDB\BSON\ObjectId {}

    final public function getServerConnectionId(): ?int {}
}
