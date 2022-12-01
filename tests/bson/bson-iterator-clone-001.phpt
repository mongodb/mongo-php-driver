--TEST--
MongoDB\BSON\Iterator can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$iterator = MongoDB\BSON\Document::fromJSON('{ "foo": [ 1, 2, 3 ]}')->getIterator();
$clone = clone $iterator;

var_dump($clone == $iterator);
var_dump($clone === $iterator);

unset($iterator);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\Iterator)#%d (%d) {
  ["bson"]=>
  object(MongoDB\BSON\Document)#%d (%d) {
    ["data"]=>
    string(36) "%s"
    ["length"]=>
    int(36)
  }
}
===DONE===
