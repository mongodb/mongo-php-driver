--TEST--
MongoDB\BSON\Binary::fromVector() construction errors for VectorType::Int8
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\BSON\Binary::fromVector([1 => 1], MongoDB\BSON\VectorType::Int8);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\BSON\Binary::fromVector([1, 2.0], MongoDB\BSON\VectorType::Int8);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\BSON\Binary::fromVector([1, -129], MongoDB\BSON\VectorType::Int8);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\BSON\Binary::fromVector([1, 128], MongoDB\BSON\VectorType::Int8);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected vector to be a list
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected vector[1] to be an integer, float given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected vector[1] to be a signed 8-bit integer, -129 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected vector[1] to be a signed 8-bit integer, 128 given
===DONE===
