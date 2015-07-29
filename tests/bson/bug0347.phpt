--TEST--
Test for PHPC-347: Memory leak decoding empty buffer
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

try {
    var_dump(toPHP(''));
} catch (MongoDB\Driver\Exception\UnexpectedValueException $e) {
    echo $e->getMessage(), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Could not read document from BSON reader
===DONE===
