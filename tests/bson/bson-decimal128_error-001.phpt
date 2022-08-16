--TEST--
MongoDB\BSON\Decimal128 requires valid decimal string
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    new MongoDB\BSON\Decimal128([]);
}, TypeError::class), "\n";

echo throws(function() {
    new MongoDB\BSON\Decimal128('foo');
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got TypeError
%SMongoDB\BSON\Decimal128::__construct()%sstring, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing Decimal128 string: foo
===DONE===
