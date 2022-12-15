--TEST--
MongoDB\BSON\Iterator cannot be serialized
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$array = MongoDB\BSON\PackedArray::fromPHP([0, 1, 'foo', (object) ['foo' => 'bar']]);
echo throws(function () use ($array) {
    serialize($array->getIterator());
}, \Exception::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got Exception
Serialization of 'MongoDB\BSON\Iterator' is not allowed
===DONE===
