--TEST--
MongoDB\BSON\ObjectID #002 generates ObjectID for null or missing constructor argument
--FILE--
<?php

var_dump(new MongoDB\BSON\ObjectID);
var_dump(new MongoDB\BSON\ObjectID(null));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
object(MongoDB\BSON\ObjectID)#%d (%d) {
  ["oid"]=>
  string(24) "%x"
}
===DONE===
