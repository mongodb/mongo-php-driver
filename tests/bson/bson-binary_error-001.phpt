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

throws(function() use($classname) {
    $b = new $classname("random binary data without type");
    echo "FAIL: Constructed BSON\Binary without type!\n";
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

$binary->getData(2);
$binary->getType(2);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException

Warning: %s\Binary::getData() expects exactly 0 parameters, 1 given in %s on line %d

Warning: %s\Binary::getType() expects exactly 0 parameters, 1 given in %s on line %d
===DONE===

