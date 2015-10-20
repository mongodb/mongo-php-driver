--TEST--
BSON BSON\Javascript cannot be serialized
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

echo throws(function() {
    serialize(new BSON\Javascript('function() {}'));
}, 'Exception'), "\n";

echo throws(function() {
    $classname = BSON_NAMESPACE . '\Javascript';
    unserialize(sprintf('C:%d:"%s":0:{}', strlen($classname), $classname));
}, 'Exception'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got Exception
Serialization of '%SBSON\Javascript' is not allowed
OK: Got Exception
Unserialization of '%SBSON\Javascript' is not allowed
===DONE===
