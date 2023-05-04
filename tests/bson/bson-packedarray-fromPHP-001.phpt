--TEST--
MongoDB\BSON\PackedArray::fromPHP(): create from PHP array
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

var_dump(MongoDB\BSON\PackedArray::fromPHP([1, 2, 3]));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\PackedArray)#%d (%d) {
  ["data"]=>
  string(36) "GgAAABAwAAEAAAAQMQACAAAAEDIAAwAAAAA="
  ["value"]=>
  array(3) {
    [0]=>
    int(1)
    [1]=>
    int(2)
    [2]=>
    int(3)
  }
}
===DONE===
