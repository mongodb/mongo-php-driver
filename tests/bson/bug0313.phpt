--TEST--
PHPC-313: BSON should throw when encountering 64-bit integer on 32-bit platform
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$tests = [
    '{ "x": { "$numberLong": "4294967294" }}',
    '{ "x": { "$numberLong": "4294967295" }}',
    '{ "x": { "$numberLong": "9223372036854775807" }}',
    '{ "longFieldName": { "$numberLong": "4294967294" }}',
    '{ "longFieldName": { "$numberLong": "4294967295" }}',
    '{ "longFieldName": { "$numberLong": "9223372036854775807" }}',
];

foreach ($tests as $json) {
    throws(function() use ($json) { var_dump(toPHP(fromJSON($json))); }, 'MongoDB\Driver\Exception\InvalidArgumentException');
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
===DONE===
