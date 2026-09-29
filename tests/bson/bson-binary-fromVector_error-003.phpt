--TEST--
MongoDB\BSON\Binary::fromVector() construction errors for VectorType::PackedBit
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\BSON\Binary::fromVector([1 => 1], MongoDB\BSON\VectorType::PackedBit);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\BSON\Binary::fromVector([true, 1.0], MongoDB\BSON\VectorType::PackedBit);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    MongoDB\BSON\Binary::fromVector([true, 2], MongoDB\BSON\VectorType::PackedBit);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected vector to be a list
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected vector[1] to be 0, 1, or a boolean, float given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected vector[1] to be 0 or 1, 2 given
===DONE===
