--TEST--
MongoDB\Driver\Session::endSession() Calling method multiple times
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php skip_if_not_live(); ?>
<?php skip_if_server_version('<', '3.6'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(URI);

$sessionA = $manager->startSession();
$sessionA->endSession();
$sessionA->endSession();
$sessionA->endSession();
?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
