<?php

use MongoDB\Driver\Exception\ConnectionException;
use MongoDB\Driver\Exception\RuntimeException;

require_once __DIR__ . '/basic.inc';
require_once __DIR__ . '/tools.php';

/**
 * Skips the test if the topology is a sharded cluster.
 */
function skip_if_mongos()
{
    is_mongos(URI) and exit('skip topology is a sharded cluster');
}

/**
 * Skips the test if the topology is not a shard cluster.
 */
function skip_if_not_mongos()
{
    is_mongos(URI) or exit('skip topology is not a sharded cluster');
}

/**
 * Skips the test if the topology is a replica set.
 */
function skip_if_replica_set()
{
    is_replica_set(URI) and exit('skip topology is a replica set');
}

/**
 * Skips the test if the topology is not a replica set.
 */
function skip_if_not_replica_set()
{
    is_replica_set(URI) or exit('skip topology is not a replica set');
}

/**
 * Skips the test if the topology is a standalone.
 */
function skip_if_standalone()
{
    is_standalone(URI) and exit('skip topology is a standalone');
}

/**
 * Skips the test if the topology is not a standalone.
 */
function skip_if_not_standalone()
{
    is_standalone(URI) or exit('skip topology is not a standalone');
}

/**
 * Skips the test if the connection string uses SSL.
 */
function skip_if_ssl()
{
    is_ssl(URI) and exit('skip URI is using SSL');
}

/**
 * Skips the test if the connection string uses SSL.
 */
function skip_if_not_ssl()
{
    is_ssl(URI) or exit('skip URI is not using SSL');
}

/**
 * Skips the test if the connection string is using auth.
 */
function skip_if_auth()
{
    is_auth(URI) and exit('skip URI is using auth');
}

/**
 * Skips the test if the connection string is not using auth.
 */
function skip_if_not_auth()
{
    is_auth(URI) or exit('skip URI is not using auth');
}

/**
 * Skips the test if the connection string is not using a particular
 * authMechanism.
 *
 * @param string $authMechanism
 */
function skip_if_not_auth_mechanism($authMechanism)
{
    $uriAuthMechanism = get_uri_option(URI, 'authMechanism');

    if ($uriAuthMechanism === null) {
        exit('skip URI is not using authMechanism');
    }

    if ($uriAuthMechanism !== $authMechanism) {
        exit("skip URI authMechanism is '$uriAuthMechanism' (needed: '$authMechanism')");
    }
}

/**
 * Skips the test if the server is not accessible.
 */
function skip_if_not_live()
{
    try {
        get_primary_server(URI);
    } catch (ConnectionException $e) {
        exit('skip server is not accessible: ' . $e->getMessage());
    }
}

/**
 * Skips the test if the server version satisfies a comparison.
 *
 * @see http://php.net/version_compare
 * @param string $operator Comparison operator
 * @param string $version  Version to compare against
 */
function skip_if_server_version($operator, $version)
{
    $serverVersion = get_server_version(URI);

    if (version_compare($serverVersion, $version, $operator)) {
        exit("skip Server version '$serverVersion' $operator '$version'");
    }
}

/**
 * Skips the test if the server not using a particular storage engine.
 *
 * @param string $storageEngine Storage engine name
 */
function skip_if_not_server_storage_engine($storageEngine)
{
    $serverStorageEngine = get_server_storage_engine(URI);

    if ($serverStorageEngine !== $storageEngine) {
        exit("skip Server storage engine is '$serverStorageEngine' (needed '$storageEngine')");
    }
}

/**
 * Skips the test if the server does not support test commands.
 */
function skip_if_test_commands_disabled()
{
    if (!get_server_parameter(URI, 'enableTestCommands')) {
        exit('skip test commands are disabled');
    }
}

/**
 * Skips the test if libmongoc does not support crypto.
 *
 * If one or more libaries are provided, additionally check that the reported
 * library is in that array. Possible values are "libcrypto", "Common Crypto",
 * and "CNG".
 *
 * @param array $libs Optional list of crypto libraries to require
 */
function skip_if_not_libmongoc_crypto(array $libs = [])
{
    $lib = get_module_info('libmongoc crypto library');

    if ($lib === null) {
        exit('skip libmongoc crypto is not enabled');
    }

    if (!empty($libs) && !in_array($lib, $libs)) {
        exit('skip Needs libmongoc crypto library ' . implode(', ', $libs) . ', but found ' . $lib);
    }
}

/**
 * Skips the test if libmongoc does not support SSL.
 *
 * If one or more libaries are provided, additionally check that the reported
 * library is in that array. Possible values are "OpenSSL", "LibreSSL",
 * "Secure Transport", and "Secure Channel".
 *
 * @param array $libs Optional list of SSL libraries to require
 */
function skip_if_not_libmongoc_ssl(array $libs = [])
{
    $lib = get_module_info('libmongoc SSL library');

    if ($lib === null) {
        exit('skip libmongoc SSL is not enabled');
    }

    if (!empty($libs) && !in_array($lib, $libs)) {
        exit('skip Needs libmongoc SSL library ' . implode(', ', $libs) . ', but found ' . $lib);
    }
}

/**
 * Skips the test if the collection cannot be dropped.
 *
 * @param string $databaseName   Database name
 * @param string $collectionName Collection name
 */
function skip_if_not_clean($databaseName = DATABASE_NAME, $collectionName = COLLECTION_NAME)
{
    try {
        drop_collection(URI, $databaseName, $collectionName);
    } catch (RuntimeException $e) {
        exit("skip Could not drop '$databaseName.$collectionName': " . $e->getMessage());
    }
}
