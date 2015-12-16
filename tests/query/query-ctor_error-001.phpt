--TEST--
MongoDB\Driver\ReadConcern construction (invalid readConcern type)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    1,
    1.0,
    'string',
    true,
    [],
    new stdClass,
    null,
];

foreach ($tests as $test) {
    echo throws(function() use ($test) {
        new MongoDB\Driver\Query([], ['readConcern' => $test]);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, integer given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, %r(double|float)%r given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, string given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, boolean given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, object given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Expected "readConcern" option to be MongoDB\Driver\ReadConcern, null given
===DONE===
