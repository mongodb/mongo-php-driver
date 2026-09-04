--TEST--
MongoDB\BSON\Binary::fromVector() construction errors for VectorType::Float32
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\BSON\Binary::fromVector([1 => 1.0], MongoDB\BSON\VectorType::Float32);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\BSON\Binary::fromVector([1.0, 2], MongoDB\BSON\VectorType::Float32);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected vector to be a list
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected vector[1] to be a float, int given
===DONE===
