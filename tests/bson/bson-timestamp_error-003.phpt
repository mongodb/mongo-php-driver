--TEST--
MongoDB\BSON\Timestamp constructor requires positive unsigned 32-bit integers
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\BSON\Timestamp(-1, 0);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    /* I realise that "-2147483647 - 1" could be written as "-2147483648", *however*, PHP considers
     * the latter a floating point number, as it parses "-" and "2147483648" separately, and
     * "2147483648" doesn't fit in the 32-bit signed range. */
    new MongoDB\BSON\Timestamp(-2147483647 - 1, 0);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\Timestamp(0, -1);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

echo throws(function() {
    new MongoDB\BSON\Timestamp(0, -2147483647 - 1);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected increment to be an unsigned 32-bit integer, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected increment to be an unsigned 32-bit integer, -2147483648 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected timestamp to be an unsigned 32-bit integer, -1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected timestamp to be an unsigned 32-bit integer, -2147483648 given
===DONE===
