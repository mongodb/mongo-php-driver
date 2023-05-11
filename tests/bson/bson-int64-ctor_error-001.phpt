--TEST--
MongoDB\BSON\Int64 constructor with invalid data
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    '0x2', // Hex data
    '9223372036854775807LL', // int64 suffix
    '9223372036854775808', // int64 max + 1
    '-9223372036854775809', // int64 min - 1
    1.5, // Float
];

foreach($tests as $test) {
    echo throws(function () use ($test) {
        new MongoDB\BSON\Int64($test);
    }, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "0x2" as 64-bit integer for MongoDB\BSON\Int64 initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "9223372036854775807LL" as 64-bit integer for MongoDB\BSON\Int64 initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "9223372036854775808" as 64-bit integer for MongoDB\BSON\Int64 initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing "-9223372036854775809" as 64-bit integer for MongoDB\BSON\Int64 initialization
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected value to be integer or string, float given
===DONE===
