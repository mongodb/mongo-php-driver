<?php

/**
 * @generate-class-entries static
 * @generate-legacy-arginfo 80100
 */

namespace MongoDB\Driver\Monitoring;

/** @not-serializable */
final class CommandStartedEvent
{
    public readonly string $host;
    public readonly int $port;
    public readonly string $commandName;
    public readonly string $databaseName;
    public readonly object $command;
    public readonly string $operationId;
    public readonly string $requestId;
    public readonly ?\MongoDB\BSON\ObjectId $serviceId;
    public readonly ?int $serverConnectionId;

    final private function __construct() {}

    final public function getCommand(): object {}

    final public function getCommandName(): string {}

    final public function getDatabaseName(): string {}

    final public function getHost(): string {}

    final public function getOperationId(): string {}

    final public function getPort(): int {}

    final public function getRequestId(): string {}

    final public function getServiceId(): ?\MongoDB\BSON\ObjectId {}

    final public function getServerConnectionId(): ?int {}
}
