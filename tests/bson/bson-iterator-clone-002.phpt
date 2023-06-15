--TEST--
MongoDB\BSON\Iterator is rewound when cloning
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$iterator = MongoDB\BSON\Document::fromJSON('[ 1, 2, 3 ]')->getIterator();
$iterator->next();

$clone = clone $iterator;

var_dump($iterator->current());
var_dump($clone->current());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(2)
}
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "32-bit integer"
  ["value"]=>
  int(1)
}
===DONE===
