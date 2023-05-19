--TEST--
MongoDB\BSON\Int64 operations: exponentiation
--FILE--
<?php

$value = new MongoDB\BSON\Int64(10);

var_dump($value ** 3);
var_dump($value ** 0);
var_dump($value ** -3);
var_dump($value ** 13);
var_dump(new MongoDB\BSON\Int64(2) ** 65);
var_dump(new MongoDB\BSON\Int64(0) ** 3);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(4) "1000"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "1"
}
float(0.001)
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(14) "10000000000000"
}
float(3.68%d%r[eE]%r+19)
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(1) "0"
}
===DONE===
