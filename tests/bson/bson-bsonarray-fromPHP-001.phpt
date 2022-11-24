--TEST--
MongoDB\BSON\BSONArray::fromPHP(): create from PHP array
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

var_dump(MongoDB\BSON\BSONArray::fromPHP([1, 2, 3]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\BSONArray)#%d (%d) {
  ["data"]=>
  string(26) "%s"
  ["length"]=>
  int(26)
}
===DONE===
