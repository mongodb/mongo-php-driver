--TEST--
MongoDB\BSON\DBPointer debug handler
--FILE--
<?php

$dbptr = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b405ac12" }  }} }'));
var_dump($dbptr->dbref);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\DBPointer)#1 (2) {
  ["ref"]=>
  string(11) "phongo.test"
  ["id"]=>
  string(24) "5a2e78accd485d55b405ac12"
}
===DONE===
