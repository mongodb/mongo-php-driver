--TEST--
MongoDB\Driver\ReadConcern construction (invalid level type)
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    [],
    new stdClass,
];

foreach ($tests as $test) {
    echo throws(function() use ($test) {
        new MongoDB\Driver\ReadConcern($test);
    }, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadConcern::__construct() expects parameter 1 to be string, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadConcern::__construct() expects parameter 1 to be string, object given
===DONE===
