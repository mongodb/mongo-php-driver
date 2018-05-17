--TEST--
MongoDB\BSON\UTCDateTime constructor requires integer or string argument
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

/* UTCDateTime::__construct() internally converts floats to integers, so we will
 * not use a float to test for an invalid value. We also don't test an object,
 * since that is used for validating a possible DateTimeInterface argument. */
$invalidValues = [true, []];

foreach ($invalidValues as $invalidValue) {
    echo throws(function() use ($invalidValue) {
        new MongoDB\BSON\UTCDateTime($invalidValue);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer or string, bool%S given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected integer or string, array given
===DONE===
