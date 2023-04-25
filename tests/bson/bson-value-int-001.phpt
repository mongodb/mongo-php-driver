--TEST--
MongoDB\BSON\Value integer tests (64-bit platforms)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    'int32' => 2,
    'int64' => 2147483648,
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
int(2147483648)
bool(true)
===DONE===
