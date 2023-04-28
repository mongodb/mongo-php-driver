--TEST--
MongoDB\BSON\Value: var_export 64-bit values (32-bit platforms)
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$int64 = unserialize('C:18:"MongoDB\BSON\Int64":38:{a:1:{s:7:"integer";s:10:"2147483648";}}');
$bsonValue = MongoDB\BSON\Value::fromPHP($int64);
echo var_export($bsonValue, true), "\n";

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing int64:
%r\\?%rMongoDB\BSON\Value::__set_state(array(
   'type' => 18,
   'value' => '2147483648',
))
===DONE===
