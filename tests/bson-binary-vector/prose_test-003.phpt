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

// PackedBit vector with padding:7 and all-zero bits
$b1 = new MongoDB\BSON\Binary("\x10\x07\x80", MongoDB\BSON\Binary::TYPE_VECTOR);
$v1 = $b1->toArray();

// PackedBit vector with padding:7 and all-zero bits (constructed from PHP array)
$b3 = MongoDB\BSON\Binary::fromVector([1], MongoDB\BSON\VectorType::PackedBit);
$v3 = $b3->toArray();

// Assert that both Binary and array representations are equal
var_dump($b1 == $b3);
var_dump($v1 === $v3);

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
===DONE===