--TEST--
MongoDB\BSON\Value::fromPHP(): 64-bit integer (64-bit platforms)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$tests = [
    'int' => 2147483648,
    'Int64 object' => unserialize('C:18:"MongoDB\BSON\Int64":28:{a:1:{s:7:"integer";s:1:"2";}}'),
];

foreach ($tests as $name => $value) {
    printf("Testing %s:\n", $name);

    $bsonValue = MongoDB\BSON\Value::fromPHP($value);

    var_dump($bsonValue);
    var_dump($bsonValue->getValue());
    var_dump($bsonValue->getInt64());
    var_dump($bsonValue->getType() === MongoDB\BSON\Value::TYPE_INT64);
    var_dump($bsonValue->isInt64());
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
Testing int:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "64-bit integer"
  ["value"]=>
  int(2147483648)
}
int(2147483648)
int(2147483648)
bool(true)
bool(true)
Testing Int64 object:
object(MongoDB\BSON\Value)#%d (%d) {
  ["type"]=>
  string(14) "64-bit integer"
  ["value"]=>
  int(2)
}
int(2)
int(2)
bool(true)
bool(true)
===DONE===
