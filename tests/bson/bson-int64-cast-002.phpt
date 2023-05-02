--TEST--
MongoDB\BSON\Int64 casting (64-bit platform)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$int64 = new MongoDB\BSON\Int64(9223372036854775807);

var_dump($int64);
var_dump((int) $int64);
var_dump((string) $int64);
var_dump((bool) $int64);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\Int64)#%d (%d) {
  ["integer"]=>
  string(19) "9223372036854775807"
}
int(9223372036854775807)
string(19) "9223372036854775807"
bool(true)
===DONE===
