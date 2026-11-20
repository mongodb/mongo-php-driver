--TEST--
MongoDB\BSON\Int64 operations: shift count at or above operand width
--FILE--
<?php

$pos = new MongoDB\BSON\Int64(10);
$neg = new MongoDB\BSON\Int64(-10);

// Left shift at or above 64 always returns 0
var_dump($pos << 64);
var_dump($neg << 64);
var_dump($pos << 100);

// Right shift at or above 64 returns 0 for positive, -1 for negative
var_dump($pos >> 64);
var_dump($neg >> 64);
var_dump($pos >> 100);
var_dump($neg >> 100);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "0"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "0"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "0"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "0"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "-1"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "0"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "-1"
}
===DONE===
