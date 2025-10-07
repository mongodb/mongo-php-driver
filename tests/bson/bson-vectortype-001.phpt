--TEST--
MongoDB\BSON\VectorType cases
--FILE--
<?php

foreach (MongoDB\BSON\VectorType::cases() as $case) {
    var_dump($case);
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
enum(MongoDB\BSON\VectorType::Float32)
enum(MongoDB\BSON\VectorType::Int8)
enum(MongoDB\BSON\VectorType::PackedBit)
===DONE===
