<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class Manager
{
    final public function __construct(?string $uri = null, array $uriOptions = [], array $driverOptions = []) {}

    final public function addSubscriber(Monitoring\Subscriber $subscriber): void {}

    final public function createClientEncryption(array $options): ClientEncryption {}

#if PHP_VERSION_ID >= 80000
    final public function executeBulkWrite(string $namespace, BulkWrite $bulk, array|WriteConcern $options = []): WriteResult {}
#else
    /** @param array|WriteConcern $options */
    final public function executeBulkWrite(string $namespace, BulkWrite $bulk, $options = []): WriteResult {}
#endif

#if PHP_VERSION_ID >= 80000
    final public function executeCommand(string $db, Command $command, array|ReadPreference $options = []): Cursor {}
#else
    /** @param array|ReadPreference $options */
    final public function executeCommand(string $db, Command $command, $options = []): Cursor {}
#endif

#if PHP_VERSION_ID >= 80000
    final public function executeQuery(string $namespace, Query $query, array|ReadPreference $options = []): Cursor {}
#else
    /** @param array|ReadPreference $options */
    final public function executeQuery(string $namespace, Query $query, $options = []): Cursor {}
#endif

    final public function executeReadCommand(string $db, Command $command, array $options = []): Cursor {}

    final public function executeReadWriteCommand(string $db, Command $command, array $options = []): Cursor {}

    final public function executeWriteCommand(string $db, Command $command, array $options = []): Cursor {}

#if PHP_VERSION_ID >= 80000
    final public function getEncryptedFieldsMap(): array|object|null {}
#else
    /** @return array|object|null */
    final public function getEncryptedFieldsMap() {}
#endif

    final public function getReadConcern(): ReadConcern {}

    final public function getReadPreference(): ReadPreference {}

    final public function getServers(): array {}

    final public function getWriteConcern(): WriteConcern {}

    final public function removeSubscriber(Monitoring\Subscriber $subscriber): void {}

    final public function selectServer(?ReadPreference $readPreference = null): Server {}

    final public function startSession(array $options = []): Session {}

    final public function __wakeup(): void {}
}
