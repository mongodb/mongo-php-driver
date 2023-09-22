--TEST--
MongoDB\BSON\PackedArray array access does not allow writing (ArrayAccess methods)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$array = MongoDB\BSON\PackedArray::fromPHP(["foo", new MongoDB\BSON\Int64(123)]);

// Writing causes exception
echo throws(function() use ($array) {
    $array->offsetSet(0, 'bar');
}, MongoDB\Driver\Exception\LogicException::class), "\n";

// Unsetting causes exception
echo throws(function() use ($array) {
    $array->offsetUnset(0);
}, MongoDB\Driver\Exception\LogicException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\LogicException
Cannot write to MongoDB\BSON\PackedArray offset
OK: Got MongoDB\Driver\Exception\LogicException
Cannot unset MongoDB\BSON\PackedArray offset
===DONE===
