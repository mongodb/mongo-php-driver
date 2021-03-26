--TEST--
MongoDB\BSON\Regex argument count errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$regex = new MongoDB\BSON\Regex("regex", "i");

echo throws(function() use ($regex) {
    $regex->getPattern(true);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() use ($regex) {
    $regex->getFlags(true);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    new MongoDB\BSON\Regex;
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex::getPattern() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex::getFlags() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Regex::__construct() expects at least 1 %r(argument|parameter)%r, 0 given
===DONE===
