--TEST--
MongoDB\BSON\UTCDateTime constructor type validation
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$invalidValues = [
    true,
    [],
    // Numeric strings are no longer supported as of 2.0
    '1416445411987',
    '1234.5678',
    // Floats are no longer supported as of 2.0
    1234.5678,
    // Only DateTimeInterface or MongoDB\BSON\Int64 is accepted
    new stdClass,
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
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer or object, float given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected instance of DateTimeInterface or MongoDB\BSON\Int64, stdClass given
===DONE===
