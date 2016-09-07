--TEST--
MongoDB\BSON\UTCDateTime construction from 64-bit integer
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--FILE--
<?php

require_once __DIR__ . '/../utils/tools.php';

$utcdatetime = new MongoDB\BSON\UTCDateTime(1416445411987);

var_dump($utcdatetime);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}
===DONE===
