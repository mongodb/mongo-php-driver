--TEST--
MongoDB\BSON\BSONDocument can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$bson = MongoDB\BSON\BSONDocument::fromJSON('{ "foo": [ 1, 2, 3 ]}');
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
object(MongoDB\BSON\BSONDocument)#%d (%d) {
  ["data"]=>
  string(36) "%s"
  ["length"]=>
  int(36)
}
===DONE===
