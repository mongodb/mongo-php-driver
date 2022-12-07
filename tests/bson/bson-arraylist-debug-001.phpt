--TEST--
MongoDB\BSON\ArrayList debug output
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

var_dump(MongoDB\BSON\ArrayList::fromPHP([]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\ArrayList)#%d (%d) {
  ["data"]=>
  string(8) "BQAAAAA="
  ["length"]=>
  int(5)
}
===DONE===
