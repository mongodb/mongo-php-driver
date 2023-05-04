--TEST--
MongoDB\BSON\PackedArray debug output
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

var_dump(MongoDB\BSON\PackedArray::fromPHP([]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(8) "BQAAAAA="
  ["value"]=>
  array(0) {
  }
}
===DONE===
