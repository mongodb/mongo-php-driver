--TEST--
MongoDB\BSON\ObjectID::__set_state()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_export(MongoDB\BSON\ObjectID::__set_state([
   'oid' => '576c25db6118fd406e6e6471',
]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\BSON\ObjectID::__set_state(array(
   'oid' => '576c25db6118fd406e6e6471',
))
===DONE===
