--TEST--
MongoDB\Driver\Session spec test: snapshot option is incompatible with writes
--DESCRIPTION--
PHPC-1875: Disable writes on snapshot sessions
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '5.0'); ?>
<?php skip_if_not_replica_set(); /* TODO: SERVER-58176 */ ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession(['snapshot' => true]);

$bulk = new MongoDB\Driver\BulkWrite();
$bulk->insert(['x' => 1]);

try {
    $manager->executeBulkWrite(NS, $bulk, ['session' => $session]);
} catch (MongoDB\Driver\Exception\BulkWriteException $e) {
    /* Note: we intentionally do not assert the server's error message for the
     * client specifying a read concern on a write command. It is sufficient to
     * assert that the error code is InvalidOptions(72). */
    var_dump($e->getCode() === 72);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
