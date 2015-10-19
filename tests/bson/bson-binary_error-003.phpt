--TEST--
BSON BSON\Binary cannot be serialized
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

use MongoDB\BSON as BSON;

echo throws(function() {
    serialize(new BSON\Binary('foo', BSON\Binary::TYPE_GENERIC));
}, 'Exception'), "\n";

echo throws(function() {
    $classname = BSON_NAMESPACE . '\Binary';
    unserialize(sprintf('C:%d:"%s":0:{}', strlen($classname), $classname));
}, 'Exception'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got Exception
Serialization of '%SBSON\Binary' is not allowed
OK: Got Exception
Unserialization of '%SBSON\Binary' is not allowed
===DONE===
