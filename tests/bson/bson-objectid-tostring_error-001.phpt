--TEST--
MongoDB\BSON\ObjectId raises warning on invalid arguments
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$pregenerated = new MongoDB\BSON\ObjectId("53e28b650640fd3162152de1");

echo throws(function()  use($pregenerated) {
    $pregenerated->__toString(1);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\ObjectId::__toString() expects exactly 0 parameters, 1 given
===DONE===
