--TEST--
MongoDB\BSON\UTCDateTime can be cloned (PHP >= 8.2)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('<', '8.2'); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$utcdatetime = new MongoDB\BSON\UTCDateTime("1416445411987");

$clone = clone $utcdatetime;

var_dump($clone == $utcdatetime);
var_dump($clone === $utcdatetime);

unset($utcdatetime);

var_dump($clone);
?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
bool(true)
bool(false)
object(MongoDB\BSON\UTCDateTime)#%d (1) {
  ["milliseconds"]=>
  string(13) "1416445411987"
}
===DONE===
