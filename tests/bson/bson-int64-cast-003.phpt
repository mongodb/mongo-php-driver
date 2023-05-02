--TEST--
MongoDB\BSON\Int64 casting (32-bit platform)
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/basic.inc';

$int64 = new MongoDB\BSON\Int64('9223372036854775807');

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

Warning: Truncating 64-bit integer value 9223372036854775807 to 32 bits %s
int(-1)
string(19) "9223372036854775807"
bool(true)
===DONE===
