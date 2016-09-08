--TEST--
MongoDB\BSON\ObjectID serialization
--FILE--
<?php

var_dump($oid = new MongoDB\BSON\ObjectID('576c25db6118fd406e6e6471'));
var_dump($s = serialize($oid));
var_dump(unserialize($s));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "576c25db6118fd406e6e6471"
}
string(82) "C:21:"MongoDB\BSON\ObjectID":48:{a:1:{s:3:"oid";s:24:"576c25db6118fd406e6e6471";}}"
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "576c25db6118fd406e6e6471"
}
===DONE===
