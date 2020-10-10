--TEST--
MongoDB\Driver\ReadConcern construction (invalid level type)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
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
%SMongoDB\Driver\ReadConcern::__construct()%sstring, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
%SMongoDB\Driver\ReadConcern::__construct()%sstring, %r(object|stdClass)%r given
===DONE===
