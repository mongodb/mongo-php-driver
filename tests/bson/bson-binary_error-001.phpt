--TEST--
BSON BSON\Binary #001 error
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM handles parameter parsing differently"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . "\\Binary";

$binary = new $classname("random binary data", $classname::TYPE_GENERIC);
$binary->getData(2);
$binary->getType(2);

throws(function() use($classname) {
    new $classname("random binary data without type");
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Warning: %s\Binary::getData() expects exactly 0 parameters, 1 given in %s on line %d

Warning: %s\Binary::getType() expects exactly 0 parameters, 1 given in %s on line %d
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===

