--TEST--
MongoDB\Driver\WriteConcern construction (invalid w type)
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = array(
    1.0,
    true,
    array(),
    new stdClass,
    null,
);

foreach ($tests as $test) {
    echo throws(function() use ($test) {
        new MongoDB\Driver\WriteConcern($test);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, float given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, bool given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, null given
===DONE===
