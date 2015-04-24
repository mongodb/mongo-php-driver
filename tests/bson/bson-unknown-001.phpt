--TEST--
BSON Serializing a PHP resource should throw exception
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

throws(function() {
    $a = array("stderr" => STDERR);

    $b = BSON\fromArray($a);
}, "MongoDB\Driver\Exception\UnexpectedValueException");

throws(function() {
    $a = array("stderr" => STDERR, "stdout" => STDOUT);

    $b = BSON\fromArray($a);
}, "MongoDB\Driver\Exception\UnexpectedValueException");


?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
OK: Got MongoDB\Driver\Exception\UnexpectedValueException
===DONE===
