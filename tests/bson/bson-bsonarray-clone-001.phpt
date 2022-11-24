--TEST--
MongoDB\BSON\BSONArray can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bson = MongoDB\BSON\BSONArray::fromPHP([1, 2, 3]);
$clone = clone $bson;

var_dump($clone == $bson);
var_dump($clone === $bson);

unset($bson);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\BSONArray)#%d (%d) {
  ["data"]=>
  string(26) "%s"
  ["length"]=>
  int(26)
}
===DONE===
