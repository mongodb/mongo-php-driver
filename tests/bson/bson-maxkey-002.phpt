--TEST--
MongoDB\BSON\MaxKey serialization
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

var_dump($maxkey = new MongoDB\BSON\MaxKey);
var_dump($s = serialize($maxkey));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\MaxKey)#%d (%d) {
}
string(31) "O:19:"MongoDB\BSON\MaxKey":0:{}"
object(MongoDB\BSON\MaxKey)#%d (%d) {
}
===DONE===
