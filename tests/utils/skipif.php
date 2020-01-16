<?php

use MongoDB\Driver\Command;
use MongoDB\Driver\Manager;
use MongoDB\Driver\ReadPreference;
use MongoDB\Driver\Server;
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
 * Skips the test if the topology contains multiple mongos nodes.
 *
 * This is particularly useful for tests that rely on configureFailPoint, since
 * randomized server selection can interfere with testing.
 */
function skip_if_multiple_mongos()
{
    $manager = new Manager(URI);

    // Ensure SDAM is initialized before calling Manager::getServers()
    $manager->selectServer(new ReadPreference('nearest'));

    $mongosNodes = array_filter($manager->getServers(), function(Server $server) {
        return $server->getType() === Server::TYPE_MONGOS;
    });

    if (count($mongosNodes) > 1) {
        exit('skip topology contains multiple mongos nodes');
    }
}

/**
 * Skips the test if the topology is not a shard cluster.
 */
function skip_if_not_mongos()
{
    is_mongos(URI) or exit('skip topology is not a sharded cluster');
}

function skip_if_not_mongos_with_replica_set()
{
    is_mongos_with_replica_set(URI) or exit('skip topology is not a sharded cluster with replica set');
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
 * Skips the test if the topology is not a replica set or sharded cluster backed by replica sets
 */
function skip_if_not_replica_set_or_mongos_with_replica_set()
{
    is_replica_set(URI) or is_mongos_with_replica_set(URI) or exit('skip topology is not a replica set or sharded cluster with replica set');
}

function skip_if_no_transactions()
{
    if (is_mongos_with_replica_set(URI)) {
        skip_if_server_version('<', '4.2');
    } elseif (is_replica_set(URI)) {
        skip_if_server_version('<', '4.0');
    } else {
        exit('skip topology does not support transactions');
    }
}

/**
 * Skips the test if the topology has no arbiter.
 */
function skip_if_no_arbiter()
{
    try {
        $primary = get_primary_server(URI);
    } catch (ConnectionException $e) {
        exit('skip primary server is not accessible: ' . $e->getMessage());
    }
    $info = $primary->getInfo();

    if (!isset($info['arbiters']) || count($info['arbiters']) < 1) {
        exit('skip no arbiters available');
    }
}

/**
 * Skips the test if the topology has no secondary.
 */
function skip_if_no_secondary()
{
    try {
        $primary = get_primary_server(URI);
    } catch (ConnectionException $e) {
        exit('skip primary server is not accessible: ' . $e->getMessage());
    }
    $info = $primary->getInfo();

    if (!isset($info['hosts']) || count($info['hosts']) < 2) {
        exit('skip no secondaries available');
    }
}

/**
 * Skips the test if the topology does not have enough data carrying nodes
 */
function skip_if_not_enough_data_nodes($requiredNodes, $maxNodeCount = null)
{
    try {
        $primary = get_primary_server(URI);
    } catch (ConnectionException $e) {
        exit('skip primary server is not accessible: ' . $e->getMessage());
    }
    $info = $primary->getInfo();

    $dataNodeCount = isset($info['hosts']) ? count($info['hosts']) : 0;

    if ($dataNodeCount < $requiredNodes) {
        exit("skip not enough nodes available (wanted: {$requiredNodes}, available: " . count($info['hosts']) . ')');
    }
    if ($maxNodeCount !== null && $dataNodeCount > $requiredNodes) {
        exit("skip too many nodes available (wanted: {$requiredNodes}, available: " . count($info['hosts']) . ')');
    }
}

/**
 * Skips the test if the topology does not have enough nodes
 */
function skip_if_not_enough_nodes($requiredNodes, $maxNodeCount = null)
{
    try {
        $primary = get_primary_server(URI);
    } catch (ConnectionException $e) {
        exit('skip primary server is not accessible: ' . $e->getMessage());
    }
    $info = $primary->getInfo();

    $nodeCount =
        (isset($info['hosts']) ? count($info['hosts']) : 0) +
        (isset($info['arbiters']) ? count($info['arbiters']) : 0);

    if ($nodeCount < $requiredNodes) {
        exit("skip not enough nodes available (wanted: {$requiredNodes}, available: " . count($info['hosts']) . ')');
    }
    if ($maxNodeCount !== null && $nodeCount > $requiredNodes) {
        exit("skip too many nodes available (wanted: {$requiredNodes}, available: " . count($info['hosts']) . ')');
    }
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

    if ($uriAuthMechanism === null && $authMechanism !== null) {
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
 * Skips the test if the PHP version satisfies a comparison.
 *
 * @see http://php.net/version_compare
 * @param string $operator Comparison operator
 * @param string $version  Version to compare against
 */
function skip_if_php_version($operator, $version)
{
    if (version_compare(PHP_VERSION, $version, $operator)) {
        exit("skip PHP version '" . PHP_VERSION . "' $operator '$version'");
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
 * Skips the test if the server does not support the sleep command.
 */
function skip_if_sleep_command_unavailable()
{
    if (!command_works(URI, ['sleep' => 1, 'secs' => 1, 'w' => false])) {
        exit('skip sleep command not available');
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
 * Skips the test if the driver was not compiled with support for FLE
 */
function skip_if_not_libmongocrypt()
{
    $lib = get_module_info('libmongocrypt');

    if ($lib === 'disabled') {
        exit('skip libmongocrypt is not enabled');
    }
}

/**
 * Skips the test if the driver was compiled with support for FLE
 */
function skip_if_libmongocrypt()
{
    $lib = get_module_info('libmongocrypt');

    if ($lib !== 'disabled') {
        exit('skip libmongocrypt is enabled');
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

function skip_if_no_getmore_failpoint()
{
    $serverVersion = get_server_version(URI);

    if (
        version_compare($serverVersion, '3.2', '>=') &&
        version_compare($serverVersion, '4.0', '<')
    ) {
        exit("skip Server version '$serverVersion' does not support a getMore failpoint'");
    }
}

function skip_if_no_failcommand_failpoint()
{
    skip_if_test_commands_disabled();

    $serverVersion = get_server_version(URI);

    if (is_mongos(URI) && version_compare($serverVersion, '4.1.8', '<')) {
        exit("skip mongos version '$serverVersion' does not support 'failCommand' failpoint'");
    } elseif (version_compare($serverVersion, '4.0', '<')) {
        exit("skip mongod version '$serverVersion' does not support 'failCommand' failpoint'");
    }
}

function skip_if_no_mongo_orchestration()
{
    $ctx = stream_context_create(['http' => ['timeout' => 0.5]]);
    $result = @file_get_contents(MONGO_ORCHESTRATION_URI, false, $ctx);

    /* Note: file_get_contents emits an E_WARNING on failure, which will be
     * caught by the error handler in basic-skipif.inc. In that case, this may
     * never be reached. */
    if ($result === false) {
        exit("skip mongo-orchestration is not accessible: '" . MONGO_ORCHESTRATION_URI . "'");
    }
}
