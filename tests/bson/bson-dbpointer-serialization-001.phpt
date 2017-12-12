--TEST--
MongoDB\BSON\DBPointer serialization
--FILE--
<?php

$test = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b4050000" }  }} }'))->dbref;

var_dump($test);
var_dump($s = serialize($test));
var_dump(unserialize($s));
echo "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\DBPointer)#1 (2) {
  ["ref"]=>
  string(11) "phongo.test"
  ["id"]=>
  string(24) "5a2e78accd485d55b4050000"
}
string(111) "C:22:"MongoDB\BSON\DBPointer":76:{a:2:{s:3:"ref";s:11:"phongo.test";s:2:"id";s:24:"5a2e78accd485d55b4050000";}}"
object(MongoDB\BSON\DBPointer)#2 (2) {
  ["ref"]=>
  string(11) "phongo.test"
  ["id"]=>
  string(24) "5a2e78accd485d55b4050000"
}

===DONE===
