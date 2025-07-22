--TEST--
MongoDB\BSON\VectorType
--FILE--
<?php

var_dump(MongoDB\BSON\VectorType::Float32);
var_dump(MongoDB\BSON\VectorType::Int8);
var_dump(MongoDB\BSON\VectorType::PackedBit);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
enum(MongoDB\BSON\VectorType::Float32)
enum(MongoDB\BSON\VectorType::Int8)
enum(MongoDB\BSON\VectorType::PackedBit)
===DONE===
