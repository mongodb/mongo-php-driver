--TEST--
MongoDB\Driver\Manager::__construct(): Specifying a driver option implicitly enables TLS
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_ssl(['OpenSSL', 'LibreSSL']); ?>
<?php skip_if_ssl(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

// Ensure that the server is up to ensure we're not hitting a different failure below
$manager = create_test_manager();
$manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => 1]));

$manager = create_test_manager(URI, [], ['ca_dir' => 'foo']);

throws(function () use ($manager) {
    // Note that this command will not fail if the server was configured with allowSSL or preferSSL for net.ssl.mode.
    $manager->executeCommand(DATABASE_NAME, new MongoDB\Driver\Command(['ping' => true]));
}, MongoDB\Driver\Exception\ConnectionException::class);

?>
===DONE===
--EXPECTF--
OK: Got MongoDB\Driver\Exception\ConnectionException
===DONE===
