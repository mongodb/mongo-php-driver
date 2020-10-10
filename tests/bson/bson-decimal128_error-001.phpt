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
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Decimal128::__construct() expects parameter 1 to be string, array given
===DONE===
