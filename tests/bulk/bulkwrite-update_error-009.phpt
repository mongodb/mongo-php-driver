--TEST--
MongoDB\Driver\BulkWrite::update() with multi:true prohibits sort option
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = create_test_manager();
$bulk = new MongoDB\Driver\BulkWrite;

echo throws(function() use ($manager, $bulk) {
    $bulk->update(['x' => ['$gt' => 1]], ['$set' => ['y' => 11]], ['multi' => true, 'sort' => ['x' => 1]]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid option 'sort'
===DONE===
