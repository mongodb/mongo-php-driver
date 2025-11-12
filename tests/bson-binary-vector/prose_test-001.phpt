--TEST--
Binary vector: Encoding PackedBit vector with non-zero, padded bits fails
--DESCRIPTION--
Binary vector prose test #1
https://github.com/mongodb/specifications/blob/master/source/bson-binary-vector/tests/README.md#prose-tests
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

// PackedBit vector with padding:7 and non-zero bits is invalid
echo throws(function() {
    var_dump(new MongoDB\BSON\Binary("\x10\x07\xff", MongoDB\BSON\Binary::TYPE_VECTOR));
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

/* PHPC does not allow constructing a PackedBit vector with non-zero, padded
 * bits. As a result, the corresponding decoding test (i.e. prose test #2)
 * cannot be implemented. */

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Binary vector data is invalid
===DONE===
