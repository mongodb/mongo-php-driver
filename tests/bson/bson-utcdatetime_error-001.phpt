--TEST--
BSON BSON\UTCDateTime #001 error
--INI--
date.timezone=America/Los_Angeles
--SKIPIF--
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM handles parameter parsing differently"); ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"; CLEANUP(STANDALONE) ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(STANDALONE);

$classname = BSON_NAMESPACE . "\\UTCDateTime";

throws(function() use($classname) {
    $d = new $classname;
}, "MongoDB\\Driver\\Exception\\InvalidArgumentException");
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===
