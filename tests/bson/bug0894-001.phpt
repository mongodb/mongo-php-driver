--TEST--
PHPC-849: BSON get_properties handlers leak during gc_possible_root() checks
--FILE--
<?php

$objects = [
    new MongoDB\BSON\Binary('foo', MongoDB\BSON\Binary::TYPE_GENERIC),
    new MongoDB\BSON\Decimal128('3.14'),
    new MongoDB\BSON\Javascript('function foo() { return bar; }', ['bar' => 42]),
    new MongoDB\BSON\MaxKey,
    new MongoDB\BSON\MinKey,
    new MongoDB\BSON\ObjectId,
    new MongoDB\BSON\Regex('foo', 'i'),
    new MongoDB\BSON\Timestamp(1234, 5678),
    new MongoDB\BSON\UTCDateTime,
];

printf("Created array of %d BSON objects\n", count($objects));

gc_collect_cycles();

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
Created array of 9 BSON objects
===DONE===
