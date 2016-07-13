--TEST--
MongoDB\BSON\MaxKey::__set_state()
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_export(MongoDB\BSON\MaxKey::__set_state([]));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
MongoDB\BSON\MaxKey::__set_state(array(
))
===DONE===
