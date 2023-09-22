--TEST--
MongoDB\BSON\PackedArray array access checks types (dimension object handlers)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$array = MongoDB\BSON\PackedArray::fromPHP(["foo", new MongoDB\BSON\Int64(123)]);

echo throws(function() use ($array) {
    $array['foo'];
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

echo throws(function() use ($array) {
    $array['0'];
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

echo throws(function() use ($array) {
    $array[0.1];
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

echo throws(function() use ($array) {
    $array[false];
}, MongoDB\Driver\Exception\RuntimeException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\RuntimeException
Could not find index of type "string" in BSON array
OK: Got MongoDB\Driver\Exception\RuntimeException
Could not find index of type "string" in BSON array
OK: Got MongoDB\Driver\Exception\RuntimeException
Could not find index of type "float" in BSON array
OK: Got MongoDB\Driver\Exception\RuntimeException
Could not find index of type "bool" in BSON array
===DONE===
