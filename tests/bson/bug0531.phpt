--TEST--
PHPC-531: Segfault due to double free by corrupt BSON visitor
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$bson = fromPHP(["hello" => "world"]);
$bson[4] = 1;

echo throws(function() use ($bson) {
    toPHP($bson);
}, 'MongoDB\Driver\Exception\UnexpectedValueException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
Could not convert BSON document to a PHP variable
===DONE===
