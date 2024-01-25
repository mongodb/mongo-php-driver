<?php

/**
 * @generate-class-entries static
 * @generate-function-entries static
 */

namespace MongoDB\Driver;

final class Manager
{
    final public function __construct(?string $uri = null, ?array $uriOptions = null, ?array $driverOptions = null) {}

    final public function addSubscriber(Monitoring\Subscriber $subscriber): void {}

    final public function createClientEncryption(array $options): ClientEncryption {}

#if PHP_VERSION_ID >= 80000
    final public function executeBulkWrite(string $namespace, BulkWrite $bulk, array|WriteConcern|null $options = null): WriteResult {}
#else
    /** @param array|WriteConcern|null $options */
    final public function executeBulkWrite(string $namespace, BulkWrite $bulk, $options = null): WriteResult {}
#endif

#if PHP_VERSION_ID >= 80000
    final public function executeCommand(string $db, Command $command, array|ReadPreference|null $options = null): Cursor {}
#else
    /** @param array|ReadPreference|null $options */
    final public function executeCommand(string $db, Command $command, $options = null): Cursor {}
#endif

#if PHP_VERSION_ID >= 80000
    final public function executeQuery(string $namespace, Query $query, array|ReadPreference|null $options = null): Cursor {}
#else
    /** @param array|ReadPreference|null $options */
    final public function executeQuery(string $namespace, Query $query, $options = null): Cursor {}
#endif

    final public function executeReadCommand(string $db, Command $command, ?array $options = null): Cursor {}

    final public function executeReadWriteCommand(string $db, Command $command, ?array $options = null): Cursor {}

    final public function executeWriteCommand(string $db, Command $command, ?array $options = null): Cursor {}

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

    final public function startSession(?array $options = null): Session {}
}
