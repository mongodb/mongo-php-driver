--TEST--
PHPC-487: check_closed stream handler should not report open socket as closed
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; SLOW(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = new MongoDB\Driver\Manager(STANDALONE);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$wr = $m->executeBulkWrite(NS, $bulk);
var_dump($wr->getInsertedCount());

sleep(1);

$bulk = new MongoDB\Driver\BulkWrite;
$bulk->insert(['x' => 1]);
$wr = $m->executeBulkWrite(NS, $bulk);
var_dump($wr->getInsertedCount());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(1)
int(1)
===DONE===
