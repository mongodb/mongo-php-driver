--TEST--
MongoDB\Driver\ReadConcern construction (invalid arguments)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

echo throws(function() {
    new MongoDB\Driver\ReadConcern("string", 1);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\Driver\ReadConcern::__construct() expects at most 1 %r(argument|parameter)%r, 2 given
===DONE===
