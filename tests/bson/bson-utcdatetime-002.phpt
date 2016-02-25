--TEST--
BSON BSON\UTCDateTime debug handler
--SKIPIF--
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
  %rint\(|string\(13\) "|%r1416445411987%r"|\)%r
}
===DONE===
