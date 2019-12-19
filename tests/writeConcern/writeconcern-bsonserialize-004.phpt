--TEST--
MongoDB\Driver\WriteConcern::bsonSerialize() encodes 64-bit wtimeoutms as Int64 (32-bit)
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
--FILE--
<?php

$wc = MongoDB\Driver\WriteConcern::__set_state(['w' => 2, 'wtimeout' => '2147483648']);
var_dump($wc->bsonSerialize());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(stdClass)#%d (%d) {
  ["w"]=>
  int(2)
  ["wtimeout"]=>
  object(MongoDB\BSON\Int64)#%d (%d) {
    ["integer"]=>
    string(10) "2147483648"
  }
}
===DONE===
