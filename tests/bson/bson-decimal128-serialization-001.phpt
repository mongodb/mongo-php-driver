--TEST--
MongoDB\BSON\Decimal128 serialization
--SKIPIF--
<?php if (!class_exists('MongoDB\BSON\Decimal128')) { die('skip MongoDB\BSON\Decimal128 is not available'); } ?>
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
string(68) "C:23:"MongoDB\BSON\Decimal128":32:{a:1:{s:3:"dec";s:9:"1234.5678";}}"
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(9) "1234.5678"
}

object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(10) "-1234.5678"
}
string(70) "C:23:"MongoDB\BSON\Decimal128":34:{a:1:{s:3:"dec";s:10:"-1234.5678";}}"
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(10) "-1234.5678"
}

object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(11) "1.23456E-75"
}
string(71) "C:23:"MongoDB\BSON\Decimal128":35:{a:1:{s:3:"dec";s:11:"1.23456E-75";}}"
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(11) "1.23456E-75"
}

object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(8) "Infinity"
}
string(67) "C:23:"MongoDB\BSON\Decimal128":31:{a:1:{s:3:"dec";s:8:"Infinity";}}"
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(8) "Infinity"
}

object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(3) "NaN"
}
string(62) "C:23:"MongoDB\BSON\Decimal128":26:{a:1:{s:3:"dec";s:3:"NaN";}}"
object(MongoDB\BSON\Decimal128)#%d (%d) {
  ["dec"]=>
  string(3) "NaN"
}

===DONE===
