--TEST--
PHPC-1151: Segfault if session unset before cursor is killed (find)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_libmongoc_crypto(); ?>
<?php NEEDS('STANDALONE'); ?>
<?php NEEDS_ATLEAST_MONGODB_VERSION(STANDALONE, "3.6"); ?>
<?php CLEANUP(STANDALONE); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['_id' => 1]);
$bulk->insert(['_id' => 2]);
$bulk->insert(['_id' => 3]);
$manager->executeBulkWrite(NS, $bulk);

$query = new MongoDB\Driver\Query([], ['batchSize' => 2]);
$session = $manager->startSession();

$cursor = $manager->executeQuery(NS, $query, ['session' => $session]);
unset($session);
unset($cursor);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
===DONE===
