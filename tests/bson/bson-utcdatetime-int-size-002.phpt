--TEST--
MongoDB\BSON\UTCDateTime integer parsing from number (64-bit)
--SKIPIF--
<?php if (8 !== PHP_INT_SIZE) { die('skip Only for 64-bit platform'); } ?>
--INI--
date.timezone=UTC
error_reporting=-1
dislay_errors=1
--FILE--
<?php

$utcdatetime = new MongoDB\BSON\UTCDateTime(1416445411987);
var_dump($utcdatetime);
var_dump($utcdatetime->toDateTime());

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\BSON\UTCDateTime)#%d (1) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}
object(DateTime)#%d (3) {
  ["date"]=>
  string(26) "2014-11-20 01:03:31.987000"
  ["timezone_type"]=>
  int(1)
  ["timezone"]=>
  string(6) "+00:00"
}
===DONE===
