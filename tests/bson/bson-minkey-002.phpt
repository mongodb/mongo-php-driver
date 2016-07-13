--TEST--
MongoDB\BSON\MinKey serialization
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_dump($minkey = new MongoDB\BSON\MinKey);
var_dump($s = serialize($minkey));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\MinKey)#%d (%d) {
}
string(31) "O:19:"MongoDB\BSON\MinKey":0:{}"
object(MongoDB\BSON\MinKey)#%d (%d) {
}
===DONE===
