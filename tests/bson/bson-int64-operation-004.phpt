--TEST--
MongoDB\BSON\Int64 operations: bitwise operators
--FILE--
<?php

$value = new MongoDB\BSON\Int64(10);
$int64Max = new MongoDB\BSON\Int64('9223372036854775807');
$int64Min = new MongoDB\BSON\Int64('-9223372036854775808');

var_dump($value << 3);
var_dump($value >> 1);

var_dump($int64Max << 1);

var_dump($value | 1); // 11
var_dump($value & 8); // 8
var_dump($value ^ 2); // 8
var_dump(~$value); // -11
var_dump(~$int64Min); // 9223372036854775807

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
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
  string(2) "-2"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "11"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "8"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "8"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(3) "-11"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(19) "9223372036854775807"
}
===DONE===
