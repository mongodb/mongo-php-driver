--TEST--
MongoDB\BSON\DBPointer can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$test = MongoDB\BSON\toPHP(MongoDB\BSON\fromJSON('{ "dbref": {"$dbPointer": {"$ref": "phongo.test", "$id" : { "$oid" : "5a2e78accd485d55b405ac12" }  }} }'));

$dbPointer = $test->dbref;
$dbPointer->foo = 'bar';
$clone = clone($dbPointer);

var_dump($clone == $dbPointer);
var_dump($clone === $dbPointer);

unset($dbPointer);

var_dump($clone);
var_dump($clone->foo);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\DBPointer)#%d (2) {
  ["ref"]=>
  string(11) "phongo.test"
  ["id"]=>
  string(24) "5a2e78accd485d55b405ac12"
}
string(3) "bar"
===DONE===
