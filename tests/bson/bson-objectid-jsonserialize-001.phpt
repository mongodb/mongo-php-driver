--TEST--
MongoDB\BSON\ObjectID::jsonSerialize() return value
--FILE--
<?php

$oid = new MongoDB\BSON\ObjectID('5820ca4bef62d52d9924d0d8');
var_dump($oid->jsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
array(1) {
  ["$oid"]=>
  string(24) "5820ca4bef62d52d9924d0d8"
}
===DONE===
