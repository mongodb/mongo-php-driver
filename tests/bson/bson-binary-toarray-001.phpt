--TEST--
MongoDB\BSON\Binary::toArray()
--FILE--
<?php

var_dump(MongoDB\BSON\Binary::fromVector([1.0, -1.0, 0.5, -0.5], MongoDB\BSON\VectorType::Float32)->toArray());
var_dump(MongoDB\BSON\Binary::fromVector([1, 2, 3, 4], MongoDB\BSON\VectorType::Int8)->toArray());
var_dump(MongoDB\BSON\Binary::fromVector([1, 0, true, false], MongoDB\BSON\VectorType::PackedBit)->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
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
array(4) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
  [3]=>
  int(4)
}
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
===DONE===
