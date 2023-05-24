--TEST--
MongoDB\BSON\Int64 operations: arithmetics
--FILE--
<?php

$value = new MongoDB\BSON\Int64(10);
$int64Max = new MongoDB\BSON\Int64('9223372036854775807');
$int64Min = new MongoDB\BSON\Int64('-9223372036854775808');

var_dump($value + 2);
var_dump($value - 2);
var_dump($value * 2);
var_dump($value / 2);
var_dump($value % 3);

var_dump($int64Max + 1);
var_dump($int64Min - 1);
var_dump($int64Max * 2);

// This division produces a float
var_dump($int64Max / 2);

// This division produces an int
var_dump(($int64Max - 1) / 2);

// This produces overflows
var_dump($int64Min * -1);
var_dump($int64Min / -1);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "12"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "8"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "20"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "5"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "1"
}
float(9.22%d%r[eE]%r+18)
float(-9.22%d%r[eE]%r+18)
float(1.844%d%r[eE]%r+19)
float(4.61%d%r[eE]%r+18)
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(19) "4611686018427387903"
}
float(9.22%d%r[eE]%r+18)
float(9.22%d%r[eE]%r+18)
===DONE===
