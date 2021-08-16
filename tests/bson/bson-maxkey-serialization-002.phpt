--TEST--
MongoDB\BSON\MaxKey serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

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
