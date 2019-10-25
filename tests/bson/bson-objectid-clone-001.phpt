--TEST--
MongoDB\BSON\ObjectId can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$objectId = new MongoDB\BSON\ObjectId("53e2a1c40640fd72175d4603");
$objectId->foo = 'bar';
$clone = clone $objectId;

var_dump($clone == $objectId);
var_dump($clone === $objectId);

unset($objectId);

var_dump($clone);
var_dump($clone->foo);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\ObjectId)#%d (1) {
  ["oid"]=>
  string(24) "53e2a1c40640fd72175d4603"
}
string(3) "bar"
===DONE===
