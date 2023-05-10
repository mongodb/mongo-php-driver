--TEST--
MongoDB\BSON\Int64 operations: increment/decrement
--FILE--
<?php

$value = new MongoDB\BSON\Int64(10);

var_dump($value++); // This will print 11 instead of 10 due to PHP not implementing operator overloading for increment
var_dump($value);
var_dump(++$value);
var_dump($value--); // This will print 11 instead of 12 due to PHP not implementing operator overloading for decrement
var_dump($value);
var_dump(--$value);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "11"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "11"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "12"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "11"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "11"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(2) "10"
}
===DONE===
