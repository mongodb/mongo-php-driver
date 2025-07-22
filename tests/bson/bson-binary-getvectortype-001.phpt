--TEST--
MongoDB\BSON\Binary::getVectorType()
--FILE--
<?php

var_dump(MongoDB\BSON\Binary::fromVector([1.0, -1.0, 0.5, -0.5], MongoDB\BSON\VectorType::Float32)->getVectorType());
var_dump(MongoDB\BSON\Binary::fromVector([1, 2, 3, 4], MongoDB\BSON\VectorType::Int8)->getVectorType());
var_dump(MongoDB\BSON\Binary::fromVector([1, 0, true, false], MongoDB\BSON\VectorType::PackedBit)->getVectorType());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
enum(MongoDB\BSON\VectorType::Float32)
enum(MongoDB\BSON\VectorType::Int8)
enum(MongoDB\BSON\VectorType::PackedBit)
===DONE===

