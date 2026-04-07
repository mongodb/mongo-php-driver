--TEST--
MongoDB\Driver\Session::getServer()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$session = $manager->startSession();

/* Session::getServer() returns null, as sessions are not pinned to a server.
 * Note: mongos pinning for sharded transactions was removed in MongoDB 6.0. */
var_dump($session->getServer());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
NULL
===DONE===
