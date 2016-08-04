--TEST--
MongoDB\BSON\Decimal128 requires valid decimal string
--SKIPIF--
<?php if (!class_exists('MongoDB\BSON\Decimal128')) { die('skip MongoDB\BSON\Decimal128 is not available'); } ?>
<?php if (defined("HHVM_VERSION_ID")) exit("skip HHVM handles parameter parsing differently"); ?>
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
