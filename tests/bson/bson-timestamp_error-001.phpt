--TEST--
MongoDB\BSON\Timestamp argument count errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    new MongoDB\BSON\Timestamp;
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n"

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Timestamp::__construct() expects exactly 2 %r(argument|parameter)%rs, 0 given
===DONE===

