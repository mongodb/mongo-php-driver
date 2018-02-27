--TEST--
MongoDB\Driver\BulkWrite::count() should return the number of operations
--FILE--
<?php

$bulk = new MongoDB\Driver\BulkWrite;
var_dump($bulk->count());

$bulk->insert(['x' => 1]);
var_dump($bulk->count());

$bulk->insert(['x' => 2]);
var_dump($bulk->count());

$bulk->update(['x' => 3], ['$set' => ['y' => 3]]);
var_dump($bulk->count());

$bulk->update(['x' => 4], ['$set' => ['y' => 4]]);
var_dump($bulk->count());

$bulk->delete(['x' => 5]);
var_dump($bulk->count());

$bulk->delete(['x' => 6]);
var_dump($bulk->count());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
int(0)
int(1)
int(2)
int(3)
int(4)
int(5)
int(6)
===DONE===
