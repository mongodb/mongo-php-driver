--TEST--
MongoDB\BSON\Decimal128::__set_state() requires "dec" string field
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

echo throws(function() {
    MongoDB\BSON\Decimal128::__set_state(['dec' => 0]);
}, 'MongoDB\Driver\Exception\InvalidArgumentException'), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
OK: Got MongoDB\Driver\Exception\InvalidArgumentException
MongoDB\BSON\Decimal128 initialization requires "dec" string field
===DONE===
