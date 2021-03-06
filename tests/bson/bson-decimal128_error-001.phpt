--TEST--
MongoDB\BSON\Decimal128 requires valid decimal string
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.99'); ?>
<?php if (!class_exists('MongoDB\BSON\Decimal128')) { die('skip MongoDB\BSON\Decimal128 is not available'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

echo throws(function() {
    new MongoDB\BSON\Decimal128([]);
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

echo throws(function() {
    new MongoDB\BSON\Decimal128('foo');
}, MongoDB\Driver\Exception\InvalidArgumentException::class), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
%SMongoDB\BSON\Decimal128::__construct()%sstring, array given
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing Decimal128 string: foo
===DONE===
