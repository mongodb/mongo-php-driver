--TEST--
MongoDB\Driver\BulkWriteCommand::deleteMany() prohibits PackedArray for document values
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$bulk = new MongoDB\Driver\BulkWriteCommand;

echo throws(function() use ($bulk) {
    $bulk->deleteMany(NS, MongoDB\BSON\PackedArray::fromPHP([]));
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

echo throws(function() use ($bulk) {
    $bulk->deleteMany(NS, [], ['collation' => MongoDB\BSON\PackedArray::fromPHP([])]);
}, MongoDB\Driver\Exception\UnexpectedValueException::class), "\n";

// Expected "hint" option to yield string or document but got "array"
echo throws(function() use ($bulk) {
    $bulk->deleteMany(NS, [], ['hint' => MongoDB\BSON\PackedArray::fromPHP([])]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
MongoDB\BSON\PackedArray cannot be serialized as a root document
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "hint" option to yield string or document but got "array"
===DONE===
