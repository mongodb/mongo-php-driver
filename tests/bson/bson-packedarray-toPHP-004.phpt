--TEST--
MongoDB\BSON\PackedArray::toPHP(): Use bson as root type
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$packedArray = MongoDB\BSON\PackedArray::fromPHP([1, 2, 3]);
var_dump($packedArray->toPHP(['root' => 'bson']));

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
