--TEST--
MongoDB\BSON\Int64 operations: other operations
--FILE--
<?php

$value = new MongoDB\BSON\Int64(10);

// Testing single operation to ensure op1 and op2 can be an int64
var_dump(2 + $value);

// Testing addition with unsupported numeric values
// This results in PHP casting the Int64 to a number and adding it to a float
var_dump($value + 3.14);
var_dump(3.14 + $value);
var_dump($value + 3.0);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "12"
}
float(13.14)
float(13.14)
float(13)
===DONE===
