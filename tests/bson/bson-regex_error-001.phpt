--TEST--
BSON BSON\Regex #001 error
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM handles parameter parsing differently"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . "\\Regex";

$regexp = new $classname("regexp", "i");
$regexp->getPattern(true);
$regexp->getFlags(true);

throws(function() use($classname) {
    new $classname;
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Warning: %s\Regex::getPattern() expects exactly 0 parameters, 1 given in %s on line %d

Warning: %s\Regex::getFlags() expects exactly 0 parameters, 1 given in %s on line %d
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===
