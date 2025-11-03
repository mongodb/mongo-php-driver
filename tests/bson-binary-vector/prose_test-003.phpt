--TEST--
Binary vector: PackedBit vector comparisons
--DESCRIPTION--
Binary vector prose test #3
https://github.com/mongodb/specifications/blob/master/source/bson-binary-vector/tests/README.md#prose-tests
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* Although PHPC does not allow constructing a vector with non-zero, padded
 * bits, we can still test comparisons between two valid vector objects
 * constructed from raw data and a PHP array, respectively. */
$b1 = new MongoDB\BSON\Binary("\x10\x07\x80", MongoDB\BSON\Binary::TYPE_VECTOR);

// PackedBit vector with padding:7 and all-zero bits (constructed from PHP array)
$b2 = MongoDB\BSON\Binary::fromVector([1], MongoDB\BSON\VectorType::PackedBit);

var_dump($b1 == $b2);
var_dump($b1->toArray() === $b2->toArray());

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
===DONE===