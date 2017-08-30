--TEST--
MongoDB\BSON\ObjectId #002 generates ObjectId for null or missing constructor argument
--FILE--
<?php

var_dump(new MongoDB\BSON\ObjectId);
var_dump(new MongoDB\BSON\ObjectId(null));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
object(MongoDB\BSON\ObjectId)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
===DONE===
