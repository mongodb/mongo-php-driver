--TEST--
BSON BSON\UTCDateTime #001 error
--INI--
date.timezone=America/Los_Angeles
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM handles parameter parsing differently"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

throws(function() {
    $classname = BSON_NAMESPACE . "\\UTCDateTime";
    new $classname;
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===
