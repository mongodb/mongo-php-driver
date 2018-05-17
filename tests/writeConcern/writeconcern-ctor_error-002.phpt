--TEST--
MongoDB\Driver\WriteConcern construction (invalid w type)
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

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
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, %r(double|float)%r given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, bool%S given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, stdClass given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, %r(null|NULL)%r given
===DONE===
