--TEST--
MongoDB\BSON\ObjectId serialization
--FILE--
<?php

var_dump($oid = new MongoDB\BSON\ObjectId('576c25db6118fd406e6e6471'));
var_dump($s = serialize($oid));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "576c25db6118fd406e6e6471"
}
string(82) "C:21:"MongoDB\BSON\ObjectId":48:{a:1:{s:3:"oid";s:24:"576c25db6118fd406e6e6471";}}"
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "576c25db6118fd406e6e6471"
}
===DONE===
