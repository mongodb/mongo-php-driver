--TEST--
MongoDB\Driver\BulkWrite::delete() prohibits PackedArray for document values
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$bulk = new MongoDB\Driver\BulkWrite;

echo throws(function() use ($bulk) {
    $bulk->delete(MongoDB\BSON\PackedArray::fromPHP([]));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo throws(function() use ($bulk) {
    $bulk->delete([], ['collation' => MongoDB\BSON\PackedArray::fromPHP([])]);
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo throws(function() use ($bulk) {
    $bulk->delete([], ['hint' => MongoDB\BSON\PackedArray::fromPHP([])]);
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
===DONE===
