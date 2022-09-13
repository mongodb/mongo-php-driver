--TEST--
MongoDB\BSON\Binary argument count errors
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$binary = new MongoDB\BSON\Binary("random binary data", MongoDB\BSON\Binary::TYPE_GENERIC);

echo throws(function() use ($binary) {
    $binary->getData(2);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() use ($binary) {
    $binary->getType(2);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    new MongoDB\BSON\Binary();
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary::getData() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary::getType() expects exactly 0 %r(argument|parameter)%rs, 1 given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Binary::__construct() expects at least 1 %r(argument|parameter)%r, 0 given
===DONE===

