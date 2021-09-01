--TEST--
MongoDB\BSON\Decimal128 serialization (__serialize and __unserialize)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '7.4.0'); ?>
--FILE--
<?php

$tests = [
    '1234.5678',
    '-1234.5678',
    1234.56e-78,
    INF,
    NAN,
];

foreach ($tests as $value) {
    var_dump($decimal = new MongoDB\BSON\Decimal128($value));
    var_dump($s = serialize($decimal));
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(9) "1234.5678"
}
string(61) "O:23:"MongoDB\BSON\Decimal128":1:{s:3:"dec";s:9:"1234.5678";}"
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(9) "1234.5678"
}

object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(10) "-1234.5678"
}
string(63) "O:23:"MongoDB\BSON\Decimal128":1:{s:3:"dec";s:10:"-1234.5678";}"
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(10) "-1234.5678"
}

object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(11) "1.23456E-75"
}
string(64) "O:23:"MongoDB\BSON\Decimal128":1:{s:3:"dec";s:11:"1.23456E-75";}"
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(11) "1.23456E-75"
}

object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(8) "Infinity"
}
string(60) "O:23:"MongoDB\BSON\Decimal128":1:{s:3:"dec";s:8:"Infinity";}"
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(8) "Infinity"
}

object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(3) "NaN"
}
string(55) "O:23:"MongoDB\BSON\Decimal128":1:{s:3:"dec";s:3:"NaN";}"
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(3) "NaN"
}

===DONE===
