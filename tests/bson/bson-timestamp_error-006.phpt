--TEST--
MongoDB\BSON\Timestamp constructor requires integer or string arguments
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$invalidValues = [null, 1234.5, true, [], new stdClass];

foreach ($invalidValues as $invalidValue) {
    echo throws(function() use ($invalidValue) {
        new MongoDB\BSON\Timestamp($invalidValue, 0);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
}

foreach ($invalidValues as $invalidValue) {
    echo throws(function() use ($invalidValue) {
        new MongoDB\BSON\Timestamp(0, $invalidValue);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected increment to be an unsigned 32-bit integer or string, null given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected increment to be an unsigned 32-bit integer or string, float given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected increment to be an unsigned 32-bit integer or string, bool given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected increment to be an unsigned 32-bit integer or string, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected increment to be an unsigned 32-bit integer or string, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected timestamp to be an unsigned 32-bit integer or string, null given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected timestamp to be an unsigned 32-bit integer or string, float given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected timestamp to be an unsigned 32-bit integer or string, bool given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected timestamp to be an unsigned 32-bit integer or string, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected timestamp to be an unsigned 32-bit integer or string, stdClass given
===DONE===
