<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver\Monitoring;

final class CommandFailedEvent
{
    final private function __construct() {}

    final public function getCommandName(): string {}

    final public function getDurationMicros(): int {}

    final public function getError(): \Throwable {}

    final public function getOperationId(): string {}

    final public function getReply(): object {}

    final public function getRequestId(): string {}

    final public function getServer(): \MongoDB\Driver\Server {}

    final public function getServiceId(): ?\MongoDB\BSON\ObjectId {}

    final public function getServerConnectionId(): ?int {}

    final public function __wakeup(): void {}
}
