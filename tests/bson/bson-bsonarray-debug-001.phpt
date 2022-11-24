--TEST--
MongoDB\BSON\BSONArray debug output
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

var_dump(MongoDB\BSON\BSONArray::fromPHP([]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\BSONArray)#%d (%d) {
  ["data"]=>
  string(5) "%s"
  ["length"]=>
  int(5)
}
===DONE===
