--TEST--
MongoDB\BSON\PackedArray::toPHP(): int64 values are returned as Int64 instances
--FILE--
<?php

$bson = MongoDB\BSON\PackedArray::fromPHP([
    123,
    new MongoDB\BSON\Int64(123),
]);

var_dump($bson->toPHP());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
array(2) {
  [0]=>
  int(123)
  [1]=>
  object(MongoDB\BSON\Int64)#%d (1) {
    ["integer"]=>
    string(3) "123"
  }
}
===DONE===
