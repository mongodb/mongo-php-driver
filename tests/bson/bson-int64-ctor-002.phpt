--TEST--
MongoDB\BSON\Int64 constructor (64-bit int value)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

var_dump(new MongoDB\BSON\Int64(PHP_INT_MAX));
var_dump(new MongoDB\BSON\Int64(PHP_INT_MIN));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(19) "9223372036854775807"
}
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(20) "-9223372036854775808"
}
===DONE===
