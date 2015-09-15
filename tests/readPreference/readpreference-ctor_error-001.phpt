--TEST--
MongoDB\Driver\ReadPreference construction (invalid arguments)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY, array("tag" => "one"));
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\Driver\ReadPreference(42);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid tagSet
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Invalid ReadPreference
===DONE===
