--TEST--
MongoDB\Driver\ReadConcern can be cloned
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$readConcern = new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::LOCAL);
$clone = clone $readConcern;

var_dump($clone == $readConcern);
var_dump($clone === $readConcern);

unset($readConcern);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\Driver\ReadConcern)#%d (%d) {
  ["level"]=>
  string(5) "local"
}
===DONE===
