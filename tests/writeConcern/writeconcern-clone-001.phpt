--TEST--
MongoDB\Driver\WriteConcern can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$writeConcern = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY, 1000, true);
$clone = clone $writeConcern;

var_dump($clone == $writeConcern);
var_dump($clone === $writeConcern);

unset($writeConcern);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  string(8) "majority"
  ["j"]=>
  bool(true)
  ["wtimeout"]=>
  int(1000)
}
===DONE===
