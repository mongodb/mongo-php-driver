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

/* Session is not pinned to a server until a transaction operation is executed
 * on a sharded cluster. Returns null when the session is not pinned. */
var_dump($session->getServer());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
NULL
===DONE===
