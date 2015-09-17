--TEST--
MongoDB\Driver\WriteConcern construction (invalid w type)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

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
Expected w to be integer or string, double given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, object given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected w to be integer or string, %r(null|NULL)%r given
===DONE===
