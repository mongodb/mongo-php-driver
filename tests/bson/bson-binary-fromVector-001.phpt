--TEST--
MongoDB\BSON\Binary::fromVector() construction of various vector types
--FILE--
<?php

var_dump(MongoDB\BSON\Binary::fromVector([1.0, -1.0, 0.5, -0.5], MongoDB\BSON\VectorType::Float32));
var_dump(MongoDB\BSON\Binary::fromVector([-128, 0, 1, 127], MongoDB\BSON\VectorType::Int8));
var_dump(MongoDB\BSON\Binary::fromVector([1, 0, true, false], MongoDB\BSON\VectorType::PackedBit));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(%d) "%a"
  ["type"]=>
  int(9)
  ["vector"]=>
  array(4) {
    [0]=>
    float(1)
    [1]=>
    float(-1)
    [2]=>
    float(0.5)
    [3]=>
    float(-0.5)
  }
  ["vectorType"]=>
  enum(MongoDB\BSON\VectorType::Float32)
}
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(%d) "%a"
  ["type"]=>
  int(9)
  ["vector"]=>
  array(4) {
    [0]=>
    int(-128)
    [1]=>
    int(0)
    [2]=>
    int(1)
    [3]=>
    int(127)
  }
  ["vectorType"]=>
  enum(MongoDB\BSON\VectorType::Int8)
}
object(MongoDB\BSON\Binary)#%d (%d) {
  ["data"]=>
  string(%d) "%a"
  ["type"]=>
  int(9)
  ["vector"]=>
  array(4) {
    [0]=>
    int(1)
    [1]=>
    int(0)
    [2]=>
    int(1)
    [3]=>
    int(0)
  }
  ["vectorType"]=>
  enum(MongoDB\BSON\VectorType::PackedBit)
}
===DONE===
