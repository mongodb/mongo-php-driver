--TEST--
MongoDB\Driver\BulkWrite implements Countable
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$bulk = new MongoDB\Driver\BulkWrite;
var_dump($bulk instanceof Countable);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
===DONE===
