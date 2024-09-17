--TEST--
MongoDB\BSON\PackedArray::fromJSON(): invalid JSON
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\BSON\PackedArray::fromJSON('foo');
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), PHP_EOL;

echo throws(function() {
    MongoDB\BSON\PackedArray::fromJSON('{ "foo": "bar" }');
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), PHP_EOL;

echo throws(function() {
    MongoDB\BSON\PackedArray::fromJSON('{ "00": "bar", "1": "bar" }');
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), PHP_EOL;

echo throws(function() {
    MongoDB\BSON\PackedArray::fromJSON('{ "0": "bar", "foo": "bar" }');
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), PHP_EOL;

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Got parse error at "o", position 1: "SPECIAL_EXPECTED"
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Received invalid JSON array: expected key 0, but found "foo"
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Received invalid JSON array: expected key 0, but found "00"
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Received invalid JSON array: expected key 1, but found "foo"
===DONE===
