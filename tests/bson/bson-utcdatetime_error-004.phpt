--TEST--
MongoDB\BSON\UTCDateTime constructor requires integer or string argument
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

/* UTCDateTime::__construct() internally converts floats to integers, so we will
 * not use a float to test for an invalid value. We also don't test an object,
 * since that is used for validating a possible DateTimeInterface argument. */
$invalidValues = [
    true,
    [],
    // Numeric strings are no longer supported as of 2.0
    '1416445411987',
    '1234.5678',
];

foreach ($invalidValues as $invalidValue) {
    echo throws(
        fn () => new MongoDB\BSON\UTCDateTime($invalidValue),
        MongoDB\Driver\Exception\InvalidArgumentException::class,
    ), PHP_EOL;
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer or object, bool given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer or object, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer or object, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer or object, string given
===DONE===
