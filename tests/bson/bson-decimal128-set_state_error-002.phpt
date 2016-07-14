--TEST--
MongoDB\BSON\Decimal128::__set_state() requires valid decimal string
--SKIPIF--
<?php require __DIR__ . '/../utils/basic-skipif.inc'?>
--FILE--
<?php
require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\BSON\Decimal128::__set_state(['dec' => 'INVALID']);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
Error parsing decimal string: INVALID
===DONE===
