--TEST--
BSON BSON\UTCDateTime debug handler (32-bit)
--SKIPIF--
<?php if (4 !== PHP_INT_SIZE) { die('skip Only for 32-bit platform'); } ?>
<?php require __DIR__ . "/../utils/basic-skipif.inc"?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$classname = BSON_NAMESPACE . '\UTCDateTime';
$utcdatetime = new $classname('1416445411987');

var_dump($utcdatetime);

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(%SBSON\UTCDateTime)#%d (%d) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}
===DONE===
