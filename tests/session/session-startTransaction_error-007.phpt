--TEST--
MongoDB\Driver\Session::startTransaction() throws an error on sharded clusters < 4.2
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto() ?>
<?php skip_if_not_sharded_cluster_with_replica_set(); ?>
<?php skip_if_server_version('>=', '4.2'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession();

echo throws(function () use ($session) {
    $session->startTransaction();
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\RuntimeException
Multi-document transactions are not supported by this server version
===DONE===
