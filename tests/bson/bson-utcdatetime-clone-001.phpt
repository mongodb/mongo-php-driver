--TEST--
MongoDB\BSON\UTCDateTime can be cloned
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$utcdatetime = new MongoDB\BSON\UTCDateTime("1416445411987");
$utcdatetime->foo = 'bar';

$clone = clone $utcdatetime;

var_dump($clone == $utcdatetime);
var_dump($clone === $utcdatetime);

unset($utcdatetime);

var_dump($clone);
var_dump($clone->foo);
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
string(3) "bar"
===DONE===
