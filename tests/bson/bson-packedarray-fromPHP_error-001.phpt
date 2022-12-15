--TEST--
MongoDB\BSON\PackedArray::fromPHP(): requires a packed array
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    [1 => 1, 2, 3], // Keys: 1, 2, 3
    [1, 2 => 2, 3], // Keys: 0, 2, 3
    [0, 1, 'foo' => 'bar'], // Keys: 0, 1, bar
];

foreach ($tests as $test) {
    echo throws(function() use ($test) {
        MongoDB\BSON\PackedArray::fromPHP($test);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected value to be a list, but given array is not.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected value to be a list, but given array is not.
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected value to be a list, but given array is not.
===DONE===
