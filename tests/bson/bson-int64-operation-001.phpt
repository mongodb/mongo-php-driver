--TEST--
MongoDB\BSON\Int64 operations
--FILE--
<?php

$value = new MongoDB\BSON\Int64(10);

var_dump($value);
var_dump($value + 2);
var_dump($value - 2);
var_dump($value * 2);
var_dump($value / 2);
var_dump($value % 3);
var_dump($value << 3);
var_dump($value >> 1);
var_dump($value ** 3);

// Testing single operation to ensure op1 and op2 can be an int64
var_dump(2 + $value);

// Testing addition with unsupported numeric values
// This results in PHP casting the Int64 to a number and adding it to a float
var_dump($value + 3.14);
var_dump(3.14 + $value);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "10"
}
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
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "80"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "5"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(4) "1000"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "12"
}
float(13.14)
float(13.14)
===DONE===
