--TEST--
MongoDB\BSON\Value integer tests (32-bit platforms)
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    'int32' => 2,
    'int64' => unserialize('C:18:"MongoDB\BSON\Int64":38:{a:1:{s:7:"integer";s:10:"2147483648";}}'),
];

foreach ($tests as $name => $value) {
    printf("Testing %s:\n", $name);

    $bsonValue = MongoDB\BSON\Value::fromPHP($value);
    var_dump($bsonValue->getInt());
    var_dump($bsonValue->isInt());
}


?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing int32:
int(2)
bool(true)
Testing int64:
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(10) "2147483648"
}
bool(true)
===DONE===
