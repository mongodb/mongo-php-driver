--TEST--
MongoDB\Driver\WriteConcern::bsonSerialize() encodes 64-bit wtimeoutms as integer (64-bit)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

$wc = new MongoDB\Driver\WriteConcern(2, 2147483648);
var_dump($wc->bsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["w"]=>
  int(2)
  ["wtimeout"]=>
  int(2147483648)
}
===DONE===
