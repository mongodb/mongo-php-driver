--TEST--
MongoDB\Driver\WriteConcern construction with 64-bit wtimeoutms
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

var_dump(new MongoDB\Driver\WriteConcern(1, 2147483648));

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\WriteConcern)#%d (%d) {
  ["w"]=>
  int(1)
  ["wtimeout"]=>
  int(2147483648)
}
===DONE===
